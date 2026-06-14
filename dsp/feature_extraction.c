/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "../include/dsp.h"
#include "../include/feature_extractor.h"
#include "fsl_debug_console.h"

float fft_magnitude[FFT_SIZE / 2];
feature_vector_t features_vector = {0};

void extract_features(void)
{
    PRINTF(">>> Extracting features...\r\n");

    /* Extract all features from signal buffer */
    features_vector = ExtractFeatures(
        signal_buffer,
        FFT_SIZE,
        fft_latest_result.peak_magnitude,
        fft_latest_result.peak_frequency
    );

    /* Print feature vector */
    PrintFeatureVector(&features_vector);
}
