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

    for (int i = 0; i < FFT_SIZE; i++)
    {
        if (signal_buffer[i] > threshold)
        {
            PRINTF("ANOMALY DETECTED\r\n");
            return;
        }
    }

    PRINTF("Signal NORMAL\r\n");
}
