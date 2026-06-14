/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../include/feature_extractor.h"
#include "arm_math.h"
#include "fsl_debug_console.h"
#include <math.h>

/**
 * @brief Calculate mean (average) of signal samples
 */
float CalculateMean(const float *buffer, uint32_t size)
{
    if (buffer == NULL || size == 0U)
    {
        return 0.0f;
    }

    float sum = 0.0f;
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        sum += buffer[i];
    }

    return sum / (float)size;
}

/**
 * @brief Calculate variance of signal samples
 *
 * Variance = sum((x[i] - mean)^2) / size
 */
float CalculateVariance(const float *buffer, uint32_t size)
{
    if (buffer == NULL || size == 0U)
    {
        return 0.0f;
    }

    float mean = CalculateMean(buffer, size);
    float variance = 0.0f;
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        float diff = buffer[i] - mean;
        variance += diff * diff;
    }

    return variance / (float)size;
}

/**
 * @brief Calculate standard deviation of signal samples
 *
 * StdDev = sqrt(variance)
 */
float CalculateStdDeviation(const float *buffer, uint32_t size)
{
    float variance = CalculateVariance(buffer, size);
    return sqrtf(variance);
}

/**
 * @brief Calculate energy of signal samples
 *
 * Energy = sum(x[i]^2)
 */
float CalculateEnergy(const float *buffer, uint32_t size)
{
    if (buffer == NULL || size == 0U)
    {
        return 0.0f;
    }

    float energy = 0.0f;
    uint32_t i;

    for (i = 0U; i < size; i++)
    {
        energy += buffer[i] * buffer[i];
    }

    return energy;
}

/**
 * @brief Calculate zero crossing rate of signal samples
 *
 * ZCR = number of times signal changes sign / (size - 1)
 * Returned as count of zero crossings (not normalized)
 */
uint32_t CalculateZeroCrossingRate(const float *buffer, uint32_t size)
{
    if (buffer == NULL || size < 2U)
    {
        return 0U;
    }

    uint32_t zero_crossings = 0U;
    uint32_t i;

    for (i = 1U; i < size; i++)
    {
        /* Check if sign changes between consecutive samples */
        if ((buffer[i - 1] * buffer[i]) < 0.0f)
        {
            zero_crossings++;
        }
    }

    return zero_crossings;
}

/**
 * @brief Extract complete feature vector from signal buffer
 */
feature_vector_t ExtractFeatures(
    const float *buffer,
    uint32_t size,
    float peak_mag,
    float peak_freq)
{
    feature_vector_t features;
    float rms = 0.0f;

    if (buffer != NULL && size > 0U)
    {
        arm_rms_f32((float *)buffer, size, &rms);
    }

    features.rms = rms;
    features.mean = CalculateMean(buffer, size);
    features.variance = CalculateVariance(buffer, size);
    features.std_dev = CalculateStdDeviation(buffer, size);
    features.energy = CalculateEnergy(buffer, size);
    features.peak_magnitude = peak_mag;
    features.peak_frequency = peak_freq;
    features.zcr = CalculateZeroCrossingRate(buffer, size);

    return features;
}

/**
 * @brief Print feature vector to UART
 */
void PrintFeatureVector(const feature_vector_t *features)
{
    if (features == NULL)
    {
        PRINTF("ERROR: NULL feature vector\r\n");
        return;
    }

    PRINTF("=== FEATURE VECTOR ===\r\n\n");

    /* Helper macro for printing floats with 4 decimal places */
#define PRINT_FEATURE(label, value) \
    do { \
        int32_t scaled = (int32_t)((value * 10000.0f) + ((value >= 0.0f) ? 0.5f : -0.5f)); \
        const char *sign = ""; \
        if (scaled < 0) { sign = "-"; scaled = -scaled; } \
        unsigned int whole = (unsigned int)(scaled / 10000); \
        unsigned int frac = (unsigned int)(scaled % 10000); \
        PRINTF("%-15s: %s%u.%u%u%u%u\r\n", \
               label, sign, whole, \
               frac / 1000U, (frac / 100U) % 10U, \
               (frac / 10U) % 10U, frac % 10U); \
    } while(0)

    PRINT_FEATURE("RMS", features->rms);
    PRINT_FEATURE("Mean", features->mean);
    PRINT_FEATURE("Variance", features->variance);
    PRINT_FEATURE("Std Dev", features->std_dev);
    PRINT_FEATURE("Energy", features->energy);
    PRINT_FEATURE("Peak Magnitude", features->peak_magnitude);
    PRINTF("%-15s: ", "Peak Frequency");

    /* Print peak frequency with proper formatting */
    int32_t freq_scaled = (int32_t)((features->peak_frequency * 100.0f) + 0.5f);
    unsigned int freq_whole = (unsigned int)(freq_scaled / 100);
    unsigned int freq_frac = (unsigned int)(freq_scaled % 100);
    PRINTF("%u.%u%u Hz\r\n", freq_whole, freq_frac / 10U, freq_frac % 10U);

    PRINTF("%-15s: %u\r\n", "ZCR", (unsigned int)features->zcr);

    PRINTF("\nFeature extraction complete\r\n\r\n");

#undef PRINT_FEATURE
}
