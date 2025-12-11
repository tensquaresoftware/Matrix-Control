# Setup script for Windows development environment
# Run this script in PowerShell from the project root directory

Write-Host "Matrix-Control - Windows Setup Script" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# Check if CMake is installed
Write-Host "Checking for CMake..." -ForegroundColor Yellow
$cmakeVersion = cmake --version 2>$null
if ($cmakeVersion) {
    Write-Host "✓ CMake found" -ForegroundColor Green
    cmake --version | Select-Object -First 1
} else {
    Write-Host "✗ CMake not found. Please install CMake 3.22+ from https://cmake.org/download/" -ForegroundColor Red
    exit 1
}

Write-Host ""

# Check if Visual Studio is installed
Write-Host "Checking for Visual Studio 2022..." -ForegroundColor Yellow
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community"
if (Test-Path $vsPath) {
    Write-Host "✓ Visual Studio 2022 Community found" -ForegroundColor Green
} else {
    $vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Professional"
    if (Test-Path $vsPath) {
        Write-Host "✓ Visual Studio 2022 Professional found" -ForegroundColor Green
    } else {
        $vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
        if (Test-Path $vsPath) {
            Write-Host "✓ Visual Studio 2022 Enterprise found" -ForegroundColor Green
        } else {
            Write-Host "✗ Visual Studio 2022 not found. Please install from https://visualstudio.microsoft.com/" -ForegroundColor Red
            Write-Host "  Make sure to install 'Desktop development with C++' workload" -ForegroundColor Yellow
            exit 1
        }
    }
}

Write-Host ""

# Check JUCE_DIR environment variable
Write-Host "Checking JUCE installation..." -ForegroundColor Yellow
$juceDir = $env:JUCE_DIR
if (-not $juceDir) {
    Write-Host "⚠ JUCE_DIR environment variable not set" -ForegroundColor Yellow
    Write-Host "Please set it to your JUCE installation path:" -ForegroundColor Yellow
    Write-Host '  [System.Environment]::SetEnvironmentVariable("JUCE_DIR", "C:\JUCE", "User")' -ForegroundColor Cyan
    Write-Host ""
    $jucePath = Read-Host "Enter JUCE installation path (or press Enter to skip)"
    if ($jucePath -and (Test-Path $jucePath)) {
        $env:JUCE_DIR = $jucePath
        Write-Host "✓ JUCE_DIR set to: $jucePath" -ForegroundColor Green
    } else {
        Write-Host "✗ JUCE not found. Please install JUCE 8.0.10 from https://juce.com/get-juce" -ForegroundColor Red
        exit 1
    }
} else {
    if (Test-Path $juceDir) {
        Write-Host "✓ JUCE_DIR found: $juceDir" -ForegroundColor Green
    } else {
        Write-Host "✗ JUCE_DIR points to invalid path: $juceDir" -ForegroundColor Red
        exit 1
    }
}

Write-Host ""

# Copy VS Code configuration files
Write-Host "Setting up VS Code configuration..." -ForegroundColor Yellow
if (Test-Path ".vscode\settings-windows.json.example") {
    if (-not (Test-Path ".vscode\settings-windows.json")) {
        Copy-Item ".vscode\settings-windows.json.example" ".vscode\settings-windows.json"
        Write-Host "✓ Created .vscode\settings-windows.json" -ForegroundColor Green
        Write-Host "  Please edit it and update JUCE_DIR path if needed" -ForegroundColor Yellow
    } else {
        Write-Host "⚠ .vscode\settings-windows.json already exists, skipping..." -ForegroundColor Yellow
    }
} else {
    Write-Host "⚠ settings-windows.json.example not found" -ForegroundColor Yellow
}

if (Test-Path ".vscode\tasks-windows.json.example") {
    if (-not (Test-Path ".vscode\tasks-windows.json")) {
        Copy-Item ".vscode\tasks-windows.json.example" ".vscode\tasks-windows.json"
        Write-Host "✓ Created .vscode\tasks-windows.json" -ForegroundColor Green
    } else {
        Write-Host "⚠ .vscode\tasks-windows.json already exists, skipping..." -ForegroundColor Yellow
    }
} else {
    Write-Host "⚠ tasks-windows.json.example not found" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Setup complete!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Edit .vscode\settings-windows.json and update JUCE_DIR if needed" -ForegroundColor White
Write-Host "2. Configure CMake: cmake -B build -G `"Visual Studio 17 2022`" -A x64" -ForegroundColor White
Write-Host "3. Build: cmake --build build --config Debug" -ForegroundColor White
