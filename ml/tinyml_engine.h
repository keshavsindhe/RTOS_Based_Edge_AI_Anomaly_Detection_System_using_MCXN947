/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TINYML_ENGINE_H
#define TINYML_ENGINE_H

#include <stdint.h>
#include "../include/feature_extractor.h"
#include "../app/ai_classifier.h"

/**
 * @brief TinyML inference engine for vibration anomaly detection
 * 
 * This module integrates TensorFlow Lite for Microcontrollers with
 * the RTOS-based edge AI system on FRDM-MCXN947.
 */

/* Model input/output dimensions */
#define TINYML_INPUT_SIZE       8       // 8 features from feature_vector_t
#define TINYML_OUTPUT_SIZE      3       // 3 classes: NORMAL, WARNING, FAULT

/* Tensor arena size for TFLite Micro (adjust based on model requirements) */
#define TINYML_TENSOR_ARENA_SIZE    (16 * 1024)     // 16 KB

/**
 * @brief TinyML inference result structure
 */
typedef struct
{
    float scores[TINYML_OUTPUT_SIZE];   /**< Class probabilities [0.0 - 1.0] */
    machine_state_t predicted_class;    /**< Predicted machine state */
    float confidence;                   /**< Confidence of prediction */
} tinyml_result_t;

/**
 * @brief Initialize TinyML inference engine
 * 
 * Loads the TFLite model and sets up the interpreter with
 * required operations and memory allocation.
 * 
 * @return 0 on success, -1 on failure
 */
int TinyML_Init(void);

/**
 * @brief Run inference on feature vector
 * 
 * Takes the extracted feature vector and runs TFLite inference
 * to classify machine state.
 * 
 * @param features Pointer to feature vector (8 features)
 * @param result Pointer to store inference result
 * @return 0 on success, -1 on failure
 */
int TinyML_Infer(const feature_vector_t *features, tinyml_result_t *result);

/**
 * @brief Get model input size
 * 
 * @return Number of input features expected by model
 */
uint32_t TinyML_GetInputSize(void);

/**
 * @brief Get model output size
 * 
 * @return Number of output classes
 */
uint32_t TinyML_GetOutputSize(void);

/**
 * @brief Print inference results to console
 * 
 * @param result Pointer to inference result
 */
void TinyML_PrintResult(const tinyml_result_t *result);

/**
 * @brief Get model memory usage
 * 
 * @return Memory used by TFLite interpreter (bytes)
 */
uint32_t TinyML_GetMemoryUsage(void);

#endif /* TINYML_ENGINE_H */
