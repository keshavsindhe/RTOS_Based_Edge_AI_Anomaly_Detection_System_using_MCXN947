/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_tasks.h"
#include "../include/dsp.h"
#include "../include/feature_extractor.h"
#include "fsl_debug_console.h"

/* External feature vector */
extern feature_vector_t features_vector;

/* Feature extraction task runs periodically and processes if FFT data available */
void FeatureTask(void *pvParameters)
{
    static uint8_t first_run = 1;
    fft_msg_t fft_msg;
    feature_msg_t feature_msg;
    
    if (first_run)
    {
        PRINTF("[FeatureTask] Started (waiting for FFT)\r\n");
        first_run = 0;
    }
    
    /* Check for FFT results */
    if (QueueReceive(xFFTQueue, &fft_msg))
    {
        PRINTF("[FeatureTask] ← Received from FFTTask\r\n");
        
        /* Extract features from signal buffer */
        extract_features();
        
        /* Prepare message with key features */
        feature_msg.ready = 1;
        feature_msg.rms = features_vector.rms;
        feature_msg.energy = features_vector.energy;
        feature_msg.peak_frequency = features_vector.peak_frequency;
        
        PRINTF("[FeatureTask] RMS = ");
        print_float_4("", feature_msg.rms, "\r\n");
        
        /* Send features to AI classification task */
        if (QueueSend(xFeatureQueue, &feature_msg))
        {
            PRINTF("[FeatureTask] → Sent to AITask\r\n\r\n");
        }
        else
        {
            PRINTF("[FeatureTask] ERROR: Queue send failed\r\n");
        }
    }
}
