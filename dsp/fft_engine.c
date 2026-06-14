/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "arm_math.h"
#include "../include/dsp.h"
#include "fsl_debug_console.h"
#include <stddef.h>

float fft_output[FFT_SIZE];
fft_result_t fft_latest_result = {0};

static arm_rfft_fast_instance_f32 fft_instance;
static float fft_input[FFT_SIZE];
static uint8_t fft_initialized = 0U;

static void FFT_Init(void)
{
    if (fft_initialized == 0U)
    {
        PRINTF(">>> Initializing FFT (size=%d)...\r\n", FFT_SIZE);
        (void)arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);
        fft_initialized = 1U;
        PRINTF("FFT initialization complete\r\n");
    }
}

static void print_float_2(const char *prefix, float value, const char *suffix)
{
    int32_t scaled = (int32_t)((value * 100.0f) + ((value >= 0.0f) ? 0.5f : -0.5f));
    const char *sign = "";

    if (scaled < 0)
    {
        sign = "-";
        scaled = -scaled;
    }

    unsigned int whole = (unsigned int)(scaled / 100);
    unsigned int frac = (unsigned int)(scaled % 100);

    PRINTF("%s%s%u.%u%u%s", prefix, sign, whole, frac / 10U, frac % 10U, suffix);
}

void FFT_Process(float *input, fft_result_t *result)
{
    float bin_resolution = SAMPLE_RATE / (float)FFT_SIZE;
    uint32_t max_index = 0U;

    if ((input == NULL) || (result == NULL))
    {
        return;
    }

    FFT_Init();

    for (uint16_t i = 0U; i < FFT_SIZE; i++)
    {
        fft_input[i] = input[i];
    }

    arm_rfft_fast_f32(&fft_instance, fft_input, fft_output, 0);

    fft_magnitude[0] = (fft_output[0] >= 0.0f) ? fft_output[0] : -fft_output[0];
    arm_cmplx_mag_f32(&fft_output[2], &fft_magnitude[1], (FFT_SIZE / 2U) - 1U);
    arm_max_f32(fft_magnitude, FFT_SIZE / 2U, &result->peak_magnitude, &max_index);

    result->peak_index = (uint16_t)max_index;
    result->peak_frequency = (float)result->peak_index * bin_resolution;
}

void run_fft(void)
{
    PRINTF(">>> Running FFT...\r\n");
    FFT_Process(signal_buffer, &fft_latest_result);

    PRINTF("\r\n=== FFT RESULTS ===\r\n");
    print_float_4("Peak Magnitude : ", fft_latest_result.peak_magnitude, "\r\n");
    PRINTF("Peak Index : %u\r\n", (unsigned int)fft_latest_result.peak_index);
    print_float_2("Peak Frequency : ", fft_latest_result.peak_frequency, " Hz\r\n");
    PRINTF("FFT complete\r\n");
}
