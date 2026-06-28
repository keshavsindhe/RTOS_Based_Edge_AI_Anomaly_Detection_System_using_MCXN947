/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"
#include <stdlib.h>

float signal_buffer[FFT_SIZE];

/* Operating mode enumeration */
typedef enum {
    MODE_NORMAL = 0,
    MODE_WARNING = 1,
    MODE_FAULT = 2
} operating_mode_t;

/* Current operating mode (cycles through modes) */
static operating_mode_t current_mode = MODE_NORMAL;
static uint32_t mode_cycle_count = 0;

/**
 * @brief Generate signal based on current operating mode
 * 
 * NORMAL:  Clean sine wave, low amplitude, minimal noise
 * WARNING: Moderate amplitude, some noise, slight irregularities
 * FAULT:   High amplitude, significant noise, major irregularities
 */
void generate_signal(void)
{
    float base_frequency = 10.0f;  // 10 Hz base frequency
    float amplitude;
    float noise_level;
    float irregularity;
    
    /* Determine signal parameters based on mode */
    switch (current_mode)
    {
        case MODE_NORMAL:
            amplitude = 0.5f;      // Low amplitude (RMS ~0.35)
            noise_level = 0.05f;   // Minimal noise
            irregularity = 0.0f;   // No irregularity
            PRINTF(">>> Generating NORMAL signal (size=%d)...\r\n", FFT_SIZE);
            break;
            
        case MODE_WARNING:
            amplitude = 1.0f;      // Moderate amplitude (RMS ~0.7)
            noise_level = 0.15f;   // Moderate noise
            irregularity = 0.3f;   // Slight irregularity
            PRINTF(">>> Generating WARNING signal (size=%d)...\r\n", FFT_SIZE);
            break;
            
        case MODE_FAULT:
            amplitude = 2.0f;      // High amplitude (RMS ~1.4)
            noise_level = 0.4f;    // Significant noise
            irregularity = 0.8f;   // Major irregularity
            PRINTF(">>> Generating FAULT signal (size=%d)...\r\n", FFT_SIZE);
            break;
            
        default:
            amplitude = 0.5f;
            noise_level = 0.05f;
            irregularity = 0.0f;
            break;
    }
    
    /* Generate signal with mode-specific characteristics */
    for (int i = 0; i < FFT_SIZE; i++)
    {
        float t = (float)i / FFT_SIZE;
        
        /* Base sine wave */
        float base_signal = amplitude * arm_sin_f32(2.0f * PI * base_frequency * t);
        
        /* Add second harmonic for irregularity */
        if (irregularity > 0.0f)
        {
            base_signal += irregularity * arm_sin_f32(2.0f * PI * 25.0f * t);
        }
        
        /* Add random noise (simple pseudo-random) */
        float noise = noise_level * ((float)(i % 100) / 50.0f - 1.0f);
        
        signal_buffer[i] = base_signal + noise;
    }
    
    PRINTF("Signal generated: Mode=%s, Amplitude=",
           (current_mode == MODE_NORMAL) ? "NORMAL" :
           (current_mode == MODE_WARNING) ? "WARNING" : "FAULT");
    print_float_4("", amplitude, ", Noise=");
    print_float_4("", noise_level, "\r\n");
}

/**
 * @brief Update operating mode (called by SignalTask)
 * Rotates through modes every 10 cycles
 */
void update_operating_mode(void)
{
    mode_cycle_count++;
    
    /* Rotate mode every 10 cycles */
    if (mode_cycle_count >= 10)
    {
        mode_cycle_count = 0;
        
        /* Advance to next mode */
        current_mode = (operating_mode_t)((current_mode + 1) % 3);
        
        PRINTF("\r\n");
        PRINTF("╔════════════════════════════════════════╗\r\n");
        PRINTF("║   MODE CHANGE: %s%-18s║\r\n",
               (current_mode == MODE_NORMAL) ? "NORMAL" :
               (current_mode == MODE_WARNING) ? "WARNING" : "FAULT",
               "");
        PRINTF("╚════════════════════════════════════════╝\r\n");
        PRINTF("\r\n");
    }
}

/**
 * @brief Get current operating mode (for verification)
 */
uint8_t get_current_mode(void)
{
    return (uint8_t)current_mode;
}

/**
 * @brief Get mode cycle count
 */
uint32_t get_mode_cycle_count(void)
{
    return mode_cycle_count;
}
