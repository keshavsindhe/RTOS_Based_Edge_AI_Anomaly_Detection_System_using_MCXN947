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
    uint32_t cycle_count = 0;

    BOARD_InitHardware();

    PRINTF("\n========================================\r\n");
    PRINTF("Embedded AI Runtime Started\r\n");
    PRINTF("========================================\r\n\n");

    while (1)
    {
        cycle_count++;
        PRINTF("\n[CYCLE %lu] ----------------------------------------\r\n", cycle_count);

        generate_signal();

        run_fft();

        if (!printed_diagnostics)
        {
            PRINTF("\n=== STARTUP DIAGNOSTICS ===\r\n");
            print_startup_diagnostics();
            PRINTF("=== END DIAGNOSTICS ===\r\n\n");
            printed_diagnostics = true;
        }

        extract_features();

        detect_anomaly();

        PRINTF("[CYCLE %lu] Complete\r\n", cycle_count);
        SDK_DelayAtLeastUs(1000000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    }
}
