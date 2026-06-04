/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "dsp.h"
#include "fsl_debug_console.h"

float fft_magnitude[FFT_SIZE / 2];
float feature_rms = 0.0f;
float feature_fft_peak = 0.0f;

void extract_features(void)
{
    PRINTF(">>> Extracting features...\r\n");

    arm_rms_f32(signal_buffer, FFT_SIZE, &feature_rms);
    feature_fft_peak = fft_latest_result.peak_magnitude;

    PRINTF("--- Feature Results ---\r\n");
    print_float_4("RMS Value: ", feature_rms, "\r\n");
    print_float_4("FFT Peak: ", feature_fft_peak, "");
    PRINTF(" at index %u", (unsigned int)fft_latest_result.peak_index);
    print_float_4(" (", fft_latest_result.peak_frequency, " Hz)\r\n");
    PRINTF("Features extraction complete\r\n");
}
