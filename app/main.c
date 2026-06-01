/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "arm_math.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "../include/dsp.h"
#include "../include/ai.h"
#include <stdbool.h>

void BOARD_InitHardware(void);

static void print_startup_diagnostics(void)
{
    PRINTF("Signal samples [0..9]:\r\n");
    for (int i = 0; i < 10; i++)
    {
        PRINTF("%.2f\r\n", (double)signal_buffer[i]);
    }

    PRINTF("Signal samples near anomaly:\r\n");
    for (int i = SIGNAL_ANOMALY_START; i <= SIGNAL_ANOMALY_END; i++)
    {
        PRINTF("%.2f\r\n", (double)signal_buffer[i]);
    }

    PRINTF("FFT values [0..9]:\r\n");
    for (int i = 0; i < 10; i++)
    {
        PRINTF("%.2f\r\n", (double)fft_output[i]);
    }
}

int main(void)
{
    bool printed_diagnostics = false;

    BOARD_InitHardware();
    BOARD_InitDebugConsole();

    PRINTF("Embedded AI Runtime Started\r\n");

    while (1)
    {
        generate_signal();

        run_fft();

        if (!printed_diagnostics)
        {
            print_startup_diagnostics();
            printed_diagnostics = true;
        }

        extract_features();

        detect_anomaly();

        SDK_DelayAtLeastUs(1000000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}
