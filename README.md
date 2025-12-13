# Matrix-Control

Version: 0.0.0  
Manufacturer: Ten Square Software  
Formats: AU VST3 Standalone (macOS) / VST3 Standalone (Windows)

## Build Instructions

### Prerequisites

#### macOS
- macOS Tahoe or later
- Cursor 2
- CMake 3.22+
- Ninja build system
- JUCE 8 installed (set `JUCE_DIR` environment variable to your JUCE installation path, e.g., `/Applications/JUCE`)

#### Windows
- Windows 11 or later
- Cursor 2
- CMake 3.22+ (add to system PATH during installation)
- Visual Studio 2022 with "Desktop development with C++" workload
- JUCE 8 installed (set `JUCE_DIR` environment variable to your JUCE installation path, e.g., `C:\JUCE`)

### Environment Setup

Set the `JUCE_DIR` environment variable to point to your JUCE installation:

**macOS:**
```bash
export JUCE_DIR=/Applications/JUCE
```

**Windows:**
```powershell
# System environment variable (recommended)
# Set via: System Properties → Environment Variables → System variables
# Name: JUCE_DIR
# Value: C:\JUCE
```

### Build

#### macOS

```bash
# Configure (using Ninja)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Build all formats
cmake --build build --config Debug

# Or build specific format
cmake --build build --target Matrix-Control_Standalone --config Debug
cmake --build build --target Matrix-Control_AU --config Debug
cmake --build build --target Matrix-Control_VST3 --config Debug
```

#### Windows

```powershell
# Configure (using Visual Studio 2022)
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug

# Build all formats
cmake --build build --config Debug

# Or build specific format
cmake --build build --target Matrix-Control_Standalone --config Debug
cmake --build build --target Matrix-Control_VST3 --config Debug
```

**Note:** Audio Unit (AU) format is only available on macOS. On Windows, only VST3 and Standalone formats are built.

### Using Cursor IDE

1. Open the project folder in Cursor
2. CMake Tools extension will automatically detect the project
3. Select your build kit (CMake Tools will prompt you)
4. Use the build tasks:
   - `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) → "Tasks: Run Task"
   - Select "Build: All" or a specific format

### Plugin Installation

#### macOS
- **AU**: Copy `.component` file to `~/Library/Audio/Plug-Ins/Components/`
- **VST3**: Copy `.vst3` bundle to `~/Library/Audio/Plug-Ins/VST3/`
- **Standalone**: Run the `.app` directly

#### Windows
- **VST3**: Copy `.vst3` folder to `C:\Program Files\Common Files\VST3\`
- **Standalone**: Run the `.exe` directly

### Debugging

Debug configurations are available in `.vscode/launch.json`:
- **macOS**: Standalone, AU in Logic Pro, VST3 in Reaper, AU in Ableton Live
- **Windows**: Standalone, VST3 in Reaper

Press `F5` in Cursor to start debugging.
