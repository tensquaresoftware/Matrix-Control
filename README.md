# Matrix-Control

Version: 0.0.0  
Manufacturer: Ten Square Software  
Formats: AU VST3 Standalone (macOS) | VST3 Standalone (Windows)

## Build Instructions

### Prerequisites

#### macOS

- macOS Tahoe or later
- Cursor 2
- CMake 3.22+
- Ninja build system
- JUCE 8 installed at `/Applications/JUCE`

#### Windows

- Windows 11
- Cursor 2
- CMake 3.22+
- Visual Studio 2022 (Community, Professional, or Enterprise) with:
  - Desktop development with C++ workload
  - Windows 10/11 SDK
- JUCE 8 installed (e.g., at `C:\JUCE`)

### Build

#### macOS

```bash
# Configure
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Build all formats
cmake --build build --config Debug

# Or build specific format
cmake --build build --target Matrix-Control_Standalone --config Debug
cmake --build build --target Matrix-Control_AU --config Debug
cmake --build build --target Matrix-Control_VST3 --config Debug
```

#### Windows

**1. Install Visual Studio 2022**

Download and install Visual Studio 2022 from [visualstudio.com](https://visualstudio.microsoft.com/). During installation, select:
- Desktop development with C++ workload
- Windows 10/11 SDK (latest version)

**2. Install JUCE**

- Download JUCE 8.0.10 from [juce.com](https://juce.com/get-juce)
- Extract it to a location like `C:\JUCE`
- Set the environment variable `JUCE_DIR` to point to this location:
  ```powershell
  # In PowerShell (run as Administrator for system-wide setting)
  [System.Environment]::SetEnvironmentVariable("JUCE_DIR", "C:\JUCE", "User")
  ```
  Or set it temporarily in your current session:
  ```powershell
  $env:JUCE_DIR = "C:\JUCE"
  ```

**3. Configure VS Code for Windows**

Copy the example configuration files:
```powershell
# In the project root directory
Copy-Item .vscode\settings-windows.json.example .vscode\settings-windows.json
Copy-Item .vscode\tasks-windows.json.example .vscode\tasks-windows.json
```

Edit `.vscode/settings-windows.json` and update the `JUCE_DIR` path if your JUCE installation is in a different location.

**4. Build**

```powershell
# Configure (using Visual Studio generator)
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug

# Build all formats
cmake --build build --config Debug

# Or build specific format
cmake --build build --target Matrix-Control_Standalone --config Debug
cmake --build build --target Matrix-Control_VST3 --config Debug
```

**Note:** On Windows, only VST3 and Standalone formats are built (AU is macOS-only).

### Plugin Installation

#### macOS

- **VST3**: Copy to `~/Library/Audio/Plug-Ins/VST3/`
- **AU**: Copy to `~/Library/Audio/Plug-Ins/Components/`
- **Standalone**: Copy `.app` to Applications folder

#### Windows

- **VST3**: Copy to `C:\Program Files\Common Files\VST3\` (or `%COMMONPROGRAMFILES%\VST3\`)
- **Standalone**: Copy `.exe` to desired location