# Matrix-Control

## Cahier des Charges Technique

**Auteur :** Guillaume DUPONT  
**Organisation :** Ten Square Software  
**Date de modification :** 2025-11-18  
**Version :** 1.0.0

---

## Document complémentaire

**Cahier des Charges Fonctionnel :**

- Généralités, contexte, exigences fonctionnelles
- Exigences techniques pures
- Validation, scope v1.0

## 1. Architecture générale

### 1.1 Principes directeurs

- **Single Responsibility Principle (SRP)** : Chaque classe = 1 responsabilité
- **Separation of Concerns (SoC)** : Métier, MIDI, GUI bien séparés
- **Thread-safety** : APVTS ValueTree (listeners thread-safe)
- **Clean Code** : Lisibilité, noms explicites, no magic numbers, exceptions
- **Pragmatisme** : Classes concrètes plutôt que polymorphisme excessif

### 1.2 Diagramme architecture

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

## 2. Factory et Descriptors

### 2.1 Conceptualisation

**Descriptors = plans de la maison (statiques)**

- Fichiers `.h` déclarant tous paramètres synthé + widgets UI

**Factory = bâtisseurs de la maison, à partir des plans**

- Lit les Descriptors
- Construit APVTS ParameterLayout
- Construit GUI (sections, modules, components)
- Valide cohérence 1:1 (pas doublons, pas oublis)

**Avantages :**

- Source unique de vérité
- Validation au startup (exception si problème)
- Facilite ajouts/modifications (1 ligne descriptors = auto GUI + APVTS)

### 2.2 Structures de descriptors

```cpp
// Data/Descriptors/*.h (statiques, inline const)

struct ParameterDescriptor {
    std::string parameterId;         // "dco1Frequency"
    std::string parameterName;       // "DCO 1 Frequency"
    float minValue, maxValue, defaultValue;
    uint8_t sysExOffset;             // Offset dans SysEx patch
    int sysExParameterId;            // ID param SysEx (0x00, 0x01, etc.)
};

struct WidgetDescriptor {
    enum class Type { kSlider, kComboBox, kNumber, kButton, kLabel, kSeparator };
    Type type;
    std::string label;
    std::string parameterId;  // "" si widget standalone
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

### 2.3 Organisation fichiers descriptors

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

**Note pour le développeur :** La Factory vérifie au startup qu'aucun ID paramètre ni offset SysEx n'est dupliqué. Exception lancée si problème détecté.

## 3. Classes métier

### 3.1 PatchModel

**Responsabilité :** Représentation en mémoire d'un patch Matrix-1000.

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

**Dépendances :** SysExSerializer (pour conversion bytes)

### 3.2 SysExParser

**Responsabilité :** Parser et valider messages SysEx Matrix-1000.

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

**Exceptions lancées :**

```cpp
class SysExException : public std::exception {
    std::string message_;
public:
    explicit SysExException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};
```

### 3.3 MidiManager

**Responsabilité :** Thread MIDI dédié pour I/O SysEx, gestion timeouts, parsing, mise à jour APVTS.

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

**Note pour le développeur :** MidiManager run() dans thread dédié → **pas de blocage audio thread**. Envoi/réception SysEx, gestion timeouts, parsing → update APVTS thread-safe (via ValueTree listeners).

### 3.4 PatchManager

**Responsabilité :** Chargement/sauvegarde fichiers .syx, validation, tri alphabétique.

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

**Responsabilité :** Implémenter cycle Fetch-Update-Send pour paramètres MASTER.

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

**Responsabilité :** Clipboard type-aware, validation compatibilité, persistance.

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

## 4. Threading et gestion MIDI

### 4.1 Stratégie threading

**Trois threads distincts :**

| Thread    | Responsabilités                       | Contraintes                                        |
| --------- | ------------------------------------- | -------------------------------------------------- |
| **Audio** | `processBlock()` DAW                  | NO blocking, NO I/O, updates APVTS only            |
| **GUI**   | User events, component updates        | Listen APVTS, dispatch MIDI commands               |
| **MIDI**  | Send/receive SysEx, parsing, timeouts | Blocking OK, I/O allowed, update APVTS thread-safe |

### 4.2 Communication inter-threads

**APVTS ValueTree = source unique de vérité thread-safe**

```
User modifie paramètre GUI (slider, combobox)
    ↓
Slider attachment → APVTS listener triggered
    ↓
PluginProcessor notified (audio thread?)
    ↓
Message envoyé à MidiThread (lock-free FIFO)
    ↓
MidiThread envoie SysEx au synthé
    ↓
Synthé répond SysEx
    ↓
MidiThread parse et update APVTS (thread-safe)
    ↓
GUI listeners triggered
    ↓
GUI components update (thread-safe)
```

### 4.3 Implémentation MidiThread

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

**Note pour le développeur :** Pas de locks directs thread audio ↔ MIDI. APVTS ValueTree listeners gèrent la synchronisation automatiquement (JUCE's internal locking).

## 5. Gestion d'erreurs

### 5.1 Exceptions métier

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

### 5.2 Propagation vers GUI

**MidiManager catch → APVTS → MessageBar display**

```cpp
// Dans MidiManager
try {
    auto patch = sysExParser_.parsePatchSysEx(data);
    // ... process patch
} catch (const SysExException& e) {
    apvts_.state.setProperty("lastError", e.what(), nullptr);
    apvts_.state.setProperty("errorType", "SysEx", nullptr);
}

// Dans PluginEditor
apvts_.state.addListener(this);

void valueTreePropertyChanged(juce::ValueTree& tree,
                             const juce::Identifier& property) override {
    if (property == "lastError") {
        messageBar_->showError(
            tree.getProperty("lastError").toString().toStdString());
    }
}
```

## 6. Protocole MIDI et SysEx - Détails techniques

### 6.1 Auto-détection du synthé (Device Inquiry / Device ID)

Au démarrage du plugin, ou lors du changement de port MIDI, la détection automatique du synthé s'effectue en deux étapes :

**Étape 1 : Envoi du Device Inquiry (commande MIDI universelle)**

```
F0H 7EH <chan> 06H 01H F7H
```

où `<chan>` = 7FH (adresse globale pour "any device")

**Étape 2 : Attente de la réponse Device ID**

```
F0H 7EH <chan> 06H 02H 10H 06H 00H 02H 00H <rev-0> <rev-1> <rev-2> <rev-3> F7H
```

**Critères de validation :**

- Manufacturer ID = 10H (Oberheim)
- Family Low = 06H (Matrix-6/6R/1000)
- Family High = 00H
- Member Low = 02H (Matrix-1000)
- Member High = 00H
- `<rev-0>` à `<rev-3>` = version firmware (ex: 1.10 = 20H 31H 31H 30H, soit " 1.10")

**Implémentation dans MidiManager :**

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

### 6.2 Algorithme de checksum Oberheim

Le checksum Oberheim valide l'intégrité des messages SysEx PATCH et MASTER.

**Calcul du checksum (à la transmission) :**

1. Initialiser `checksum = 0`
2. Pour chaque byte de données **packées** (avant unpacking en nibbles) :
   - `checksum += byte`
3. Transmettre `(checksum & 0x7F)` comme byte de checksum
4. Transmettre 0xF7 (End of SysEx)

**Validation du checksum (à la réception) :**

1. Initialiser `checksum = 0`
2. Pour chaque byte reçu (après unpacking des nibbles en bytes packés) :
   - `checksum += byte`
3. Vérifier que `(checksum & 0x7F)` == checksum reçu
4. Si égalité → OK, sinon → **message d'erreur**

**Exemple concret :**

Pour un patch 01H transmis : `F0H 10H 06H 01H NN DATA... CS F7H`

- Header : F0H 10H 06H 01H NN (5 bytes, pas inclus dans checksum)
- DATA : 268 nibbles = 134 bytes packés
- CS = (somme des 134 bytes packés) & 0x7F

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

### 6.3 Conversion de types et packing/unpacking des données

Le format de patch du Matrix-1000 utilise des champs de tailles variables (1 à 7 bits) dans une structure binaire packed. Lors de la conversion entre représentation interne (APVTS) et représentation transmise (SysEx), deux opérations inverse sont nécessaires :

**Unpacking (SysEx → représentation interne) :**

Pour extraire un champ de N bits situé à la position START_BIT d'un buffer packed :

```cpp
uint8_t extractBitField(const juce::uint8* buffer, 
                       uint8_t byteIndex, 
                       uint8_t startBit, 
                       uint8_t numBits) {
    uint8_t value = (buffer[byteIndex] >> startBit) & ((1 << numBits) - 1);
    return value;
}
```

**Packing (représentation interne → SysEx) :**

Pour insérer une valeur VALUE de N bits à la position START_BIT :

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

**Gestion des valeurs signées :**

Pour tous les paramètres signés (sauf VCF Frequency), extension de signe depuis bit 6 vers bit 7 :

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

// VCF Frequency (paramètre 121) : exception, pas d'extension de signe
// Utiliser directement la plage 0-127 sans conversion
```

## 7. Structure projet

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
│   │   │   ├── (... autres descriptors ...)
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

## 8. Planning et phases

### 8.1 Phase #1 : UI Components (briques élémentaires)

**Durée :** 3-4 jours

**Livrables :**

- `MatrixSlider`, `MatrixComboBox`, `MatrixButton`, `MatrixNumber` (widgets personnalisés)
- `MessageBar` (affichage messages temps réel)
- Tests unitaires pour chaque component

**Objectif :** Components indépendants, testables, thématisables, scalables (zoom).

### 8.2 Phase #2 : Descriptors (plans)

**Durée :** 2-3 jours

**Livrables :**

- `DcoModuleDescriptors.h`, `EnvModuleDescriptors.h`, `LfoModuleDescriptors.h`, etc.
- Tous les ParameterDescriptor + offsets SysEx exacts (consulter doc Matrix-1000)
- Documentation des descriptors

**Objectif :** Source unique de vérité complète et bien structurée.

### 8.3 Phase #3 : Factory + Validation

**Durée :** 2-3 jours

**Livrables :**

- `PluginParameterFactory` (Factory qui lit Descriptors)
- `validateParameterConsistency()` (test 1:1 APVTS ↔ GUI ↔ SysEx)
- Intégration dans `PluginProcessor`
- Tests unitaires Factory

**Objectif :** APVTS layout complet construit et validé au startup.

### 8.4 Phase #4 : Business Logic (métier)

**Durée :** 5-7 jours

**Livrables :**

- `PatchModel`, `SysExParser` (parsing, validation, checksum)
- `MidiManager` (thread dédié, I/O MIDI, timeouts, Device Inquiry)
- `PatchManager`, `MasterManager`, `ClipboardManager`
- `SysExSerializer`, `FileLoader`, `FileWriter`
- Tests unitaires (SysExParser, PatchModel, MidiManager, ClipboardManager)

**Objectif :** Logique métier robuste, testée, thread-safe.

### 8.5 Phase #5 : UI Modules (utilise Components + Descriptors)

**Durée :** 4-5 jours

**Livrables :**

- `DcoModule`, `EnvModule`, `LfoModule`, `VcfVcaModule`, `ModulationBus`
- Lisent les Descriptors, créent Components (sliders, comboboxes, etc.)
- Layout responsive

**Objectif :** Modules UI complexes construits à partir de Components + Descriptors.

### 8.6 Phase #6 : UI Sections (utilise Modules + Factory)

**Durée :** 4-5 jours

**Livrables :**

- `PatchEditSection`, `MatrixModulationSection`, `PatchManagerSection`, `MasterEditSection`
- Intègrent les modules, gèrent interactions utilisateur
- Dispatch actions vers PluginProcessor/MidiManager

**Objectif :** Sections complètes et interactives.

### 8.7 Phase #7 : MainEditor + Thèmes

**Durée :** 2-3 jours

**Livrables :**

- `MainEditor` (assemble toutes sections)
- `MatrixLookAndFeelBase`, `MatrixLookAndFeelBlack`, `MatrixLookAndFeelCream`
- `ThemeManager` (switch Black/Cream)
- UI scalable (zoom 50-200%)

**Objectif :** Interface complète, themable, resizable.

### 8.8 Phase #9 : Integration + Testing

**Durée :** 3-5 jours

**Livrables :**

- Tests d'intégration complets
- Déboggage MIDI avec vrai Matrix-1000 (si possible)
- Optimisations performances
- Refinement UI/UX

**Objectif :** Produit stable v1.0 prêt release.

### 8.9 Dépendances entre phases

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

### 8.10 Timeline estimée

| Phase | Durée | Cumul  |
| ----- | ----- | ------ |
| 1     | 3-4j  | 3-4j   |
| 2     | 2-3j  | 5-7j   |
| 3     | 2-3j  | 7-10j  |
| 4     | 5-7j  | 12-17j |
| 5     | 4-5j  | 16-22j |
| 6     | 4-5j  | 20-27j |
| 7     | 2-3j  | 22-30j |
| 8     | 3-5j  | 25-35j |

**Total estimé : 4-6 semaines (25-35 jours de dev complet)**

---

Copyright © 2025 Ten Square Software. Tous droits réservés.
