/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_tasks.h"
#include "../include/dsp.h"
#include "../include/fft_engine.h"
#include "fsl_debug_console.h"

/* External FFT result */
extern fft_result_t fft_latest_result;

/* FFT task runs periodically and processes if data is available */
void FFTTask(void *pvParameters)
{
    static uint8_t first_run = 1;
    signal_msg_t signal_msg;
    fft_msg_t fft_msg;
    
    if (first_run)
    {
        PRINTF("[FFTTask] Started (waiting for signal)\r\n");
        first_run = 0;
    }
    
    /* Check if signal buffer is ready */
    if (QueueReceive(xSignalQueue, &signal_msg))
    {
        PRINTF("[FFTTask] ← Received from SignalTask\r\n");
        
        /* Run FFT analysis */
        run_fft();
        
        /* Extract results */
        fft_msg.ready = 1;
        fft_msg.peak_magnitude = fft_latest_result.peak_magnitude;
        fft_msg.peak_frequency = fft_latest_result.peak_frequency;
        
        PRINTF("[FFTTask] Peak Frequency = ");
        print_float_4("", fft_msg.peak_frequency, " Hz\r\n");
        
        /* Send results to feature extraction task */
        if (QueueSend(xFFTQueue, &fft_msg))
        {
            PRINTF("[FFTTask] → Sent to FeatureTask\r\n\r\n");
        }
        else
        {
            PRINTF("[FFTTask] ERROR: Queue send failed\r\n");
        }
    }
}
