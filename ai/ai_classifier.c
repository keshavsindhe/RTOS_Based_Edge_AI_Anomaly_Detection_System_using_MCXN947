/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "../include/ai_classifier.h"
#include "fsl_debug_console.h"

/* Classification thresholds */
#define RMS_FAULT_THRESHOLD       1.5f
#define RMS_WARNING_THRESHOLD     1.0f
#define FREQ_ABNORMAL_LOW         10.0f
#define FREQ_ABNORMAL_HIGH        100.0f
#define ENERGY_FAULT_THRESHOLD    500.0f
#define ENERGY_WARNING_THRESHOLD  300.0f

/**
 * @brief Initialize AI classifier
 */
void AI_Classifier_Init(void)
{
    PRINTF("\r\n================================\r\n");
    PRINTF("AI CLASSIFIER INITIALIZATION\r\n");
    PRINTF("================================\r\n");
    PRINTF("Classification Thresholds:\r\n");
    PRINTF("  RMS Fault:    > %.2f\r\n", RMS_FAULT_THRESHOLD);
    PRINTF("  RMS Warning:  > %.2f\r\n", RMS_WARNING_THRESHOLD);
    PRINTF("  Freq Range:   %.1f - %.1f Hz\r\n", FREQ_ABNORMAL_LOW, FREQ_ABNORMAL_HIGH);
    PRINTF("  Energy Fault: > %.1f\r\n", ENERGY_FAULT_THRESHOLD);
    PRINTF("  Energy Warn:  > %.1f\r\n", ENERGY_WARNING_THRESHOLD);
    PRINTF("================================\r\n\r\n");
}

/**
 * @brief Classify machine state based on feature vector
 * 
 * Rule-based AI classification logic:
 * - Check RMS level (primary indicator)
 * - Check signal energy (secondary indicator)
 * - Check peak frequency range (abnormal operation)
 * 
 * @param features Pointer to feature vector
 * @return machine_state_t Classification result
 */
machine_state_t AI_Classifier_Predict(const feature_vector_t *features)
{
    if (features == NULL)
    {
        return MACHINE_FAULT;
    }

    /* Primary check: RMS level */
    if (features->rms > RMS_FAULT_THRESHOLD)
    {
        return MACHINE_FAULT;
    }
    
    if (features->rms > RMS_WARNING_THRESHOLD)
    {
        return MACHINE_WARNING;
    }

    /* Secondary check: Signal energy */
    if (features->energy > ENERGY_FAULT_THRESHOLD)
    {
        return MACHINE_FAULT;
    }
    
    if (features->energy > ENERGY_WARNING_THRESHOLD)
    {
        return MACHINE_WARNING;
    }

    /* Tertiary check: Frequency range */
    if (features->peak_frequency < FREQ_ABNORMAL_LOW || 
        features->peak_frequency > FREQ_ABNORMAL_HIGH)
    {
        return MACHINE_WARNING;
    }

    /* All checks passed - normal operation */
    return MACHINE_NORMAL;
}

/**
 * @brief Get human-readable state name
 * 
 * @param state Machine state
 * @return const char* State name string
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
 * @brief Print classification results to UART
 * 
 * @param features Pointer to feature vector
 * @param state Classification result
 */
void AI_Classifier_PrintResults(const feature_vector_t *features, machine_state_t state)
{
    PRINTF("\r\n================================\r\n");
    PRINTF("AI CLASSIFICATION\r\n");
    PRINTF("================================\r\n");
    
    if (features != NULL)
    {
        PRINTF("RMS              = %.2f\r\n", features->rms);
        PRINTF("Peak Frequency   = %.2f Hz\r\n", features->peak_frequency);
        PRINTF("Signal Energy    = %.2f\r\n", features->energy);
        PRINTF("Peak Magnitude   = %.2f\r\n", features->peak_magnitude);
    }
    
    PRINTF("\r\nMachine State    : %s\r\n", AI_Classifier_GetStateName(state));
    PRINTF("================================\r\n");
}
