/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"

float signal_buffer[FFT_SIZE];

void generate_signal(void)
{
    PRINTF(">>> Generating signal (size=%d)...\r\n", FFT_SIZE);

    for (int i = 0; i < FFT_SIZE; i++)
    {
        float t = (float)i / FFT_SIZE;

        signal_buffer[i] = arm_sin_f32(2.0f * PI * 10.0f * t);

#if SIGNAL_ENABLE_ANOMALY
        if ((i >= SIGNAL_ANOMALY_START) && (i <= SIGNAL_ANOMALY_END))
        {
            signal_buffer[i] += 3.0f;
        }
#endif
    }

#if SIGNAL_ENABLE_ANOMALY
    PRINTF("Signal generated with ANOMALY injection (indices %d-%d)\r\n", 
           SIGNAL_ANOMALY_START, SIGNAL_ANOMALY_END);
#else
    PRINTF("Signal generated (NORMAL - no anomaly)\r\n");
#endif
}
