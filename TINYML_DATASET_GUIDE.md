# PART 9 - TinyML Dataset Preparation
## Dataset Generator for Edge AI Classification

### Overview
This dataset generator creates training data for TinyML classification models. It produces 300 labeled samples (100 per class) with 7 features extracted from synthetic vibration signals.

---

## Generated Files

### 1. **dataset_generator.c/h**
Signal generation with three distinct patterns:

**NORMAL Signals:**
- Low amplitude (0.6-0.8)
- Minimal noise (0.05-0.08)
- No anomalies
- Stable frequency (8-12 Hz)

**WARNING Signals:**
- Moderate amplitude (0.75-1.05)
- Moderate noise (0.10-0.15)
- Small anomalies (spike: 1.0-1.5)
- Variable frequency (7-13 Hz)

**FAULT Signals:**
- High amplitude (0.8-1.3)
- High noise (0.15-0.25)
- Large anomalies (spike: 2.0-3.5)
- Unstable frequency (5-15 Hz)

### 2. **feature_logger.c/h**
CSV export functionality:
- Prints feature vectors in CSV format
- Includes labels (NORMAL/WARNING/FAULT)
- Compatible with Edge Impulse and TensorFlow

### 3. **main_dataset_generator.c**
Standalone application that:
- Generates 300 samples (100 per class)
- Extracts 7 features per sample
- Outputs CSV to UART
- Prints generation summary

---

## Feature Vector (7 Features)

| Feature | Description | Range |
|---------|-------------|-------|
| **RMS** | Root Mean Square | 0.5 - 3.0 |
| **Mean** | Signal average | -0.5 - 0.5 |
| **Variance** | Signal variance | 0.1 - 5.0 |
| **StdDev** | Standard deviation | 0.3 - 2.5 |
| **Energy** | Total signal energy | 50 - 1000 |
| **PeakMagnitude** | FFT peak magnitude | 10 - 100 |
| **PeakFrequency** | Dominant frequency (Hz) | 5 - 15 |

---

## Building the Dataset Generator

### Option 1: Replace Main Application (Temporary)

**1. Edit CMakeLists.txt:**
```cmake
# Comment out the multitasking main
# mcux_add_source(SOURCES app/main_sensor.c ...)

# Add dataset generator main
mcux_add_source(
    SOURCES app/main_dataset_generator.c 
            app/dataset_generator.c 
            app/feature_logger.c 
            dsp/fft_engine.c 
            dsp/feature_extractor.c
)
```

**2. Build:**
```bash
cmake --preset=debug
cmake --build debug
```

**3. Flash and capture output:**
- Flash firmware
- Open serial terminal (115200 baud)
- Wait for CSV output
- Copy all CSV data

**4. Restore original main:**
```cmake
# Uncomment multitasking main
mcux_add_source(SOURCES app/main_sensor.c ...)
```

### Option 2: Create Separate Build Target

Create `CMakeLists_dataset.txt`:
```cmake
# Dataset generator build configuration
mcux_add_source(
    SOURCES app/main_dataset_generator.c 
            app/dataset_generator.c 
            app/feature_logger.c 
            dsp/fft_engine.c 
            dsp/feature_extractor.c
)
```

---

## Expected Output

### CSV Header
```
sample_id,rms,mean,variance,std_dev,energy,peak_magnitude,peak_frequency,label
```

### Sample Data
```
0,0.7071,0.0234,0.2500,0.5000,125.5432,45.2345,10.1234,NORMAL
1,0.7234,0.0187,0.2678,0.5175,132.4567,46.7890,9.8765,NORMAL
...
100,1.2345,0.0456,0.8765,0.9362,456.7890,65.4321,11.2345,WARNING
101,1.3456,0.0523,0.9234,0.9609,487.6543,68.9012,10.7654,WARNING
...
200,1.8765,0.0678,1.5678,1.2521,876.5432,89.1234,12.3456,FAULT
201,1.9876,0.0745,1.6789,1.2956,923.4567,92.4567,11.8765,FAULT
...
```

### Summary
```
========================================
  DATASET GENERATION COMPLETE
========================================
Total Samples:   300
  NORMAL:        100
  WARNING:       100
  FAULT:         100
========================================
```

---

## Dataset Processing

### 1. Save CSV File

**Copy from serial terminal:**
```bash
# Select all CSV output (header + 300 rows)
# Save as: training_dataset.csv
```

**Verify:**
```bash
# Check line count (should be 301: header + 300 samples)
wc -l training_dataset.csv
```

### 2. Split by Class (Optional)

**Python script:**
```python
import pandas as pd

# Read dataset
df = pd.read_csv('training_dataset.csv')

# Split by label
df[df['label'] == 'NORMAL'].to_csv('normal.csv', index=False)
df[df['label'] == 'WARNING'].to_csv('warning.csv', index=False)
df[df['label'] == 'FAULT'].to_csv('fault.csv', index=False)

print(f"NORMAL: {len(df[df['label'] == 'NORMAL'])} samples")
print(f"WARNING: {len(df[df['label'] == 'WARNING'])} samples")
print(f"FAULT: {len(df[df['label'] == 'FAULT'])} samples")
```

**Bash script:**
```bash
# Extract header
head -1 training_dataset.csv > header.csv

# Split by label
grep ",NORMAL$" training_dataset.csv > normal_data.csv
grep ",WARNING$" training_dataset.csv > warning_data.csv
grep ",FAULT$" training_dataset.csv > fault_data.csv

# Add headers
cat header.csv normal_data.csv > normal.csv
cat header.csv warning_data.csv > warning.csv
cat header.csv fault_data.csv > fault.csv
```

---

## Edge Impulse Integration

### 1. Create Project

1. Go to [Edge Impulse](https://edgeimpulse.com/)
2. Create new project: "Edge AI Vibration Classification"
3. Select "Accelerometer data" as sensor type

### 2. Upload Dataset

**Data Acquisition → Upload data:**
- Upload `training_dataset.csv`
- Select "Automatically split data" (80/20 train/test)
- Label column: `label`
- Features: `rms, mean, variance, std_dev, energy, peak_magnitude, peak_frequency`

### 3. Create Impulse

**Impulse Design:**
- **Processing block:** Raw Data (no preprocessing needed)
- **Learning block:** Classification (Keras)
- **Output features:** 3 classes (NORMAL, WARNING, FAULT)

### 4. Configure Neural Network

**NN Classifier settings:**
```
Number of training cycles: 100
Learning rate: 0.001
Architecture:
  - Dense layer: 20 neurons, ReLU
  - Dropout: 0.2
  - Dense layer: 10 neurons, ReLU
  - Dense layer: 3 neurons, Softmax
```

### 5. Train Model

- Click "Start training"
- Wait for training to complete
- Check accuracy (target: >90%)
- Review confusion matrix

### 6. Export Model

**Deployment:**
- Select "C++ library"
- Choose "TensorFlow Lite"
- Download `.zip` file
- Extract to project folder

---

## TensorFlow Lite Micro Integration

### 1. Download Model Files

From Edge Impulse deployment:
```
edge-impulse-sdk/
  ├── classifier/
  ├── dsp/
  ├── porting/
  └── tensorflow/
      └── lite/
          └── micro/
tflite-model/
  └── trained_model_compiled.cpp
```

### 2. Add to Project

Copy to your project:
```
RTOS_Based_Edge_AI_Anomaly_Detection_System_using_MCXN947/
  ├── tinyml/
  │   ├── edge-impulse-sdk/
  │   └── trained_model_compiled.cpp
  └── CMakeLists.txt (update to include TinyML files)
```

### 3. Update CMakeLists.txt

```cmake
# Add TinyML sources
mcux_add_source(
    SOURCES app/main_sensor.c 
            app/ai_classifier.c 
            app/tinyml_classifier.c      # NEW
            tinyml/trained_model_compiled.cpp  # NEW
            # ... other sources
)

# Add TinyML includes
mcux_add_include(
    INCLUDES . include app tinyml tinyml/edge-impulse-sdk
)
```

### 4. Create TinyML Classifier Wrapper

**app/tinyml_classifier.c:**
```c
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

machine_state_t TinyML_Classify(const feature_vector_t *features)
{
    // Prepare input tensor
    float input[7] = {
        features->rms,
        features->mean,
        features->variance,
        features->std_dev,
        features->energy,
        features->peak_magnitude,
        features->peak_frequency
    };
    
    // Run inference
    ei_impulse_result_t result;
    run_classifier(input, &result);
    
    // Get prediction
    if (result.classification[0].value > 0.7) return MACHINE_NORMAL;
    if (result.classification[1].value > 0.7) return MACHINE_WARNING;
    if (result.classification[2].value > 0.7) return MACHINE_FAULT;
    
    return MACHINE_NORMAL;  // Default
}
```

### 5. Update AITask

**app/task_ai.c:**
```c
// Replace rule-based classifier with TinyML
// OLD: machine_state = AI_Classifier_Predict(&features_vector);
// NEW:
machine_state = TinyML_Classify(&features_vector);
```

---

## Verification

### 1. Dataset Quality Check

**Python verification:**
```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('training_dataset.csv')

# Check class distribution
print(df['label'].value_counts())

# Check feature ranges
print(df.describe())

# Visualize features by class
for feature in ['rms', 'energy', 'peak_frequency']:
    df.boxplot(column=feature, by='label')
    plt.show()
```

**Expected:**
- Balanced classes (100 each)
- Clear feature separation between classes
- No NaN or infinite values

### 2. Model Performance

**Metrics to monitor:**
- Training accuracy: >90%
- Validation accuracy: >85%
- Inference time: <10 ms
- Model size: <50 KB
- RAM usage: <20 KB

### 3. Real-Time Testing

After TinyML integration:
1. Flash firmware with TinyML classifier
2. Monitor serial output
3. Verify classifications match expectations
4. Compare TinyML vs rule-based accuracy

---

## Troubleshooting

### Issue: Low Model Accuracy

**Solutions:**
- Increase training samples (200-500 per class)
- Add more signal variation
- Increase NN complexity
- Normalize feature values
- Balance class distribution

### Issue: Large Model Size

**Solutions:**
- Reduce NN layers/neurons
- Use quantization (int8)
- Remove unused features
- Use simpler architecture

### Issue: Slow Inference

**Solutions:**
- Enable hardware acceleration
- Reduce model complexity
- Optimize tensor operations
- Use quantized models

---

## Next Steps

1. **Generate Dataset:**
   - Flash dataset generator
   - Capture CSV output
   - Save as `training_dataset.csv`

2. **Train Model:**
   - Upload to Edge Impulse
   - Configure neural network
   - Train and validate
   - Export TensorFlow Lite model

3. **Integrate TinyML:**
   - Add Edge Impulse SDK
   - Create TinyML wrapper
   - Update AITask
   - Test on hardware

4. **Validate:**
   - Compare TinyML vs rule-based
   - Measure inference time
   - Verify accuracy
   - Optimize if needed

---

## Files Summary

| File | Purpose | Status |
|------|---------|--------|
| **dataset_generator.c/h** | Signal generation | ✅ Complete |
| **feature_logger.c/h** | CSV export | ✅ Complete |
| **main_dataset_generator.c** | Dataset generation app | ✅ Complete |
| **training_dataset.csv** | Generated by running app | ⏳ Pending |
| **normal.csv** | NORMAL samples only | ⏳ Pending |
| **warning.csv** | WARNING samples only | ⏳ Pending |
| **fault.csv** | FAULT samples only | ⏳ Pending |

---

**Status:** Dataset generation tools complete. Ready to build and run.

**Next Action:** Build dataset generator, flash to board, capture CSV output.
