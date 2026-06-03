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
    uint32_t max_index = 0U;

    PRINTF(">>> Extracting features...\r\n");

    arm_rms_f32(signal_buffer, FFT_SIZE, &feature_rms);

    fft_magnitude[0] = (fft_output[0] >= 0.0f) ? fft_output[0] : -fft_output[0];
    arm_cmplx_mag_f32(&fft_output[2], &fft_magnitude[1], (FFT_SIZE / 2U) - 1U);
    arm_max_f32(fft_magnitude, FFT_SIZE / 2U, &feature_fft_peak, &max_index);

    PRINTF("--- Feature Results ---\r\n");
    print_float_4("RMS Value: ", feature_rms, "\r\n");
    print_float_4("FFT Peak: ", feature_fft_peak, "");
    PRINTF(" at index %u\r\n", (unsigned int)max_index);
    PRINTF("Features extraction complete\r\n");
}
