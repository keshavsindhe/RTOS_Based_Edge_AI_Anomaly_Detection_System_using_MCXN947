/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tinyml_engine.h"
#include "model_data.h"
#include "fsl_debug_console.h"
#include <string.h>
#include <math.h>

/* TensorFlow Lite for Microcontrollers headers */
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

/* Static memory allocation for TFLite */
static uint8_t tensor_arena[TINYML_TENSOR_ARENA_SIZE] __attribute__((aligned(16)));

/* TFLite objects */
static const tflite::Model* model = nullptr;
static tflite::MicroInterpreter* interpreter = nullptr;
static TfLiteTensor* input_tensor = nullptr;
static TfLiteTensor* output_tensor = nullptr;

/* Operation resolver */
static tflite::MicroMutableOpResolver<10> micro_op_resolver;

/* Initialization status */
static uint8_t initialized = 0;

/**
 * @brief Initialize TinyML inference engine
 */
int TinyML_Init(void)
{
    PRINTF("[TinyML] Initializing inference engine...\r\n");
    
    /* Initialize TFLite Micro system */
    tflite::InitializeTarget();
    
    /* Load model from flash */
    model = tflite::GetModel(g_model);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        PRINTF("[TinyML] ERROR: Model schema version mismatch!\r\n");
        PRINTF("[TinyML] Expected: %d, Got: %d\r\n", 
               TFLITE_SCHEMA_VERSION, model->version());
        return -1;
    }
    PRINTF("[TinyML] Model loaded (version %d)\r\n", model->version());
    PRINTF("[TinyML] Model size: %u bytes\r\n", g_model_len);
    
    /* Register required operations for the model */
    /* Adjust based on your specific model architecture */
    if (micro_op_resolver.AddFullyConnected() != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Failed to add FullyConnected op\r\n");
        return -1;
    }
    if (micro_op_resolver.AddSoftmax() != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Failed to add Softmax op\r\n");
        return -1;
    }
    if (micro_op_resolver.AddRelu() != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Failed to add ReLU op\r\n");
        return -1;
    }
    if (micro_op_resolver.AddQuantize() != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Failed to add Quantize op\r\n");
        return -1;
    }
    if (micro_op_resolver.AddDequantize() != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Failed to add Dequantize op\r\n");
        return -1;
    }
    
    PRINTF("[TinyML] Operations registered\r\n");
    
    /* Create interpreter */
    static tflite::MicroInterpreter static_interpreter(
        model, micro_op_resolver, tensor_arena, TINYML_TENSOR_ARENA_SIZE);
    interpreter = &static_interpreter;
    
    /* Allocate memory for tensors */
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Tensor allocation failed!\r\n");
        return -1;
    }
    PRINTF("[TinyML] Tensor arena allocated: %u bytes\r\n", TINYML_TENSOR_ARENA_SIZE);
    
    /* Get pointers to input and output tensors */
    input_tensor = interpreter->input(0);
    output_tensor = interpreter->output(0);
    
    /* Validate tensor dimensions */
    if (input_tensor->dims->size != 2 || input_tensor->dims->data[1] != TINYML_INPUT_SIZE)
    {
        PRINTF("[TinyML] ERROR: Input tensor shape mismatch!\r\n");
        PRINTF("[TinyML] Expected: (1, %d), Got: (", TINYML_INPUT_SIZE);
        for (int i = 0; i < input_tensor->dims->size; i++)
        {
            PRINTF("%d", input_tensor->dims->data[i]);
            if (i < input_tensor->dims->size - 1) PRINTF(", ");
        }
        PRINTF(")\r\n");
        return -1;
    }
    
    if (output_tensor->dims->size != 2 || output_tensor->dims->data[1] != TINYML_OUTPUT_SIZE)
    {
        PRINTF("[TinyML] ERROR: Output tensor shape mismatch!\r\n");
        PRINTF("[TinyML] Expected: (1, %d), Got: (", TINYML_OUTPUT_SIZE);
        for (int i = 0; i < output_tensor->dims->size; i++)
        {
            PRINTF("%d", output_tensor->dims->data[i]);
            if (i < output_tensor->dims->size - 1) PRINTF(", ");
        }
        PRINTF(")\r\n");
        return -1;
    }
    
    PRINTF("[TinyML] Input tensor: type=%d, bytes=%u\r\n", 
           input_tensor->type, input_tensor->bytes);
    PRINTF("[TinyML] Output tensor: type=%d, bytes=%u\r\n", 
           output_tensor->type, output_tensor->bytes);
    
    /* Print memory usage */
    size_t used_bytes = interpreter->arena_used_bytes();
    PRINTF("[TinyML] Arena memory used: %u / %u bytes (%.1f%%)\r\n",
           (unsigned int)used_bytes, TINYML_TENSOR_ARENA_SIZE,
           (float)(used_bytes * 100.0f / TINYML_TENSOR_ARENA_SIZE));
    
    initialized = 1;
    PRINTF("[TinyML] Initialization complete!\r\n\r\n");
    
    return 0;
}

/**
 * @brief Run inference on feature vector
 */
int TinyML_Infer(const feature_vector_t *features, tinyml_result_t *result)
{
    if (!initialized)
    {
        PRINTF("[TinyML] ERROR: Engine not initialized!\r\n");
        return -1;
    }
    
    if (!features || !result)
    {
        PRINTF("[TinyML] ERROR: NULL pointer!\r\n");
        return -1;
    }
    
    /* Prepare input data - map feature_vector_t to input tensor */
    /* Order: rms, mean, variance, std_dev, energy, peak_magnitude, peak_frequency, zcr */
    float input_data[TINYML_INPUT_SIZE] = {
        features->rms,
        features->mean,
        features->variance,
        features->std_dev,
        features->energy,
        features->peak_magnitude,
        features->peak_frequency,
        (float)features->zcr
    };
    
    /* Copy input data to tensor */
    /* Handle quantized vs float models */
    if (input_tensor->type == kTfLiteFloat32)
    {
        /* Float model */
        memcpy(input_tensor->data.f, input_data, TINYML_INPUT_SIZE * sizeof(float));
    }
    else if (input_tensor->type == kTfLiteInt8)
    {
        /* Quantized model (int8) */
        float input_scale = input_tensor->params.scale;
        int32_t input_zero_point = input_tensor->params.zero_point;
        
        for (int i = 0; i < TINYML_INPUT_SIZE; i++)
        {
            int32_t quantized_value = (int32_t)(input_data[i] / input_scale) + input_zero_point;
            quantized_value = (quantized_value < -128) ? -128 : quantized_value;
            quantized_value = (quantized_value > 127) ? 127 : quantized_value;
            input_tensor->data.int8[i] = (int8_t)quantized_value;
        }
    }
    else
    {
        PRINTF("[TinyML] ERROR: Unsupported input tensor type: %d\r\n", input_tensor->type);
        return -1;
    }
    
    /* Run inference */
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk)
    {
        PRINTF("[TinyML] ERROR: Inference failed!\r\n");
        return -1;
    }
    
    /* Extract output data */
    /* Handle quantized vs float models */
    if (output_tensor->type == kTfLiteFloat32)
    {
        /* Float model */
        memcpy(result->scores, output_tensor->data.f, TINYML_OUTPUT_SIZE * sizeof(float));
    }
    else if (output_tensor->type == kTfLiteInt8)
    {
        /* Quantized model (int8) - dequantize */
        float output_scale = output_tensor->params.scale;
        int32_t output_zero_point = output_tensor->params.zero_point;
        
        for (int i = 0; i < TINYML_OUTPUT_SIZE; i++)
        {
            result->scores[i] = (output_tensor->data.int8[i] - output_zero_point) * output_scale;
        }
    }
    else
    {
        PRINTF("[TinyML] ERROR: Unsupported output tensor type: %d\r\n", output_tensor->type);
        return -1;
    }
    
    /* Find predicted class (argmax) */
    result->predicted_class = MACHINE_NORMAL;
    result->confidence = result->scores[0];
    
    for (int i = 1; i < TINYML_OUTPUT_SIZE; i++)
    {
        if (result->scores[i] > result->confidence)
        {
            result->confidence = result->scores[i];
            result->predicted_class = (machine_state_t)i;
        }
    }
    
    return 0;
}

/**
 * @brief Get model input size
 */
uint32_t TinyML_GetInputSize(void)
{
    return TINYML_INPUT_SIZE;
}

/**
 * @brief Get model output size
 */
uint32_t TinyML_GetOutputSize(void)
{
    return TINYML_OUTPUT_SIZE;
}

/**
 * @brief Print inference results to console
 */
void TinyML_PrintResult(const tinyml_result_t *result)
{
    const char *state_names[TINYML_OUTPUT_SIZE] = {
        "NORMAL", "WARNING", "FAULT"
    };
    
    PRINTF("[TinyML] Inference Results:\r\n");
    PRINTF("  Class Probabilities:\r\n");
    for (int i = 0; i < TINYML_OUTPUT_SIZE; i++)
    {
        PRINTF("    %s: %.4f (%.1f%%)\r\n", 
               state_names[i], 
               result->scores[i],
               result->scores[i] * 100.0f);
    }
    
    PRINTF("  Predicted: %s (confidence: %.4f / %.1f%%)\r\n",
           state_names[result->predicted_class],
           result->confidence,
           result->confidence * 100.0f);
}

/**
 * @brief Get model memory usage
 */
uint32_t TinyML_GetMemoryUsage(void)
{
    if (!initialized || !interpreter)
    {
        return 0;
    }
    
    return (uint32_t)interpreter->arena_used_bytes();
}
