/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSP_IMPL_H_
#define DSP_IMPL_H_

#include <stdint.h>
#include "../include/dsp.h"

typedef struct {
    float rms;
    float fft_peak;
    uint16_t buffer_idx;
} dsp_context_t;

extern dsp_context_t dsp_ctx;

#endif /* DSP_IMPL_H_ */
