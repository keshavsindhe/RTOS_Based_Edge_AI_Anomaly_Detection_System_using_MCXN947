/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * PART 11 - END-TO-END TINYML INTEGRATION
 *
 * Complete Edge AI Pipeline with TinyML Inference
 * 
 * Pipeline Flow:
 *   SignalTask → FFTTask → FeatureTask → TinyMLTask → UART Output
 *
 * Board: FRDM-MCXN947
 * Core:  Cortex-M33 (CM33_CORE0)
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "task_scheduler.h"
#include "rtos_tasks.h"
#include "../ml/tinyml_engine.h"

void BOARD_InitHardware(void);
void SysTick_Handler(void);

/* Queue Handles */
QueueHandle_t xSignalQueue = NULL;
QueueHandle_t xFFTQueue = NULL;
QueueHandle_t xFeatureQueue = NULL;

/* SysTick interrupt handler for scheduler timing */
void SysTick_Handler(void)
{
    Scheduler_Tick();
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("╔══════════════════════════════════════════════╗\r\n");
    PRINTF("║  PART 11: End-to-End TinyML Integration     ║\r\n");
    PRINTF("║  Edge AI Predictive Maintenance System      ║\r\n");
    PRINTF("╚══════════════════════════════════════════════╝\r\n");
    PRINTF("\r\n");
    PRINTF("Target:   FRDM-MCXN947\r\n");
    PRINTF("Core:     Cortex-M33 @ CM33_CORE0\r\n");
    PRINTF("Scheduler: Bare-Metal (Custom RTOS)\r\n");
    PRINTF("Mode:     Synthetic Vibration Data\r\n");
    PRINTF("\r\n");
    PRINTF("Pipeline:\r\n");
    PRINTF("  1. SignalTask    → Generate sensor data\r\n");
    PRINTF("  2. FFTTask       → Frequency analysis\r\n");
    PRINTF("  3. FeatureTask   → Extract 7 features\r\n");
    PRINTF("  4. TinyMLTask    → Neural network inference\r\n");
    PRINTF("  5. UART Output   → Display prediction\r\n");
    PRINTF("\r\n");
    PRINTF("════════════════════════════════════════════════\r\n\r\n");

    PRINTF("Initializing Task Scheduler...\r\n");
    PRINTF("════════════════════════════════════════════════\r\n");

    /* Initialize task scheduler */
    Scheduler_Init();

    /* Create Queues */
    PRINTF("[1/3] Creating inter-task queues...\r\n");
    xSignalQueue = QueueCreate(SIGNAL_QUEUE_SIZE, sizeof(signal_msg_t));
    xFFTQueue = QueueCreate(FFT_QUEUE_SIZE, sizeof(fft_msg_t));
    xFeatureQueue = QueueCreate(FEATURE_QUEUE_SIZE, sizeof(feature_msg_t));

    if ((xSignalQueue == NULL) || (xFFTQueue == NULL) || (xFeatureQueue == NULL))
    {
        PRINTF("[ERROR] Queue creation failed!\r\n");
        PRINTF("        System halted.\r\n");
        while (1);
    }

    PRINTF("      ✓ Signal Queue:  %d slots × %d bytes\r\n", 
           SIGNAL_QUEUE_SIZE, sizeof(signal_msg_t));
    PRINTF("      ✓ FFT Queue:     %d slots × %d bytes\r\n", 
           FFT_QUEUE_SIZE, sizeof(fft_msg_t));
    PRINTF("      ✓ Feature Queue: %d slots × %d bytes\r\n\r\n", 
           FEATURE_QUEUE_SIZE, sizeof(feature_msg_t));

    /* Add Tasks */
    PRINTF("[2/3] Registering pipeline tasks...\r\n");

    /* Add Signal Generation Task */
    if (Scheduler_AddTask(SignalTask, NULL, SIGNAL_TASK_PERIOD, SIGNAL_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] SignalTask creation failed!\r\n");
        while (1);
    }
    PRINTF("      ✓ SignalTask    (Period: %3dms, Priority: %d)\r\n", 
           SIGNAL_TASK_PERIOD, SIGNAL_TASK_PRIORITY);

    /* Add FFT Processing Task */
    if (Scheduler_AddTask(FFTTask, NULL, FFT_TASK_PERIOD, FFT_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] FFTTask creation failed!\r\n");
        while (1);
    }
    PRINTF("      ✓ FFTTask       (Period: %3dms, Priority: %d)\r\n", 
           FFT_TASK_PERIOD, FFT_TASK_PRIORITY);

    /* Add Feature Extraction Task */
    if (Scheduler_AddTask(FeatureTask, NULL, FEATURE_TASK_PERIOD, FEATURE_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] FeatureTask creation failed!\r\n");
        while (1);
    }
    PRINTF("      ✓ FeatureTask   (Period: %3dms, Priority: %d)\r\n", 
           FEATURE_TASK_PERIOD, FEATURE_TASK_PRIORITY);

    /* Add TinyML Inference Task */
    if (Scheduler_AddTask(TinyMLTask, NULL, TINYML_TASK_PERIOD, TINYML_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] TinyMLTask creation failed!\r\n");
        while (1);
    }
    PRINTF("      ✓ TinyMLTask    (Period: %3dms, Priority: %d)\r\n\r\n", 
           TINYML_TASK_PERIOD, TINYML_TASK_PRIORITY);

    /* Configure SysTick for 1ms timer */
    PRINTF("[3/3] Starting system timer...\r\n");
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        PRINTF("[ERROR] SysTick configuration failed!\r\n");
        while (1);
    }
    PRINTF("      ✓ SysTick configured (1ms tick)\r\n\r\n");

    PRINTF("════════════════════════════════════════════════\r\n");
    PRINTF("All systems initialized successfully!\r\n");
    PRINTF("Starting scheduler...\r\n");
    PRINTF("════════════════════════════════════════════════\r\n\r\n");

    /* Start the scheduler (infinite loop) */
    Scheduler_Start();

    /* Should never reach here */
    return 0;
}
