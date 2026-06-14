/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AI_CLASSIFIER_H
#define AI_CLASSIFIER_H

#include <stdint.h>
#include "feature_extractor.h"

/**
 * @brief Machine health state enumeration
 */
typedef enum
{
    MACHINE_NORMAL = 0,   /**< Normal operation */
    MACHINE_WARNING,      /**< Warning - potential issue detected */
    MACHINE_FAULT         /**< Fault - critical issue detected */
} machine_state_t;

/**
 * @brief Initialize AI classifier
 */
void AI_Classifier_Init(void);

/**
 * @brief Classify machine state based on feature vector
 * 
 * @param features Pointer to feature vector
 * @return machine_state_t Classification result
 */
machine_state_t AI_Classifier_Predict(const feature_vector_t *features);

/**
 * @brief Get human-readable state name
 * 
 * @param state Machine state
 * @return const char* State name string
 */
const char* AI_Classifier_GetStateName(machine_state_t state);

/**
 * @brief Print classification results to UART
 * 
 * @param features Pointer to feature vector
 * @param state Classification result
 */
void AI_Classifier_PrintResults(const feature_vector_t *features, machine_state_t state);

#endif /* AI_CLASSIFIER_H */
