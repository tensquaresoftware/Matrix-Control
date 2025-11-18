# Matrix-Control

Version: 0.0.0  
Manufacturer: Ten Square Software  
Formats: AU VST3 Standalone

## Build Instructions

### Prerequisites

- macOS Tahoe or later
- Cursor 2
- CMake 3.22+
- Ninja build system
- JUCE 8 installed at `/Applications/JUCE`

### Build

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