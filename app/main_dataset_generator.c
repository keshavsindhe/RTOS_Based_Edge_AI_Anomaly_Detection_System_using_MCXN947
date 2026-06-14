/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * TinyML Dataset Generator
 * 
 * This program generates a training dataset for TinyML classification.
 * It creates 300 samples (100 NORMAL, 100 WARNING, 100 FAULT) and
 * exports features in CSV format for use with Edge Impulse or TensorFlow.
 * 
 * Output: CSV file with 7 features + label
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "dataset_generator.h"
#include "feature_logger.h"
#include "../include/fft_engine.h"
#include "../include/feature_extractor.h"

void BOARD_InitHardware(void);

/* Buffers for dataset generation */
static float dataset_signal_buffer[DATASET_FFT_SIZE];
static fft_result_t dataset_fft_result;
static feature_vector_t dataset_features;

int main(void)
{
    uint32_t sample_id = 0;
    uint32_t normal_count = 0;
    uint32_t warning_count = 0;
    uint32_t fault_count = 0;

    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("================================================\r\n");
    PRINTF("  TinyML Training Dataset Generator\r\n");
    PRINTF("  Edge AI Predictive Maintenance System\r\n");
    PRINTF("================================================\r\n");
    PRINTF("Board:          FRDM-MCXN947\r\n");
    PRINTF("Core:           Cortex-M33 @ CM33_CORE0\r\n");
    PRINTF("Samples/Class:  %d\r\n", SAMPLES_PER_CLASS);
    PRINTF("Total Samples:  %d\r\n", SAMPLES_PER_CLASS * 3);
    PRINTF("Features:       7 (RMS, Mean, Variance, StdDev, Energy, PeakMag, PeakFreq)\r\n");
    PRINTF("Labels:         3 (NORMAL, WARNING, FAULT)\r\n");
    PRINTF("================================================\r\n\r\n");

    /* Initialize modules */
    Dataset_Init();
    FeatureLogger_Init();

    PRINTF("Generating training dataset...\r\n\r\n");
    
    /* Print CSV header */
    FeatureLogger_PrintHeader();

    /* Generate NORMAL samples */
    for (uint32_t i = 0; i < SAMPLES_PER_CLASS; i++)
    {
        /* Generate signal */
        Dataset_GenerateSignal(dataset_signal_buffer, SIGNAL_TYPE_NORMAL, i);
        
        /* Run FFT */
        FFT_Process(dataset_signal_buffer, &dataset_fft_result);
        
        /* Extract features */
        dataset_features = ExtractFeatures(
            dataset_signal_buffer,
            DATASET_FFT_SIZE,
            dataset_fft_result.peak_magnitude,
            dataset_fft_result.peak_frequency
        );
        
        /* Log to CSV */
        FeatureLogger_LogFeatures(&dataset_features, SIGNAL_TYPE_NORMAL, sample_id);
        
        sample_id++;
        normal_count++;
    }

    /* Generate WARNING samples */
    for (uint32_t i = 0; i < SAMPLES_PER_CLASS; i++)
    {
        /* Generate signal */
        Dataset_GenerateSignal(dataset_signal_buffer, SIGNAL_TYPE_WARNING, i);
        
        /* Run FFT */
        FFT_Process(dataset_signal_buffer, &dataset_fft_result);
        
        /* Extract features */
        dataset_features = ExtractFeatures(
            dataset_signal_buffer,
            DATASET_FFT_SIZE,
            dataset_fft_result.peak_magnitude,
            dataset_fft_result.peak_frequency
        );
        
        /* Log to CSV */
        FeatureLogger_LogFeatures(&dataset_features, SIGNAL_TYPE_WARNING, sample_id);
        
        sample_id++;
        warning_count++;
    }

    /* Generate FAULT samples */
    for (uint32_t i = 0; i < SAMPLES_PER_CLASS; i++)
    {
        /* Generate signal */
        Dataset_GenerateSignal(dataset_signal_buffer, SIGNAL_TYPE_FAULT, i);
        
        /* Run FFT */
        FFT_Process(dataset_signal_buffer, &dataset_fft_result);
        
        /* Extract features */
        dataset_features = ExtractFeatures(
            dataset_signal_buffer,
            DATASET_FFT_SIZE,
            dataset_fft_result.peak_magnitude,
            dataset_fft_result.peak_frequency
        );
        
        /* Log to CSV */
        FeatureLogger_LogFeatures(&dataset_features, SIGNAL_TYPE_FAULT, sample_id);
        
        sample_id++;
        fault_count++;
    }

    /* Print summary */
    FeatureLogger_PrintSummary(sample_id, normal_count, warning_count, fault_count);

    PRINTF("Dataset generation complete.\r\n");
    PRINTF("Copy the CSV data above and save as 'training_dataset.csv'\r\n\r\n");

    /* Infinite loop */
    while (1)
    {
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
    }

    return 0;
}
