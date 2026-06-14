# Quick Build Guide - TinyML Dataset Generator

## Step 1: Backup Current CMakeLists.txt
```bash
Copy-Item CMakeLists.txt CMakeLists.txt.backup
```

## Step 2: Create Dataset Generator Build

Edit `CMakeLists.txt` - Replace the mcux_add_source section:

### BEFORE (Multitasking Pipeline):
```cmake
mcux_add_source(
    SOURCES app/main_sensor.c app/ai_classifier.c app/task_scheduler.c 
            app/task_signal.c app/task_fft.c app/task_feature.c app/task_ai.c 
            dsp/signal_generator.c dsp/fft_engine.c dsp/feature_extraction.c 
            dsp/feature_extractor.c
)
```

### AFTER (Dataset Generator):
```cmake
mcux_add_source(
    SOURCES app/main_dataset_generator.c app/dataset_generator.c 
            app/feature_logger.c dsp/fft_engine.c dsp/feature_extractor.c
)
```

## Step 3: Build
```bash
Remove-Item debug\CMakeCache.txt,debug\build.ninja -ErrorAction SilentlyContinue
cmake --preset=debug
cmake --build debug
```

## Step 4: Flash & Capture

1. **Flash firmware:**
   - Press F5 in VS Code
   - OR use: `linkserver flash debug/edge_ai_vibration_cm33_core0.elf`

2. **Open serial terminal** (115200 baud)

3. **Wait for output** (~30 seconds for 300 samples)

4. **Copy CSV data:**
   - Select all CSV output (header + 300 rows)
   - Save to file: `training_dataset.csv`

## Step 5: Restore Multitasking Build
```bash
Copy-Item CMakeLists.txt.backup CMakeLists.txt
cmake --preset=debug
cmake --build debug
```

---

## Expected Serial Output

```
================================================
  TinyML Training Dataset Generator
  Edge AI Predictive Maintenance System
================================================
Board:          FRDM-MCXN947
Core:           Cortex-M33 @ CM33_CORE0
Samples/Class:  100
Total Samples:  300
Features:       7
Labels:         3 (NORMAL, WARNING, FAULT)
================================================

Generating training dataset...

sample_id,rms,mean,variance,std_dev,energy,peak_magnitude,peak_frequency,label
0,0.7071,0.0234,0.2500,0.5000,125.5432,45.2345,10.1234,NORMAL
1,0.7234,0.0187,0.2678,0.5175,132.4567,46.7890,9.8765,NORMAL
...
[300 rows total]
...
========================================
  DATASET GENERATION COMPLETE
========================================
Total Samples:   300
  NORMAL:        100
  WARNING:       100
  FAULT:         100
========================================

Next Steps:
1. Copy CSV data from serial terminal
2. Save as 'training_dataset.csv'
3. Split into separate files...
```

---

## Verify Dataset

```bash
# Check file exists and has correct size
Get-Item training_dataset.csv

# Count lines (should be 301: header + 300 samples)
(Get-Content training_dataset.csv).Count

# Check first few lines
Get-Content training_dataset.csv -Head 5

# Check last few lines
Get-Content training_dataset.csv -Tail 5
```

Expected:
- File size: ~20-30 KB
- Lines: 301
- Format: CSV with 9 columns
- Labels: NORMAL, WARNING, FAULT

---

## Split Dataset (Optional)

### Using PowerShell:
```powershell
# Read the file
$data = Get-Content training_dataset.csv

# Get header
$header = $data[0]

# Filter by label
$normal = $data | Where-Object { $_ -match ",NORMAL$" }
$warning = $data | Where-Object { $_ -match ",WARNING$" }
$fault = $data | Where-Object { $_ -match ",FAULT$" }

# Write separate files
@($header) + $normal | Out-File normal.csv -Encoding utf8
@($header) + $warning | Out-File warning.csv -Encoding utf8
@($header) + $fault | Out-File fault.csv -Encoding utf8

# Verify
Write-Host "NORMAL: $($normal.Count) samples"
Write-Host "WARNING: $($warning.Count) samples"
Write-Host "FAULT: $($fault.Count) samples"
```

### Using Python:
```python
import pandas as pd

df = pd.read_csv('training_dataset.csv')

df[df['label'] == 'NORMAL'].to_csv('normal.csv', index=False)
df[df['label'] == 'WARNING'].to_csv('warning.csv', index=False)
df[df['label'] == 'FAULT'].to_csv('fault.csv', index=False)

print(f"NORMAL: {len(df[df['label'] == 'NORMAL'])}")
print(f"WARNING: {len(df[df['label'] == 'WARNING'])}")
print(f"FAULT: {len(df[df['label'] == 'FAULT'])}")
```

---

## Troubleshooting

**Build Error: "undefined reference"**
- Make sure you only included dataset generator sources
- Check that main_sensor.c is NOT in the source list

**Serial Output Truncated**
- Increase terminal buffer size
- Use log-to-file feature in terminal
- OR capture with: `mode COM3 BAUD=115200 > output.txt`

**Wrong Number of Samples**
- Check if board reset during generation
- Verify SAMPLES_PER_CLASS = 100 in dataset_generator.h
- Re-flash and try again

**CSV Format Issues**
- Verify print_float_4() function works correctly
- Check for missing commas or extra characters
- Validate CSV with: `python -c "import pandas; pandas.read_csv('training_dataset.csv')"`

---

## Quick Reference

| Task | Command |
|------|---------|
| **Build dataset generator** | `cmake --build debug` |
| **Flash firmware** | F5 in VS Code |
| **Open terminal** | 115200 baud |
| **Save CSV** | Copy from terminal |
| **Restore multitasking** | Restore CMakeLists.txt.backup |
| **Verify dataset** | `Get-Content training_dataset.csv \| Measure-Object -Line` |

---

**Ready!** Now you can generate your TinyML training dataset.
