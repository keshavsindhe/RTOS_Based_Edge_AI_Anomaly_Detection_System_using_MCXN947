/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DSP_H
#define DSP_H

#include <stdint.h>

#define FFT_SIZE 256
#define SIGNAL_ENABLE_ANOMALY 1
#define SIGNAL_ANOMALY_START 120
#define SIGNAL_ANOMALY_END 130

extern float signal_buffer[FFT_SIZE];
extern float fft_output[FFT_SIZE];
extern float fft_magnitude[FFT_SIZE / 2];
extern float feature_rms;
extern float feature_fft_peak;

void generate_signal(void);
void run_fft(void);
void extract_features(void);

#endif /* DSP_H */
