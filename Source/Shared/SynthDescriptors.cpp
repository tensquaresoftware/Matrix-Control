#include "SynthDescriptors.h"

#include <array>
#include <initializer_list>

namespace SynthDescriptors
{

// ============================================================================
// Helper function for StringArray construction
// ============================================================================

namespace
{
    juce::StringArray makeStringArray(std::initializer_list<const char*> strings)
    {
        juce::StringArray result;
        for (const char* str : strings)
        {
            result.add(str);
        }
        return result;
    }
}

// ============================================================================
// Data Definitions - Groups
// ============================================================================

const std::vector<GroupDescriptor> kAllGroups = {
    // Modes
    {
        .parentId = "",
        .groupId = GroupIds::kMaster,
        .displayName = "MASTER"
    },
    {
        .parentId = "",
        .groupId = GroupIds::kPatch,
        .displayName = "PATCH"
    },

    // Sections
    {
        .parentId = GroupIds::kMaster,
        .groupId = SectionIds::kMasterEdit,
        .displayName = "MASTER EDIT"
    },
    {
        .parentId = GroupIds::kPatch,
        .groupId = SectionIds::kPatchEdit,
        .displayName = "PATCH EDIT"
    },
    {
        .parentId = GroupIds::kPatch,
        .groupId = SectionIds::kMatrixModulation,
        .displayName = "MATRIX MODULATION"
    },
    {
        .parentId = GroupIds::kPatch,
        .groupId = SectionIds::kPatchManager,
        .displayName = "PATCH MANAGER"
    },

    // Master Edit Modules
    {
        .parentId = SectionIds::kMasterEdit,
        .groupId = ModuleIds::kMidi,
        .displayName = "MIDI"
    },
    {
        .parentId = SectionIds::kMasterEdit,
        .groupId = ModuleIds::kVibrato,
        .displayName = "VIBRATO"
    },
    {
        .parentId = SectionIds::kMasterEdit,
        .groupId = ModuleIds::kMisc,
        .displayName = "MISC"
    },

    // Patch Edit Modules
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kDco1,
        .displayName = "DCO 1"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kDco2,
        .displayName = "DCO 2"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kVcfVca,
        .displayName = "VCF/VCA"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kFmTrack,
        .displayName = "FM/TRACK"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kRampPortamento,
        .displayName = "RAMP/PORTAMENTO"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kEnvelope1,
        .displayName = "ENV 1"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kEnvelope2,
        .displayName = "ENV 2"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kEnvelope3,
        .displayName = "ENV 3"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kLfo1,
        .displayName = "LFO 1"
    },
    {
        .parentId = SectionIds::kPatchEdit,
        .groupId = ModuleIds::kLfo2,
        .displayName = "LFO 2"
    },

    // Matrix Modulation Busses
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus0,
        .displayName = "MATRIX MODULATION BUS 0"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus1,
        .displayName = "MATRIX MODULATION BUS 1"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus2,
        .displayName = "MATRIX MODULATION BUS 2"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus3,
        .displayName = "MATRIX MODULATION BUS 3"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus4,
        .displayName = "MATRIX MODULATION BUS 4"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus5,
        .displayName = "MATRIX MODULATION BUS 5"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus6,
        .displayName = "MATRIX MODULATION BUS 6"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus7,
        .displayName = "MATRIX MODULATION BUS 7"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus8,
        .displayName = "MATRIX MODULATION BUS 8"
    },
    {
        .parentId = SectionIds::kMatrixModulation,
        .groupId = MatrixModBusIds::kMatrixModBus9,
        .displayName = "MATRIX MODULATION BUS 9"
    },

    // Patch Manager Modules
    {
        .parentId = SectionIds::kPatchManager,
        .groupId = ModuleIds::kBankUtility,
        .displayName = "BANK UTILITY"
    },
    {
        .parentId = SectionIds::kPatchManager,
        .groupId = ModuleIds::kInternalPatches,
        .displayName = "INTERNAL PATCHES"
    },
    {
        .parentId = SectionIds::kPatchManager,
        .groupId = ModuleIds::kComputerPatches,
        .displayName = "COMPUTER PATCHES"
    }
};

// ============================================================================
// Data Definitions - Master Edit Parameters
// ============================================================================

const std::vector<IntParameterDescriptor> kMasterEditIntParameters = {};

const std::vector<ChoiceParameterDescriptor> kMasterEditChoiceParameters = {};

// ============================================================================
// Data Definitions - Patch Edit Parameters - DCO 1 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kDco1IntParameters = {
    {
        .parameterId = ParameterIds::kDco1Frequency,
        .displayName = DisplayNames::kDco1Frequency,
        .parentGroupId = ModuleIds::kDco1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 9,
        .sysExId = 0
    },
    {
        .parameterId = ParameterIds::kDco1FrequencyModByLfo1,
        .displayName = DisplayNames::kDco1FrequencyModByLfo1,
        .parentGroupId = ModuleIds::kDco1,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 86,
        .sysExId = 1
    },
    {
        .parameterId = ParameterIds::kDco1PulseWidth,
        .displayName = DisplayNames::kDco1PulseWidth,
        .parentGroupId = ModuleIds::kDco1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 31,
        .sysExOffset = 11,
        .sysExId = 3
    },
    {
        .parameterId = ParameterIds::kDco1PulseWidthModByLfo2,
        .displayName = DisplayNames::kDco1PulseWidthModByLfo2,
        .parentGroupId = ModuleIds::kDco1,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 87,
        .sysExId = 4
    },
    {
        .parameterId = ParameterIds::kDco1WaveShape,
        .displayName = DisplayNames::kDco1WaveShape,
        .parentGroupId = ModuleIds::kDco1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 31,
        .sysExOffset = 10,
        .sysExId = 5
    }
};

const std::vector<ChoiceParameterDescriptor> kDco1ChoiceParameters = {
    {
        .parameterId = ParameterIds::kDco1Sync,
        .displayName = DisplayNames::kDco1Sync,
        .parentGroupId = ModuleIds::kDco1,
        .choices = makeStringArray({ ChoiceLists::kSyncOff, ChoiceLists::kSyncSoft, ChoiceLists::kSyncMedium, ChoiceLists::kSyncHard }),
        .defaultIndex = 0,
        .sysExOffset = 25,
        .sysExId = 2
    },
    {
        .parameterId = ParameterIds::kDco1WaveSelect,
        .displayName = DisplayNames::kDco1WaveSelect,
        .parentGroupId = ModuleIds::kDco1,
        .choices = makeStringArray({ ChoiceLists::kWaveSelectOff, ChoiceLists::kWaveSelectPulse, ChoiceLists::kWaveSelectWave, ChoiceLists::kWaveSelectBoth }),
        .defaultIndex = 2,
        .sysExOffset = 13,
        .sysExId = 6
    },
    {
        .parameterId = ParameterIds::kDco1Levers,
        .displayName = DisplayNames::kDco1Levers,
        .parentGroupId = ModuleIds::kDco1,
        .choices = makeStringArray({ ChoiceLists::kLeversOff, ChoiceLists::kLeversL1Bend, ChoiceLists::kLeversL2Vib, ChoiceLists::kLeversBoth }),
        .defaultIndex = 3,
        .sysExOffset = 12,
        .sysExId = 7
    },
    {
        .parameterId = ParameterIds::kDco1KeyboardPortamento,
        .displayName = DisplayNames::kDco1KeyboardPortamento,
        .parentGroupId = ModuleIds::kDco1,
        .choices = makeStringArray({ ChoiceLists::kKeyboardPortamentoKeybd, ChoiceLists::kKeyboardPortamentoPorta }),
        .defaultIndex = 0,
        .sysExOffset = 21,
        .sysExId = 8
    },
    {
        .parameterId = ParameterIds::kDco1KeyClick,
        .displayName = DisplayNames::kDco1KeyClick,
        .parentGroupId = ModuleIds::kDco1,
        .choices = makeStringArray({ ChoiceLists::kOnOffChoiceOff, ChoiceLists::kOnOffChoiceOn }),
        .defaultIndex = 0,
        .sysExOffset = 22,
        .sysExId = 9
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - DCO 2 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kDco2IntParameters = {
    {
        .parameterId = ParameterIds::kDco2Frequency,
        .displayName = DisplayNames::kDco2Frequency,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 14,
        .sysExId = 10
    },
    {
        .parameterId = ParameterIds::kDco2FrequencyModByLfo1,
        .displayName = DisplayNames::kDco2FrequencyModByLfo1,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 88,
        .sysExId = 11
    },
    {
        .parameterId = ParameterIds::kDco2Detune,
        .displayName = DisplayNames::kDco2Detune,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = -31,
        .maxValue = 31,
        .defaultValue = 2,
        .sysExOffset = 19,
        .sysExId = 12
    },
    {
        .parameterId = ParameterIds::kDco2PulseWidth,
        .displayName = DisplayNames::kDco2PulseWidth,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 24,
        .sysExOffset = 16,
        .sysExId = 13
    },
    {
        .parameterId = ParameterIds::kDco2PulseWidthModByLfo2,
        .displayName = DisplayNames::kDco2PulseWidthModByLfo2,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 89,
        .sysExId = 14
    },
    {
        .parameterId = ParameterIds::kDco2WaveShape,
        .displayName = DisplayNames::kDco2WaveShape,
        .parentGroupId = ModuleIds::kDco2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 31,
        .sysExOffset = 15,
        .sysExId = 15
    }
};

const std::vector<ChoiceParameterDescriptor> kDco2ChoiceParameters = {
    {
        .parameterId = ParameterIds::kDco2WaveSelect,
        .displayName = DisplayNames::kDco2WaveSelect,
        .parentGroupId = ModuleIds::kDco2,
        .choices = makeStringArray({ ChoiceLists::kWaveSelectOff, ChoiceLists::kWaveSelectPulse, ChoiceLists::kWaveSelectWave, ChoiceLists::kWaveSelectBoth, ChoiceLists::kWaveSelectNoise }),
        .defaultIndex = 1,
        .sysExOffset = 18,
        .sysExId = 16
    },
    {
        .parameterId = ParameterIds::kDco2Levers,
        .displayName = DisplayNames::kDco2Levers,
        .parentGroupId = ModuleIds::kDco2,
        .choices = makeStringArray({ ChoiceLists::kLeversOff, ChoiceLists::kLeversL1Bend, ChoiceLists::kLeversL2Vib, ChoiceLists::kLeversBoth }),
        .defaultIndex = 3,
        .sysExOffset = 17,
        .sysExId = 17
    },
    {
        .parameterId = ParameterIds::kDco2KeyboardPortamento,
        .displayName = DisplayNames::kDco2KeyboardPortamento,
        .parentGroupId = ModuleIds::kDco2,
        .choices = makeStringArray({ ChoiceLists::kKeyboardPortamentoOff, ChoiceLists::kKeyboardPortamentoPorta, ChoiceLists::kKeyboardPortamentoKeybd }),
        .defaultIndex = 2,
        .sysExOffset = 23,
        .sysExId = 18
    },
    {
        .parameterId = ParameterIds::kDco2KeyClick,
        .displayName = DisplayNames::kDco2KeyClick,
        .parentGroupId = ModuleIds::kDco2,
        .choices = makeStringArray({ ChoiceLists::kOnOffChoiceOff, ChoiceLists::kOnOffChoiceOn }),
        .defaultIndex = 0,
        .sysExOffset = 24,
        .sysExId = 19
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - VCF/VCA Module
// ============================================================================

const std::vector<IntParameterDescriptor> kVcfVcaIntParameters = {
    {
        .parameterId = ParameterIds::kVcfBalance,
        .displayName = DisplayNames::kVcfBalance,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 31,
        .sysExOffset = 20,
        .sysExId = 20
    },
    {
        .parameterId = ParameterIds::kVcfFequency,
        .displayName = DisplayNames::kVcfFequency,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = 0,
        .maxValue = 127,
        .defaultValue = 55,
        .sysExOffset = 26,
        .sysExId = 21
    },
    {
        .parameterId = ParameterIds::kVcfFrequencyModByEnv1,
        .displayName = DisplayNames::kVcfFrequencyModByEnv1,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 42,
        .sysExOffset = 90,
        .sysExId = 22
    },
    {
        .parameterId = ParameterIds::kVcfFrequencyModByPressure,
        .displayName = DisplayNames::kVcfFrequencyModByPressure,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 91,
        .sysExId = 23
    },
    {
        .parameterId = ParameterIds::kVcfResonance,
        .displayName = DisplayNames::kVcfResonance,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 27,
        .sysExId = 24
    },
    {
        .parameterId = ParameterIds::kVca1Volume,
        .displayName = DisplayNames::kVca1Volume,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 31,
        .sysExId = 27
    },
    {
        .parameterId = ParameterIds::kVca1ModByVelocity,
        .displayName = DisplayNames::kVca1ModByVelocity,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 92,
        .sysExId = 28
    },
    {
        .parameterId = ParameterIds::kVca2ModByEnv2,
        .displayName = DisplayNames::kVca2ModByEnv2,
        .parentGroupId = ModuleIds::kVcfVca,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 93,
        .sysExId = 29
    }
};

const std::vector<ChoiceParameterDescriptor> kVcfVcaChoiceParameters = {
    {
        .parameterId = ParameterIds::kVcfLevers,
        .displayName = DisplayNames::kVcfLevers,
        .parentGroupId = ModuleIds::kVcfVca,
        .choices = makeStringArray({ ChoiceLists::kLeversOff, ChoiceLists::kLeversL1Bend, ChoiceLists::kLeversL2Vib, ChoiceLists::kLeversBoth }),
        .defaultIndex = 0,
        .sysExOffset = 28,
        .sysExId = 25
    },
    {
        .parameterId = ParameterIds::kVcfKeyboardPortamento,
        .displayName = DisplayNames::kVcfKeyboardPortamento,
        .parentGroupId = ModuleIds::kVcfVca,
        .choices = makeStringArray({ ChoiceLists::kKeyboardPortamentoOff, ChoiceLists::kKeyboardPortamentoPorta, ChoiceLists::kKeyboardPortamentoKeybd }),
        .defaultIndex = 2,
        .sysExOffset = 29,
        .sysExId = 26
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - FM/TRACK Module
// ============================================================================

const std::vector<IntParameterDescriptor> kFmTrackIntParameters = {
    {
        .parameterId = ParameterIds::kFmAmount,
        .displayName = DisplayNames::kFmAmount,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 30,
        .sysExId = 30
    },
    {
        .parameterId = ParameterIds::kFmModByEnv3,
        .displayName = DisplayNames::kFmModByEnv3,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 100,
        .sysExId = 31
    },
    {
        .parameterId = ParameterIds::kFmModByPressure,
        .displayName = DisplayNames::kFmModByPressure,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 101,
        .sysExId = 32
    },
    {
        .parameterId = ParameterIds::kTrackPoint1,
        .displayName = DisplayNames::kTrackPoint1,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 77,
        .sysExId = 34
    },
    {
        .parameterId = ParameterIds::kTrackPoint2,
        .displayName = DisplayNames::kTrackPoint2,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 15,
        .sysExOffset = 78,
        .sysExId = 35
    },
    {
        .parameterId = ParameterIds::kTrackPoint3,
        .displayName = DisplayNames::kTrackPoint3,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 31,
        .sysExOffset = 79,
        .sysExId = 36
    },
    {
        .parameterId = ParameterIds::kTrackPoint4,
        .displayName = DisplayNames::kTrackPoint4,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 47,
        .sysExOffset = 80,
        .sysExId = 37
    },
    {
        .parameterId = ParameterIds::kTrackPoint5,
        .displayName = DisplayNames::kTrackPoint5,
        .parentGroupId = ModuleIds::kFmTrack,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 81,
        .sysExId = 38
    }
};

const std::vector<ChoiceParameterDescriptor> kFmTrackChoiceParameters = {
    {
        .parameterId = ParameterIds::kTrackInput,
        .displayName = DisplayNames::kTrackInput,
        .parentGroupId = ModuleIds::kFmTrack,
        .choices = makeStringArray({ ChoiceLists::kSourceNone, ChoiceLists::kSourceEnv1, ChoiceLists::kSourceEnv2, ChoiceLists::kSourceEnv3, 
                                     ChoiceLists::kSourceLfo1, ChoiceLists::kSourceLfo2, ChoiceLists::kSourceVibrato, ChoiceLists::kSourceRamp1, ChoiceLists::kSourceRamp2,
                                     ChoiceLists::kSourceKeybd, ChoiceLists::kSourcePorta, ChoiceLists::kSourceTrack, ChoiceLists::kSourceKbGate, 
                                     ChoiceLists::kSourceVelocity, ChoiceLists::kSourceRelVel, ChoiceLists::kSourcePressure,
                                     ChoiceLists::kSourcePedal1, ChoiceLists::kSourcePedal2, ChoiceLists::kSourceLever1, ChoiceLists::kSourceLever2, ChoiceLists::kSourceLever3 }),
        .defaultIndex = 9,
        .sysExOffset = 76,
        .sysExId = 33
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - RAMP/PORTAMENTO Module
// ============================================================================

const std::vector<IntParameterDescriptor> kRampPortamentoIntParameters = {
    {
        .parameterId = ParameterIds::kRamp1Rate,
        .displayName = DisplayNames::kRamp1Rate,
        .parentGroupId = ModuleIds::kRampPortamento,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 82,
        .sysExId = 40
    },
    {
        .parameterId = ParameterIds::kRamp2Rate,
        .displayName = DisplayNames::kRamp2Rate,
        .parentGroupId = ModuleIds::kRampPortamento,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 84,
        .sysExId = 42
    },
    {
        .parameterId = ParameterIds::kPortamentoRate,
        .displayName = DisplayNames::kPortamentoRate,
        .parentGroupId = ModuleIds::kRampPortamento,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 32,
        .sysExId = 44
    },
    {
        .parameterId = ParameterIds::kPortamentoModByVelocity,
        .displayName = DisplayNames::kPortamentoModByVelocity,
        .parentGroupId = ModuleIds::kRampPortamento,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 99,
        .sysExId = 45
    }
};

const std::vector<ChoiceParameterDescriptor> kRampPortamentoChoiceParameters = {
    {
        .parameterId = ParameterIds::kRamp1Trigger,
        .displayName = DisplayNames::kRamp1Trigger,
        .parentGroupId = ModuleIds::kRampPortamento,
        .choices = makeStringArray({ ChoiceLists::kRampTriggerStrig, ChoiceLists::kRampTriggerMtrig, ChoiceLists::kRampTriggerExtrig, ChoiceLists::kRampTriggerGatedx }),
        .defaultIndex = 0,
        .sysExOffset = 83,
        .sysExId = 41
    },
    {
        .parameterId = ParameterIds::kRamp2Trigger,
        .displayName = DisplayNames::kRamp2Trigger,
        .parentGroupId = ModuleIds::kRampPortamento,
        .choices = makeStringArray({ ChoiceLists::kRampTriggerStrig, ChoiceLists::kRampTriggerMtrig, ChoiceLists::kRampTriggerExtrig, ChoiceLists::kRampTriggerGatedx }),
        .defaultIndex = 0,
        .sysExOffset = 85,
        .sysExId = 43
    },
    {
        .parameterId = ParameterIds::kPortamentoMode,
        .displayName = DisplayNames::kPortamentoMode,
        .parentGroupId = ModuleIds::kRampPortamento,
        .choices = makeStringArray({ ChoiceLists::kPortamentoModeLinear, ChoiceLists::kPortamentoModeConst, ChoiceLists::kPortamentoModeExpo }),
        .defaultIndex = 0,
        .sysExOffset = 33,
        .sysExId = 46
    },
    {
        .parameterId = ParameterIds::kPortamentoLegato,
        .displayName = DisplayNames::kPortamentoLegato,
        .parentGroupId = ModuleIds::kRampPortamento,
        .choices = makeStringArray({ ChoiceLists::kOnOffChoiceOff, ChoiceLists::kOnOffChoiceOn }),
        .defaultIndex = 0,
        .sysExOffset = 34,
        .sysExId = 47
    },
    {
        .parameterId = ParameterIds::kPortamentoKeyboardMode,
        .displayName = DisplayNames::kPortamentoKeyboardMode,
        .parentGroupId = ModuleIds::kRampPortamento,
        .choices = makeStringArray({ ChoiceLists::kPortamentoKeyboardModeRotate, ChoiceLists::kPortamentoKeyboardModeReasgn, 
                                     ChoiceLists::kPortamentoKeyboardModeUnison, ChoiceLists::kPortamentoKeyboardModeRearob }),
        .defaultIndex = 1,
        .sysExOffset = 8,
        .sysExId = 48
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - ENV 1 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kEnv1IntParameters = {
    {
        .parameterId = ParameterIds::kEnv1Delay,
        .displayName = DisplayNames::kEnv1Delay,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 50,
        .sysExId = 50
    },
    {
        .parameterId = ParameterIds::kEnv1Attack,
        .displayName = DisplayNames::kEnv1Attack,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 51,
        .sysExId = 51
    },
    {
        .parameterId = ParameterIds::kEnv1Decay,
        .displayName = DisplayNames::kEnv1Decay,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 10,
        .sysExOffset = 52,
        .sysExId = 52
    },
    {
        .parameterId = ParameterIds::kEnv1Sustain,
        .displayName = DisplayNames::kEnv1Sustain,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 50,
        .sysExOffset = 53,
        .sysExId = 53
    },
    {
        .parameterId = ParameterIds::kEnv1Release,
        .displayName = DisplayNames::kEnv1Release,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 10,
        .sysExOffset = 54,
        .sysExId = 54
    },
    {
        .parameterId = ParameterIds::kEnv1Amplitude,
        .displayName = DisplayNames::kEnv1Amplitude,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 40,
        .sysExOffset = 55,
        .sysExId = 55
    },
    {
        .parameterId = ParameterIds::kEnv1AmplitudeModByVelocity,
        .displayName = DisplayNames::kEnv1AmplitudeModByVelocity,
        .parentGroupId = ModuleIds::kEnvelope1,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 94,
        .sysExId = 56
    }
};

const std::vector<ChoiceParameterDescriptor> kEnv1ChoiceParameters = {
    {
        .parameterId = ParameterIds::kEnv1TriggerMode,
        .displayName = DisplayNames::kEnv1TriggerMode,
        .parentGroupId = ModuleIds::kEnvelope1,
        .choices = makeStringArray({ ChoiceLists::kTriggerModeStrig, ChoiceLists::kTriggerModeMtrig, ChoiceLists::kTriggerModeSreset, 
                                     ChoiceLists::kTriggerModeMreset, ChoiceLists::kTriggerModeXtrig, ChoiceLists::kTriggerModeXmtrig, 
                                     ChoiceLists::kTriggerModeXreset, ChoiceLists::kTriggerModeXmrst }),
        .defaultIndex = 0,
        .sysExOffset = 49,
        .sysExId = 57
    },
    {
        .parameterId = ParameterIds::kEnv1EnvelopeMode,
        .displayName = DisplayNames::kEnv1EnvelopeMode,
        .parentGroupId = ModuleIds::kEnvelope1,
        .choices = makeStringArray({ ChoiceLists::kEnvelopeModeNormal, ChoiceLists::kEnvelopeModeDadr, ChoiceLists::kEnvelopeModeFree, ChoiceLists::kEnvelopeModeBoth }),
        .defaultIndex = 0,
        .sysExOffset = 57,
        .sysExId = 58
    },
    {
        .parameterId = ParameterIds::kEnv1Lfo1Trigger,
        .displayName = DisplayNames::kEnv1Lfo1Trigger,
        .parentGroupId = ModuleIds::kEnvelope1,
        .choices = makeStringArray({ ChoiceLists::kLfo1TriggerNormal, ChoiceLists::kLfo1TriggerLfo1, ChoiceLists::kLfo1TriggerGLfo1 }),
        .defaultIndex = 0,
        .sysExOffset = 56,
        .sysExId = 59
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - ENV 2 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kEnv2IntParameters = {
    {
        .parameterId = ParameterIds::kEnv2Delay,
        .displayName = DisplayNames::kEnv2Delay,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 59,
        .sysExId = 60
    },
    {
        .parameterId = ParameterIds::kEnv2Attack,
        .displayName = DisplayNames::kEnv2Attack,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 60,
        .sysExId = 61
    },
    {
        .parameterId = ParameterIds::kEnv2Decay,
        .displayName = DisplayNames::kEnv2Decay,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 10,
        .sysExOffset = 61,
        .sysExId = 62
    },
    {
        .parameterId = ParameterIds::kEnv2Sustain,
        .displayName = DisplayNames::kEnv2Sustain,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 50,
        .sysExOffset = 62,
        .sysExId = 63
    },
    {
        .parameterId = ParameterIds::kEnv2Release,
        .displayName = DisplayNames::kEnv2Release,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 10,
        .sysExOffset = 63,
        .sysExId = 64
    },
    {
        .parameterId = ParameterIds::kEnv2Amplitude,
        .displayName = DisplayNames::kEnv2Amplitude,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 40,
        .sysExOffset = 64,
        .sysExId = 65
    },
    {
        .parameterId = ParameterIds::kEnv2AmplitudeModByVelocity,
        .displayName = DisplayNames::kEnv2AmplitudeModByVelocity,
        .parentGroupId = ModuleIds::kEnvelope2,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 95,
        .sysExId = 66
    }
};

const std::vector<ChoiceParameterDescriptor> kEnv2ChoiceParameters = {
    {
        .parameterId = ParameterIds::kEnv2TriggerMode,
        .displayName = DisplayNames::kEnv2TriggerMode,
        .parentGroupId = ModuleIds::kEnvelope2,
        .choices = makeStringArray({ ChoiceLists::kTriggerModeStrig, ChoiceLists::kTriggerModeMtrig, ChoiceLists::kTriggerModeSreset, 
                                     ChoiceLists::kTriggerModeMreset, ChoiceLists::kTriggerModeXtrig, ChoiceLists::kTriggerModeXmtrig, 
                                     ChoiceLists::kTriggerModeXreset, ChoiceLists::kTriggerModeXmrst }),
        .defaultIndex = 0,
        .sysExOffset = 58,
        .sysExId = 67
    },
    {
        .parameterId = ParameterIds::kEnv2EnvelopeMode,
        .displayName = DisplayNames::kEnv2EnvelopeMode,
        .parentGroupId = ModuleIds::kEnvelope2,
        .choices = makeStringArray({ ChoiceLists::kEnvelopeModeNormal, ChoiceLists::kEnvelopeModeDadr, ChoiceLists::kEnvelopeModeFree, ChoiceLists::kEnvelopeModeBoth }),
        .defaultIndex = 0,
        .sysExOffset = 66,
        .sysExId = 68
    },
    {
        .parameterId = ParameterIds::kEnv2Lfo1Trigger,
        .displayName = DisplayNames::kEnv2Lfo1Trigger,
        .parentGroupId = ModuleIds::kEnvelope2,
        .choices = makeStringArray({ ChoiceLists::kLfo1TriggerNormal, ChoiceLists::kLfo1TriggerLfo1, ChoiceLists::kLfo1TriggerGLfo1 }),
        .defaultIndex = 0,
        .sysExOffset = 65,
        .sysExId = 69
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - ENV 3 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kEnv3IntParameters = {
    {
        .parameterId = ParameterIds::kEnv3Delay,
        .displayName = DisplayNames::kEnv3Delay,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 68,
        .sysExId = 70
    },
    {
        .parameterId = ParameterIds::kEnv3Attack,
        .displayName = DisplayNames::kEnv3Attack,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 69,
        .sysExId = 71
    },
    {
        .parameterId = ParameterIds::kEnv3Decay,
        .displayName = DisplayNames::kEnv3Decay,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 20,
        .sysExOffset = 70,
        .sysExId = 72
    },
    {
        .parameterId = ParameterIds::kEnv3Sustain,
        .displayName = DisplayNames::kEnv3Sustain,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 71,
        .sysExId = 73
    },
    {
        .parameterId = ParameterIds::kEnv3Release,
        .displayName = DisplayNames::kEnv3Release,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 20,
        .sysExOffset = 72,
        .sysExId = 74
    },
    {
        .parameterId = ParameterIds::kEnv3Amplitude,
        .displayName = DisplayNames::kEnv3Amplitude,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 40,
        .sysExOffset = 73,
        .sysExId = 75
    },
    {
        .parameterId = ParameterIds::kEnv3AmplitudeModByVelocity,
        .displayName = DisplayNames::kEnv3AmplitudeModByVelocity,
        .parentGroupId = ModuleIds::kEnvelope3,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 96,
        .sysExId = 76
    }
};

const std::vector<ChoiceParameterDescriptor> kEnv3ChoiceParameters = {
    {
        .parameterId = ParameterIds::kEnv3TriggerMode,
        .displayName = DisplayNames::kEnv3TriggerMode,
        .parentGroupId = ModuleIds::kEnvelope3,
        .choices = makeStringArray({ ChoiceLists::kTriggerModeStrig, ChoiceLists::kTriggerModeMtrig, ChoiceLists::kTriggerModeSreset, 
                                     ChoiceLists::kTriggerModeMreset, ChoiceLists::kTriggerModeXtrig, ChoiceLists::kTriggerModeXmtrig, 
                                     ChoiceLists::kTriggerModeXreset, ChoiceLists::kTriggerModeXmrst }),
        .defaultIndex = 0,
        .sysExOffset = 67,
        .sysExId = 77
    },
    {
        .parameterId = ParameterIds::kEnv3EnvelopeMode,
        .displayName = DisplayNames::kEnv3EnvelopeMode,
        .parentGroupId = ModuleIds::kEnvelope3,
        .choices = makeStringArray({ ChoiceLists::kEnvelopeModeNormal, ChoiceLists::kEnvelopeModeDadr, ChoiceLists::kEnvelopeModeFree, ChoiceLists::kEnvelopeModeBoth }),
        .defaultIndex = 0,
        .sysExOffset = 75,
        .sysExId = 78
    },
    {
        .parameterId = ParameterIds::kEnv3Lfo1Trigger,
        .displayName = DisplayNames::kEnv3Lfo1Trigger,
        .parentGroupId = ModuleIds::kEnvelope3,
        .choices = makeStringArray({ ChoiceLists::kLfo1TriggerNormal, ChoiceLists::kLfo1TriggerLfo1, ChoiceLists::kLfo1TriggerGatedLfo1Trigger }),
        .defaultIndex = 0,
        .sysExOffset = 74,
        .sysExId = 79
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - LFO 1 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kLfo1IntParameters = {
    {
        .parameterId = ParameterIds::kLfo1Speed,
        .displayName = DisplayNames::kLfo1Speed,
        .parentGroupId = ModuleIds::kLfo1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 40,
        .sysExOffset = 35,
        .sysExId = 80
    },
    {
        .parameterId = ParameterIds::kLfo1SpeedModByPressure,
        .displayName = DisplayNames::kLfo1SpeedModByPressure,
        .parentGroupId = ModuleIds::kLfo1,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 102,
        .sysExId = 81
    },
    {
        .parameterId = ParameterIds::kLfo1RetriggerPoint,
        .displayName = DisplayNames::kLfo1RetriggerPoint,
        .parentGroupId = ModuleIds::kLfo1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 39,
        .sysExId = 83
    },
    {
        .parameterId = ParameterIds::kLfo1Amplitude,
        .displayName = DisplayNames::kLfo1Amplitude,
        .parentGroupId = ModuleIds::kLfo1,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 41,
        .sysExId = 84
    },
    {
        .parameterId = ParameterIds::kLfo1AmplitudeModByRamp1,
        .displayName = DisplayNames::kLfo1AmplitudeModByRamp1,
        .parentGroupId = ModuleIds::kLfo1,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 97,
        .sysExId = 85
    }
};

const std::vector<ChoiceParameterDescriptor> kLfo1ChoiceParameters = {
    {
        .parameterId = ParameterIds::kLfo1Waveform,
        .displayName = DisplayNames::kLfo1Waveform,
        .parentGroupId = ModuleIds::kLfo1,
        .choices = makeStringArray({ ChoiceLists::kLfoWaveformTriangle, ChoiceLists::kLfoWaveformUpsaw, ChoiceLists::kLfoWaveformDnsaw, 
                                     ChoiceLists::kLfoWaveformSquare, ChoiceLists::kLfoWaveformRandom, ChoiceLists::kLfoWaveformNoise, ChoiceLists::kLfoWaveformSampled }),
        .defaultIndex = 0,
        .sysExOffset = 38,
        .sysExId = 82
    },
    {
        .parameterId = ParameterIds::kLfo1TriggerMode,
        .displayName = DisplayNames::kLfo1TriggerMode,
        .parentGroupId = ModuleIds::kLfo1,
        .choices = makeStringArray({ ChoiceLists::kLfoTriggerModeOff, ChoiceLists::kLfoTriggerModeStrig, ChoiceLists::kLfoTriggerModeMtrig, ChoiceLists::kLfoTriggerModeXtrig }),
        .defaultIndex = 0,
        .sysExOffset = 36,
        .sysExId = 86
    },
    {
        .parameterId = ParameterIds::kLfo1Lag,
        .displayName = DisplayNames::kLfo1Lag,
        .parentGroupId = ModuleIds::kLfo1,
        .choices = makeStringArray({ ChoiceLists::kOnOffChoiceOff, ChoiceLists::kOnOffChoiceOn }),
        .defaultIndex = 0,
        .sysExOffset = 37,
        .sysExId = 87
    },
    {
        .parameterId = ParameterIds::kLfo1SampleInput,
        .displayName = DisplayNames::kLfo1SampleInput,
        .parentGroupId = ModuleIds::kLfo1,
        .choices = makeStringArray({ ChoiceLists::kSourceNone, ChoiceLists::kSourceEnv1, ChoiceLists::kSourceEnv2, ChoiceLists::kSourceEnv3, 
                                     ChoiceLists::kSourceLfo1, ChoiceLists::kSourceLfo2, ChoiceLists::kSourceVibrato, ChoiceLists::kSourceRamp1, ChoiceLists::kSourceRamp2,
                                     ChoiceLists::kSourceKeybd, ChoiceLists::kSourcePorta, ChoiceLists::kSourceTrack, ChoiceLists::kSourceKbGate, 
                                     ChoiceLists::kSourceVelocity, ChoiceLists::kSourceRelVel, ChoiceLists::kSourcePressure,
                                     ChoiceLists::kSourcePedal1, ChoiceLists::kSourcePedal2, ChoiceLists::kSourceLever1, ChoiceLists::kSourceLever2, ChoiceLists::kSourceLever3 }),
        .defaultIndex = 9,
        .sysExOffset = 40,
        .sysExId = 88
    }
};

// ============================================================================
// Data Definitions - Patch Edit Parameters - LFO 2 Module
// ============================================================================

const std::vector<IntParameterDescriptor> kLfo2IntParameters = {
    {
        .parameterId = ParameterIds::kLfo2Speed,
        .displayName = DisplayNames::kLfo2Speed,
        .parentGroupId = ModuleIds::kLfo2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 30,
        .sysExOffset = 42,
        .sysExId = 90
    },
    {
        .parameterId = ParameterIds::kLfo2SpeedModByKeyboard,
        .displayName = DisplayNames::kLfo2SpeedModByKeyboard,
        .parentGroupId = ModuleIds::kLfo2,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 103,
        .sysExId = 91
    },
    {
        .parameterId = ParameterIds::kLfo2RetriggerPoint,
        .displayName = DisplayNames::kLfo2RetriggerPoint,
        .parentGroupId = ModuleIds::kLfo2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 46,
        .sysExId = 93
    },
    {
        .parameterId = ParameterIds::kLfo2Amplitude,
        .displayName = DisplayNames::kLfo2Amplitude,
        .parentGroupId = ModuleIds::kLfo2,
        .minValue = 0,
        .maxValue = 63,
        .defaultValue = 0,
        .sysExOffset = 48,
        .sysExId = 94
    },
    {
        .parameterId = ParameterIds::kLfo2AmplitudeModByRamp2,
        .displayName = DisplayNames::kLfo2AmplitudeModByRamp2,
        .parentGroupId = ModuleIds::kLfo2,
        .minValue = -63,
        .maxValue = 63,
        .defaultValue = 63,
        .sysExOffset = 98,
        .sysExId = 95
    }
};

const std::vector<ChoiceParameterDescriptor> kLfo2ChoiceParameters = {
    {
        .parameterId = ParameterIds::kLfo2Waveform,
        .displayName = DisplayNames::kLfo2Waveform,
        .parentGroupId = ModuleIds::kLfo2,
        .choices = makeStringArray({ ChoiceLists::kLfoWaveformTriangle, ChoiceLists::kLfoWaveformUpsaw, ChoiceLists::kLfoWaveformDnsaw, 
                                     ChoiceLists::kLfoWaveformSquare, ChoiceLists::kLfoWaveformRandom, ChoiceLists::kLfoWaveformNoise, ChoiceLists::kLfoWaveformSampled }),
        .defaultIndex = 0,
        .sysExOffset = 45,
        .sysExId = 92
    },
    {
        .parameterId = ParameterIds::kLfo2TriggerMode,
        .displayName = DisplayNames::kLfo2TriggerMode,
        .parentGroupId = ModuleIds::kLfo2,
        .choices = makeStringArray({ ChoiceLists::kLfoTriggerModeOff, ChoiceLists::kLfoTriggerModeStrig, ChoiceLists::kLfoTriggerModeMtrig, ChoiceLists::kLfoTriggerModeXtrig }),
        .defaultIndex = 0,
        .sysExOffset = 43,
        .sysExId = 96
    },
    {
        .parameterId = ParameterIds::kLfo2Lag,
        .displayName = DisplayNames::kLfo2Lag,
        .parentGroupId = ModuleIds::kLfo2,
        .choices = makeStringArray({ ChoiceLists::kOnOffChoiceOff, ChoiceLists::kOnOffChoiceOn }),
        .defaultIndex = 0,
        .sysExOffset = 44,
        .sysExId = 97
    },
    {
        .parameterId = ParameterIds::kLfo2SampleInput,
        .displayName = DisplayNames::kLfo2SampleInput,
        .parentGroupId = ModuleIds::kLfo2,
        .choices = makeStringArray({ ChoiceLists::kSourceNone, ChoiceLists::kSourceEnv1, ChoiceLists::kSourceEnv2, ChoiceLists::kSourceEnv3, 
                                     ChoiceLists::kSourceLfo1, ChoiceLists::kSourceLfo2, ChoiceLists::kSourceVibrato, ChoiceLists::kSourceRamp1, ChoiceLists::kSourceRamp2,
                                     ChoiceLists::kSourceKeybd, ChoiceLists::kSourcePorta, ChoiceLists::kSourceTrack, ChoiceLists::kSourceKbGate, 
                                     ChoiceLists::kSourceVelocity, ChoiceLists::kSourceRelVel, ChoiceLists::kSourcePressure,
                                     ChoiceLists::kSourcePedal1, ChoiceLists::kSourcePedal2, ChoiceLists::kSourceLever1, ChoiceLists::kSourceLever2, ChoiceLists::kSourceLever3 }),
        .defaultIndex = 9,
        .sysExOffset = 47,
        .sysExId = 98
    }
};

// ============================================================================
// Data Definitions - Patch Edit - Matrix Modulation Busses
// ============================================================================

namespace
{
    const juce::StringArray kSourceChoices = makeStringArray({
        ChoiceLists::kSourceNone, ChoiceLists::kSourceEnv1, ChoiceLists::kSourceEnv2, ChoiceLists::kSourceEnv3, 
        ChoiceLists::kSourceLfo1, ChoiceLists::kSourceLfo2, ChoiceLists::kSourceVibrato, ChoiceLists::kSourceRamp1, ChoiceLists::kSourceRamp2,
        ChoiceLists::kSourceKeybd, ChoiceLists::kSourcePorta, ChoiceLists::kSourceTrack, ChoiceLists::kSourceKbGate, 
        ChoiceLists::kSourceVelocity, ChoiceLists::kSourceRelVel, ChoiceLists::kSourcePressure,
        ChoiceLists::kSourcePedal1, ChoiceLists::kSourcePedal2, ChoiceLists::kSourceLever1, ChoiceLists::kSourceLever2, ChoiceLists::kSourceLever3
    });

    const juce::StringArray kDestinationChoices = makeStringArray({
        ChoiceLists::kDestinationNone, ChoiceLists::kDestinationDco1Frequency, ChoiceLists::kDestinationDco1PulseWidth, ChoiceLists::kDestinationDco1WaveShape,
        ChoiceLists::kDestinationDco2Frequency, ChoiceLists::kDestinationDco2PulseWidth, ChoiceLists::kDestinationDco2WaveShape,
        ChoiceLists::kDestinationDco1Dco2Mix, ChoiceLists::kDestinationVcfFmByDco1, ChoiceLists::kDestinationVcfFrequency, ChoiceLists::kDestinationVcfResonance,
        ChoiceLists::kDestinationVca1Volume, ChoiceLists::kDestinationVca2Volume,
        ChoiceLists::kDestinationEnv1Delay, ChoiceLists::kDestinationEnv1Attack, ChoiceLists::kDestinationEnv1Decay, 
        ChoiceLists::kDestinationEnv1Release, ChoiceLists::kDestinationEnv1Amplitude,
        ChoiceLists::kDestinationEnv2Delay, ChoiceLists::kDestinationEnv2Attack, ChoiceLists::kDestinationEnv2Decay, 
        ChoiceLists::kDestinationEnv2Release, ChoiceLists::kDestinationEnv2Amplitude,
        ChoiceLists::kDestinationEnv3Delay, ChoiceLists::kDestinationEnv3Attack, ChoiceLists::kDestinationEnv3Decay, 
        ChoiceLists::kDestinationEnv3Release, ChoiceLists::kDestinationEnv3Amplitude,
        ChoiceLists::kDestinationLfo1Speed, ChoiceLists::kDestinationLfo1Amplitude, ChoiceLists::kDestinationLfo2Speed, ChoiceLists::kDestinationLfo2Amplitude,
        ChoiceLists::kDestinationPortamentoRate
    });
}

const std::array<std::vector<ChoiceParameterDescriptor>, kMatrixModBusCount> kMatrixModBusChoiceParameters = {{
    // Bus 0
    {
        {
            .parameterId = ParameterIds::kMatrixModBus0Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus0,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 104,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus0Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus0,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 106,
            .sysExId = -1
        }
    },
    // Bus 1
    {
        {
            .parameterId = ParameterIds::kMatrixModBus1Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus1,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 107,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus1Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus1,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 109,
            .sysExId = -1
        }
    },
    // Bus 2
    {
        {
            .parameterId = ParameterIds::kMatrixModBus2Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus2,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 110,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus2Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus2,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 112,
            .sysExId = -1
        }
    },
    // Bus 3
    {
        {
            .parameterId = ParameterIds::kMatrixModBus3Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus3,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 113,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus3Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus3,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 115,
            .sysExId = -1
        }
    },
    // Bus 4
    {
        {
            .parameterId = ParameterIds::kMatrixModBus4Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus4,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 116,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus4Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus4,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 118,
            .sysExId = -1
        }
    },
    // Bus 5
    {
        {
            .parameterId = ParameterIds::kMatrixModBus5Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus5,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 119,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus5Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus5,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 121,
            .sysExId = -1
        }
    },
    // Bus 6
    {
        {
            .parameterId = ParameterIds::kMatrixModBus6Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus6,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 122,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus6Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus6,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 124,
            .sysExId = -1
        }
    },
    // Bus 7
    {
        {
            .parameterId = ParameterIds::kMatrixModBus7Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus7,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 125,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus7Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus7,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 127,
            .sysExId = -1
        }
    },
    // Bus 8
    {
        {
            .parameterId = ParameterIds::kMatrixModBus8Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus8,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 128,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus8Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus8,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 130,
            .sysExId = -1
        }
    },
    // Bus 9
    {
        {
            .parameterId = ParameterIds::kMatrixModBus9Source,
            .displayName = DisplayNames::kMatrixModBusSource,
            .parentGroupId = MatrixModBusIds::kMatrixModBus9,
            .choices = kSourceChoices,
            .defaultIndex = 0,
            .sysExOffset = 131,
            .sysExId = -1
        },
        {
            .parameterId = ParameterIds::kMatrixModBus9Destination,
            .displayName = DisplayNames::kMatrixModBusDestination,
            .parentGroupId = MatrixModBusIds::kMatrixModBus9,
            .choices = kDestinationChoices,
            .defaultIndex = 0,
            .sysExOffset = 133,
            .sysExId = -1
        }
    }
}};

const std::array<std::vector<IntParameterDescriptor>, kMatrixModBusCount> kMatrixModBusIntParameters = {{
    // Bus 0
    {
        {
            .parameterId = ParameterIds::kMatrixModBus0Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus0,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 105,
            .sysExId = -1
        }
    },
    // Bus 1
    {
        {
            .parameterId = ParameterIds::kMatrixModBus1Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus1,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 108,
            .sysExId = -1
        }
    },
    // Bus 2
    {
        {
            .parameterId = ParameterIds::kMatrixModBus2Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus2,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 111,
            .sysExId = -1
        }
    },
    // Bus 3
    {
        {
            .parameterId = ParameterIds::kMatrixModBus3Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus3,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 114,
            .sysExId = -1
        }
    },
    // Bus 4
    {
        {
            .parameterId = ParameterIds::kMatrixModBus4Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus4,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 117,
            .sysExId = -1
        }
    },
    // Bus 5
    {
        {
            .parameterId = ParameterIds::kMatrixModBus5Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus5,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 120,
            .sysExId = -1
        }
    },
    // Bus 6
    {
        {
            .parameterId = ParameterIds::kMatrixModBus6Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus6,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 123,
            .sysExId = -1
        }
    },
    // Bus 7
    {
        {
            .parameterId = ParameterIds::kMatrixModBus7Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus7,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 126,
            .sysExId = -1
        }
    },
    // Bus 8
    {
        {
            .parameterId = ParameterIds::kMatrixModBus8Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus8,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 129,
            .sysExId = -1
        }
    },
    // Bus 9
    {
        {
            .parameterId = ParameterIds::kMatrixModBus9Amount,
            .displayName = DisplayNames::kMatrixModBusAmount,
            .parentGroupId = MatrixModBusIds::kMatrixModBus9,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 132,
            .sysExId = -1
        }
    }
}};

// ============================================================================
// Data Definitions - Standalone Widgets
// ============================================================================

// Master Edit Standalone Widgets
const std::vector<StandaloneWidgetDescriptor> kMasterEditStandaloneWidgets = {};

// Patch Edit Standalone Widgets - DCO Modules
const std::vector<StandaloneWidgetDescriptor> kDcoStandaloneWidgets = {
    {
        .widgetId = WidgetIds::kDco1Init,
        .displayName = WidgetDisplayNames::kDco1Init,
        .parentGroupId = ModuleIds::kDco1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kDco1Copy,
        .displayName = WidgetDisplayNames::kDco1Copy,
        .parentGroupId = ModuleIds::kDco1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kDco1Paste,
        .displayName = WidgetDisplayNames::kDco1Paste,
        .parentGroupId = ModuleIds::kDco1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kDco2Init,
        .displayName = WidgetDisplayNames::kDco2Init,
        .parentGroupId = ModuleIds::kDco2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kDco2Copy,
        .displayName = WidgetDisplayNames::kDco2Copy,
        .parentGroupId = ModuleIds::kDco2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kDco2Paste,
        .displayName = WidgetDisplayNames::kDco2Paste,
        .parentGroupId = ModuleIds::kDco2,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// Patch Edit Standalone Widgets - Envelope Modules
const std::vector<StandaloneWidgetDescriptor> kEnvStandaloneWidgets = {
    {
        .widgetId = WidgetIds::kEnv1Init,
        .displayName = WidgetDisplayNames::kEnv1Init,
        .parentGroupId = ModuleIds::kEnvelope1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv1Copy,
        .displayName = WidgetDisplayNames::kEnv1Copy,
        .parentGroupId = ModuleIds::kEnvelope1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv1Paste,
        .displayName = WidgetDisplayNames::kEnv1Paste,
        .parentGroupId = ModuleIds::kEnvelope1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv2Init,
        .displayName = WidgetDisplayNames::kEnv2Init,
        .parentGroupId = ModuleIds::kEnvelope2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv2Copy,
        .displayName = WidgetDisplayNames::kEnv2Copy,
        .parentGroupId = ModuleIds::kEnvelope2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv2Paste,
        .displayName = WidgetDisplayNames::kEnv2Paste,
        .parentGroupId = ModuleIds::kEnvelope2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv3Init,
        .displayName = WidgetDisplayNames::kEnv3Init,
        .parentGroupId = ModuleIds::kEnvelope3,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv3Copy,
        .displayName = WidgetDisplayNames::kEnv3Copy,
        .parentGroupId = ModuleIds::kEnvelope3,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kEnv3Paste,
        .displayName = WidgetDisplayNames::kEnv3Paste,
        .parentGroupId = ModuleIds::kEnvelope3,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// Patch Edit Standalone Widgets - LFO Modules
const std::vector<StandaloneWidgetDescriptor> kLfoStandaloneWidgets = {
    {
        .widgetId = WidgetIds::kLfo1Init,
        .displayName = WidgetDisplayNames::kLfo1Init,
        .parentGroupId = ModuleIds::kLfo1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLfo1Copy,
        .displayName = WidgetDisplayNames::kLfo1Copy,
        .parentGroupId = ModuleIds::kLfo1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLfo1Paste,
        .displayName = WidgetDisplayNames::kLfo1Paste,
        .parentGroupId = ModuleIds::kLfo1,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLfo2Init,
        .displayName = WidgetDisplayNames::kLfo2Init,
        .parentGroupId = ModuleIds::kLfo2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLfo2Copy,
        .displayName = WidgetDisplayNames::kLfo2Copy,
        .parentGroupId = ModuleIds::kLfo2,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLfo2Paste,
        .displayName = WidgetDisplayNames::kLfo2Paste,
        .parentGroupId = ModuleIds::kLfo2,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// Patch Edit Standalone Widgets - Bank Utility Module
const std::vector<StandaloneWidgetDescriptor> kBankUtilityWidgets = {
    {
        .widgetId = WidgetIds::kUnlockBank,
        .displayName = WidgetDisplayNames::kUnlockBank,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank0,
        .displayName = WidgetDisplayNames::kSelectBank0,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank1,
        .displayName = WidgetDisplayNames::kSelectBank1,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank2,
        .displayName = WidgetDisplayNames::kSelectBank2,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank3,
        .displayName = WidgetDisplayNames::kSelectBank3,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank4,
        .displayName = WidgetDisplayNames::kSelectBank4,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank5,
        .displayName = WidgetDisplayNames::kSelectBank5,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank6,
        .displayName = WidgetDisplayNames::kSelectBank6,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank7,
        .displayName = WidgetDisplayNames::kSelectBank7,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank8,
        .displayName = WidgetDisplayNames::kSelectBank8,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectBank9,
        .displayName = WidgetDisplayNames::kSelectBank9,
        .parentGroupId = ModuleIds::kBankUtility,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// Patch Edit Standalone Widgets - Internal Patches Module
const std::vector<StandaloneWidgetDescriptor> kInternalPatchesWidgets = {
    {
        .widgetId = WidgetIds::kLoadPrevInternalPatch,
        .displayName = WidgetDisplayNames::kLoadPrevInternalPatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLoadNextInternalPatch,
        .displayName = WidgetDisplayNames::kLoadNextInternalPatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kCurrentBank,
        .displayName = WidgetDisplayNames::kCurrentBank,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kNumber
    },
    {
        .widgetId = WidgetIds::kCurrentPatch,
        .displayName = WidgetDisplayNames::kCurrentPatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kNumber
    },
    {
        .widgetId = WidgetIds::kInitPatch,
        .displayName = WidgetDisplayNames::kInitPatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kCopyPatch,
        .displayName = WidgetDisplayNames::kCopyPatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kPastePatch,
        .displayName = WidgetDisplayNames::kPastePatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kStorePatch,
        .displayName = WidgetDisplayNames::kStorePatch,
        .parentGroupId = ModuleIds::kInternalPatches,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// Patch Edit Standalone Widgets - Computer Patches Module
const std::vector<StandaloneWidgetDescriptor> kComputerPatchesWidgets = {
    {
        .widgetId = WidgetIds::kLoadPrevComputerPatch,
        .displayName = WidgetDisplayNames::kLoadPrevComputerPatch,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kLoadNextComputerPatch,
        .displayName = WidgetDisplayNames::kLoadNextComputerPatch,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSelectComputerPatch,
        .displayName = WidgetDisplayNames::kSelectComputerPatch,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kComboBox
    },
    {
        .widgetId = WidgetIds::kOpenComputerPatchFolder,
        .displayName = WidgetDisplayNames::kOpenComputerPatchFolder,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSaveComputerPatchAs,
        .displayName = WidgetDisplayNames::kSaveComputerPatchAs,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kButton
    },
    {
        .widgetId = WidgetIds::kSaveComputerPatch,
        .displayName = WidgetDisplayNames::kSaveComputerPatch,
        .parentGroupId = ModuleIds::kComputerPatches,
        .widgetType = StandaloneWidgetType::kButton
    }
};

// ============================================================================
// Helper Functions Implementation
// ============================================================================

juce::String getGroupDisplayName(const juce::String& groupId)
{
    for (const auto& group : kAllGroups)
    {
        if (group.groupId == groupId)
        {
            return group.displayName;
        }
    }
    return groupId;  // Fallback to ID if not found
}

juce::String getSectionDisplayName(const char* sectionId)
{
    return getGroupDisplayName(sectionId);
}

} // namespace SynthDescriptors

