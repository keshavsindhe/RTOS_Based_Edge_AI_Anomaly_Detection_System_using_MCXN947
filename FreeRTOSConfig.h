/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

/* Task priorities */
#define TASK_PRIORITY_IDLE      0
#define TASK_PRIORITY_LOW       1
#define TASK_PRIORITY_MEDIUM    2
#define TASK_PRIORITY_HIGH      3

/* Maximum number of tasks */
#define MAX_TASKS               8

/* Task states */
typedef enum {
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPENDED
} task_state_t;

/* Task control block */
typedef struct {
    void (*task_function)(void *);
    void *parameters;
    uint32_t period_ms;
    uint32_t last_wake_time;
    uint8_t priority;
    task_state_t state;
    bool enabled;
} task_t;

/* Simple message queue structure */
typedef struct {
    void *buffer;
    uint16_t item_size;
    uint16_t capacity;
    uint16_t count;
    uint16_t head;
    uint16_t tail;
} queue_t;

/* Queue handle type */
typedef queue_t* QueueHandle_t;

/* Scheduler functions */
void Scheduler_Init(void);
int Scheduler_AddTask(void (*task_func)(void*), void *params, uint32_t period_ms, uint8_t priority);
void Scheduler_Start(void);
void Scheduler_Tick(void);

/* Queue functions */
QueueHandle_t QueueCreate(uint16_t capacity, uint16_t item_size);
bool QueueSend(QueueHandle_t queue, const void *item);
bool QueueReceive(QueueHandle_t queue, void *item);
uint16_t QueueCount(QueueHandle_t queue);

/* Delay function */
void Task_Delay(uint32_t ms);

#endif /* TASK_SCHEDULER_H */
