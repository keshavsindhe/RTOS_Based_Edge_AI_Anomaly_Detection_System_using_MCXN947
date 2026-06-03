/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "dsp.h"
#include "fsl_debug_console.h"

float fft_output[FFT_SIZE];

static arm_rfft_fast_instance_f32 fft_instance;
static float fft_input[FFT_SIZE];
static uint8_t fft_initialized = 0U;

void run_fft(void)
{
    if (fft_initialized == 0U)
    {
        PRINTF(">>> Initializing FFT (size=%d)...\r\n", FFT_SIZE);
        (void)arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);
        fft_initialized = 1U;
        PRINTF("FFT initialization complete\r\n");
    }

    PRINTF(">>> Running FFT...\r\n");
    for (int i = 0; i < FFT_SIZE; i++)
    {
        fft_input[i] = signal_buffer[i];
    }

    arm_rfft_fast_f32(&fft_instance, fft_input, fft_output, 0);

    // Calculate magnitudes
    float max_magnitude = 0.0f;
    for (int i = 0; i < FFT_SIZE / 2; i++)
    {
        float real = fft_output[2 * i];
        float imag = fft_output[2 * i + 1];
        // Calculate magnitude: sqrt(real^2 + imag^2)
        fft_magnitude[i] = real * real + imag * imag;  // Start with magnitude squared
        
        if (fft_magnitude[i] > max_magnitude)
        {
            max_magnitude = fft_magnitude[i];
        }
    }
    
    print_float_4("FFT complete. Max magnitude (squared): ", max_magnitude, "\r\n");
}
