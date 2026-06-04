/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DSP_H
#define DSP_H

#include "fsl_debug_console.h"
#include "fft_engine.h"
#include <stdint.h>

#define SIGNAL_ENABLE_ANOMALY 1
#define SIGNAL_ANOMALY_START 120
#define SIGNAL_ANOMALY_END 130

extern float signal_buffer[FFT_SIZE];
extern float fft_output[FFT_SIZE];
extern float fft_magnitude[FFT_SIZE / 2];
extern float feature_rms;
extern float feature_fft_peak;

static inline void print_float_4(const char *prefix, float value, const char *suffix)
{
    int32_t scaled = (int32_t)((value * 10000.0f) + ((value >= 0.0f) ? 0.5f : -0.5f));
    const char *sign = "";

    if (scaled < 0)
    {
        sign = "-";
        scaled = -scaled;
    }

    unsigned int whole = (unsigned int)(scaled / 10000);
    unsigned int frac = (unsigned int)(scaled % 10000);

    PRINTF("%s%s%u.%u%u%u%u%s",
           prefix,
           sign,
           whole,
           frac / 1000U,
           (frac / 100U) % 10U,
           (frac / 10U) % 10U,
           frac % 10U,
           suffix);
}

void generate_signal(void);
void extract_features(void);

#endif /* DSP_H */
