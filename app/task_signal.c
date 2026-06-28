/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rtos_tasks.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"

/* External functions from signal_generator.c */
extern void update_operating_mode(void);
extern uint8_t get_current_mode(void);
extern uint32_t get_mode_cycle_count(void);

/* Signal task runs every 100ms - generates new signal data */
void SignalTask(void *pvParameters)
{
    static uint8_t first_run = 1;
    static uint32_t cycle_count = 0;
    signal_msg_t msg;
    
    if (first_run)
    {
        PRINTF("\r\n");
        PRINTF("========================================\r\n");
        PRINTF("  PART 12: Dynamic Fault Scenarios\r\n");
        PRINTF("========================================\r\n");
        PRINTF("[SignalTask] Started (100ms period)\r\n");
        PRINTF("[SignalTask] Mode rotation: Every 10 cycles\r\n");
        PRINTF("[SignalTask] Cycle 1-10:   NORMAL\r\n");
        PRINTF("[SignalTask] Cycle 11-20:  WARNING\r\n");
        PRINTF("[SignalTask] Cycle 21-30:  FAULT\r\n");
        PRINTF("========================================\r\n\r\n");
        first_run = 0;
    }
    
    /* Update operating mode (rotates every 10 cycles) */
    update_operating_mode();
    
    /* Generate synthetic vibration signal based on current mode */
    generate_signal();
    
    cycle_count++;
    
    PRINTF("[SignalTask] Cycle %u (Mode=%s, ModeCount=%u)\r\n", 
           cycle_count,
           (get_current_mode() == 0) ? "NORMAL" :
           (get_current_mode() == 1) ? "WARNING" : "FAULT",
           get_mode_cycle_count() + 1);
    
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
