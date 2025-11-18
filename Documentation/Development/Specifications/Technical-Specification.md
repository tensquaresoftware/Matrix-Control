# Matrix-Control

## Technical Specification

**Author:** Guillaume DUPONT  
**Organization:** Ten Square Software  
**Last modified:** 2025-11-18  
**Version:** 1.0.0

---

## Complementary document

**Functional Specification:**

- General information, context, functional requirements
- Pure technical requirements
- Validation, scope v1.0

## 1. General architecture

### 1.1 Guiding principles

- **Single Responsibility Principle (SRP):** Each class = 1 responsibility
- **Separation of Concerns (SoC):** Business, MIDI, GUI well separated
- **Thread-safety:** APVTS ValueTree (thread-safe listeners)
- **Clean Code:** Readability, explicit names, no magic numbers, exceptions
- **Pragmatism:** Concrete classes rather than excessive polymorphism

### 1.2 Architecture diagram

```
┌──────────────────────────────────────────────────────────────────┐
│                    PluginEditor (GUI Thread)                     │
│                                                                  │
│  ┌────────────────────────────────────────────────────────────┐  │
│  │  MainEditor                                                │  │
│  │  ├─ PatchEditSection (DCO, ENV, LFO, etc.)                 │  │
│  │  ├─ MatrixModulationSection (10 buses)                     │  │
│  │  ├─ PatchManagerSection (BANK, INTERNAL, COMPUTER)         │  │
│  │  ├─ MasterEditSection (MIDI, VIBRATO, MISC)                │  │
│  │  └─ MessageBar (listen APVTS errors)                       │  │
│  └────────────────────────────────────────────────────────────┘  │
│                           ▲                                      │
│                           │ APVTS Listeners (user actions)       │
└───────────────────────────┼──────────────────────────────────────┘
                            │ (Lock-free FIFO)
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                           ▼                                      │
│       AudioProcessorValueTreeState (APVTS) - Thread-safe         │
│       ├─ 120+ Patch parameters                                   │
│       ├─ 30+ Master parameters                                   │
│       ├─ Settings (theme, ports, paths)                          │
│       └─ Listeners (GUI updates, error → MessageBar)             │
│                           ▲                                      │
└───────────────────────────┼──────────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────────┐
│                           ▼                                      │
│  PluginProcessor (Audio Thread + Orchestration)                  │
│  ├─ processBlock() : NO blocking, NO MIDI I/O                    │
│  └─ MidiThread (dedicated background thread)                     │
│     ├─ Send SysEx to synth                                       │
│     ├─ Receive SysEx responses (blocking OK)                     │
│     ├─ Handle timeouts (2 seconds)                               │
│     ├─ Parse SysEx responses                                     │
│     └─ Update APVTS (thread-safe)                                │
│                                                                  │
│  Business Logic (orchestrated by PluginProcessor)                │
│  ├─ SysExParser (parse/validate F0...F7)                         │
│  ├─ PatchModel (in-memory patch representation)                  │
│  ├─ PatchManager (load/save .syx files)                          │
│  ├─ MasterManager (Fetch-Update-Send)                            │
│  ├─ ClipboardManager (type-aware clipboard)                      │
│  ├─ PluginParameterFactory (descriptors → APVTS/GUI)             │
│  ├─ SysExSerializer (PatchModel ↔ bytes)                         │
│  ├─ FileLoader (load .syx from disk)                             │
│  └─ FileWriter (save .syx to disk)                               │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
                            │
                            ▼ (MIDI OUT via port)
                  ┌─────────────────────┐
                  │  Matrix-1000 Synth  │
                  │  (USB/MIDI I/O)     │
                  └─────────────────────┘
```

---

## 2. Factory and Descriptors

### 2.1 Conceptualization

**Descriptors = house plans (static)**

- `.h` files declaring all synth parameters + UI widgets

**Factory = house builders, from plans**

- Reads Descriptors
- Builds APVTS ParameterLayout
- Builds GUI (sections, modules, components)
- Validates 1:1 consistency (no duplicates, no omissions)

**Advantages:**

- Single source of truth
- Validation at startup (exception if problem)
- Facilitates additions/modifications (1 descriptor line = auto GUI + APVTS)

### 2.2 Descriptor structures

```cpp
// Data/Descriptors/*.h (static, inline const)

struct ParameterDescriptor {
    std::string parameterId;         // "dco1Frequency"
    std::string parameterName;       // "DCO 1 Frequency"
    float minValue, maxValue, defaultValue;
    uint8_t sysExOffset;             // Offset in SysEx patch
    int sysExParameterId;            // SysEx param ID (0x00, 0x01, etc.)
};

struct WidgetDescriptor {
    enum class Type { kSlider, kComboBox, kNumber, kButton, kLabel, kSeparator };
    Type type;
    std::string label;
    std::string parameterId;  // "" if standalone widget
};

struct ModuleDescriptor {
    std::string moduleName;          // "DCO 1"
    std::vector<ParameterDescriptor> parameters;
    std::vector<WidgetDescriptor> widgets;
    std::vector<std::string> buttonLabels;  // ["I", "C", "P"]
};

struct SectionDescriptor {
    std::string sectionName;         // "PATCH EDIT"
    std::vector<ModuleDescriptor> modules;
};
```

### 2.3 Descriptor file organization

```
Data/Descriptors/
├── DcoModuleDescriptors.h       (DCO 1, DCO 2)
├── EnvModuleDescriptors.h       (ENV 1/2/3)
├── LfoModuleDescriptors.h       (LFO 1/2)
├── VcfVcaModuleDescriptors.h    (VCF/VCA)
├── FmTrackModuleDescriptors.h   (FM/TRACK)
├── RampPortamentoModuleDescriptors.h
├── MatrixModulationDescriptors.h (10 buses)
├── PatchManagerDescriptors.h    (BANK UTILITY, INTERNAL, COMPUTER)
├── MasterEditDescriptors.h      (MIDI, VIBRATO, MISC)
└── SettingsDescriptors.h
```

### 2.4 PluginParameterFactory

```cpp
// Business/PluginParameterFactory.h

class PluginParameterFactory {
public:
    // Factory methods
    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();

    static SectionDescriptor createPatchEditSection();
    static SectionDescriptor createMatrixModulationSection();
    static SectionDescriptor createMasterEditSection();

    // Validation
    static bool validateParameterConsistency();

private:
    static void addParametersFromDescriptor(
        std::vector<std::unique_ptr<juce::AudioParameterFloat>>& params,
        const ModuleDescriptor& module);
};
```

**Note for developer:** Factory checks at startup that no parameter ID or SysEx offset is duplicated. Exception thrown if problem detected.

## 3. Business classes

### 3.1 PatchModel

**Responsibility:** In-memory representation of a Matrix-1000 patch.

```cpp
class PatchModel {
public:
    explicit PatchModel(const std::string& name = "");

    std::string getName() const;
    void setName(const std::string& name);

    float getParameterValue(const std::string& parameterId) const;
    void setParameterValue(const std::string& parameterId, float value);

    bool isValid() const;

    juce::MemoryBlock toSysExBytes() const;
    static PatchModel fromSysExBytes(const juce::MemoryBlock& data);

private:
    std::string name_;
    std::map<std::string, float> parameters_;
};
```

**Dependencies:** SysExSerializer (for byte conversion)

### 3.2 SysExParser

**Responsibility:** Parse and validate Matrix-1000 SysEx messages.

```cpp
class SysExParser {
public:
    struct ParseResult {
        bool isValid;
        std::string errorMessage;
        PatchModel patch;
    };

    ParseResult parsePatchSysEx(const juce::MemoryBlock& sysEx);
    ParseResult parseMasterSysEx(const juce::MemoryBlock& sysEx);

    bool validateChecksum(const juce::MemoryBlock& sysEx) const;

private:
    uint8_t calculateChecksum(const juce::uint8* data, size_t length) const;

    static constexpr uint8_t kSysExStart = 0xF0;
    static constexpr uint8_t kSysExEnd = 0xF7;
    static constexpr uint8_t kOberheimManufacturerId = 0x10;
};
```

**Exceptions thrown:**

```cpp
class SysExException : public std::exception {
    std::string message_;
public:
    explicit SysExException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};
```

### 3.3 MidiManager

**Responsibility:** Dedicated MIDI thread for SysEx I/O, timeout handling, parsing, APVTS updates.

```cpp
class MidiManager : public juce::Thread {
public:
    MidiManager(juce::AudioDeviceManager& audioDeviceManager,
                juce::AudioProcessorValueTreeState& apvts);

    ~MidiManager() override;

    void run() override;

    // Send (thread-safe)
    void sendPatchToSynth(const PatchModel& patch);
    void sendMasterToSynth(const juce::MemoryBlock& masterData);
    void sendProgramChange(uint8_t programNumber);

    // Request
    void requestCurrentPatch();
    void requestMasterData();
    void performDeviceInquiry();

    // Configuration
    void setMidiInputPort(const std::string& portName);
    void setMidiOutputPort(const std::string& portName);

private:
    void handleIncomingMessage(const juce::MidiMessage& message);
    void handleSysEx(const juce::MemoryBlock& sysEx);
    bool detectSynthDevice();

    juce::AudioDeviceManager& audioDeviceManager_;
    juce::AudioProcessorValueTreeState& apvts_;

    std::unique_ptr<juce::MidiInput> midiInput_;
    std::unique_ptr<juce::MidiOutput> midiOutput_;

    SysExParser sysExParser_;

    static constexpr int kTimeoutMs = 2000;
};
```

**Note for developer:** MidiManager run() in dedicated thread → **no audio thread blocking**. Send/receive SysEx, timeout handling, parsing → update APVTS thread-safe (via ValueTree listeners).

### 3.4 PatchManager

**Responsibility:** Load/save .syx files, validation, alphabetical sorting.

```cpp
class PatchManager {
public:
    explicit PatchManager(const juce::File& defaultFolder);

    std::vector<PatchModel> loadPatchesFromFolder(const juce::File& folder);
    PatchModel loadPatchFromFile(const juce::File& file);

    void savePatchToFile(const PatchModel& patch, const juce::File& file);

    bool isPatchFileValid(const juce::File& file);

private:
    juce::File defaultFolder_;
    FileLoader fileLoader_;
    FileWriter fileWriter_;
};
```

### 3.5 MasterManager

**Responsibility:** Implement Fetch-Update-Send cycle for MASTER parameters.

```cpp
class MasterManager {
public:
    explicit MasterManager(MidiManager& midiManager);

    void onMasterParameterChanged(const std::string& parameterId, float newValue);
    void receiveMasterSysEx(const juce::MemoryBlock& sysEx);

private:
    void fetchMasterFromSynth();
    void updateMasterLocally(const std::string& parameterId, float value);
    void sendMasterToSynth();

    MidiManager& midiManager_;
    juce::MemoryBlock currentMasterData_;
};
```

### 3.6 ClipboardManager

**Responsibility:** Type-aware clipboard, compatibility validation, persistence.

```cpp
class ClipboardManager {
public:
    enum class ClipboardType { kEmpty, kDco, kEnv, kLfo, kPatch };

    void copyDcoModule(const std::string& dcoId);
    void copyEnvModule(const std::string& envId);
    void copyLfoModule(const std::string& lfoId);
    void copyCompletePatch(const PatchModel& patch);

    bool canPasteToDcoModule() const;
    bool canPasteToEnvModule() const;
    bool canPasteToLfoModule() const;

    std::vector<std::pair<std::string, float>> getClipboardParameters() const;

private:
    ClipboardType type_ = ClipboardType::kEmpty;
    std::vector<std::pair<std::string, float>> parameters_;
};
```

## 4. Threading and MIDI management

### 4.1 Threading strategy

**Three distinct threads:**

| Thread   | Responsibilities                    | Constraints                                        |
| -------- | ----------------------------------- | -------------------------------------------------- |
| **Audio** | `processBlock()` DAW                | NO blocking, NO I/O, updates APVTS only           |
| **GUI**  | User events, component updates      | Listen APVTS, dispatch MIDI commands              |
| **MIDI** | Send/receive SysEx, parsing, timeouts | Blocking OK, I/O allowed, update APVTS thread-safe |

### 4.2 Inter-thread communication

**APVTS ValueTree = single thread-safe source of truth**

```
User modifies GUI parameter (slider, combobox)
    ↓
Slider attachment → APVTS listener triggered
    ↓
PluginProcessor notified (audio thread?)
    ↓
Message sent to MidiThread (lock-free FIFO)
    ↓
MidiThread sends SysEx to synth
    ↓
Synth responds SysEx
    ↓
MidiThread parses and updates APVTS (thread-safe)
    ↓
GUI listeners triggered
    ↓
GUI components update (thread-safe)
```

### 4.3 MidiThread implementation

```cpp
// Business/MidiManager.cpp (run() method)

void MidiManager::run() {
    while (!threadShouldExit()) {
        // 1. Check incoming messages (non-blocking)
        if (midiInput_) {
            juce::MidiMessage msg = midiInput_->getNextMessage();
            if (msg.isSysEx()) {
                handleSysEx(msg.getSysExData(), msg.getSysExDataSize());
            }
        }

        // 2. Update APVTS (thread-safe, no locks)
        apvts_.state.setProperty("lastPatchLoaded", patchName_, nullptr);

        // 3. Avoid busy-waiting
        wait(10);  // 10ms sleep
    }
}
```

**Note for developer:** No direct locks audio thread ↔ MIDI. APVTS ValueTree listeners handle synchronization automatically (JUCE's internal locking).

## 5. Error handling

### 5.1 Business exceptions

```cpp
// Data/Exceptions.h

class SysExException : public std::exception {
    std::string message_;
public:
    explicit SysExException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};

class FileLoadException : public std::exception {
    std::string message_;
public:
    explicit FileLoadException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};

class MidiTimeoutException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Matrix-1000 not responding. Check connection and try again.";
    }
};
```

### 5.2 Propagation to GUI

**MidiManager catch → APVTS → MessageBar display**

```cpp
// In MidiManager
try {
    auto patch = sysExParser_.parsePatchSysEx(data);
    // ... process patch
} catch (const SysExException& e) {
    apvts_.state.setProperty("lastError", e.what(), nullptr);
    apvts_.state.setProperty("errorType", "SysEx", nullptr);
}

// In PluginEditor
apvts_.state.addListener(this);

void valueTreePropertyChanged(juce::ValueTree& tree,
                             const juce::Identifier& property) override {
    if (property == "lastError") {
        messageBar_->showError(
            tree.getProperty("lastError").toString().toStdString());
    }
}
```

## 6. MIDI and SysEx protocol - Technical details

### 6.1 Synth auto-detection (Device Inquiry / Device ID)

At plugin startup, or when changing MIDI port, automatic synth detection is performed in two steps:

**Step 1: Send Device Inquiry (universal MIDI command)**

```
F0H 7EH <chan> 06H 01H F7H
```

where `<chan>` = 7FH (global address for "any device")

**Step 2: Wait for Device ID response**

```
F0H 7EH <chan> 06H 02H 10H 06H 00H 02H 00H <rev-0> <rev-1> <rev-2> <rev-3> F7H
```

**Validation criteria:**

- Manufacturer ID = 10H (Oberheim)
- Family Low = 06H (Matrix-6/6R/1000)
- Family High = 00H
- Member Low = 02H (Matrix-1000)
- Member High = 00H
- `<rev-0>` to `<rev-3>` = firmware version (ex: 1.10 = 20H 31H 31H 30H, i.e., " 1.10")

**Implementation in MidiManager:**

```cpp
bool MidiManager::detectSynthDevice() {
    // Send Device Inquiry
    juce::MemoryBlock deviceInquiry(5);
    deviceInquiry[0] = 0xF0;
    deviceInquiry[1] = 0x7E;
    deviceInquiry[2] = 0x7F;  // Global channel
    deviceInquiry[3] = 0x06;
    deviceInquiry[4] = 0x01;  // Device Inquiry

    midiOutput_->sendMessageNow(
        juce::MidiMessage::createSysExMessage(deviceInquiry));

    // Wait for Device ID response (timeout 2 seconds)
    auto startTime = std::chrono::high_resolution_clock::now();

    while (!threadShouldExit()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime).count();

        if (elapsed > kTimeoutMs) {
            return false;  // Timeout
        }

        // Check for incoming Device ID response
        // Parse and validate: manufacturer=0x10, family=0x06, member=0x02
        // Return true if all criteria match

        wait(50);  // Check every 50ms
    }

    return false;
}
```

### 6.2 Oberheim checksum algorithm

The Oberheim checksum validates the integrity of PATCH and MASTER SysEx messages.

**Checksum calculation (at transmission):**

1. Initialize `checksum = 0`
2. For each **packed** data byte (before unpacking to nibbles):
   - `checksum += byte`
3. Transmit `(checksum & 0x7F)` as checksum byte
4. Transmit 0xF7 (End of SysEx)

**Checksum validation (at reception):**

1. Initialize `checksum = 0`
2. For each received byte (after unpacking nibbles to packed bytes):
   - `checksum += byte`
3. Verify that `(checksum & 0x7F)` == received checksum
4. If equal → OK, otherwise → **error message**

**Concrete example:**

For a transmitted patch 01H: `F0H 10H 06H 01H NN DATA... CS F7H`

- Header: F0H 10H 06H 01H NN (5 bytes, not included in checksum)
- DATA: 268 nibbles = 134 packed bytes
- CS = (sum of 134 packed bytes) & 0x7F

```cpp
uint8_t SysExParser::calculateChecksum(const juce::uint8* data, 
                                       size_t length) const {
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i) {
        checksum += data[i];
    }
    return checksum & 0x7F;
}

bool SysExParser::validateChecksum(const juce::MemoryBlock& sysEx) const {
    // sysEx format: F0 10 06 NN [packed data...] [CS] F7
    // Assuming data starts after header (4 bytes) and ends before CS and F7

    if (sysEx.getSize() < 6) {
        return false;  // Too short
    }

    // Extract packed data and received checksum
    size_t dataLength = sysEx.getSize() - 3;  // Exclude F0, CS, F7
    const auto* data = static_cast<const juce::uint8*>(sysEx.getData());
    uint8_t receivedChecksum = data[dataLength];

    uint8_t calculated = calculateChecksum(&data[1], dataLength - 1);
    return (calculated & 0x7F) == receivedChecksum;
}
```

### 6.3 Type conversion and data packing/unpacking

The Matrix-1000 patch format uses variable-size fields (1 to 7 bits) in a packed binary structure. During conversion between internal representation (APVTS) and transmitted representation (SysEx), two inverse operations are necessary:

**Unpacking (SysEx → internal representation):**

To extract an N-bit field located at position START_BIT in a packed buffer:

```cpp
uint8_t extractBitField(const juce::uint8* buffer, 
                       uint8_t byteIndex, 
                       uint8_t startBit, 
                       uint8_t numBits) {
    uint8_t value = (buffer[byteIndex] >> startBit) & ((1 << numBits) - 1);
    return value;
}
```

**Packing (internal representation → SysEx):**

To insert a VALUE of N bits at position START_BIT:

```cpp
void insertBitField(juce::uint8* buffer, 
                   uint8_t byteIndex, 
                   uint8_t startBit, 
                   uint8_t numBits, 
                   uint8_t value) {
    uint8_t mask = ((1 << numBits) - 1) << startBit;
    buffer[byteIndex] = (buffer[byteIndex] & ~mask) | 
                       ((value & ((1 << numBits) - 1)) << startBit);
}
```

**Signed value handling:**

For all signed parameters (except VCF Frequency), sign extension from bit 6 to bit 7:

```cpp
int8_t signExtend7Bit(uint8_t value) {
    // Extend sign from bit 6 to bit 7
    if (value & (1 << 6)) {
        value |= ~((1 << 7) - 1);
    }
    return static_cast<int8_t>(value);
}

uint8_t compressTo7Bit(int8_t value) {
    // Compress int8 to 7 bits with sign in bit 6
    return static_cast<uint8_t>(value) & 0x7F;
}

// VCF Frequency (parameter 121): exception, no sign extension
// Use directly the 0-127 range without conversion
```

## 7. Project structure

```
Matrix-Control/
│
├── Source/
│   ├── Plugin/
│   │   ├── PluginProcessor.h/cpp
│   │   ├── PluginEditor.h/cpp
│   │   └── MainEditor.h/cpp
│   │
│   ├── Business/
│   │   ├── PatchModel.h/cpp
│   │   ├── SysExParser.h/cpp
│   │   ├── MidiManager.h/cpp
│   │   ├── PatchManager.h/cpp
│   │   ├── MasterManager.h/cpp
│   │   ├── ClipboardManager.h/cpp
│   │   └── PluginParameterFactory.h/cpp
│   │
│   ├── UI/
│   │   ├── Components/
│   │   │   ├── MatrixSlider.h/cpp
│   │   │   ├── MatrixComboBox.h/cpp
│   │   │   ├── MatrixButton.h/cpp
│   │   │   ├── MatrixNumber.h/cpp
│   │   │   └── MessageBar.h/cpp
│   │   │
│   │   ├── Modules/
│   │   │   ├── DcoModule.h/cpp
│   │   │   ├── EnvModule.h/cpp
│   │   │   ├── LfoModule.h/cpp
│   │   │   ├── VcfVcaModule.h/cpp
│   │   │   └── ModulationBus.h/cpp
│   │   │
│   │   ├── Sections/
│   │   │   ├── PatchEditSection.h/cpp
│   │   │   ├── MatrixModulationSection.h/cpp
│   │   │   ├── PatchManagerSection.h/cpp
│   │   │   └── MasterEditSection.h/cpp
│   │   │
│   │   ├── Themes/
│   │   │   ├── MatrixLookAndFeelBase.h/cpp
│   │   │   ├── MatrixLookAndFeelBlack.h/cpp
│   │   │   └── MatrixLookAndFeelCream.h/cpp
│   │   │
│   │   └── MainEditor.h/cpp
│   │
│   ├── Serialization/
│   │   ├── SysExSerializer.h/cpp
│   │   ├── FileLoader.h/cpp
│   │   └── FileWriter.h/cpp
│   │
│   ├── Data/
│   │   ├── Descriptors/
│   │   │   ├── DcoModuleDescriptors.h
│   │   │   ├── EnvModuleDescriptors.h
│   │   │   ├── (... other descriptors ...)
│   │   │   └── SettingsDescriptors.h
│   │   ├── ParameterDescriptor.h
│   │   ├── ModuleDescriptor.h
│   │   ├── SectionDescriptor.h
│   │   ├── Exceptions.h
│   │   ├── Constants.h
│   │   ├── ParameterIds.h
│   │   ├── SynthSpecs.h
│   │   └── Types.h
│   │
│   └── Utilities/
│       ├── Logger.h/cpp
│       └── Helpers.h/cpp
│
├── Tests/
│   ├── SysExParserTests.cpp
│   ├── PatchModelTests.cpp
│   ├── MidiManagerTests.cpp
│   ├── ClipboardManagerTests.cpp
│   └── PluginParameterFactoryTests.cpp
│
├── Builds/
│   ├── MacOSX/Matrix-Control.xcodeproj
│   └── VisualStudio2022/Matrix-Control.sln
│
├── CMakeLists.txt
├── jucer_project_file.jucer
└── README.md
```

## 8. Planning and phases

### 8.1 Phase #1: UI Components (basic building blocks)

**Duration:** 3-4 days

**Deliverables:**

- `MatrixSlider`, `MatrixComboBox`, `MatrixButton`, `MatrixNumber` (custom widgets)
- `MessageBar` (real-time message display)
- Unit tests for each component

**Objective:** Independent components, testable, themable, scalable (zoom).

### 8.2 Phase #2: Descriptors (plans)

**Duration:** 2-3 days

**Deliverables:**

- `DcoModuleDescriptors.h`, `EnvModuleDescriptors.h`, `LfoModuleDescriptors.h`, etc.
- All ParameterDescriptor + exact SysEx offsets (consult Matrix-1000 doc)
- Descriptor documentation

**Objective:** Complete and well-structured single source of truth.

### 8.3 Phase #3: Factory + Validation

**Duration:** 2-3 days

**Deliverables:**

- `PluginParameterFactory` (Factory that reads Descriptors)
- `validateParameterConsistency()` (1:1 test APVTS ↔ GUI ↔ SysEx)
- Integration into `PluginProcessor`
- Factory unit tests

**Objective:** Complete APVTS layout built and validated at startup.

### 8.4 Phase #4: Business Logic (business)

**Duration:** 5-7 days

**Deliverables:**

- `PatchModel`, `SysExParser` (parsing, validation, checksum)
- `MidiManager` (dedicated thread, MIDI I/O, timeouts, Device Inquiry)
- `PatchManager`, `MasterManager`, `ClipboardManager`
- `SysExSerializer`, `FileLoader`, `FileWriter`
- Unit tests (SysExParser, PatchModel, MidiManager, ClipboardManager)

**Objective:** Robust, tested, thread-safe business logic.

### 8.5 Phase #5: UI Modules (uses Components + Descriptors)

**Duration:** 4-5 days

**Deliverables:**

- `DcoModule`, `EnvModule`, `LfoModule`, `VcfVcaModule`, `ModulationBus`
- Read Descriptors, create Components (sliders, comboboxes, etc.)
- Responsive layout

**Objective:** Complex UI modules built from Components + Descriptors.

### 8.6 Phase #6: UI Sections (uses Modules + Factory)

**Duration:** 4-5 days

**Deliverables:**

- `PatchEditSection`, `MatrixModulationSection`, `PatchManagerSection`, `MasterEditSection`
- Integrate modules, handle user interactions
- Dispatch actions to PluginProcessor/MidiManager

**Objective:** Complete and interactive sections.

### 8.7 Phase #7: MainEditor + Themes

**Duration:** 2-3 days

**Deliverables:**

- `MainEditor` (assembles all sections)
- `MatrixLookAndFeelBase`, `MatrixLookAndFeelBlack`, `MatrixLookAndFeelCream`
- `ThemeManager` (switch Black/Cream)
- Scalable UI (zoom 50-200%)

**Objective:** Complete interface, themable, resizable.

### 8.8 Phase #9: Integration + Testing

**Duration:** 3-5 days

**Deliverables:**

- Complete integration tests
- MIDI debugging with real Matrix-1000 (if possible)
- Performance optimizations
- UI/UX refinement

**Objective:** Stable v1.0 product ready for release.

### 8.9 Dependencies between phases

```
Phase #1 (Components)
    ↓
Phase #2 (Descriptors)
    ↓
Phase #3 (Factory)
    ↓
Phase #4 (Business Logic)
    ↓
Phase #5 (UI Modules)
    ↓
Phase #6 (UI Sections)
    ↓
Phase #7 (MainEditor)
    ↓
Phase #8 (Integration + Testing)
```

### 8.10 Estimated timeline

| Phase | Duration | Cumulative |
| ----- | -------- | ---------- |
| 1     | 3-4d     | 3-4d       |
| 2     | 2-3d     | 5-7d       |
| 3     | 2-3d     | 7-10d      |
| 4     | 5-7d     | 12-17d     |
| 5     | 4-5d     | 16-22d     |
| 6     | 4-5d     | 20-27d     |
| 7     | 2-3d     | 22-30d     |
| 8     | 3-5d     | 25-35d     |

**Total estimated: 4-6 weeks (25-35 full development days)**

---

Copyright © 2025 Ten Square Software. All rights reserved.

