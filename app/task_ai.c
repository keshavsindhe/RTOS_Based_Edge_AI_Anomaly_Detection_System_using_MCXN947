/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_tasks.h"
#include "../include/feature_extractor.h"
#include "ai_classifier.h"
#include "fsl_debug_console.h"

/* External feature vector */
extern feature_vector_t features_vector;

/* AI classification task runs periodically and processes if feature data available */
void AITask(void *pvParameters)
{
    static uint8_t first_run = 1;
    static uint32_t cycle_count = 0;
    feature_msg_t feature_msg;
    machine_state_t machine_state;
    
    if (first_run)
    {
        PRINTF("[AITask] Started (waiting for features)\r\n\r\n");
        first_run = 0;
    }
    
    /* Check for feature extraction results */
    if (QueueReceive(xFeatureQueue, &feature_msg))
    {
        cycle_count++;
        
        PRINTF("[AITask] ← Received from FeatureTask\r\n");
        PRINTF("[AITask] Processing Cycle %lu\r\n", (unsigned long)cycle_count);
        
        /* Run AI classification on full feature vector */
        machine_state = AI_Classifier_Predict(&features_vector);
        
        /* Print classification result */
        PRINTF("[AITask] Machine State = %s\r\n", AI_Classifier_GetStateName(machine_state));
        
        /* Print detailed results */
        PRINTF("\r\n");
        AI_Classifier_PrintResults(&features_vector, machine_state);
        
        PRINTF("========================================\r\n");
        PRINTF("Pipeline Cycle %lu Complete\r\n", (unsigned long)cycle_count);
        PRINTF("========================================\r\n\r\n");
    }
}
