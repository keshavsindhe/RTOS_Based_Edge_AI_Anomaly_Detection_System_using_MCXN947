/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_tasks.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"

/* Signal task runs every 100ms - generates new signal data */
void SignalTask(void *pvParameters)
{
    static uint8_t first_run = 1;
    signal_msg_t msg;
    
    if (first_run)
    {
        PRINTF("\r\n[SignalTask] Started (100ms period)\r\n");
        first_run = 0;
    }
    
    /* Generate synthetic vibration signal */
    generate_signal();
    
    PRINTF("[SignalTask] Buffer Generated (%d samples)\r\n", FFT_SIZE);
    
    /* Prepare message */
    msg.ready = 1;
    msg.timestamp = Scheduler_GetTick();
    
    /* Send notification to FFT task */
    if (QueueSend(xSignalQueue, &msg))
    {
        PRINTF("[SignalTask] → Sent to FFTTask\r\n\r\n");
    }
    else
    {
        PRINTF("[SignalTask] ERROR: Queue send failed\r\n");
    }
}
