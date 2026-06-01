/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "dsp.h"

float fft_output[FFT_SIZE];

static arm_rfft_fast_instance_f32 fft_instance;
static uint8_t fft_initialized = 0U;

void run_fft(void)
{
    if (fft_initialized == 0U)
    {
        (void)arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);
        fft_initialized = 1U;
    }

    arm_rfft_fast_f32(&fft_instance, signal_buffer, fft_output, 0);
}
