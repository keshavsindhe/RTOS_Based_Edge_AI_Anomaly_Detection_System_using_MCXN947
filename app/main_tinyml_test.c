/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * PART 10 - FIRST MCU TINYML INFERENCE TEST
 *
 * This program performs standalone TinyML inference testing on FRDM-MCXN947
 * WITHOUT FreeRTOS integration. It validates that the TFLite model loads
 * correctly and produces expected predictions.
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "../ml/tinyml_engine.h"
#include "../include/feature_extractor.h"
#include <string.h>

/* Board initialization */
extern void BOARD_InitHardware(void);

/* Test case structure */
typedef struct
{
    const char *name;
    float features[8];
    machine_state_t expected_class;
    const char *description;
} tinyml_test_case_t;

/* Test cases */
static const tinyml_test_case_t test_cases[] = {
    {
        .name = "Test #1 - NORMAL Operation",
        .features = {
            0.87f,      // RMS
            0.01f,      // Mean
            0.07f,      // Variance
            0.08f,      // Std Dev
            194.0f,     // Energy
            113.0f,     // Peak Magnitude
            39.06f,     // Peak Frequency (Hz)
            0.10f       // ZCR
        },
        .expected_class = MACHINE_NORMAL,
        .description = "Healthy machine operation with low vibration"
    },
    {
        .name = "Test #2 - FAULT Condition",
        .features = {
            2.20f,      // RMS
            0.50f,      // Mean
            2.00f,      // Variance
            1.40f,      // Std Dev
            500.0f,     // Energy
            160.0f,     // Peak Magnitude
            45.0f,      // Peak Frequency (Hz)
            0.10f       // ZCR
        },
        .expected_class = MACHINE_FAULT,
        .description = "Severe fault with high vibration and energy"
    }
};

#define NUM_TEST_CASES (sizeof(test_cases) / sizeof(test_cases[0]))

/**
 * @brief Print a horizontal separator line
 */
static void PrintSeparator(void)
{
    PRINTF("========================================\r\n");
}

/**
 * @brief Print test header
 */
static void PrintTestHeader(void)
{
    PRINTF("\r\n");
    PrintSeparator();
    PRINTF("  PART 10: TinyML Inference Test\r\n");
    PRINTF("  Target: FRDM-MCXN947\r\n");
    PrintSeparator();
    PRINTF("\r\n");
}

/**
 * @brief Print feature vector details
 */
static void PrintFeatures(const float *features)
{
    PRINTF("  Input Features:\r\n");
    PRINTF("    [0] RMS:             %.4f\r\n", features[0]);
    PRINTF("    [1] Mean:            %.4f\r\n", features[1]);
    PRINTF("    [2] Variance:        %.4f\r\n", features[2]);
    PRINTF("    [3] Std Dev:         %.4f\r\n", features[3]);
    PRINTF("    [4] Energy:          %.4f\r\n", features[4]);
    PRINTF("    [5] Peak Magnitude:  %.4f\r\n", features[5]);
    PRINTF("    [6] Peak Frequency:  %.4f Hz\r\n", features[6]);
    PRINTF("    [7] ZCR:             %.4f\r\n", features[7]);
}

/**
 * @brief Convert machine state enum to string
 */
static const char* GetStateString(machine_state_t state)
{
    switch (state)
    {
        case MACHINE_NORMAL:  return "NORMAL";
        case MACHINE_WARNING: return "WARNING";
        case MACHINE_FAULT:   return "FAULT";
        default:              return "UNKNOWN";
    }
}

/**
 * @brief Run a single test case
 */
static int RunTestCase(const tinyml_test_case_t *test_case, uint32_t test_num)
{
    tinyml_result_t result;
    feature_vector_t features;
    int status;
    int pass = 0;
    
    PRINTF("\r\n");
    PrintSeparator();
    PRINTF("Test Case %lu: %s\r\n", (unsigned long)test_num, test_case->name);
    PrintSeparator();
    PRINTF("Description: %s\r\n", test_case->description);
    PRINTF("Expected Prediction: %s\r\n\r\n", GetStateString(test_case->expected_class));
    
    /* Pack features into feature_vector_t structure */
    features.rms = test_case->features[0];
    features.mean = test_case->features[1];
    features.variance = test_case->features[2];
    features.std_dev = test_case->features[3];
    features.energy = test_case->features[4];
    features.peak_magnitude = test_case->features[5];
    features.peak_frequency = test_case->features[6];
    features.zcr = (uint32_t)(test_case->features[7] * 1000.0f);  // Scale ZCR
    
    /* Print input features */
    PrintFeatures(test_case->features);
    PRINTF("\r\n");
    
    /* Run inference */
    PRINTF("[TinyML] Running inference...\r\n");
    status = TinyML_Infer(&features, &result);
    
    if (status != 0)
    {
        PRINTF("[TinyML] ERROR: Inference failed!\r\n");
        return 0;
    }
    
    PRINTF("[TinyML] Inference complete\r\n\r\n");
    
    /* Print raw output tensor values */
    PRINTF("  Raw Output Tensor:\r\n");
    PRINTF("    Class 0 (NORMAL):  %.6f\r\n", result.scores[0]);
    PRINTF("    Class 1 (WARNING): %.6f\r\n", result.scores[1]);
    PRINTF("    Class 2 (FAULT):   %.6f\r\n", result.scores[2]);
    PRINTF("\r\n");
    
    /* Print prediction results */
    PRINTF("  Prediction Results:\r\n");
    PRINTF("    Predicted Class:   %s\r\n", GetStateString(result.predicted_class));
    PRINTF("    Confidence:        %.4f (%.1f%%)\r\n", 
           result.confidence, result.confidence * 100.0f);
    PRINTF("\r\n");
    
    /* Verify against expected result */
    if (result.predicted_class == test_case->expected_class)
    {
        PRINTF("  ✓ PASS: Prediction matches expected class\r\n");
        pass = 1;
    }
    else
    {
        PRINTF("  ✗ FAIL: Expected %s, got %s\r\n",
               GetStateString(test_case->expected_class),
               GetStateString(result.predicted_class));
        pass = 0;
    }
    
    return pass;
}

/**
 * @brief Main entry point
 */
int main(void)
{
    int status;
    uint32_t passed = 0;
    uint32_t failed = 0;
    
    /* Initialize hardware */
    BOARD_InitHardware();
    
    /* Print test header */
    PrintTestHeader();
    
    /* Initialize TinyML engine */
    PRINTF("========================================\r\n");
    PRINTF("  STEP 1: Initialize TinyML Runtime\r\n");
    PRINTF("========================================\r\n\r\n");
    
    status = TinyML_Init();
    
    if (status != 0)
    {
        PRINTF("\r\n[ERROR] TinyML initialization failed!\r\n");
        PRINTF("[ERROR] Test sequence aborted.\r\n");
        while (1) { }  // Halt
    }
    
    /* Verify initialization */
    PRINTF("\r\n");
    PRINTF("[TinyML] ✓ Model Loaded Successfully\r\n");
    PRINTF("[TinyML] ✓ Input Tensor Size = %lu\r\n", (unsigned long)TinyML_GetInputSize());
    PRINTF("[TinyML] ✓ Output Classes = %lu\r\n", (unsigned long)TinyML_GetOutputSize());
    PRINTF("[TinyML] ✓ Memory Usage = %lu bytes\r\n", (unsigned long)TinyML_GetMemoryUsage());
    PRINTF("[TinyML] ✓ TinyML Ready\r\n");
    
    /* Run test cases */
    PRINTF("\r\n\r\n");
    PRINTF("========================================\r\n");
    PRINTF("  STEP 2: Run Fixed Input Tests\r\n");
    PRINTF("========================================\r\n");
    
    for (uint32_t i = 0; i < NUM_TEST_CASES; i++)
    {
        if (RunTestCase(&test_cases[i], i + 1))
        {
            passed++;
        }
        else
        {
            failed++;
        }
    }
    
    /* Print final summary */
    PRINTF("\r\n\r\n");
    PrintSeparator();
    PRINTF("  TEST SUMMARY\r\n");
    PrintSeparator();
    PRINTF("Total Tests:  %lu\r\n", (unsigned long)NUM_TEST_CASES);
    PRINTF("Passed:       %lu\r\n", (unsigned long)passed);
    PRINTF("Failed:       %lu\r\n", (unsigned long)failed);
    
    if (failed == 0)
    {
        PRINTF("\r\n✓ ALL TESTS PASSED\r\n");
        PRINTF("✓ TinyML inference verified on FRDM-MCXN947\r\n");
        PRINTF("✓ Ready for FreeRTOS integration\r\n");
    }
    else
    {
        PRINTF("\r\n✗ SOME TESTS FAILED\r\n");
        PRINTF("✗ Review model predictions\r\n");
    }
    
    PrintSeparator();
    PRINTF("\r\n[TinyML] Test sequence complete.\r\n");
    PRINTF("[TinyML] System halted.\r\n\r\n");
    
    /* Halt */
    while (1) { }
    
    return 0;
}
