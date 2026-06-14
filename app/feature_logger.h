/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FEATURE_LOGGER_H
#define FEATURE_LOGGER_H

#include "../include/feature_extractor.h"
#include "dataset_generator.h"
#include <stdint.h>

/**
 * @brief Initialize feature logger
 */
void FeatureLogger_Init(void);

/**
 * @brief Print CSV header
 */
void FeatureLogger_PrintHeader(void);

/**
 * @brief Log feature vector to CSV format
 * 
 * @param features Feature vector to log
 * @param label Signal type label
 * @param sample_id Sample number
 */
void FeatureLogger_LogFeatures(const feature_vector_t *features, 
                                signal_type_t label, 
                                uint32_t sample_id);

/**
 * @brief Print dataset generation summary
 * 
 * @param total_samples Total samples generated
 * @param normal_count NORMAL samples
 * @param warning_count WARNING samples
 * @param fault_count FAULT samples
 */
void FeatureLogger_PrintSummary(uint32_t total_samples,
                                 uint32_t normal_count,
                                 uint32_t warning_count,
                                 uint32_t fault_count);

#endif /* FEATURE_LOGGER_H */
