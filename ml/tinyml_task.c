/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tinyml_engine.h"
#include "../app/rtos_tasks.h"
#include "../include/feature_extractor.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"

/* Statistics tracking */
static uint32_t inference_count = 0;
static uint32_t inference_errors = 0;

/* Classification accuracy tracking */
static uint32_t correct_predictions = 0;
static uint32_t total_predictions = 0;
static uint32_t normal_correct = 0, normal_total = 0;
static uint32_t warning_correct = 0, warning_total = 0;
static uint32_t fault_correct = 0, fault_total = 0;

/* External functions */
extern uint8_t get_current_mode(void);

/* Mode name helper */
static const char* get_mode_name(uint8_t mode)
{
    switch (mode)
    {
        case 0: return "NORMAL";
        case 1: return "WARNING";
        case 2: return "FAULT";
        default: return "UNKNOWN";
    }
}

/**
 * @brief TinyML Task - Runs neural network inference on feature vector
 * 
 * This task receives feature extraction results from the FeatureTask queue,
 * runs TFLite inference using the trained neural network model, and
 * classifies the machine state (NORMAL, WARNING, FAULT).
 * 
 * @param pvParameters Task parameters (unused)
 */
void TinyMLTask(void *pvParameters)
{
    static uint8_t first_run = 1;
    static uint8_t initialized = 0;
    feature_msg_t feature_msg;
    feature_vector_t features;
    tinyml_result_t inference_result;
    int status;
    
    if (first_run)
    {
        PRINTF("\r\n");
        PRINTF("========================================\r\n");
        PRINTF("  TinyML Inference Task Starting\r\n");
        PRINTF("========================================\r\n");
        
        /* Initialize TinyML inference engine */
        status = TinyML_Init();
        if (status != 0)
        {
            PRINTF("[TinyMLTask] WARNING: Using STUB inference engine\r\n");
            PRINTF("[TinyMLTask] (TensorFlow Lite not integrated)\r\n");
            PRINTF("[TinyMLTask] Pipeline will run with rule-based classification\r\n");
            initialized = 1;  // Allow stub mode to run
        }
        else
        {
            initialized = 1;
            PRINTF("[TinyMLTask] Ready for inference\r\n");
            PRINTF("[TinyMLTask] Model: %u inputs → %u outputs\r\n",
                   TinyML_GetInputSize(), TinyML_GetOutputSize());
            PRINTF("[TinyMLTask] Memory: %u bytes\r\n", TinyML_GetMemoryUsage());
        }
        
        PRINTF("========================================\r\n\r\n");
        first_run = 0;
    }
    
    /* Skip if not initialized */
    if (!initialized)
    {
        return;
    }
    
    /* Wait for feature extraction results */
    if (QueueReceive(xFeatureQueue, &feature_msg))
    {
        inference_count++;
        
        PRINTF("[TinyMLTask] ← Received from FeatureTask\r\n");
        PRINTF("[TinyMLTask] Inference Cycle %u\r\n", inference_count);
        
        /* Populate feature vector from message */
        features.rms = feature_msg.rms;
        features.mean = feature_msg.mean;
        features.variance = feature_msg.variance;
        features.std_dev = feature_msg.std_dev;
        features.energy = feature_msg.energy;
        features.peak_magnitude = feature_msg.peak_magnitude;
        features.peak_frequency = feature_msg.peak_frequency;
        features.zcr = 0;  // Default value
        
        /* Run TinyML inference */
        status = TinyML_Infer(&features, &inference_result);
        
        if (status != 0)
        {
            inference_errors++;
            PRINTF("[TinyMLTask] ERROR: Inference failed! (errors: %u/%u)\r\n",
                   inference_errors, inference_count);
        }
        else
        {
            /* Get actual operating mode from signal generator */
            uint8_t actual_mode = get_current_mode();
            const char* actual_mode_str = get_mode_name(actual_mode);
            
            /* Print prediction result */
            PRINTF("\r\n");
            PRINTF("========================================\r\n");
            PRINTF("   TinyML PREDICTION RESULT\r\n");
            PRINTF("========================================\r\n");
            
            /* Print class prediction */
            const char* prediction_str;
            uint8_t predicted_mode;
            switch (inference_result.predicted_class)
            {
                case MACHINE_NORMAL:
                    prediction_str = "NORMAL";
                    predicted_mode = 0;
                    break;
                case MACHINE_WARNING:
                    prediction_str = "WARNING";
                    predicted_mode = 1;
                    break;
                case MACHINE_FAULT:
                    prediction_str = "FAULT";
                    predicted_mode = 2;
                    break;
                default:
                    prediction_str = "UNKNOWN";
                    predicted_mode = 255;
                    break;
            }
            
            PRINTF("Prediction:  %s\r\n", prediction_str);
            PRINTF("Actual Mode: %s\r\n", actual_mode_str);
            PRINTF("Confidence:  %u%%\r\n", (unsigned int)(inference_result.confidence * 100.0f));
            
            /* Check if prediction is correct */
            uint8_t is_correct = (predicted_mode == actual_mode);
            PRINTF("Result:      %s\r\n", is_correct ? "CORRECT ✓" : "INCORRECT ✗");
            
            PRINTF("----------------------------------------\r\n");
            PRINTF("Class Probabilities:\r\n");
            PRINTF("  NORMAL:  %u%%\r\n", (unsigned int)(inference_result.scores[0] * 100.0f));
            PRINTF("  WARNING: %u%%\r\n", (unsigned int)(inference_result.scores[1] * 100.0f));
            PRINTF("  FAULT:   %u%%\r\n", (unsigned int)(inference_result.scores[2] * 100.0f));
            
            /* Update accuracy statistics */
            total_predictions++;
            if (is_correct)
            {
                correct_predictions++;
            }
            
            /* Track per-class accuracy */
            switch (actual_mode)
            {
                case 0: // NORMAL
                    normal_total++;
                    if (is_correct) normal_correct++;
                    break;
                case 1: // WARNING
                    warning_total++;
                    if (is_correct) warning_correct++;
                    break;
                case 2: // FAULT
                    fault_total++;
                    if (is_correct) fault_correct++;
                    break;
            }
            
            /* Print accuracy statistics */
            PRINTF("----------------------------------------\r\n");
            PRINTF("Classification Accuracy:\r\n");
            if (total_predictions > 0)
            {
                unsigned int overall_accuracy = (correct_predictions * 100) / total_predictions;
                PRINTF("  Overall: %u/%u (%u%%)\r\n", 
                       correct_predictions, total_predictions, overall_accuracy);
            }
            if (normal_total > 0)
            {
                unsigned int normal_acc = (normal_correct * 100) / normal_total;
                PRINTF("  NORMAL:  %u/%u (%u%%)\r\n", 
                       normal_correct, normal_total, normal_acc);
            }
            if (warning_total > 0)
            {
                unsigned int warning_acc = (warning_correct * 100) / warning_total;
                PRINTF("  WARNING: %u/%u (%u%%)\r\n", 
                       warning_correct, warning_total, warning_acc);
            }
            if (fault_total > 0)
            {
                unsigned int fault_acc = (fault_correct * 100) / fault_total;
                PRINTF("  FAULT:   %u/%u (%u%%)\r\n", 
                       fault_correct, fault_total, fault_acc);
            }
            
            PRINTF("========================================\r\n");
        }
        
        PRINTF("========================================\r\n");
        PRINTF("TinyML Pipeline Cycle %u Complete\r\n", inference_count);
        PRINTF("========================================\r\n\r\n");
    }
    
    /* Task runs periodically - scheduler handles timing */
}

/**
 * @brief Get TinyML task statistics
 * 
 * Helper function to retrieve inference statistics from other tasks or modules.
 * 
 * @param total_inferences Pointer to store total inference count
 * @param total_errors Pointer to store error count
 */
void TinyMLTask_GetStats(uint32_t *total_inferences, uint32_t *total_errors)
{
    if (total_inferences)
    {
        *total_inferences = inference_count;
    }
    
    if (total_errors)
    {
        *total_errors = inference_errors;
    }
}

/**
 * @brief Reset TinyML task statistics
 */
void TinyMLTask_ResetStats(void)
{
    inference_count = 0;
    inference_errors = 0;
    PRINTF("[TinyMLTask] Statistics reset\r\n");
}
