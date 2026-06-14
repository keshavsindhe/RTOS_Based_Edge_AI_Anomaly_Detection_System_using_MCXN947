/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ai_classifier.h"
#include "fsl_debug_console.h"
#include "../include/dsp.h"

/**
 * @brief Initialize AI Classifier
 */
void AI_Classifier_Init(void)
{
    PRINTF("\r\n");
    PRINTF("========================================\r\n");
    PRINTF("  AI CLASSIFIER INITIALIZATION\r\n");
    PRINTF("========================================\r\n");
    PRINTF("Classification Thresholds:\r\n");
    PRINTF("  RMS WARNING   : ");
    print_float_4("", RMS_WARNING_THRESHOLD, " \r\n");
    PRINTF("  RMS FAULT     : ");
    print_float_4("", RMS_FAULT_THRESHOLD, " \r\n");
    PRINTF("  ENERGY WARNING: ");
    print_float_4("", ENERGY_WARNING_THRESHOLD, " \r\n");
    PRINTF("  ENERGY FAULT  : ");
    print_float_4("", ENERGY_FAULT_THRESHOLD, " \r\n");
    PRINTF("  FREQ RANGE    : ");
    print_float_4("", FREQ_ABNORMAL_LOW, " - ");
    print_float_4("", FREQ_ABNORMAL_HIGH, " Hz\r\n");
    PRINTF("========================================\r\n\r\n");
}

/**
 * @brief Rule-Based AI Classification
 * 
 * Classification Rules:
 * 1. FAULT: RMS > 1.5 OR Energy > 500
 * 2. WARNING: RMS > 1.0 OR Energy > 300 OR Frequency out of range
 * 3. NORMAL: All thresholds within limits
 */
machine_state_t AI_Classifier_Predict(const feature_vector_t *features)
{
    /* Check for FAULT conditions (highest priority) */
    if (features->rms > RMS_FAULT_THRESHOLD)
    {
        return MACHINE_FAULT;
    }

    if (features->energy > ENERGY_FAULT_THRESHOLD)
    {
        return MACHINE_FAULT;
    }

    /* Check for WARNING conditions (medium priority) */
    if (features->rms > RMS_WARNING_THRESHOLD)
    {
        return MACHINE_WARNING;
    }

    if (features->energy > ENERGY_WARNING_THRESHOLD)
    {
        return MACHINE_WARNING;
    }

    /* Check frequency range */
    if ((features->peak_frequency < FREQ_ABNORMAL_LOW) || 
        (features->peak_frequency > FREQ_ABNORMAL_HIGH))
    {
        return MACHINE_WARNING;
    }

    /* All checks passed - NORMAL operation */
    return MACHINE_NORMAL;
}

/**
 * @brief Get state name string
 */
const char* AI_Classifier_GetStateName(machine_state_t state)
{
    switch (state)
    {
        case MACHINE_NORMAL:
            return "NORMAL";
        case MACHINE_WARNING:
            return "WARNING";
        case MACHINE_FAULT:
            return "FAULT";
        default:
            return "UNKNOWN";
    }
}

/**
 * @brief Print classification results
 */
void AI_Classifier_PrintResults(const feature_vector_t *features, machine_state_t state)
{
    PRINTF("# ================================\r\n");
    PRINTF("  AI CLASSIFICATION\r\n");
    PRINTF("# ================================\r\n");
    
    /* Print key features */
    PRINTF("RMS             : ");
    print_float_4("", features->rms, "\r\n");
    
    PRINTF("Peak Frequency  : ");
    print_float_4("", features->peak_frequency, " Hz\r\n");
    
    PRINTF("\r\n");
    
    /* Print classification result */
    PRINTF("Machine State   : %s\r\n", AI_Classifier_GetStateName(state));
    
    PRINTF("# ================================\r\n\r\n");
}
