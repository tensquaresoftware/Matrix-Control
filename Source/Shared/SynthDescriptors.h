#pragma once

#include <vector>

#include <juce_core/juce_core.h>

// SynthDescriptors.h
// Central file for parameter IDs, names, values, and groups
// Used by both PluginProcessor (for APVTS) and PluginEditor (for GUI)

namespace SynthDescriptors
{

// ============================================================================
// Group Descriptors
// ============================================================================

struct GroupDescriptor
{
    juce::String parentId;  // Empty string for root groups (modes)
    juce::String groupId;
    juce::String displayName;
};

// ============================================================================
// Parameter Descriptors
// ============================================================================

enum class ParameterType
{
    kInt,      // AudioParameterInt (Slider)
    kChoice    // AudioParameterChoice (ComboBox)
};

struct IntParameterDescriptor
{
    juce::String parameterId;
    juce::String displayName;
    juce::String parentGroupId;
    int minValue;
    int maxValue;
    int defaultValue;
    int sysExOffset;
    int sysExId;
};

struct ChoiceParameterDescriptor
{
    juce::String parameterId;
    juce::String displayName;
    juce::String parentGroupId;
    juce::StringArray choices;
    int defaultIndex;
    int sysExOffset;
    int sysExId;
};

// ============================================================================
// Standalone Widget Descriptors
// ============================================================================

enum class StandaloneWidgetType
{
    kButton,
    kComboBox,
    kNumber,
    kLabel
};

struct StandaloneWidgetDescriptor
{
    juce::String widgetId;  // ValueTree property ID
    juce::String displayName;
    juce::String parentGroupId;
    StandaloneWidgetType widgetType;
};

// ============================================================================
// Group IDs (Modes)
// ============================================================================

namespace GroupIds
{
    constexpr const char* kPatch = "patch";
    constexpr const char* kMaster = "master";
}

// ============================================================================
// Group IDs (Sections)
// ============================================================================

namespace SectionIds
{
    constexpr const char* kPatchEdit = "patchEdit";
    constexpr const char* kMatrixModulation = "matrixModulation";
    constexpr const char* kPatchManager = "patchManager";
    constexpr const char* kMasterEdit = "masterEdit";
}

// ============================================================================
// Group IDs (Modules)
// ============================================================================

namespace ModuleIds
{
    // Patch Edit modules
    constexpr const char* kDco1 = "dco1";
    constexpr const char* kDco2 = "dco2";
    constexpr const char* kVcfVca = "vcfVca";
    constexpr const char* kFmTrack = "fmTrack";
    constexpr const char* kRampPortamento = "rampPortamento";
    constexpr const char* kEnvelope1 = "envelope1";
    constexpr const char* kEnvelope2 = "envelope2";
    constexpr const char* kEnvelope3 = "envelope3";
    constexpr const char* kLfo1 = "lfo1";
    constexpr const char* kLfo2 = "lfo2";

    // Patch Manager modules
    constexpr const char* kBankUtility = "bankUtility";
    constexpr const char* kInternalPatches = "internalPatches";
    constexpr const char* kComputerPatches = "computerPatches";

    // Master Edit modules
    constexpr const char* kMidi = "midi";
    constexpr const char* kVibrato = "vibrato";
    constexpr const char* kMisc = "misc";
}

// ============================================================================
// Group IDs (Modulation Busses)
// ============================================================================

namespace BusIds
{
    constexpr const char* kMatrixModBus0 = "matrixModBus0";
    constexpr const char* kMatrixModBus1 = "matrixModBus1";
    constexpr const char* kMatrixModBus2 = "matrixModBus2";
    constexpr const char* kMatrixModBus3 = "matrixModBus3";
    constexpr const char* kMatrixModBus4 = "matrixModBus4";
    constexpr const char* kMatrixModBus5 = "matrixModBus5";
    constexpr const char* kMatrixModBus6 = "matrixModBus6";
    constexpr const char* kMatrixModBus7 = "matrixModBus7";
    constexpr const char* kMatrixModBus8 = "matrixModBus8";
    constexpr const char* kMatrixModBus9 = "matrixModBus9";
}

// ============================================================================
// Group Definitions
// ============================================================================

inline const std::vector<GroupDescriptor> getAllGroups()
{
    return {
        // Modes
        { "", GroupIds::kPatch,  "PATCH" },
        { "", GroupIds::kMaster, "MASTER" },

        // Sections
        { GroupIds::kPatch,  SectionIds::kPatchEdit,        "PATCH EDIT" },
        { GroupIds::kPatch,  SectionIds::kMatrixModulation, "MATRIX MODULATION" },
        { GroupIds::kPatch,  SectionIds::kPatchManager,     "PATCH MANAGER" },
        { GroupIds::kMaster, SectionIds::kMasterEdit,       "MASTER EDIT" },

        // Patch Edit Modules
        { SectionIds::kPatchEdit,  ModuleIds::kDco1,           "DCO 1" },
        { SectionIds::kPatchEdit,  ModuleIds::kDco2,           "DCO 2" },
        { SectionIds::kPatchEdit,  ModuleIds::kVcfVca,         "VCF/VCA" },
        { SectionIds::kPatchEdit,  ModuleIds::kFmTrack,        "FM/TRACK" },
        { SectionIds::kPatchEdit, ModuleIds::kRampPortamento, "RAMP/PORTAMENTO" },
        { SectionIds::kPatchEdit, ModuleIds::kEnvelope1,      "ENV 1" },
        { SectionIds::kPatchEdit, ModuleIds::kEnvelope2,      "ENV 2" },
        { SectionIds::kPatchEdit, ModuleIds::kEnvelope3,      "ENV 3" },
        { SectionIds::kPatchEdit, ModuleIds::kLfo1,           "LFO 1" },
        { SectionIds::kPatchEdit, ModuleIds::kLfo2,           "LFO 2" },

        // Patch Manager Modules
        { SectionIds::kPatchManager, ModuleIds::kBankUtility,     "BANK UTILITY" },
        { SectionIds::kPatchManager, ModuleIds::kInternalPatches, "INTERNAL PATCHES" },
        { SectionIds::kPatchManager, ModuleIds::kComputerPatches, "COMPUTER PATCHES" },

        // Master Edit Modules
        { SectionIds::kMasterEdit, ModuleIds::kMidi,    "MIDI" },
        { SectionIds::kMasterEdit, ModuleIds::kVibrato, "VIBRATO" },
        { SectionIds::kMasterEdit, ModuleIds::kMisc,    "MISC" },

        // Modulation Busses
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus0, "MODULATION BUS 0" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus1, "MODULATION BUS 1" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus2, "MODULATION BUS 2" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus3, "MODULATION BUS 3" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus4, "MODULATION BUS 4" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus5, "MODULATION BUS 5" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus6, "MODULATION BUS 6" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus7, "MODULATION BUS 7" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus8, "MODULATION BUS 8" },
        { SectionIds::kMatrixModulation, BusIds::kMatrixModBus9, "MODULATION BUS 9" }
    };
}

// ============================================================================
// Helper Functions
// ============================================================================

inline juce::String getSectionDisplayName(const char* sectionId)
{
    const auto groups = getAllGroups();
    for (const auto& group : groups)
    {
        if (group.groupId == sectionId)
        {
            return group.displayName;
        }
    }
    return juce::String();
}

// ============================================================================
// Parameter Definitions - DCO 1
// ============================================================================

inline const std::vector<IntParameterDescriptor> getDco1IntParameters()
{
    return {
        { "dco1Frequency", "FREQUENCY", ModuleIds::kDco1, 0, 63, 0, 9, 0 },
        { "dco1FrequencyModByLfo1", "FREQ MOD BY LFO 1", ModuleIds::kDco1, -63, 63, 0, 86, 1 },
        { "dco1PulseWidth", "PULSE WIDTH", ModuleIds::kDco1, 0, 63, 31, 11, 3 },
        { "dco1PulseWidthModByLfo2", "PW MOD BY LFO 2", ModuleIds::kDco1, -63, 63, 0, 87, 4 },
        { "dco1WaveShape", "WAVE SHAPE", ModuleIds::kDco1, 0, 63, 31, 10, 5 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getDco1ChoiceParameters()
{
    return {
        { "dco1Sync", "SYNC", ModuleIds::kDco1, { "OFF", "SOFT", "MEDIUM", "HARD" }, 0, 25, 2 },
        { "dco1WaveSelect", "WAVE SELECT", ModuleIds::kDco1, { "OFF", "PULSE", "WAVE", "BOTH" }, 2, 13, 6 },
        { "dco1Levers", "LEVERS", ModuleIds::kDco1, { "OFF", "L1/BEND", "L2/VIB", "BOTH" }, 3, 12, 7 },
        { "dco1KeyboardPortamento", "KEYBOARD/PORTAMENTO", ModuleIds::kDco1, { "KEYBD", "PORTA" }, 0, 21, 8 },
        { "dco1KeyClick", "KEY CLICK", ModuleIds::kDco1, { "OFF", "ON" }, 0, 22, 9 }
    };
}

// ============================================================================
// Parameter Definitions - DCO 2
// ============================================================================

inline const std::vector<IntParameterDescriptor> getDco2IntParameters()
{
    return {
        { "dco2Frequency", "FREQUENCY", ModuleIds::kDco2, 0, 63, 0, 14, 10 },
        { "dco2FrequencyModByLfo1", "FREQ MOD BY LFO 1", ModuleIds::kDco2, -63, 63, 0, 88, 11 },
        { "dco2Detune", "DETUNE", ModuleIds::kDco2, -31, 31, 2, 19, 12 },
        { "dco2PulseWidth", "PULSE WIDTH", ModuleIds::kDco2, 0, 63, 24, 16, 13 },
        { "dco2PulseWidthModByLfo2", "PW MOD BY LFO 2", ModuleIds::kDco2, -63, 63, 0, 89, 14 },
        { "dco2WaveShape", "WAVE SHAPE", ModuleIds::kDco2, 0, 63, 31, 15, 15 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getDco2ChoiceParameters()
{
    return {
        { "dco2WaveSelect", "WAVE SELECT", ModuleIds::kDco2, { "OFF", "PULSE", "WAVE", "BOTH", "NOISE" }, 1, 18, 16 },
        { "dco2Levers", "LEVERS", ModuleIds::kDco2, { "OFF", "L1/BEND", "L2/VIB", "BOTH" }, 3, 17, 17 },
        { "dco2KeyboardPortamento", "KEYBOARD/PORTAMENTO", ModuleIds::kDco2, { "OFF", "PORTA", "KEYBD" }, 2, 23, 18 },
        { "dco2KeyClick", "KEY CLICK", ModuleIds::kDco2, { "OFF", "ON" }, 0, 24, 19 }
    };
}

// ============================================================================
// Parameter Definitions - VCF/VCA
// ============================================================================

inline const std::vector<IntParameterDescriptor> getVcfVcaIntParameters()
{
    return {
        { "vcfBalance", "BALANCE", ModuleIds::kVcfVca, 0, 63, 31, 20, 20 },
        { "vcfFequency", "FREQUENCY", ModuleIds::kVcfVca, 0, 127, 55, 26, 21 },
        { "vcfFrequencyModByEnv1", "FREQ MOD BY ENV 1", ModuleIds::kVcfVca, -63, 63, 42, 90, 22 },
        { "vcfFrequencyModByPressure", "FREQ MOD BY PRESSURE", ModuleIds::kVcfVca, -63, 63, 0, 91, 23 },
        { "vcfResonance", "RESONANCE", ModuleIds::kVcfVca, 0, 63, 0, 27, 24 },
        { "vca1Volume", "VCA 1 VOLUME", ModuleIds::kVcfVca, 0, 63, 63, 31, 27 },
        { "vca1ModByVelocity", "VCA 1 MOD BY VELOCITY", ModuleIds::kVcfVca, -63, 63, 0, 92, 28 },
        { "vca2ModByEnv2", "VCA 2 MOD BY ENV 2", ModuleIds::kVcfVca, -63, 63, 63, 93, 29 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getVcfVcaChoiceParameters()
{
    return {
        { "vcfLevers", "LEVERS", ModuleIds::kVcfVca, { "OFF", "L1/BEND", "L2/VIB", "BOTH" }, 0, 28, 25 },
        { "vcfKeyboardPortamento", "KEYBOARD/PORTAMENTO", ModuleIds::kVcfVca, { "OFF", "PORTA", "KEYBD" }, 2, 29, 26 }
    };
}

// ============================================================================
// Parameter Definitions - FM/TRACK
// ============================================================================

inline const std::vector<IntParameterDescriptor> getFmTrackIntParameters()
{
    return {
        { "fmAmount", "VCF FM AMOUNT", ModuleIds::kFmTrack, 0, 63, 0, 30, 30 },
        { "fmModByEnv3", "FM MOD BY ENV 3", ModuleIds::kFmTrack, -63, 63, 0, 100, 31 },
        { "fmModByPressure", "FM MOD BY PRESSURE", ModuleIds::kFmTrack, -63, 63, 0, 101, 32 },
        { "trackPoint1", "TRACK POINT 1", ModuleIds::kFmTrack, 0, 63, 0, 77, 34 },
        { "trackPoint2", "TRACK POINT 2", ModuleIds::kFmTrack, 0, 63, 15, 78, 35 },
        { "trackPoint3", "TRACK POINT 3", ModuleIds::kFmTrack, 0, 63, 31, 79, 36 },
        { "trackPoint4", "TRACK POINT 4", ModuleIds::kFmTrack, 0, 63, 47, 80, 37 },
        { "trackPoint5", "TRACK POINT 5", ModuleIds::kFmTrack, 0, 63, 63, 81, 38 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getFmTrackChoiceParameters()
{
    return {
        { "trackInput", "TRACK INPUT", ModuleIds::kFmTrack,
          { "NONE", "ENV 1", "ENV 2", "ENV 3", "LFO 1", "LFO 2", "VIBRATO", "RAMP 1", "RAMP 2",
            "KEYBD", "PORTA", "TRACK", "KB GATE", "VELOCITY", "REL VEL", "PRESSURE",
            "PEDAL 1", "PEDAL 2", "LEVER 1", "LEVER 2", "LEVER 3" },
          9, 76, 33 }
    };
}

// ============================================================================
// Parameter Definitions - RAMP/PORTAMENTO
// ============================================================================

inline const std::vector<IntParameterDescriptor> getRampPortamentoIntParameters()
{
    return {
        { "ramp1Rate", "RAMP 1 RATE", ModuleIds::kRampPortamento, 0, 63, 0, 82, 40 },
        { "ramp2Rate", "RAMP 2 RATE", ModuleIds::kRampPortamento, 0, 63, 0, 84, 42 },
        { "portamentoRate", "PORTAMENTO RATE", ModuleIds::kRampPortamento, 0, 63, 0, 32, 44 },
        { "portamentoModByVelocity", "PORT MOD BY VELOCITY", ModuleIds::kRampPortamento, -63, 63, 0, 99, 45 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getRampPortamentoChoiceParameters()
{
    return {
        { "ramp1Trigger", "RAMP 1 TRIGGER", ModuleIds::kRampPortamento,
          { "STRIG", "MTRIG", "EXTRIG", "GATEDX" }, 0, 83, 41 },
        { "ramp2Trigger", "RAMP 2 TRIGGER", ModuleIds::kRampPortamento,
          { "STRIG", "MTRIG", "EXTRIG", "GATEDX" }, 0, 85, 43 },
        { "portamentoMode", "PORTAMENTO MODE", ModuleIds::kRampPortamento,
          { "LINEAR", "CONST", "EXPO" }, 0, 33, 46 },
        { "portamentoLegato", "LEGATO PORTAMENTO", ModuleIds::kRampPortamento,
          { "OFF", "ON" }, 0, 34, 47 },
        { "portamentoKeyboardMode", "KEYBOARD MODE", ModuleIds::kRampPortamento,
          { "ROTATE", "REASGN", "UNISON", "REAROB" }, 1, 8, 48 }
    };
}

// ============================================================================
// Parameter Definitions - ENV 1
// ============================================================================

inline const std::vector<IntParameterDescriptor> getEnv1IntParameters()
{
    return {
        { "env1Delay", "DELAY", ModuleIds::kEnvelope1, 0, 63, 0, 50, 50 },
        { "env1Attack", "ATTACK", ModuleIds::kEnvelope1, 0, 63, 0, 51, 51 },
        { "env1Decay", "DECAY", ModuleIds::kEnvelope1, 0, 63, 10, 52, 52 },
        { "env1Sustain", "SUSTAIN", ModuleIds::kEnvelope1, 0, 63, 50, 53, 53 },
        { "env1Release", "RELEASE", ModuleIds::kEnvelope1, 0, 63, 10, 54, 54 },
        { "env1Amplitude", "AMPLITUDE", ModuleIds::kEnvelope1, 0, 63, 40, 55, 55 },
        { "env1AmplitudeModByVelocity", "AMP MOD BY VELOCITY", ModuleIds::kEnvelope1, -63, 63, 63, 94, 56 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getEnv1ChoiceParameters()
{
    return {
        { "env1TriggerMode", "TRIGGER MODE", ModuleIds::kEnvelope1,
          { "STRIG", "MTRIG", "SRESET", "MRESET", "XTRIG", "XMTRIG", "XRESET", "XMRST" },
          0, 49, 57 },
        { "env1EnvelopeMode", "ENVELOPE MODE", ModuleIds::kEnvelope1,
          { "NORMAL", "DADR", "FREE", "BOTH" }, 0, 57, 58 },
        { "env1Lfo1Trigger", "LFO 1 TRIGGER", ModuleIds::kEnvelope1,
          { "NORMAL", "LFO 1", "G-LFO 1" }, 0, 56, 59 }
    };
}

// ============================================================================
// Parameter Definitions - ENV 2
// ============================================================================

inline const std::vector<IntParameterDescriptor> getEnv2IntParameters()
{
    return {
        { "env2Delay", "DELAY", ModuleIds::kEnvelope2, 0, 63, 0, 59, 60 },
        { "env2Attack", "ATTACK", ModuleIds::kEnvelope2, 0, 63, 0, 60, 61 },
        { "env2Decay", "DECAY", ModuleIds::kEnvelope2, 0, 63, 10, 61, 62 },
        { "env2Sustain", "SUSTAIN", ModuleIds::kEnvelope2, 0, 63, 50, 62, 63 },
        { "env2Release", "RELEASE", ModuleIds::kEnvelope2, 0, 63, 10, 63, 64 },
        { "env2Amplitude", "AMPLITUDE", ModuleIds::kEnvelope2, 0, 63, 40, 64, 65 },
        { "env2AmplitudeModByVelocity", "AMP MOD BY VELOCITY", ModuleIds::kEnvelope2, -63, 63, 63, 95, 66 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getEnv2ChoiceParameters()
{
    return {
        { "env2TriggerMode", "TRIGGER MODE", ModuleIds::kEnvelope2,
          { "STRIG", "MTRIG", "SRESET", "MRESET", "XTRIG", "XMTRIG", "XRESET", "XMRST" },
          0, 58, 67 },
        { "env2EnvelopeMode", "ENVELOPE MODE", ModuleIds::kEnvelope2,
          { "NORMAL", "DADR", "FREE", "BOTH" }, 0, 66, 68 },
        { "env2Lfo1Trigger", "LFO 1 TRIGGER", ModuleIds::kEnvelope2,
          { "NORMAL", "LFO 1", "G-LFO 1" }, 0, 65, 69 }
    };
}

// ============================================================================
// Parameter Definitions - ENV 3
// ============================================================================

inline const std::vector<IntParameterDescriptor> getEnv3IntParameters()
{
    return {
        { "env3Delay", "DELAY", ModuleIds::kEnvelope3, 0, 63, 0, 68, 70 },
        { "env3Attack", "ATTACK", ModuleIds::kEnvelope3, 0, 63, 0, 69, 71 },
        { "env3Decay", "DECAY", ModuleIds::kEnvelope3, 0, 63, 20, 70, 72 },
        { "env3Sustain", "SUSTAIN", ModuleIds::kEnvelope3, 0, 63, 0, 71, 73 },
        { "env3Release", "RELEASE", ModuleIds::kEnvelope3, 0, 63, 20, 72, 74 },
        { "env3Amplitude", "AMPLITUDE", ModuleIds::kEnvelope3, 0, 63, 40, 73, 75 },
        { "env3AmplitudeModByVelocity", "AMP MOD BY VELOCITY", ModuleIds::kEnvelope3, -63, 63, 63, 96, 76 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getEnv3ChoiceParameters()
{
    return {
        { "env3TriggerMode", "TRIGGER MODE", ModuleIds::kEnvelope3,
          { "STRIG", "MTRIG", "SRESET", "MRESET", "XTRIG", "XMTRIG", "XRESET", "XMRST" },
          0, 67, 77 },
        { "env3EnvelopeMode", "ENVELOPE MODE", ModuleIds::kEnvelope3,
          { "NORMAL", "DADR", "FREE", "BOTH" }, 0, 75, 78 },
        { "env3Lfo1Trigger", "LFO 1 TRIGGER", ModuleIds::kEnvelope3,
          { "NORMAL", "LFO 1", "GATED LFO 1 TRIGGER" }, 0, 74, 79 }
    };
}

// ============================================================================
// Parameter Definitions - LFO 1
// ============================================================================

inline const std::vector<IntParameterDescriptor> getLfo1IntParameters()
{
    return {
        { "lfo1Speed", "SPEED", ModuleIds::kLfo1, 0, 63, 40, 35, 80 },
        { "lfo1SpeedModByPressure", "SPEED MOD BY PRESSURE", ModuleIds::kLfo1, -63, 63, 0, 102, 81 },
        { "lfo1RetriggerPoint", "RETRIGGER POINT", ModuleIds::kLfo1, 0, 63, 0, 39, 83 },
        { "lfo1Amplitude", "AMPLITUDE", ModuleIds::kLfo1, 0, 63, 0, 41, 84 },
        { "lfo1AmplitudeModByRamp1", "AMP MOD BY RAMP 1", ModuleIds::kLfo1, -63, 63, 63, 97, 85 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getLfo1ChoiceParameters()
{
    return {
        { "lfo1Waveform", "WAVEFORM", ModuleIds::kLfo1,
          { "TRIANGLE", "UPSAW", "DNSAW", "SQUARE", "RANDOM", "NOISE", "SAMPLED" },
          0, 38, 82 },
        { "lfo1TriggerMode", "TRIGGER MODE", ModuleIds::kLfo1,
          { "OFF", "STRIG", "MTRIG", "XTRIG" }, 0, 36, 86 },
        { "lfo1Lag", "LAG", ModuleIds::kLfo1, { "OFF", "ON" }, 0, 37, 87 },
        { "lfo1SampleInput", "SAMPLE INPUT", ModuleIds::kLfo1,
          { "NONE", "ENV 1", "ENV 2", "ENV 3", "LFO 1", "LFO 2", "VIBRATO", "RAMP 1", "RAMP 2",
            "KEYBD", "PORTA", "TRACK", "KB GATE", "VELOCITY", "REL VEL", "PRESSURE",
            "PEDAL 1", "PEDAL 2", "LEVER 1", "LEVER 2", "LEVER 3" },
          9, 40, 88 }
    };
}

// ============================================================================
// Parameter Definitions - LFO 2
// ============================================================================

inline const std::vector<IntParameterDescriptor> getLfo2IntParameters()
{
    return {
        { "lfo2Speed", "SPEED", ModuleIds::kLfo2, 0, 63, 30, 42, 90 },
        { "lfo2SpeedModByKeyboard", "SPEED MOD BY KEYBD", ModuleIds::kLfo2, -63, 63, 0, 103, 91 },
        { "lfo2RetriggerPoint", "RETRIGGER POINT", ModuleIds::kLfo2, 0, 63, 0, 46, 93 },
        { "lfo2Amplitude", "AMPLITUDE", ModuleIds::kLfo2, 0, 63, 0, 48, 94 },
        { "lfo2AmplitudeModByRamp2", "AMP MOD BY RAMP 2", ModuleIds::kLfo2, -63, 63, 63, 98, 95 }
    };
}

inline const std::vector<ChoiceParameterDescriptor> getLfo2ChoiceParameters()
{
    return {
        { "lfo2Waveform", "WAVEFORM", ModuleIds::kLfo2,
          { "TRIANGLE", "UPSAW", "DNSAW", "SQUARE", "RANDOM", "NOISE", "SAMPLED" },
          0, 45, 92 },
        { "lfo2TriggerMode", "TRIGGER MODE", ModuleIds::kLfo2,
          { "OFF", "STRIG", "MTRIG", "XTRIG" }, 0, 43, 96 },
        { "lfo2Lag", "LAG", ModuleIds::kLfo2, { "OFF", "ON" }, 0, 44, 97 },
        { "lfo2SampleInput", "SAMPLE INPUT", ModuleIds::kLfo2,
          { "NONE", "ENV 1", "ENV 2", "ENV 3", "LFO 1", "LFO 2", "VIBRATO", "RAMP 1", "RAMP 2",
            "KEYBD", "PORTA", "TRACK", "KB GATE", "VELOCITY", "REL VEL", "PRESSURE",
            "PEDAL 1", "PEDAL 2", "LEVER 1", "LEVER 2", "LEVER 3" },
          9, 47, 98 }
    };
}

// ============================================================================
// Parameter Definitions - Matrix Modulation Busses
// ============================================================================

inline const std::vector<ChoiceParameterDescriptor> getMatrixModBusChoiceParameters(int busNumber)
{
    const juce::String busId = "matrixModBus" + juce::String(busNumber);
    const juce::String busGroupId = "matrixModBus" + juce::String(busNumber);

    const juce::StringArray sourceChoices = {
        "NONE", "ENV 1", "ENV 2", "ENV 3", "LFO 1", "LFO 2", "VIBRATO", "RAMP 1", "RAMP 2",
        "KEYBD", "PORTA", "TRACK", "KB GATE", "VELOCITY", "REL VEL", "PRESSURE",
        "PEDAL 1", "PEDAL 2", "LEVER 1", "LEVER 2", "LEVER 3"
    };

    const juce::StringArray destinationChoices = {
        "NONE", "DCO 1 FREQUENCY", "DCO 1 PULSE WIDTH", "DCO 1 WAVE SHAPE",
        "DCO 2 FREQUENCY", "DCO 2 PULSE WIDTH", "DCO 2 WAVE SHAPE",
        "DCO 1 - DCO 2 MIX", "VCF FM BY DCO 1", "VCF FREQUENCY", "VCF RESONANCE",
        "VCA 1 VOLUME", "VCA 2 VOLUME",
        "ENV 1 DELAY", "ENV 1 ATTACK", "ENV 1 DECAY", "ENV 1 RELEASE", "ENV 1 AMPLITUDE",
        "ENV 2 DELAY", "ENV 2 ATTACK", "ENV 2 DECAY", "ENV 2 RELEASE", "ENV 2 AMPLITUDE",
        "ENV 3 DELAY", "ENV 3 ATTACK", "ENV 3 DECAY", "ENV 3 RELEASE", "ENV 3 AMPLITUDE",
        "LFO 1 SPEED", "LFO 1 AMPLITUDE", "LFO 2 SPEED", "LFO 2 AMPLITUDE",
        "PORTAMENTO RATE"
    };

    const int baseOffset = 104 + (busNumber * 3);

    return {
        { busId + "Source", "SOURCE", busGroupId, sourceChoices, 0, baseOffset, -1 },
        { busId + "Destination", "DESTINATION", busGroupId, destinationChoices, 0, baseOffset + 2, -1 }
    };
}

inline const std::vector<IntParameterDescriptor> getMatrixModBusIntParameters(int busNumber)
{
    const juce::String busId = "matrixModBus" + juce::String(busNumber);
    const juce::String busGroupId = "matrixModBus" + juce::String(busNumber);
    const int baseOffset = 104 + (busNumber * 3);

    return {
        { busId + "Amount", "AMOUNT", busGroupId, -63, 63, 0, baseOffset + 1, -1 }
    };
}

// ============================================================================
// Standalone Widget Definitions - Patch Edit (DCO, ENV, LFO buttons)
// ============================================================================

inline const std::vector<StandaloneWidgetDescriptor> getDcoStandaloneWidgets()
{
    return {
        { "dco1Init", "I", ModuleIds::kDco1, StandaloneWidgetType::kButton },
        { "dco1Copy", "C", ModuleIds::kDco1, StandaloneWidgetType::kButton },
        { "dco1Paste", "P", ModuleIds::kDco1, StandaloneWidgetType::kButton },
        { "dco2Init", "I", ModuleIds::kDco2, StandaloneWidgetType::kButton },
        { "dco2Copy", "C", ModuleIds::kDco2, StandaloneWidgetType::kButton },
        { "dco2Paste", "P", ModuleIds::kDco2, StandaloneWidgetType::kButton }
    };
}

inline const std::vector<StandaloneWidgetDescriptor> getEnvStandaloneWidgets()
{
    return {
        { "env1Init", "I", ModuleIds::kEnvelope1, StandaloneWidgetType::kButton },
        { "env1Copy", "C", ModuleIds::kEnvelope1, StandaloneWidgetType::kButton },
        { "env1Paste", "P", ModuleIds::kEnvelope1, StandaloneWidgetType::kButton },
        { "env2Init", "I", ModuleIds::kEnvelope2, StandaloneWidgetType::kButton },
        { "env2Copy", "C", ModuleIds::kEnvelope2, StandaloneWidgetType::kButton },
        { "env2Paste", "P", ModuleIds::kEnvelope2, StandaloneWidgetType::kButton },
        { "env3Init", "I", ModuleIds::kEnvelope3, StandaloneWidgetType::kButton },
        { "env3Copy", "C", ModuleIds::kEnvelope3, StandaloneWidgetType::kButton },
        { "env3Paste", "P", ModuleIds::kEnvelope3, StandaloneWidgetType::kButton }
    };
}

inline const std::vector<StandaloneWidgetDescriptor> getLfoStandaloneWidgets()
{
    return {
        { "lfo1Init", "I", ModuleIds::kLfo1, StandaloneWidgetType::kButton },
        { "lfo1Copy", "C", ModuleIds::kLfo1, StandaloneWidgetType::kButton },
        { "lfo1Paste", "P", ModuleIds::kLfo1, StandaloneWidgetType::kButton },
        { "lfo2Init", "I", ModuleIds::kLfo2, StandaloneWidgetType::kButton },
        { "lfo2Copy", "C", ModuleIds::kLfo2, StandaloneWidgetType::kButton },
        { "lfo2Paste", "P", ModuleIds::kLfo2, StandaloneWidgetType::kButton }
    };
}

// ============================================================================
// Standalone Widget Definitions - Patch Manager
// ============================================================================

inline const std::vector<StandaloneWidgetDescriptor> getBankUtilityWidgets()
{
    return {
        { "unlockBank", "UNLOCK", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank0", "0", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank1", "1", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank2", "2", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank3", "3", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank4", "4", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank5", "5", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank6", "6", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank7", "7", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank8", "8", ModuleIds::kBankUtility, StandaloneWidgetType::kButton },
        { "selectBank9", "9", ModuleIds::kBankUtility, StandaloneWidgetType::kButton }
    };
}

inline const std::vector<StandaloneWidgetDescriptor> getInternalPatchesWidgets()
{
    return {
        { "loadPrevInternalPatch", "<", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton },
        { "loadNextInternalPatch", ">", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton },
        { "currentBank", "", ModuleIds::kInternalPatches, StandaloneWidgetType::kNumber },
        { "currentPatch", "", ModuleIds::kInternalPatches, StandaloneWidgetType::kNumber },
        { "initPatch", "INIT", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton },
        { "copyPatch", "COPY", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton },
        { "pastePatch", "PASTE", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton },
        { "storePatch", "STORE", ModuleIds::kInternalPatches, StandaloneWidgetType::kButton }
    };
}

inline const std::vector<StandaloneWidgetDescriptor> getComputerPatchesWidgets()
{
    return {
        { "loadPrevComputerPatch", "<", ModuleIds::kComputerPatches, StandaloneWidgetType::kButton },
        { "loadNextComputerPatch", ">", ModuleIds::kComputerPatches, StandaloneWidgetType::kButton },
        { "selectComputerPatch", "", ModuleIds::kComputerPatches, StandaloneWidgetType::kComboBox },
        { "openComputerPatchFolder", "OPEN", ModuleIds::kComputerPatches, StandaloneWidgetType::kButton },
        { "savePatchAs", "SAVE AS", ModuleIds::kComputerPatches, StandaloneWidgetType::kButton },
        { "saveComputerPatch", "SAVE", ModuleIds::kComputerPatches, StandaloneWidgetType::kButton }
    };
}

// ============================================================================
// Parameter Definitions - Master Edit
// ============================================================================
// TODO: Add Master Edit parameters when available

inline const std::vector<IntParameterDescriptor> getMasterEditIntParameters()
{
    return {};
}

inline const std::vector<ChoiceParameterDescriptor> getMasterEditChoiceParameters()
{
    return {};
}

inline const std::vector<StandaloneWidgetDescriptor> getMasterEditStandaloneWidgets()
{
    return {};
}

// ============================================================================
// Helper Functions - Get All Parameters
// ============================================================================

inline const std::vector<IntParameterDescriptor> getAllIntParameters()
{
    std::vector<IntParameterDescriptor> allParams;

    auto addParams = [&allParams](const std::vector<IntParameterDescriptor>& params)
    {
        allParams.insert(allParams.end(), params.begin(), params.end());
    };

    addParams(getDco1IntParameters());
    addParams(getDco2IntParameters());
    addParams(getVcfVcaIntParameters());
    addParams(getFmTrackIntParameters());
    addParams(getRampPortamentoIntParameters());
    addParams(getEnv1IntParameters());
    addParams(getEnv2IntParameters());
    addParams(getEnv3IntParameters());
    addParams(getLfo1IntParameters());
    addParams(getLfo2IntParameters());

    // Add matrix modulation bus parameters
    for (int bus = 0; bus < 10; ++bus)
    {
        addParams(getMatrixModBusIntParameters(bus));
    }

    addParams(getMasterEditIntParameters());

    return allParams;
}

inline const std::vector<ChoiceParameterDescriptor> getAllChoiceParameters()
{
    std::vector<ChoiceParameterDescriptor> allParams;

    auto addParams = [&allParams](const std::vector<ChoiceParameterDescriptor>& params)
    {
        allParams.insert(allParams.end(), params.begin(), params.end());
    };

    addParams(getDco1ChoiceParameters());
    addParams(getDco2ChoiceParameters());
    addParams(getVcfVcaChoiceParameters());
    addParams(getFmTrackChoiceParameters());
    addParams(getRampPortamentoChoiceParameters());
    addParams(getEnv1ChoiceParameters());
    addParams(getEnv2ChoiceParameters());
    addParams(getEnv3ChoiceParameters());
    addParams(getLfo1ChoiceParameters());
    addParams(getLfo2ChoiceParameters());

    // Add matrix modulation bus parameters
    for (int bus = 0; bus < 10; ++bus)
    {
        addParams(getMatrixModBusChoiceParameters(bus));
    }

    addParams(getMasterEditChoiceParameters());

    return allParams;
}

inline const std::vector<StandaloneWidgetDescriptor> getAllStandaloneWidgets()
{
    std::vector<StandaloneWidgetDescriptor> allWidgets;

    auto addWidgets = [&allWidgets](const std::vector<StandaloneWidgetDescriptor>& widgets)
    {
        allWidgets.insert(allWidgets.end(), widgets.begin(), widgets.end());
    };

    addWidgets(getDcoStandaloneWidgets());
    addWidgets(getEnvStandaloneWidgets());
    addWidgets(getLfoStandaloneWidgets());
    addWidgets(getBankUtilityWidgets());
    addWidgets(getInternalPatchesWidgets());
    addWidgets(getComputerPatchesWidgets());
    addWidgets(getMasterEditStandaloneWidgets());

    return allWidgets;
}

} // namespace SynthDescriptors

