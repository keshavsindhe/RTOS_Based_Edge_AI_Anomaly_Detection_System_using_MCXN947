/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dataset_generator.h"
#include "arm_math.h"
#include <stdlib.h>

/* Random seed for reproducibility */
static uint32_t random_seed = 12345;

/**
 * @brief Simple pseudo-random number generator
 */
static float random_float(float min, float max)
{
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    float normalized = (float)random_seed / (float)0x7fffffff;
    return min + normalized * (max - min);
}

/**
 * @brief Initialize dataset generator
 */
void Dataset_Init(void)
{
    random_seed = 12345;  // Reset for reproducible datasets
}

/**
 * @brief Get signal parameters based on type and sample index
 */
signal_params_t Dataset_GetParams(signal_type_t type, uint32_t sample_index)
{
    signal_params_t params;
    
    switch (type)
    {
        case SIGNAL_TYPE_NORMAL:
            /* Normal operation: low amplitude, minimal noise, no anomalies */
            params.frequency_hz = 10.0f + random_float(-2.0f, 2.0f);
            params.amplitude = 0.7f + random_float(-0.1f, 0.1f);
            params.noise_level = 0.05f + random_float(0.0f, 0.03f);
            params.anomaly_amplitude = 0.0f;
            params.anomaly_start = 0;
            params.anomaly_end = 0;
            break;
            
        case SIGNAL_TYPE_WARNING:
            /* Warning: moderate amplitude, moderate noise, small anomalies */
            params.frequency_hz = 10.0f + random_float(-3.0f, 3.0f);
            params.amplitude = 0.9f + random_float(-0.15f, 0.15f);
            params.noise_level = 0.1f + random_float(0.0f, 0.05f);
            params.anomaly_amplitude = 1.2f + random_float(-0.2f, 0.3f);
            params.anomaly_start = 120 + (uint16_t)random_float(-10, 10);
            params.anomaly_end = 130 + (uint16_t)random_float(-10, 10);
            break;
            
        case SIGNAL_TYPE_FAULT:
            /* Fault: high amplitude, high noise, large anomalies */
            params.frequency_hz = 10.0f + random_float(-5.0f, 5.0f);
            params.amplitude = 1.0f + random_float(-0.2f, 0.3f);
            params.noise_level = 0.15f + random_float(0.0f, 0.1f);
            params.anomaly_amplitude = 2.5f + random_float(-0.5f, 1.0f);
            params.anomaly_start = 100 + (uint16_t)random_float(-20, 20);
            params.anomaly_end = 150 + (uint16_t)random_float(-20, 20);
            break;
            
        default:
            /* Default to normal */
            params.frequency_hz = 10.0f;
            params.amplitude = 0.7f;
            params.noise_level = 0.05f;
            params.anomaly_amplitude = 0.0f;
            params.anomaly_start = 0;
            params.anomaly_end = 0;
            break;
    }
    
    return params;
}

/**
 * @brief Generate synthetic signal for dataset
 */
void Dataset_GenerateSignal(float *buffer, signal_type_t type, uint32_t sample_index)
{
    signal_params_t params = Dataset_GetParams(type, sample_index);
    
    /* Generate base sinusoidal signal with noise */
    for (uint16_t i = 0; i < DATASET_FFT_SIZE; i++)
    {
        float t = (float)i / (float)DATASET_FFT_SIZE;
        
        /* Base sinusoid */
        float signal = params.amplitude * arm_sin_f32(2.0f * PI * params.frequency_hz * t);
        
        /* Add noise */
        float noise = random_float(-params.noise_level, params.noise_level);
        signal += noise;
        
        /* Add anomaly if within range */
        if ((i >= params.anomaly_start) && (i <= params.anomaly_end))
        {
            signal += params.anomaly_amplitude;
        }
        
        buffer[i] = signal;
    }
}

/**
 * @brief Get signal type name
 */
const char* Dataset_GetTypeName(signal_type_t type)
{
    switch (type)
    {
        case SIGNAL_TYPE_NORMAL:
            return "NORMAL";
        case SIGNAL_TYPE_WARNING:
            return "WARNING";
        case SIGNAL_TYPE_FAULT:
            return "FAULT";
        default:
            return "UNKNOWN";
    }
}
