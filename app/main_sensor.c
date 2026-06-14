/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "task_scheduler.h"
#include "rtos_tasks.h"
#include "ai_classifier.h"

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
    PRINTF("================================================\r\n");
    PRINTF("  Edge AI Predictive Maintenance System\r\n");
    PRINTF("  Multitasking Architecture (Bare-Metal)\r\n");
    PRINTF("================================================\r\n");
    PRINTF("Board:    FRDM-MCXN947\r\n");
    PRINTF("Core:     Cortex-M33 @ CM33_CORE0\r\n");
    PRINTF("RTOS:     Bare-Metal Scheduler\r\n");
    PRINTF("Mode:     Synthetic Vibration Data\r\n");
    PRINTF("Sensor:   MPU6050 (Not Connected)\r\n");
    PRINTF("================================================\r\n\r\n");

    /* Initialize AI Classifier */
    AI_Classifier_Init();

    PRINTF("Creating Task Scheduler...\r\n");
    PRINTF("================================================\r\n");

    /* Initialize task scheduler */
    Scheduler_Init();

    /* Create Queues */
    xSignalQueue = QueueCreate(SIGNAL_QUEUE_SIZE, sizeof(signal_msg_t));
    xFFTQueue = QueueCreate(FFT_QUEUE_SIZE, sizeof(fft_msg_t));
    xFeatureQueue = QueueCreate(FEATURE_QUEUE_SIZE, sizeof(feature_msg_t));

    if ((xSignalQueue == NULL) || (xFFTQueue == NULL) || (xFeatureQueue == NULL))
    {
        PRINTF("[ERROR] Queue creation failed!\r\n");
        while (1);
    }

    PRINTF("[OK] Queues created\r\n");
    PRINTF("     - Signal Queue:  %d slots\r\n", SIGNAL_QUEUE_SIZE);
    PRINTF("     - FFT Queue:     %d slots\r\n", FFT_QUEUE_SIZE);
    PRINTF("     - Feature Queue: %d slots\r\n\r\n", FEATURE_QUEUE_SIZE);

    /* Add Signal Generation Task */
    if (Scheduler_AddTask(SignalTask, NULL, SIGNAL_TASK_PERIOD, SIGNAL_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] SignalTask creation failed!\r\n");
        while (1);
    }
    PRINTF("[OK] SignalTask added (Period: %dms, Priority: %d)\r\n", 
           SIGNAL_TASK_PERIOD, SIGNAL_TASK_PRIORITY);

    /* Add FFT Processing Task */
    if (Scheduler_AddTask(FFTTask, NULL, FFT_TASK_PERIOD, FFT_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] FFTTask creation failed!\r\n");
        while (1);
    }
    PRINTF("[OK] FFTTask added (Period: %dms, Priority: %d)\r\n", 
           FFT_TASK_PERIOD, FFT_TASK_PRIORITY);

    /* Add Feature Extraction Task */
    if (Scheduler_AddTask(FeatureTask, NULL, FEATURE_TASK_PERIOD, FEATURE_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] FeatureTask creation failed!\r\n");
        while (1);
    }
    PRINTF("[OK] FeatureTask added (Period: %dms, Priority: %d)\r\n", 
           FEATURE_TASK_PERIOD, FEATURE_TASK_PRIORITY);

    /* Add AI Classification Task */
    if (Scheduler_AddTask(AITask, NULL, AI_TASK_PERIOD, AI_TASK_PRIORITY) < 0)
    {
        PRINTF("[ERROR] AITask creation failed!\r\n");
        while (1);
    }
    PRINTF("[OK] AITask added (Period: %dms, Priority: %d)\r\n\r\n", 
           AI_TASK_PERIOD, AI_TASK_PRIORITY);

    PRINTF("================================================\r\n");
    PRINTF("Starting Task Scheduler...\r\n");
    PRINTF("================================================\r\n\r\n");

    /* Configure SysTick for 1ms tick */
    SysTick_Config(SystemCoreClock / 1000);

    /* Start the scheduler (never returns) */
    Scheduler_Start();

    /* Should never reach here */
    PRINTF("[ERROR] Scheduler exited!\r\n");
    while (1);

    return 0;
}
