/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "feature_logger.h"
#include "fsl_debug_console.h"
#include "../include/dsp.h"

/**
 * @brief Initialize feature logger
 */
void FeatureLogger_Init(void)
{
    /* Nothing to initialize currently */
}

/**
 * @brief Print CSV header
 */
void FeatureLogger_PrintHeader(void)
{
    PRINTF("sample_id,rms,mean,variance,std_dev,energy,peak_magnitude,peak_frequency,label\r\n");
}

/**
 * @brief Log feature vector to CSV format
 */
void FeatureLogger_LogFeatures(const feature_vector_t *features, 
                                signal_type_t label, 
                                uint32_t sample_id)
{
    /* Print sample ID */
    PRINTF("%lu,", (unsigned long)sample_id);
    
    /* Print RMS */
    print_float_4("", features->rms, ",");
    
    /* Print Mean */
    print_float_4("", features->mean, ",");
    
    /* Print Variance */
    print_float_4("", features->variance, ",");
    
    /* Print Std Dev */
    print_float_4("", features->std_dev, ",");
    
    /* Print Energy */
    print_float_4("", features->energy, ",");
    
    /* Print Peak Magnitude */
    print_float_4("", features->peak_magnitude, ",");
    
    /* Print Peak Frequency */
    print_float_4("", features->peak_frequency, ",");
    
    /* Print Label */
    PRINTF("%s\r\n", Dataset_GetTypeName(label));
}

/**
 * @brief Print dataset generation summary
 */
void FeatureLogger_PrintSummary(uint32_t total_samples,
                                 uint32_t normal_count,
                                 uint32_t warning_count,
                                 uint32_t fault_count)
{
    PRINTF("\r\n");
    PRINTF("========================================\r\n");
    PRINTF("  DATASET GENERATION COMPLETE\r\n");
    PRINTF("========================================\r\n");
    PRINTF("Total Samples:   %u\r\n", total_samples);
    PRINTF("  NORMAL:        %u\r\n", normal_count);
    PRINTF("  WARNING:       %u\r\n", warning_count);
    PRINTF("  FAULT:         %u\r\n", fault_count);
    PRINTF("========================================\r\n\r\n");
    
    PRINTF("Next Steps:\r\n");
    PRINTF("1. Copy CSV data from serial terminal\r\n");
    PRINTF("2. Save as 'training_dataset.csv'\r\n");
    PRINTF("3. Split into separate files:\r\n");
    PRINTF("   - normal.csv (label=NORMAL)\r\n");
    PRINTF("   - warning.csv (label=WARNING)\r\n");
    PRINTF("   - fault.csv (label=FAULT)\r\n");
    PRINTF("4. Import to Edge Impulse or TensorFlow\r\n");
    PRINTF("5. Train classification model\r\n");
    PRINTF("6. Export TensorFlow Lite Micro model\r\n");
    PRINTF("7. Integrate into AITask\r\n\r\n");
}
