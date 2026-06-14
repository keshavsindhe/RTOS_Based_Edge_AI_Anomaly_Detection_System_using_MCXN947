/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RTOS_TASKS_H
#define RTOS_TASKS_H

#include "task_scheduler.h"
#include <stdint.h>

/* Task Priorities */
#define SIGNAL_TASK_PRIORITY    TASK_PRIORITY_LOW
#define FFT_TASK_PRIORITY       TASK_PRIORITY_MEDIUM
#define FEATURE_TASK_PRIORITY   TASK_PRIORITY_MEDIUM
#define AI_TASK_PRIORITY        TASK_PRIORITY_MEDIUM

/* Task Periods (in milliseconds) */
#define SIGNAL_TASK_PERIOD      100     // 100 ms
#define FFT_TASK_PERIOD         10      // Check every 10 ms
#define FEATURE_TASK_PERIOD     10      // Check every 10 ms
#define AI_TASK_PERIOD          10      // Check every 10 ms

/* Queue Sizes */
#define SIGNAL_QUEUE_SIZE       2
#define FFT_QUEUE_SIZE          2
#define FEATURE_QUEUE_SIZE      2

/* Queue Data Structures */
typedef struct {
    uint8_t ready;
    uint32_t timestamp;
} signal_msg_t;

typedef struct {
    uint8_t ready;
    float peak_magnitude;
    float peak_frequency;
} fft_msg_t;

typedef struct {
    uint8_t ready;
    float rms;
    float energy;
    float peak_frequency;
} feature_msg_t;

/* Queue Handles (extern - defined in main) */
extern QueueHandle_t xSignalQueue;
extern QueueHandle_t xFFTQueue;
extern QueueHandle_t xFeatureQueue;

/* Task Function Prototypes */
void SignalTask(void *pvParameters);
void FFTTask(void *pvParameters);
void FeatureTask(void *pvParameters);
void AITask(void *pvParameters);

#endif /* RTOS_TASKS_H */
