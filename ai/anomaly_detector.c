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
    PRINTF("Threshold: %.4f\r\n", threshold);
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
            PRINTF("Value: %.4f\r\n", signal_buffer[i]);
            PRINTF("Threshold: %.4f\r\n", threshold);
            PRINTF("Deviation: %.4f\r\n", signal_buffer[i] - threshold);
            PRINTF("=== ANOMALY DETECTION END ===\r\n\n");
            return;
        }
    }

    PRINTF("Signal NORMAL\r\n");
    PRINTF("Max Value: %.4f at index %d\r\n", max_value, max_index);
    PRINTF("RMS Feature: %.4f\r\n", feature_rms);
    PRINTF("FFT Peak: %.4f\r\n", feature_fft_peak);
    PRINTF("=== ANOMALY DETECTION END ===\r\n\n");
}
