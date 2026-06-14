# AI Classifier Integration Guide

## Overview
The AI Classification Engine has been implemented as standalone modules that can be integrated into your existing working DSP pipeline. Since your FFT, Feature Extraction, and synthetic signal generation are already working, you just need to add the classifier as the final stage.

## Files Created

### 1. `ai/ai_classifier.c` (120 lines)
Core classification engine implementing rule-based anomaly detection.

**Key Functions:**
- `AI_Classifier_Init()` - Initializes and prints threshold configuration
- `AI_Classifier_Predict(const feature_vector_t *features)` - Main classification function
- `AI_Classifier_GetStateName(machine_state_t state)` - Returns state name string
- `AI_Classifier_PrintResults()` - Formatted UART output

**Classification Logic:**
```
FAULT Conditions:
  - RMS > 1.5
  - Energy > 500

WARNING Conditions:
  - RMS > 1.0
  - Energy > 300
  - Peak Frequency < 10 Hz or > 100 Hz

NORMAL:
  - All thresholds within limits
```

### 2. `include/ai_classifier.h` (65 lines)
Public API header with all interface definitions.

**Key Types:**
```c
typedef enum {
    MACHINE_NORMAL = 0,
    MACHINE_WARNING,
    MACHINE_FAULT
} machine_state_t;
```

## Integration Steps

### Step 1: Understand Your Existing Pipeline Structure

Your working DSP pipeline likely has these stages:
1. Signal generation (synthetic data)
2. FFT processing
3. Feature extraction → produces a feature vector

**YOU NEED TO TELL ME:**
- Where is your main application code? (Which file contains your main loop?)
- What is the structure of your feature vector?
- What are the field names in your feature structure?
- Do you already have functions like `DSP_Process()` or do you call FFT and feature extraction separately?

### Step 2: Add AI Classifier to Build System

Edit `CMakeLists.txt` and add the AI classifier source to your existing source list:

**BEFORE:**
```cmake
mcux_add_source(
    SOURCES app/main_sensor.c dsp/mpu6050.c
)
```

**AFTER:**
```cmake
mcux_add_source(
    SOURCES app/[YOUR_MAIN_FILE].c ai/ai_classifier.c [YOUR_OTHER_DSP_FILES].c
)
```

Also add include paths:
```cmake
mcux_add_include(INCLUDES . include ai)
```

### Step 3: Include Header in Your Main Application

```c
#include "../include/ai_classifier.h"
```

### Step 4: Initialize Classifier

In your main function, after hardware initialization:

```c
int main(void) {
    BOARD_InitHardware();
    
    // Your existing DSP initialization
    // ...
    
    // Add AI classifier initialization
    AI_Classifier_Init();
    
    while (1) {
        // Your existing code
    }
}
```

### Step 5: Call Classifier in Your Processing Loop

After your feature extraction produces the feature vector:

```c
while (1) {
    // Your existing signal generation + FFT + feature extraction
    // This produces a feature vector (e.g., feature_vector_t features)
    
    // ADD THIS: Run AI classification
    machine_state_t state = AI_Classifier_Predict(&features);
    
    // ADD THIS: Print results
    AI_Classifier_PrintResults(&features, state);
    
    // Your delay
    SDK_DelayAtLeastUs(1000000U, SystemCoreClock);  // 1 second
}
```

## Expected Feature Vector Structure

The AI classifier expects this structure (defined in `feature_extractor.h`):

```c
typedef struct {
    float rms;                  // Root Mean Square
    float peak_magnitude;       // Peak FFT magnitude
    float peak_frequency;       // Peak frequency in Hz
    float mean;                 // Signal mean
    float std_dev;              // Standard deviation
    float energy;               // Signal energy
    float zero_crossing_rate;   // Zero crossing rate
    uint32_t timestamp;         // Timestamp
} feature_vector_t;
```

**CRITICAL:** If your feature structure has different field names, you need to either:
1. Change your structure to match this, OR
2. Modify `ai/ai_classifier.c` to use your field names

### Checking Your Feature Structure

Run this command to see your feature structure definition:
```bash
grep -A 10 "typedef struct.*feature" include/feature_extractor.h
```

If the fields don't match, modify `AI_Classifier_Predict()` in `ai/ai_classifier.c`.

## Expected Output Format

```
========================================
AI CLASSIFICATION RESULTS
========================================
RMS:              0.87
Energy:           245.30
Peak Frequency:   39.06 Hz
Peak Magnitude:   0.45
Mean:             0.02
Std Dev:          0.83
Zero Crossing:    0.15

Machine State:    NORMAL
========================================
```

## Testing Plan

### Test 1: Normal Operation
With your existing synthetic signal generator producing normal vibration:
- **Expected:** `Machine State: NORMAL`
- **If FAULT/WARNING:** Reduce signal amplitude in your generator

### Test 2: Anomaly Injection
Modify your signal generator to inject high-amplitude spikes:
- **Expected:** `Machine State: WARNING` or `FAULT`
- **If NORMAL:** Increase anomaly amplitude

### Test 3: Threshold Tuning
If classifications don't match expectations, edit thresholds in `ai/ai_classifier.c`:

```c
// Adjust these values
#define RMS_WARNING_THRESHOLD   1.0f
#define RMS_FAULT_THRESHOLD     1.5f
#define ENERGY_WARNING_THRESHOLD 300.0f
#define ENERGY_FAULT_THRESHOLD   500.0f
```

## Next Steps

1. **Tell me about your current working pipeline:**
   - What file contains your main loop?
   - How do you call FFT and feature extraction?
   - What is your feature vector structure?

2. **I can then provide specific integration code** for your exact setup.

3. **Build and test:**
   - Add classifier to CMakeLists.txt
   - Rebuild: `cmake --build debug`
   - Flash firmware
   - Observe serial output at 115200 baud

## Troubleshooting

### Build Error: "undeclared identifier 'feature_vector_t'"
- Make sure `#include "../include/feature_extractor.h"` is in your main file
- Check that `include/` is in your CMake include paths

### Build Error: "No such file or directory: ai_classifier.h"
- Verify `mcux_add_include(INCLUDES . include ai)` is in CMakeLists.txt
- Check file paths are correct

### Runtime: All classifications are FAULT
- Your signal amplitude is too high
- Lower the thresholds in `ai_classifier.c`
- Or reduce signal amplitude in your generator

### Runtime: All classifications are NORMAL
- Signal amplitude too low OR anomalies too subtle
- Increase thresholds in `ai_classifier.c`
- Or increase anomaly magnitude in your generator

---

**Status:** AI Classifier modules are complete and ready for integration. The MPU6050 test firmware is currently built and ready to flash. Once you provide details about your existing DSP pipeline structure, I can integrate the AI classifier directly into your working code.
