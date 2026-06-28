# PART 11 - TinyML Integration Build Guide

## Overview

This guide walks you through building and verifying the complete end-to-end TinyML integration on the FRDM-MCXN947 board.

**Pipeline Flow:**
```
SignalTask → FFTTask → FeatureTask → TinyMLTask → UART Output
```

## Prerequisites

✓ FRDM-MCXN947 board connected  
✓ MCUXpresso IDE or VSCode with CMake Tools  
✓ ARM GCC toolchain installed  
✓ TensorFlow Lite Micro library integrated  
✓ Trained model (model_data.cc/h) present in ml/ folder  

## Build Configuration

### Current CMakeLists.txt Configuration

The project is configured to build the TinyML pipeline with the following source files:

**Application Files:**
- `app/main_tinyml.c` - Main entry point with TinyML pipeline
- `app/task_scheduler.c` - Bare-metal task scheduler
- `app/task_signal.c` - Signal generation task
- `app/task_fft.c` - FFT processing task
- `app/task_feature.c` - Feature extraction task

**DSP Files:**
- `dsp/signal_generator.c` - Synthetic vibration data generator
- `dsp/fft_engine.c` - FFT implementation
- `dsp/feature_extraction.c` - Feature extraction core
- `dsp/feature_extractor.c` - Feature vector builder

**TinyML Files:**
- `ml/tinyml_task.c` - TinyML inference task
- `ml/tinyml_engine.c` - TensorFlow Lite wrapper
- `ml/model_data.cc` - Trained neural network model

## Build Steps

### Method 1: VSCode with CMake Tools

1. **Clean previous build:**
   ```powershell
   cd debug
   Remove-Item CMakeCache.txt -Force -ErrorAction SilentlyContinue
   Remove-Item CMakeFiles -Recurse -Force -ErrorAction SilentlyContinue
   cd ..
   ```

2. **Configure CMake:**
   - Press `Ctrl+Shift+P`
   - Select "CMake: Configure"
   - Wait for configuration to complete

3. **Build project:**
   - Press `Ctrl+Shift+P`
   - Select "CMake: Build"
   - Or use the build task: `Ctrl+Shift+B`

4. **Verify build output:**
   ```
   [58/58] Linking C executable edge_ai_vibration_cm33_core0.elf
   Memory region         Used Size  Region Size  %age Used
       m_interrupts:         688 B         1 KB     67.19%
             m_text:      XXX KB        767 KB     XX.XX%
            m_data:        XXX B        312 KB      X.XX%
   ```

### Method 2: Command Line Build

```powershell
# Navigate to project root
cd C:\Users\A544483\Desktop\T3\RTOS_Based_Edge_AI_Anomaly_Detection_System_using_MCXN947

# Clean build
.\build.ps1 clean

# Configure and build
cmake --preset debug
cmake --build debug --target all
```

## Flash and Run

### Flash the Binary

1. **Using LinkServer (recommended):**
   - Connect FRDM-MCXN947 via USB
   - Press `F5` in VSCode (Debug mode)
   - Or use "Run → Start Debugging"

2. **Using command line:**
   ```powershell
   LinkServer flash edge_ai_vibration_cm33_core0.elf
   ```

### Open Serial Terminal

1. **Find COM port:**
   - Open Device Manager
   - Look under "Ports (COM & LPT)"
   - Note the COM port (e.g., COM3)

2. **Connect to serial:**
   - Baud rate: 115200
   - Data bits: 8
   - Parity: None
   - Stop bits: 1

## Expected Output

### Initialization Phase

```
╔══════════════════════════════════════════════╗
║  PART 11: End-to-End TinyML Integration     ║
║  Edge AI Predictive Maintenance System      ║
╚══════════════════════════════════════════════╝

Target:   FRDM-MCXN947
Core:     Cortex-M33 @ CM33_CORE0
Scheduler: Bare-Metal (Custom RTOS)
Mode:     Synthetic Vibration Data

Pipeline:
  1. SignalTask    → Generate sensor data
  2. FFTTask       → Frequency analysis
  3. FeatureTask   → Extract 7 features
  4. TinyMLTask    → Neural network inference
  5. UART Output   → Display prediction

════════════════════════════════════════════════

Initializing Task Scheduler...
════════════════════════════════════════════════
[1/3] Creating inter-task queues...
      ✓ Signal Queue:  2 slots × XX bytes
      ✓ FFT Queue:     2 slots × XX bytes
      ✓ Feature Queue: 2 slots × XX bytes

[2/3] Registering pipeline tasks...
      ✓ SignalTask    (Period: 100ms, Priority: 1)
      ✓ FFTTask       (Period:  10ms, Priority: 2)
      ✓ FeatureTask   (Period:  10ms, Priority: 2)
      ✓ TinyMLTask    (Period:  10ms, Priority: 2)

[3/3] Starting system timer...
      ✓ SysTick configured (1ms tick)

════════════════════════════════════════════════
All systems initialized successfully!
Starting scheduler...
════════════════════════════════════════════════
```

### TinyML Task Initialization

```
========================================
  TinyML Inference Task Starting
========================================
[TinyML] Initializing inference engine...
[TinyML] Model loaded (version 3)
[TinyML] Model size: XXXX bytes
[TinyML] Operations registered
[TinyML] Tensor arena allocated: 16384 bytes
[TinyML] Input tensor: type=X, bytes=32
[TinyML] Output tensor: type=X, bytes=12
[TinyML] Arena memory used: XXXX / 16384 bytes (XX.X%)
[TinyML] Initialization complete!

[TinyMLTask] Ready for inference
[TinyMLTask] Model: 8 inputs → 3 outputs
[TinyMLTask] Memory: XXXX bytes
========================================
```

### Inference Results (Example)

```
[SignalTask] Generated signal (type=NORMAL, timestamp=1000)
[SignalTask] → Sent to FFTTask

[FFTTask] ← Received from SignalTask
[FFTTask] FFT processing complete
[FFTTask] Peak: Magnitude = 85.32, Frequency = 39.06 Hz
[FFTTask] → Sent to FeatureTask

[FeatureTask] ← Received from FFTTask
[FeatureTask] RMS = 0.4523 | Energy = 52.3456
[FeatureTask] → Sent to TinyMLTask (7 features)

[TinyMLTask] ← Received from FeatureTask
[TinyMLTask] Inference Cycle 1

╔════════════════════════════════════════╗
║        TinyML PREDICTION RESULT        ║
╠════════════════════════════════════════╣
║ Prediction = NORMAL                    ║
║ Confidence = 95.32%                    ║
╠════════════════════════════════════════╣
║ Class Probabilities:                   ║
║   NORMAL:  95.32%                      ║
║   WARNING:  3.45%                      ║
║   FAULT:    1.23%                      ║
╚════════════════════════════════════════╝

[TinyMLTask] Input Features:
  RMS:             0.4523
  Mean:            0.0012
  Variance:        0.2045
  Std Dev:         0.4522
  Energy:          52.3456
  Peak Magnitude:  85.3214
  Peak Frequency:  39.0625 Hz

[TinyMLTask] Statistics:
  Total Inferences: 1
  Errors:           0
  Success Rate:     100.00%
========================================
TinyML Pipeline Cycle 1 Complete
========================================
```

## Verification Checklist

### ✓ Task Creation
- [ ] SignalTask created successfully
- [ ] FFTTask created successfully
- [ ] FeatureTask created successfully
- [ ] TinyMLTask created successfully

### ✓ Queue Operation
- [ ] Signal queue working
- [ ] FFT queue working
- [ ] Feature queue working
- [ ] No queue overflow errors

### ✓ TinyML Inference
- [ ] TensorFlow Lite model loaded
- [ ] Tensor allocation successful
- [ ] Inference running without errors
- [ ] Predictions displayed on UART

### ✓ Pipeline Flow
- [ ] SignalTask generates data
- [ ] FFTTask receives and processes
- [ ] FeatureTask extracts 7 features
- [ ] TinyMLTask receives features
- [ ] Predictions match expected classes

## Troubleshooting

### Build Errors

**Error: "model_data.h not found"**
- Ensure ml/model_data.h and ml/model_data.cc exist
- Check include paths in CMakeLists.txt

**Error: TensorFlow Lite headers missing**
- Verify TFLM library is integrated in SDK
- Check include paths for tensorflow/ directory

**Linker errors with C++ functions**
- Ensure model_data.cc is compiled as C++
- Check that .cc files are in mcux_add_source()

### Runtime Errors

**TinyML initialization failed**
- Check model schema version matches TFLite version
- Verify tensor arena size (16KB) is sufficient
- Review registered operations match model requirements

**Queue send/receive failures**
- Increase queue sizes in rtos_tasks.h
- Check task priorities and periods
- Verify all queues are created before tasks start

**Incorrect predictions**
- Validate feature extraction matches training data
- Check input normalization/scaling
- Verify model was exported correctly from training

### Performance Issues

**Slow inference**
- Check model complexity (number of layers/parameters)
- Consider quantization (int8 vs float32)
- Profile task execution times

**High memory usage**
- Reduce TINYML_TENSOR_ARENA_SIZE if possible
- Check for memory leaks in tasks
- Monitor stack usage per task

## Switching Build Configurations

### Back to Dataset Generator

Edit CMakeLists.txt:
```cmake
mcux_add_source(
    SOURCES 
    app/main_dataset_generator.c 
    app/dataset_generator.c 
    app/feature_logger.c 
    dsp/fft_engine.c 
    dsp/feature_extraction.c 
    dsp/feature_extractor.c
)
```

Then reconfigure and rebuild.

### To Standalone TinyML Test

Edit CMakeLists.txt:
```cmake
mcux_add_source(
    SOURCES 
    app/main_tinyml_test.c 
    ml/tinyml_engine.c 
    ml/model_data.cc
    dsp/feature_extractor.c
)
```

Then reconfigure and rebuild.

## Next Steps

1. **Model Training:**
   - Use generated dataset (Part 9-10) to train model
   - Export to TensorFlow Lite format
   - Convert to C array using xxd or TFLite converter
   - Replace ml/model_data.cc

2. **Real Sensor Integration:**
   - Replace SignalTask with MPU6050 sensor reading
   - Update signal_generator.c for real accelerometer data
   - Calibrate feature extraction thresholds

3. **Production Deployment:**
   - Optimize model for embedded inference
   - Add error handling and recovery
   - Implement data logging/telemetry
   - Add over-the-air (OTA) update capability

## Files Modified in Part 11

1. `app/rtos_tasks.h` - Added TinyMLTask, expanded feature_msg_t
2. `app/task_feature.c` - Sends all 7 features
3. `ml/tinyml_task.c` - Adapted for bare-metal scheduler
4. `app/main_tinyml.c` - New main file with TinyML integration
5. `CMakeLists.txt` - Updated build configuration

## Success Criteria

✅ Build completes without errors  
✅ All 4 tasks created successfully  
✅ TinyML model initializes correctly  
✅ Feature extraction produces 7 valid features  
✅ Inference runs and displays predictions  
✅ UART output shows confidence scores  
✅ Pipeline cycles continuously  

**If all criteria met: PART 11 COMPLETE! 🎉**
