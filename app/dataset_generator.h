/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DATASET_GENERATOR_H
#define DATASET_GENERATOR_H

#include <stdint.h>

/* Signal types for dataset generation */
typedef enum {
    SIGNAL_TYPE_NORMAL = 0,
    SIGNAL_TYPE_WARNING,
    SIGNAL_TYPE_FAULT
} signal_type_t;

/* Dataset configuration */
#define SAMPLES_PER_CLASS       100
#define DATASET_FFT_SIZE        256

/* Signal generation parameters */
typedef struct {
    float frequency_hz;         // Base frequency (Hz)
    float amplitude;            // Signal amplitude
    float noise_level;          // Noise amplitude
    float anomaly_amplitude;    // Anomaly spike amplitude
    uint16_t anomaly_start;     // Anomaly start index
    uint16_t anomaly_end;       // Anomaly end index
} signal_params_t;

/**
 * @brief Initialize dataset generator
 */
void Dataset_Init(void);

/**
 * @brief Generate synthetic signal for dataset
 * 
 * @param buffer Output buffer (must be DATASET_FFT_SIZE)
 * @param type Signal type (NORMAL/WARNING/FAULT)
 * @param sample_index Sample number (for variation)
 */
void Dataset_GenerateSignal(float *buffer, signal_type_t type, uint32_t sample_index);

/**
 * @brief Get signal type name
 */
const char* Dataset_GetTypeName(signal_type_t type);

/**
 * @brief Get signal parameters for type
 */
signal_params_t Dataset_GetParams(signal_type_t type, uint32_t sample_index);

#endif /* DATASET_GENERATOR_H */
