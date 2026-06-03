/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "dsp.h"
#include "fsl_debug_console.h"

void detect_anomaly(void)
{
    float threshold = 2.0f;
    float max_value = 0.0f;
    int max_index = 0;

    PRINTF("\n=== ANOMALY DETECTION START ===\r\n");
    print_float_4("Threshold: ", threshold, "\r\n");
    PRINTF("Buffer Size: %d\r\n", FFT_SIZE);

    for (int i = 0; i < FFT_SIZE; i++)
    {
        // Track maximum value for diagnostics
        if (signal_buffer[i] > max_value)
        {
            max_value = signal_buffer[i];
            max_index = i;
        }

        if (signal_buffer[i] > threshold)
        {
            PRINTF("\n!!! ANOMALY DETECTED !!!\r\n");
            PRINTF("Index: %d\r\n", i);
            print_float_4("Value: ", signal_buffer[i], "\r\n");
            print_float_4("Threshold: ", threshold, "\r\n");
            print_float_4("Deviation: ", signal_buffer[i] - threshold, "\r\n");
            PRINTF("=== ANOMALY DETECTION END ===\r\n\n");
            return;
        }
    }

    PRINTF("Signal NORMAL\r\n");
    print_float_4("Max Value: ", max_value, "");
    PRINTF(" at index %d\r\n", max_index);
    print_float_4("RMS Feature: ", feature_rms, "\r\n");
    print_float_4("FFT Peak: ", feature_fft_peak, "\r\n");
    PRINTF("=== ANOMALY DETECTION END ===\r\n\n");
}
