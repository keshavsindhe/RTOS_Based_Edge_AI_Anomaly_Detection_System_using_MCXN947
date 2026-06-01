/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "arm_math.h"
#include "../include/dsp.h"
#include "dsp_impl.h"

dsp_context_t dsp_ctx = {0};

void DSP_Init(void)
{
    dsp_ctx.buffer_idx = 0;
    dsp_ctx.rms = 0.0f;
    dsp_ctx.fft_peak = 0.0f;

    for (uint16_t i = 0; i < FFT_SIZE; i++)
    {
        signal_buffer[i] = 0.0f;
        fft_output[i] = 0.0f;
    }
}

void DSP_Process(void)
{
    generate_signal();
    run_fft();
    extract_features();

    dsp_ctx.rms = feature_rms;
    dsp_ctx.fft_peak = feature_fft_peak;
    dsp_ctx.buffer_idx++;
}
