# MPU6050 Bring-Up Build Script
# Copyright 2026 NXP

param(
    [string]$Action = "build"
)

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MPU6050 Bring-Up Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ProjectRoot = $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "debug"

function Configure-Project {
    Write-Host "[1/2] Configuring CMake..." -ForegroundColor Yellow
    
    if (Test-Path $BuildDir) {
        Write-Host "  Build directory exists: $BuildDir" -ForegroundColor Gray
    } else {
        Write-Host "  Creating build directory: $BuildDir" -ForegroundColor Gray
    }
    
    Push-Location $ProjectRoot
    try {
        cmake --preset=debug
        if ($LASTEXITCODE -ne 0) {
            Write-Host "✗ CMake configuration failed" -ForegroundColor Red
            exit 1
        }
        Write-Host "✓ CMake configured successfully" -ForegroundColor Green
    }
    finally {
        Pop-Location
    }
}

function Build-Project {
    Write-Host "[2/2] Building project..." -ForegroundColor Yellow
    
    Push-Location $ProjectRoot
    try {
        cmake --build debug
        if ($LASTEXITCODE -ne 0) {
            Write-Host "✗ Build failed" -ForegroundColor Red
            exit 1
        }
        Write-Host "✓ Build completed successfully" -ForegroundColor Green
    }
    finally {
        Pop-Location
    }
}

function Clean-Project {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
        Write-Host "✓ Build directory cleaned" -ForegroundColor Green
    } else {
        Write-Host "Build directory does not exist" -ForegroundColor Gray
    }
}

function Show-BinaryInfo {
    $BinaryPath = Join-Path $BuildDir "edge_ai_vibration.bin"
    
    if (Test-Path $BinaryPath) {
        $FileInfo = Get-Item $BinaryPath
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Cyan
        Write-Host "  Build Output" -ForegroundColor Cyan
        Write-Host "========================================" -ForegroundColor Cyan
        Write-Host "Binary: $BinaryPath" -ForegroundColor White
        Write-Host "Size:   $($FileInfo.Length) bytes" -ForegroundColor White
        Write-Host "Modified: $($FileInfo.LastWriteTime)" -ForegroundColor White
        Write-Host ""
        Write-Host "Next Steps:" -ForegroundColor Yellow
        Write-Host "  1. Connect FRDM-MCXN947 to your PC" -ForegroundColor Gray
        Write-Host "  2. Flash using MCUXpresso IDE or command-line tools" -ForegroundColor Gray
        Write-Host "  3. Open serial terminal at 115200 baud" -ForegroundColor Gray
        Write-Host "  4. Reset the board to see output" -ForegroundColor Gray
        Write-Host ""
    } else {
        Write-Host ""
        Write-Host "Binary not found: $BinaryPath" -ForegroundColor Red
        Write-Host ""
    }
}

function Show-Help {
    Write-Host "Usage: .\build.ps1 [action]" -ForegroundColor White
    Write-Host ""
    Write-Host "Actions:" -ForegroundColor Yellow
    Write-Host "  build     - Configure and build (default)" -ForegroundColor Gray
    Write-Host "  clean     - Remove build directory" -ForegroundColor Gray
    Write-Host "  rebuild   - Clean and build" -ForegroundColor Gray
    Write-Host "  help      - Show this help" -ForegroundColor Gray
    Write-Host ""
}

# Main execution
switch ($Action.ToLower()) {
    "build" {
        Configure-Project
        Build-Project
        Show-BinaryInfo
    }
    "clean" {
        Clean-Project
    }
    "rebuild" {
        Clean-Project
        Configure-Project
        Build-Project
        Show-BinaryInfo
    }
    "help" {
        Show-Help
    }
    default {
        Write-Host "Unknown action: $Action" -ForegroundColor Red
        Show-Help
        exit 1
    }
}
