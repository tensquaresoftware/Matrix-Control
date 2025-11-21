# Matrix-Control - Testing Strategy for MIDI/SysEx Layer

**Author:** Guillaume DUPONT  
**Organization:** Ten Square Software  
**Last modified:** 2025-01-XX

---

## Overview

This document describes how to test the MIDI/SysEx business layer **without GUI**, using multiple complementary approaches.

---

## 1. Unit Tests with JUCE UnitTest

### Setup

JUCE provides a built-in unit testing framework. Tests are run via the standalone application.

### Running Tests

```bash
# Build with tests enabled
cmake --build build --target Matrix-Control_Standalone

# Run tests
./build/Matrix-Control_artefacts/Debug/Standalone/Matrix-Control.app/Contents/MacOS/Matrix-Control --unit-tests
```

### Test Structure

```
Tests/
├── SysExParserTests.cpp      # Parse/validate SysEx messages
├── PatchModelTests.cpp       # Patch data model consistency
├── MidiManagerTests.cpp      # MIDI I/O, timeouts (with mocks)
├── ChecksumTests.cpp         # Oberheim checksum algorithm
└── SerializationTests.cpp    # Pack/unpack operations
```

### Example Test

```cpp
beginTest("Valid patch SysEx structure");
{
    juce::MemoryBlock validPatch = createTestPatch();
    auto result = sysExParser.parsePatchSysEx(validPatch);
    expect(result.isValid, "Valid patch should parse");
    expectEquals(result.patch.getName(), juce::String("TestPatch"));
}
```

---

## 2. MidiLogger - Detailed Logging System

### Purpose

`MidiLogger` logs all MIDI messages with timestamps, allowing you to trace exactly what's happening without GUI.

### Usage

```cpp
#include "Source/Utilities/MidiLogger.h"

// In your code
MidiLogger::getInstance().logSysExSent(sysExMessage, "Request Edit Buffer");
MidiLogger::getInstance().logSysExReceived(response, "Patch Data");
MidiLogger::getInstance().logError("Timeout waiting for synth response");
```

### Log Levels

- `kError`: Critical errors only
- `kWarning`: Warnings and errors
- `kInfo`: Informational messages (default)
- `kDebug`: Detailed MIDI messages
- `kVerbose`: Everything

### Configuration

```cpp
// Enable file logging
MidiLogger::getInstance().setLogToFile(true, 
    juce::File("~/Music/Matrix-Control/midi_log.txt"));

// Set log level
MidiLogger::getInstance().setLogLevel(MidiLogger::LogLevel::kVerbose);
```

### Log Output Example

```
[DEBUG] 14:23:45.123 - SENT SysEx (Request Edit Buffer): F0 10 06 04 04 00 F7 (7 bytes)
[DEBUG] 14:23:45.234 - RECEIVED SysEx (Patch Data): F0 10 06 01 05 ... (275 bytes)
[INFO] 14:23:45.250 - Program Change: 5
[ERROR] 14:23:47.500 - ERROR: Timeout waiting for synth response
```

---

## 3. TestMidiTool - Standalone Testing Utility

### Purpose

A command-line tool to test MIDI functionality interactively without GUI.

### Build

```bash
cmake --build build --target TestMidiTool
```

### Run

```bash
./build/Tools/TestMidiTool
```

### Features

- List available MIDI ports
- Send test SysEx messages
- Receive and log responses
- Test Device Inquiry
- Validate checksums
- Test timeout handling

### Example Session

```
=== Matrix-Control MIDI Test Tool ===

Available MIDI Input Ports:
  [0] Matrix-1000 MIDI In
  [1] LoopMIDI Port

Available MIDI Output Ports:
  [0] Matrix-1000 MIDI Out
  [1] LoopMIDI Port

Testing Device Inquiry...
  Device Inquiry sent (check logs for details)

Testing SysEx message building...
  Request Edit Buffer message built
```

---

## 4. Mock Classes for Unit Testing

### Purpose

Mock `MidiInput` and `MidiOutput` to test without real hardware.

### Example Mock

```cpp
class MockMidiOutput : public juce::MidiOutput
{
public:
    void sendMessageNow(const juce::MidiMessage& message) override
    {
        sentMessages_.add(message);
        MidiLogger::getInstance().logSysExSent(message.getSysExData(), "Mock");
    }
    
    juce::Array<juce::MidiMessage> sentMessages_;
};
```

### Usage in Tests

```cpp
beginTest("MidiManager sends SysEx correctly");
{
    MockMidiOutput mockOutput;
    MidiManager midiManager(mockOutput);
    
    midiManager.sendPatchToSynth(testPatch);
    
    expectEquals(mockOutput.sentMessages_.size(), 1, "Should send one message");
    expect(mockOutput.sentMessages_[0].isSysEx(), "Should be SysEx");
}
```

---

## 5. Integration Tests with Real Hardware

### Setup

When you have a Matrix-1000 connected:

1. Enable verbose logging
2. Run TestMidiTool
3. Send test messages
4. Verify responses in logs

### Test Checklist

- [ ] Device Inquiry → Device ID response
- [ ] Request Edit Buffer → Patch data received
- [ ] Request Master → Master data received
- [ ] Send Patch → Synth accepts (no error)
- [ ] Program Change → Patch changes on synth
- [ ] Timeout handling → Error logged after 2s

---

## 6. Test Data Files

### Location

```
Tests/TestData/
├── valid_patch_05.syx      # Valid patch from documentation
├── invalid_checksum.syx     # Patch with bad checksum
├── wrong_manufacturer.syx   # Wrong manufacturer ID
└── master_data.syx         # Master parameters example
```

### Loading Test Data

```cpp
juce::File testFile = juce::File::getCurrentWorkingDirectory()
    .getChildFile("Tests/TestData/valid_patch_05.syx");
    
juce::MemoryBlock testData;
testFile.loadFileAsData(testData);

auto result = sysExParser.parsePatchSysEx(testData);
```

---

## 7. Debugging Workflow

### Step 1: Enable Logging

```cpp
// In PluginProcessor constructor or startup
MidiLogger::getInstance().setLogLevel(MidiLogger::LogLevel::kVerbose);
MidiLogger::getInstance().setLogToFile(true);
MidiLogger::getInstance().setLogToConsole(true);
```

### Step 2: Run Your Code

Execute your MIDI operations. All messages are logged.

### Step 3: Analyze Logs

Check `~/Music/Matrix-Control/midi_log.txt` for:
- Messages sent/received
- Timestamps
- Errors
- Timeouts

### Step 4: Fix Issues

Use log information to identify problems:
- Missing F7? → Fix message building
- Wrong checksum? → Fix checksum calculation
- Timeout? → Check MIDI port configuration

---

## 8. Continuous Testing

### Automated Test Suite

Run all tests before committing:

```bash
#!/bin/bash
# run_tests.sh

echo "Running unit tests..."
./build/Matrix-Control_Standalone --unit-tests

echo "Running integration tests..."
./build/Tools/TestMidiTool --auto-test

echo "Checking log files for errors..."
grep -i "error" ~/Music/Matrix-Control/midi_log.txt
```

---

## 9. Best Practices

1. **Always log MIDI operations** - Even in production, keep error logging
2. **Use test data files** - Don't rely on real hardware for unit tests
3. **Test edge cases** - Invalid messages, timeouts, disconnections
4. **Verify checksums** - Critical for data integrity
5. **Test timeout handling** - Synth might not respond
6. **Mock external dependencies** - MIDI ports, file I/O

---

## 10. Example: Testing SysExParser

```cpp
void testSysExParser()
{
    SysExParser parser;
    
    // Test 1: Valid patch
    juce::MemoryBlock validPatch = loadTestFile("valid_patch_05.syx");
    auto result = parser.parsePatchSysEx(validPatch);
    expect(result.isValid, "Valid patch should parse");
    
    // Test 2: Invalid structure
    juce::MemoryBlock invalid = createInvalidSysEx();
    result = parser.parsePatchSysEx(invalid);
    expect(!result.isValid, "Invalid SysEx should fail");
    expect(result.errorMessage.isNotEmpty(), "Should have error message");
    
    // Test 3: Bad checksum
    juce::MemoryBlock badChecksum = createBadChecksumPatch();
    result = parser.parsePatchSysEx(badChecksum);
    expect(!result.isValid, "Bad checksum should fail");
}
```

---

## Conclusion

With these tools, you can:
- ✅ Test MIDI/SysEx without GUI
- ✅ Debug issues with detailed logs
- ✅ Verify correctness with unit tests
- ✅ Test with real hardware when available
- ✅ Catch regressions automatically

Start with **MidiLogger** for immediate feedback, then add **unit tests** as you implement each class.

---

Copyright © 2025 Ten Square Software. All rights reserved.

