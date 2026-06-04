/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FFT_ENGINE_H
#define FFT_ENGINE_H

#include <stdint.h>

#define FFT_SIZE 256
#define SAMPLE_RATE 1000.0f

typedef struct
{
    float peak_magnitude;
    uint16_t peak_index;
    float peak_frequency;
} fft_result_t;

extern fft_result_t fft_latest_result;

void FFT_Process(float *input, fft_result_t *result);
void run_fft(void);

#endif /* FFT_ENGINE_H */
