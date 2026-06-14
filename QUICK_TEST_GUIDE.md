# Quick Test Guide - AI Classifier

## Current Configuration (Test 3 - FAULT)
The firmware is currently configured to produce **FAULT** classification:
- Anomaly injection: **ENABLED**
- Anomaly amplitude: **3.0** (large spike)

## Expected Output
```
Machine State   : FAULT
RMS             : ~1.8-2.0
```

---

## How to Test Different States

### Test NORMAL Signal

**1. Edit** `include/dsp.h` **line 12:**
```c
#define SIGNAL_ENABLE_ANOMALY 0  // ← Change from 1 to 0
```

**2. Rebuild:**
```bash
cmake --build debug
```

**3. Flash and observe:**
```
Machine State   : NORMAL
RMS             : ~0.7
```

---

### Test WARNING Signal

**1. Edit** `dsp/signal_generator.c` **line 23:**
```c
signal_buffer[i] += 1.5f;  // ← Change from 3.0f to 1.5f
```

**2. Edit** `include/dsp.h` **line 12:**
```c
#define SIGNAL_ENABLE_ANOMALY 1  // ← Make sure it's enabled
```

**3. Rebuild:**
```bash
cmake --build debug
```

**4. Flash and observe:**
```
Machine State   : WARNING
RMS             : ~1.2
```

---

### Test FAULT Signal (Current Default)

**Already configured! Just flash:**
```bash
# Flash using VS Code Debug (F5)
# or
linkserver flash debug/edge_ai_vibration_cm33_core0.elf
```

**Observe:**
```
Machine State   : FAULT
RMS             : ~1.8
```

---

## Quick Flash & Test

```bash
# 1. Build (if you made changes)
cmake --build debug

# 2. Flash using VS Code
# Press F5 or Debug → Start Debugging

# 3. Open serial terminal (115200 baud)
# 4. Press RESET on board
# 5. Observe classification output
```

---

## Tuning Thresholds

Edit `app/ai_classifier.h` lines 26-31:

```c
#define RMS_WARNING_THRESHOLD       1.0f   // Adjust as needed
#define RMS_FAULT_THRESHOLD         1.5f   // Adjust as needed
#define ENERGY_WARNING_THRESHOLD    300.0f // Adjust as needed
#define ENERGY_FAULT_THRESHOLD      500.0f // Adjust as needed
```

Lower thresholds = More sensitive (more warnings/faults)  
Higher thresholds = Less sensitive (fewer warnings/faults)

---

## Troubleshooting

**Q: All outputs show FAULT**
- Signal amplitude too high
- Lower thresholds or reduce anomaly amplitude

**Q: All outputs show NORMAL**
- Signal amplitude too low
- Raise thresholds or increase anomaly amplitude

**Q: No serial output**
- Check serial terminal baud rate (115200)
- Check COM port connection
- Press RESET button on board
