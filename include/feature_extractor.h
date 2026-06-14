/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Unified feature vector structure
 */
typedef struct
{
    float rms;              /**< Root Mean Square */
    float mean;             /**< Arithmetic mean */
    float variance;         /**< Variance */
    float std_dev;          /**< Standard deviation */
    float energy;           /**< Signal energy */
    float peak_magnitude;   /**< Peak magnitude from FFT */
    float peak_frequency;   /**< Peak frequency from FFT (Hz) */
    uint32_t zcr;           /**< Zero crossing rate */
} feature_vector_t;

/**
 * @brief Calculate mean (average) of signal samples
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @return Mean value
 */
float CalculateMean(
    const float *buffer,
    uint32_t size);

/**
 * @brief Calculate variance of signal samples
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @return Variance value
 */
float CalculateVariance(
    const float *buffer,
    uint32_t size);

/**
 * @brief Calculate standard deviation of signal samples
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @return Standard deviation value
 */
float CalculateStdDeviation(
    const float *buffer,
    uint32_t size);

/**
 * @brief Calculate energy of signal samples
 *
 * Energy = sum of squared samples
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @return Energy value
 */
float CalculateEnergy(
    const float *buffer,
    uint32_t size);

/**
 * @brief Calculate zero crossing rate of signal samples
 *
 * ZCR = count of sign changes / (2 * (size - 1))
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @return Zero crossing rate (typically 0-1, scaled to count here)
 */
uint32_t CalculateZeroCrossingRate(
    const float *buffer,
    uint32_t size);

/**
 * @brief Extract complete feature vector from signal buffer
 *
 * Computes all statistical features and returns unified feature vector.
 * Requires FFT to have been computed separately for peak_magnitude and peak_frequency.
 *
 * @param buffer Pointer to signal buffer
 * @param size Number of samples
 * @param peak_mag Peak magnitude from FFT (0 if not computed)
 * @param peak_freq Peak frequency from FFT in Hz (0 if not computed)
 * @return Feature vector with all computed features
 */
feature_vector_t ExtractFeatures(
    const float *buffer,
    uint32_t size,
    float peak_mag,
    float peak_freq);

/**
 * @brief Print feature vector to UART
 *
 * @param features Pointer to feature vector
 */
void PrintFeatureVector(const feature_vector_t *features);

#endif /* FEATURE_EXTRACTOR_H */
