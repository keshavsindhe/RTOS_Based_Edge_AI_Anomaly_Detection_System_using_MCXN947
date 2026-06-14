/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AI_CLASSIFIER_H
#define AI_CLASSIFIER_H

#include "../include/feature_extractor.h"
#include <stdint.h>

/**
 * @brief Machine State Classification
 */
typedef enum
{
    MACHINE_NORMAL = 0,
    MACHINE_WARNING,
    MACHINE_FAULT
} machine_state_t;

/**
 * @brief Classification Thresholds
 */
#define RMS_WARNING_THRESHOLD       1.0f
#define RMS_FAULT_THRESHOLD         1.5f
#define ENERGY_WARNING_THRESHOLD    300.0f
#define ENERGY_FAULT_THRESHOLD      500.0f
#define FREQ_ABNORMAL_LOW           10.0f
#define FREQ_ABNORMAL_HIGH          100.0f

/**
 * @brief Initialize AI Classifier
 * 
 * Prints configuration and threshold settings
 */
void AI_Classifier_Init(void);

/**
 * @brief Predict machine state from feature vector
 * 
 * @param features Pointer to extracted feature vector
 * @return Machine state (NORMAL, WARNING, or FAULT)
 */
machine_state_t AI_Classifier_Predict(const feature_vector_t *features);

/**
 * @brief Get machine state name as string
 * 
 * @param state Machine state enum
 * @return String representation ("NORMAL", "WARNING", "FAULT")
 */
const char* AI_Classifier_GetStateName(machine_state_t state);

/**
 * @brief Print AI classification results to UART
 * 
 * @param features Pointer to feature vector
 * @param state Classified machine state
 */
void AI_Classifier_PrintResults(const feature_vector_t *features, machine_state_t state);

#endif /* AI_CLASSIFIER_H */
