/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "task_scheduler.h"
#include "fsl_common.h"
#include <string.h>
#include <stdlib.h>

/* Task table */
static task_t task_table[MAX_TASKS];
static uint8_t task_count = 0;
static volatile uint32_t system_tick = 0;

/**
 * @brief Initialize the task scheduler
 */
void Scheduler_Init(void)
{
    task_count = 0;
    system_tick = 0;
    
    /* Clear task table */
    memset(task_table, 0, sizeof(task_table));
}

/**
 * @brief Add a task to the scheduler
 * 
 * @param task_func Task function pointer
 * @param params Task parameters
 * @param period_ms Task period in milliseconds
 * @param priority Task priority
 * @return Task ID or -1 if failed
 */
int Scheduler_AddTask(void (*task_func)(void*), void *params, uint32_t period_ms, uint8_t priority)
{
    if (task_count >= MAX_TASKS)
    {
        return -1;
    }
    
    task_t *task = &task_table[task_count];
    task->task_function = task_func;
    task->parameters = params;
    task->period_ms = period_ms;
    task->last_wake_time = 0;
    task->priority = priority;
    task->state = TASK_STATE_READY;
    task->enabled = true;
    
    return task_count++;
}

/**
 * @brief Increment system tick (call from SysTick or timer ISR)
 */
void Scheduler_Tick(void)
{
    system_tick++;
}

/**
 * @brief Get current system tick
 */
uint32_t Scheduler_GetTick(void)
{
    return system_tick;
}

/**
 * @brief Run the scheduler (cooperative round-robin)
 */
void Scheduler_Start(void)
{
    while (1)
    {
        /* Iterate through all tasks */
        for (uint8_t i = 0; i < task_count; i++)
        {
            task_t *task = &task_table[i];
            
            /* Skip disabled tasks */
            if (!task->enabled || task->state != TASK_STATE_READY)
            {
                continue;
            }
            
            /* Check if task period has elapsed */
            uint32_t current_time = Scheduler_GetTick();
            if ((current_time - task->last_wake_time) >= task->period_ms)
            {
                /* Update last wake time */
                task->last_wake_time = current_time;
                
                /* Run task */
                task->state = TASK_STATE_RUNNING;
                task->task_function(task->parameters);
                task->state = TASK_STATE_READY;
            }
        }
    }
}

/**
 * @brief Create a queue
 */
QueueHandle_t QueueCreate(uint16_t capacity, uint16_t item_size)
{
    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
    if (queue == NULL)
    {
        return NULL;
    }
    
    queue->buffer = malloc(capacity * item_size);
    if (queue->buffer == NULL)
    {
        free(queue);
        return NULL;
    }
    
    queue->item_size = item_size;
    queue->capacity = capacity;
    queue->count = 0;
    queue->head = 0;
    queue->tail = 0;
    
    return queue;
}

/**
 * @brief Send item to queue
 */
bool QueueSend(QueueHandle_t queue, const void *item)
{
    if (queue == NULL || item == NULL)
    {
        return false;
    }
    
    /* Check if queue is full */
    if (queue->count >= queue->capacity)
    {
        return false;
    }
    
    /* Copy item to queue */
    uint8_t *dest = (uint8_t*)queue->buffer + (queue->tail * queue->item_size);
    memcpy(dest, item, queue->item_size);
    
    /* Update tail and count */
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->count++;
    
    return true;
}

/**
 * @brief Receive item from queue
 */
bool QueueReceive(QueueHandle_t queue, void *item)
{
    if (queue == NULL || item == NULL)
    {
        return false;
    }
    
    /* Check if queue is empty */
    if (queue->count == 0)
    {
        return false;
    }
    
    /* Copy item from queue */
    uint8_t *src = (uint8_t*)queue->buffer + (queue->head * queue->item_size);
    memcpy(item, src, queue->item_size);
    
    /* Update head and count */
    queue->head = (queue->head + 1) % queue->capacity;
    queue->count--;
    
    return true;
}

/**
 * @brief Get number of items in queue
 */
uint16_t QueueCount(QueueHandle_t queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    return queue->count;
}

/**
 * @brief Task delay (busy wait)
 */
void Task_Delay(uint32_t ms)
{
    SDK_DelayAtLeastUs(ms * 1000U, SystemCoreClock);
}
