#pragma once

#include <vector>

#include <juce_core/juce_core.h>

// SynthDescriptors.h
// Central file for parameter IDs, names, values, and groups
// Contains ONLY data structures and declarations - no code/logic
// Single source of truth for synth structure description

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
// Synth Parameter Descriptors
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
    juce::String widgetId;
    juce::String displayName;
    juce::String parentGroupId;
    StandaloneWidgetType widgetType;
};

// ============================================================================
// Group IDs (Modes)
// ============================================================================

namespace GroupIds
{
    constexpr const char* kMaster = "master";
    constexpr const char* kPatch  = "patch";
}

// ============================================================================
// Group IDs (Sections)
// ============================================================================

namespace SectionIds
{
    constexpr const char* kMasterEdit       = "masterEdit";
    constexpr const char* kPatchEdit        = "patchEdit";
    constexpr const char* kMatrixModulation = "matrixModulation";
    constexpr const char* kPatchManager     = "patchManager";
}

// ============================================================================
// Group IDs (Modules)
// ============================================================================

namespace ModuleIds
{
    // Master Edit modules
    constexpr const char* kMidi             = "midi";
    constexpr const char* kVibrato          = "vibrato";
    constexpr const char* kMisc             = "misc";
    
    // Patch Edit modules
    constexpr const char* kDco1             = "dco1";
    constexpr const char* kDco2             = "dco2";
    constexpr const char* kVcfVca           = "vcfVca";
    constexpr const char* kFmTrack          = "fmTrack";
    constexpr const char* kRampPortamento   = "rampPortamento";
    constexpr const char* kEnvelope1        = "envelope1";
    constexpr const char* kEnvelope2        = "envelope2";
    constexpr const char* kEnvelope3        = "envelope3";
    constexpr const char* kLfo1             = "lfo1";
    constexpr const char* kLfo2             = "lfo2";

    // Patch Manager modules
    constexpr const char* kBankUtility      = "bankUtility";
    constexpr const char* kInternalPatches  = "internalPatches";
    constexpr const char* kComputerPatches  = "computerPatches";
}

// ============================================================================
// Group IDs (Matrix Modulation Busses)
// ============================================================================

constexpr int kMatrixModBusCount = 10;

namespace MatrixModBusIds
{
    constexpr const char* kMatrixModBus0    = "matrixModBus0";
    constexpr const char* kMatrixModBus1    = "matrixModBus1";
    constexpr const char* kMatrixModBus2    = "matrixModBus2";
    constexpr const char* kMatrixModBus3    = "matrixModBus3";
    constexpr const char* kMatrixModBus4    = "matrixModBus4";
    constexpr const char* kMatrixModBus5    = "matrixModBus5";
    constexpr const char* kMatrixModBus6    = "matrixModBus6";
    constexpr const char* kMatrixModBus7    = "matrixModBus7";
    constexpr const char* kMatrixModBus8    = "matrixModBus8";
    constexpr const char* kMatrixModBus9    = "matrixModBus9";
}

// ============================================================================
// Parameter IDs
// ============================================================================

namespace ParameterIds
{
    // DCO 1
    constexpr const char* kDco1Frequency              = "dco1Frequency";
    constexpr const char* kDco1FrequencyModByLfo1     = "dco1FrequencyModByLfo1";
    constexpr const char* kDco1PulseWidth             = "dco1PulseWidth";
    constexpr const char* kDco1PulseWidthModByLfo2    = "dco1PulseWidthModByLfo2";
    constexpr const char* kDco1WaveShape              = "dco1WaveShape";
    constexpr const char* kDco1Sync                   = "dco1Sync";
    constexpr const char* kDco1WaveSelect             = "dco1WaveSelect";
    constexpr const char* kDco1Levers                 = "dco1Levers";
    constexpr const char* kDco1KeyboardPortamento     = "dco1KeyboardPortamento";
    constexpr const char* kDco1KeyClick               = "dco1KeyClick";

    // DCO 2
    constexpr const char* kDco2Frequency              = "dco2Frequency";
    constexpr const char* kDco2FrequencyModByLfo1     = "dco2FrequencyModByLfo1";
    constexpr const char* kDco2Detune                 = "dco2Detune";
    constexpr const char* kDco2PulseWidth             = "dco2PulseWidth";
    constexpr const char* kDco2PulseWidthModByLfo2    = "dco2PulseWidthModByLfo2";
    constexpr const char* kDco2WaveShape              = "dco2WaveShape";
    constexpr const char* kDco2WaveSelect             = "dco2WaveSelect";
    constexpr const char* kDco2Levers                 = "dco2Levers";
    constexpr const char* kDco2KeyboardPortamento     = "dco2KeyboardPortamento";
    constexpr const char* kDco2KeyClick               = "dco2KeyClick";

    // VCF/VCA
    constexpr const char* kVcfBalance                 = "vcfBalance";
    constexpr const char* kVcfFequency                = "vcfFequency";
    constexpr const char* kVcfFrequencyModByEnv1      = "vcfFrequencyModByEnv1";
    constexpr const char* kVcfFrequencyModByPressure  = "vcfFrequencyModByPressure";
    constexpr const char* kVcfResonance               = "vcfResonance";
    constexpr const char* kVca1Volume                 = "vca1Volume";
    constexpr const char* kVca1ModByVelocity          = "vca1ModByVelocity";
    constexpr const char* kVca2ModByEnv2              = "vca2ModByEnv2";
    constexpr const char* kVcfLevers                  = "vcfLevers";
    constexpr const char* kVcfKeyboardPortamento      = "vcfKeyboardPortamento";

    // FM/TRACK
    constexpr const char* kFmAmount                   = "fmAmount";
    constexpr const char* kFmModByEnv3                = "fmModByEnv3";
    constexpr const char* kFmModByPressure            = "fmModByPressure";
    constexpr const char* kTrackPoint1                = "trackPoint1";
    constexpr const char* kTrackPoint2                = "trackPoint2";
    constexpr const char* kTrackPoint3                = "trackPoint3";
    constexpr const char* kTrackPoint4                = "trackPoint4";
    constexpr const char* kTrackPoint5                = "trackPoint5";
    constexpr const char* kTrackInput                 = "trackInput";

    // RAMP/PORTAMENTO
    constexpr const char* kRamp1Rate                  = "ramp1Rate";
    constexpr const char* kRamp2Rate                  = "ramp2Rate";
    constexpr const char* kPortamentoRate             = "portamentoRate";
    constexpr const char* kPortamentoModByVelocity    = "portamentoModByVelocity";
    constexpr const char* kRamp1Trigger               = "ramp1Trigger";
    constexpr const char* kRamp2Trigger               = "ramp2Trigger";
    constexpr const char* kPortamentoMode             = "portamentoMode";
    constexpr const char* kPortamentoLegato           = "portamentoLegato";
    constexpr const char* kPortamentoKeyboardMode     = "portamentoKeyboardMode";

    // ENV 1
    constexpr const char* kEnv1Delay                  = "env1Delay";
    constexpr const char* kEnv1Attack                 = "env1Attack";
    constexpr const char* kEnv1Decay                  = "env1Decay";
    constexpr const char* kEnv1Sustain                = "env1Sustain";
    constexpr const char* kEnv1Release                = "env1Release";
    constexpr const char* kEnv1Amplitude              = "env1Amplitude";
    constexpr const char* kEnv1AmplitudeModByVelocity = "env1AmplitudeModByVelocity";
    constexpr const char* kEnv1TriggerMode            = "env1TriggerMode";
    constexpr const char* kEnv1EnvelopeMode           = "env1EnvelopeMode";
    constexpr const char* kEnv1Lfo1Trigger            = "env1Lfo1Trigger";

    // ENV 2
    constexpr const char* kEnv2Delay                  = "env2Delay";
    constexpr const char* kEnv2Attack                 = "env2Attack";
    constexpr const char* kEnv2Decay                  = "env2Decay";
    constexpr const char* kEnv2Sustain                = "env2Sustain";
    constexpr const char* kEnv2Release                = "env2Release";
    constexpr const char* kEnv2Amplitude              = "env2Amplitude";
    constexpr const char* kEnv2AmplitudeModByVelocity = "env2AmplitudeModByVelocity";
    constexpr const char* kEnv2TriggerMode            = "env2TriggerMode";
    constexpr const char* kEnv2EnvelopeMode           = "env2EnvelopeMode";
    constexpr const char* kEnv2Lfo1Trigger            = "env2Lfo1Trigger";

    // ENV 3
    constexpr const char* kEnv3Delay                  = "env3Delay";
    constexpr const char* kEnv3Attack                 = "env3Attack";
    constexpr const char* kEnv3Decay                  = "env3Decay";
    constexpr const char* kEnv3Sustain                = "env3Sustain";
    constexpr const char* kEnv3Release                = "env3Release";
    constexpr const char* kEnv3Amplitude              = "env3Amplitude";
    constexpr const char* kEnv3AmplitudeModByVelocity = "env3AmplitudeModByVelocity";
    constexpr const char* kEnv3TriggerMode            = "env3TriggerMode";
    constexpr const char* kEnv3EnvelopeMode           = "env3EnvelopeMode";
    constexpr const char* kEnv3Lfo1Trigger            = "env3Lfo1Trigger";

    // LFO 1
    constexpr const char* kLfo1Speed                  = "lfo1Speed";
    constexpr const char* kLfo1SpeedModByPressure     = "lfo1SpeedModByPressure";
    constexpr const char* kLfo1RetriggerPoint         = "lfo1RetriggerPoint";
    constexpr const char* kLfo1Amplitude              = "lfo1Amplitude";
    constexpr const char* kLfo1AmplitudeModByRamp1    = "lfo1AmplitudeModByRamp1";
    constexpr const char* kLfo1Waveform               = "lfo1Waveform";
    constexpr const char* kLfo1TriggerMode            = "lfo1TriggerMode";
    constexpr const char* kLfo1Lag                    = "lfo1Lag";
    constexpr const char* kLfo1SampleInput            = "lfo1SampleInput";

    // LFO 2
    constexpr const char* kLfo2Speed                  = "lfo2Speed";
    constexpr const char* kLfo2SpeedModByKeyboard     = "lfo2SpeedModByKeyboard";
    constexpr const char* kLfo2RetriggerPoint         = "lfo2RetriggerPoint";
    constexpr const char* kLfo2Amplitude              = "lfo2Amplitude";
    constexpr const char* kLfo2AmplitudeModByRamp2    = "lfo2AmplitudeModByRamp2";
    constexpr const char* kLfo2Waveform               = "lfo2Waveform";
    constexpr const char* kLfo2TriggerMode            = "lfo2TriggerMode";
    constexpr const char* kLfo2Lag                    = "lfo2Lag";
    constexpr const char* kLfo2SampleInput            = "lfo2SampleInput";

    // Matrix Modulation Busses (dynamically generated, but IDs defined here)
    constexpr const char* kMatrixModBus0Source        = "matrixModBus0Source";
    constexpr const char* kMatrixModBus0Amount        = "matrixModBus0Amount";
    constexpr const char* kMatrixModBus0Destination   = "matrixModBus0Destination";

    constexpr const char* kMatrixModBus1Source        = "matrixModBus1Source";
    constexpr const char* kMatrixModBus1Amount        = "matrixModBus1Amount";
    constexpr const char* kMatrixModBus1Destination   = "matrixModBus1Destination";

    constexpr const char* kMatrixModBus2Source        = "matrixModBus2Source";
    constexpr const char* kMatrixModBus2Amount        = "matrixModBus2Amount";
    constexpr const char* kMatrixModBus2Destination   = "matrixModBus2Destination";

    constexpr const char* kMatrixModBus3Source        = "matrixModBus3Source";
    constexpr const char* kMatrixModBus3Amount        = "matrixModBus3Amount";
    constexpr const char* kMatrixModBus3Destination   = "matrixModBus3Destination";

    constexpr const char* kMatrixModBus4Source        = "matrixModBus4Source";
    constexpr const char* kMatrixModBus4Amount        = "matrixModBus4Amount";
    constexpr const char* kMatrixModBus4Destination   = "matrixModBus4Destination";

    constexpr const char* kMatrixModBus5Source        = "matrixModBus5Source";
    constexpr const char* kMatrixModBus5Amount        = "matrixModBus5Amount";
    constexpr const char* kMatrixModBus5Destination   = "matrixModBus5Destination";

    constexpr const char* kMatrixModBus6Source        = "matrixModBus6Source";
    constexpr const char* kMatrixModBus6Amount        = "matrixModBus6Amount";
    constexpr const char* kMatrixModBus6Destination   = "matrixModBus6Destination";

    constexpr const char* kMatrixModBus7Source        = "matrixModBus7Source";
    constexpr const char* kMatrixModBus7Amount        = "matrixModBus7Amount";
    constexpr const char* kMatrixModBus7Destination   = "matrixModBus7Destination";
    
    constexpr const char* kMatrixModBus8Source        = "matrixModBus8Source";
    constexpr const char* kMatrixModBus8Amount        = "matrixModBus8Amount";
    constexpr const char* kMatrixModBus8Destination   = "matrixModBus8Destination";

    constexpr const char* kMatrixModBus9Source        = "matrixModBus9Source";
    constexpr const char* kMatrixModBus9Amount        = "matrixModBus9Amount";
    constexpr const char* kMatrixModBus9Destination   = "matrixModBus9Destination";
}

// ============================================================================
// Display Names
// ============================================================================

namespace DisplayNames
{
    // DCO 1
    constexpr const char* kDco1Frequency                = "FREQUENCY";
    constexpr const char* kDco1FrequencyModByLfo1       = "FREQ < LFO 1";
    constexpr const char* kDco1PulseWidth               = "PULSE WIDTH";
    constexpr const char* kDco1PulseWidthModByLfo2      = "PW < LFO 2";
    constexpr const char* kDco1WaveShape                = "WAVE SHAPE";
    constexpr const char* kDco1Sync                     = "SYNC";
    constexpr const char* kDco1WaveSelect               = "WAVE SELECT";
    constexpr const char* kDco1Levers                   = "LEVERS";
    constexpr const char* kDco1KeyboardPortamento       = "KEYBD/PORTA";
    constexpr const char* kDco1KeyClick                 = "KEY CLICK";

    // DCO 2
    constexpr const char* kDco2Frequency                = "FREQUENCY";
    constexpr const char* kDco2FrequencyModByLfo1       = "FREQ < LFO 1";
    constexpr const char* kDco2Detune                   = "DETUNE";
    constexpr const char* kDco2PulseWidth               = "PULSE WIDTH";
    constexpr const char* kDco2PulseWidthModByLfo2      = "PW < LFO 2";
    constexpr const char* kDco2WaveShape                = "WAVE SHAPE";
    constexpr const char* kDco2WaveSelect               = "WAVE SELECT";
    constexpr const char* kDco2Levers                   = "LEVERS";
    constexpr const char* kDco2KeyboardPortamento       = "KEYBD/PORTA";
    constexpr const char* kDco2KeyClick                 = "KEY CLICK";

    // VCF/VCA
    constexpr const char* kVcfBalance                   = "BALANCE";
    constexpr const char* kVcfFequency                  = "FREQUENCY";
    constexpr const char* kVcfFrequencyModByEnv1        = "FREQ < ENV 1";
    constexpr const char* kVcfFrequencyModByPressure    = "FREQ < PRESSURE";
    constexpr const char* kVcfResonance                 = "RESONANCE";
    constexpr const char* kVca1Volume                   = "VCA 1 VOLUME";
    constexpr const char* kVca1ModByVelocity            = "VCA 1 < VELOCITY";
    constexpr const char* kVca2ModByEnv2                = "VCA 2 < ENV 2";
    constexpr const char* kVcfLevers                    = "LEVERS";
    constexpr const char* kVcfKeyboardPortamento        = "KEYBD/PORTA";

    // FM/TRACK
    constexpr const char* kFmAmount                     = "VCF FM AMOUNT";
    constexpr const char* kFmModByEnv3                  = "FM < ENV 3";
    constexpr const char* kFmModByPressure              = "FM < PRESSURE";
    constexpr const char* kTrackPoint1                  = "TRACK POINT 1";
    constexpr const char* kTrackPoint2                  = "TRACK POINT 2";
    constexpr const char* kTrackPoint3                  = "TRACK POINT 3";
    constexpr const char* kTrackPoint4                  = "TRACK POINT 4";
    constexpr const char* kTrackPoint5                  = "TRACK POINT 5";
    constexpr const char* kTrackInput                   = "TRACK INPUT";

    // RAMP/PORTAMENTO
    constexpr const char* kRamp1Rate                    = "RAMP 1 RATE";
    constexpr const char* kRamp2Rate                    = "RAMP 2 RATE";
    constexpr const char* kPortamentoRate               = "PORTAMENTO RATE";
    constexpr const char* kPortamentoModByVelocity      = "PORT < VELOCITY";
    constexpr const char* kRamp1Trigger                 = "RAMP 1 TRIGGER";
    constexpr const char* kRamp2Trigger                 = "RAMP 2 TRIGGER";
    constexpr const char* kPortamentoMode               = "PORTA MODE";
    constexpr const char* kPortamentoLegato             = "LEGATO PORTA";
    constexpr const char* kPortamentoKeyboardMode       = "KEYBOARD MODE";

    // ENV 1
    constexpr const char* kEnv1Delay                    = "DELAY";
    constexpr const char* kEnv1Attack                   = "ATTACK";
    constexpr const char* kEnv1Decay                    = "DECAY";
    constexpr const char* kEnv1Sustain                  = "SUSTAIN";
    constexpr const char* kEnv1Release                  = "RELEASE";
    constexpr const char* kEnv1Amplitude                = "AMPLITUDE";
    constexpr const char* kEnv1AmplitudeModByVelocity   = "AMP < VELOCITY";
    constexpr const char* kEnv1TriggerMode              = "TRIGGER MODE";
    constexpr const char* kEnv1EnvelopeMode             = "ENVELOPE MODE";
    constexpr const char* kEnv1Lfo1Trigger              = "LFO 1 TRIGGER";

    // ENV 2
    constexpr const char* kEnv2Delay                    = "DELAY";
    constexpr const char* kEnv2Attack                   = "ATTACK";
    constexpr const char* kEnv2Decay                    = "DECAY";
    constexpr const char* kEnv2Sustain                  = "SUSTAIN";
    constexpr const char* kEnv2Release                  = "RELEASE";
    constexpr const char* kEnv2Amplitude                = "AMPLITUDE";
    constexpr const char* kEnv2AmplitudeModByVelocity   = "AMP < VELOCITY";
    constexpr const char* kEnv2TriggerMode              = "TRIGGER MODE";
    constexpr const char* kEnv2EnvelopeMode             = "ENVELOPE MODE";
    constexpr const char* kEnv2Lfo1Trigger              = "LFO 1 TRIGGER";

    // ENV 3
    constexpr const char* kEnv3Delay                    = "DELAY";
    constexpr const char* kEnv3Attack                   = "ATTACK";
    constexpr const char* kEnv3Decay                    = "DECAY";
    constexpr const char* kEnv3Sustain                  = "SUSTAIN";
    constexpr const char* kEnv3Release                  = "RELEASE";
    constexpr const char* kEnv3Amplitude                = "AMPLITUDE";
    constexpr const char* kEnv3AmplitudeModByVelocity   = "AMP < VELOCITY";
    constexpr const char* kEnv3TriggerMode              = "TRIGGER MODE";
    constexpr const char* kEnv3EnvelopeMode             = "ENVELOPE MODE";
    constexpr const char* kEnv3Lfo1Trigger              = "LFO 1 TRIGGER";

    // LFO 1
    constexpr const char* kLfo1Speed                    = "SPEED";
    constexpr const char* kLfo1SpeedModByPressure       = "SPEED < PRESSURE";
    constexpr const char* kLfo1RetriggerPoint           = "RETRIGGER POINT";
    constexpr const char* kLfo1Amplitude                = "AMPLITUDE";
    constexpr const char* kLfo1AmplitudeModByRamp1      = "AMP < RAMP 1";
    constexpr const char* kLfo1Waveform                 = "WAVEFORM";
    constexpr const char* kLfo1TriggerMode              = "TRIGGER MODE";
    constexpr const char* kLfo1Lag                      = "LAG";
    constexpr const char* kLfo1SampleInput              = "SAMPLE INPUT";

    // LFO 2
    constexpr const char* kLfo2Speed                    = "SPEED";
    constexpr const char* kLfo2SpeedModByKeyboard       = "SPEED < KEYBD";
    constexpr const char* kLfo2RetriggerPoint           = "RETRIGGER POINT";
    constexpr const char* kLfo2Amplitude                = "AMPLITUDE";
    constexpr const char* kLfo2AmplitudeModByRamp2      = "AMP < RAMP 2";
    constexpr const char* kLfo2Waveform                 = "WAVEFORM";
    constexpr const char* kLfo2TriggerMode              = "TRIGGER MODE";
    constexpr const char* kLfo2Lag                      = "LAG";
    constexpr const char* kLfo2SampleInput              = "SAMPLE INPUT";

    // Matrix Modulation Busses
    constexpr const char* kMatrixModBusSource           = "SOURCE";
    constexpr const char* kMatrixModBusDestination      = "DESTINATION";
    constexpr const char* kMatrixModBusAmount           = "AMOUNT";
}

// ============================================================================
// Choice Lists (for ComboBox)
// ============================================================================

namespace ChoiceLists
{
    // Sync choices
    constexpr const char* kSyncOff                      = "OFF";
    constexpr const char* kSyncSoft                     = "SOFT";
    constexpr const char* kSyncMedium                   = "MEDIUM";
    constexpr const char* kSyncHard                     = "HARD";

    // Wave Select choices
    constexpr const char* kWaveSelectOff                = "OFF";
    constexpr const char* kWaveSelectPulse              = "PULSE";
    constexpr const char* kWaveSelectWave               = "WAVE";
    constexpr const char* kWaveSelectBoth               = "BOTH";
    constexpr const char* kWaveSelectNoise              = "NOISE";

    // Levers choices
    constexpr const char* kLeversOff                    = "OFF";
    constexpr const char* kLeversL1Bend                 = "L1/BEND";
    constexpr const char* kLeversL2Vib                  = "L2/VIB";
    constexpr const char* kLeversBoth                   = "BOTH";

    // Keyboard/Portamento choices
    constexpr const char* kKeyboardPortamentoOff        = "OFF";
    constexpr const char* kKeyboardPortamentoKeybd      = "KEYBD";
    constexpr const char* kKeyboardPortamentoPorta      = "PORTA";

    // On/Off choices
    constexpr const char* kOnOffChoiceOff               = "OFF";
    constexpr const char* kOnOffChoiceOn                = "ON";

    // Trigger Mode choices (ENV)
    constexpr const char* kTriggerModeStrig             = "STRIG";
    constexpr const char* kTriggerModeMtrig             = "MTRIG";
    constexpr const char* kTriggerModeSreset            = "SRESET";
    constexpr const char* kTriggerModeMreset            = "MRESET";
    constexpr const char* kTriggerModeXtrig             = "XTRIG";
    constexpr const char* kTriggerModeXmtrig            = "XMTRIG";
    constexpr const char* kTriggerModeXreset            = "XRESET";
    constexpr const char* kTriggerModeXmrst             = "XMRST";

    // Envelope Mode choices
    constexpr const char* kEnvelopeModeNormal           = "NORMAL";
    constexpr const char* kEnvelopeModeDadr             = "DADR";
    constexpr const char* kEnvelopeModeFree             = "FREE";
    constexpr const char* kEnvelopeModeBoth             = "BOTH";

    // LFO 1 Trigger choices
    constexpr const char* kLfo1TriggerNormal            = "NORMAL";
    constexpr const char* kLfo1TriggerLfo1              = "LFO 1";
    constexpr const char* kLfo1TriggerGLfo1             = "G-LFO 1";
    constexpr const char* kLfo1TriggerGatedLfo1Trigger  = "GATED LFO 1 TRIGGER";

    // LFO Trigger Mode choices
    constexpr const char* kLfoTriggerModeOff            = "OFF";
    constexpr const char* kLfoTriggerModeStrig          = "STRIG";
    constexpr const char* kLfoTriggerModeMtrig          = "MTRIG";
    constexpr const char* kLfoTriggerModeXtrig          = "XTRIG";

    // LFO Waveform choices
    constexpr const char* kLfoWaveformTriangle          = "TRIANGLE";
    constexpr const char* kLfoWaveformUpsaw             = "UPSAW";
    constexpr const char* kLfoWaveformDnsaw             = "DNSAW";
    constexpr const char* kLfoWaveformSquare            = "SQUARE";
    constexpr const char* kLfoWaveformRandom            = "RANDOM";
    constexpr const char* kLfoWaveformNoise             = "NOISE";
    constexpr const char* kLfoWaveformSampled           = "SAMPLED";

    // Ramp Trigger choices
    constexpr const char* kRampTriggerStrig             = "STRIG";
    constexpr const char* kRampTriggerMtrig             = "MTRIG";
    constexpr const char* kRampTriggerExtrig            = "EXTRIG";
    constexpr const char* kRampTriggerGatedx            = "GATEDX";

    // Portamento Mode choices
    constexpr const char* kPortamentoModeLinear         = "LINEAR";
    constexpr const char* kPortamentoModeConst          = "CONST";
    constexpr const char* kPortamentoModeExpo           = "EXPO";

    // Portamento Keyboard Mode choices
    constexpr const char* kPortamentoKeyboardModeRotate = "ROTATE";
    constexpr const char* kPortamentoKeyboardModeReasgn = "REASGN";
    constexpr const char* kPortamentoKeyboardModeUnison = "UNISON";
    constexpr const char* kPortamentoKeyboardModeRearob = "REAROB";

    // Source/Destination choices (for Matrix Modulation and Sample Input)
    constexpr const char* kSourceNone                   = "NONE";
    constexpr const char* kSourceEnv1                   = "ENV 1";
    constexpr const char* kSourceEnv2                   = "ENV 2";
    constexpr const char* kSourceEnv3                   = "ENV 3";
    constexpr const char* kSourceLfo1                   = "LFO 1";
    constexpr const char* kSourceLfo2                   = "LFO 2";
    constexpr const char* kSourceVibrato                = "VIBRATO";
    constexpr const char* kSourceRamp1                  = "RAMP 1";
    constexpr const char* kSourceRamp2                  = "RAMP 2";
    constexpr const char* kSourceKeybd                  = "KEYBD";
    constexpr const char* kSourcePorta                  = "PORTA";
    constexpr const char* kSourceTrack                  = "TRACK";
    constexpr const char* kSourceKbGate                 = "KB GATE";
    constexpr const char* kSourceVelocity               = "VELOCITY";
    constexpr const char* kSourceRelVel                 = "REL VEL";
    constexpr const char* kSourcePressure               = "PRESSURE";
    constexpr const char* kSourcePedal1                 = "PEDAL 1";
    constexpr const char* kSourcePedal2                 = "PEDAL 2";
    constexpr const char* kSourceLever1                 = "LEVER 1";
    constexpr const char* kSourceLever2                 = "LEVER 2";
    constexpr const char* kSourceLever3                 = "LEVER 3";

    // Destination choices (for Matrix Modulation)
    constexpr const char* kDestinationNone              = "NONE";
    constexpr const char* kDestinationDco1Frequency     = "DCO 1 FREQUENCY";
    constexpr const char* kDestinationDco1PulseWidth    = "DCO 1 PULSE WIDTH";
    constexpr const char* kDestinationDco1WaveShape     = "DCO 1 WAVE SHAPE";
    constexpr const char* kDestinationDco2Frequency     = "DCO 2 FREQUENCY";
    constexpr const char* kDestinationDco2PulseWidth    = "DCO 2 PULSE WIDTH";
    constexpr const char* kDestinationDco2WaveShape     = "DCO 2 WAVE SHAPE";
    constexpr const char* kDestinationDco1Dco2Mix       = "DCO 1 | DCO 2 MIX";
    constexpr const char* kDestinationVcfFmByDco1       = "VCF FM BY DCO 1";
    constexpr const char* kDestinationVcfFrequency      = "VCF FREQUENCY";
    constexpr const char* kDestinationVcfResonance      = "VCF RESONANCE";
    constexpr const char* kDestinationVca1Volume        = "VCA 1 VOLUME";
    constexpr const char* kDestinationVca2Volume        = "VCA 2 VOLUME";
    constexpr const char* kDestinationEnv1Delay         = "ENV 1 DELAY";
    constexpr const char* kDestinationEnv1Attack        = "ENV 1 ATTACK";
    constexpr const char* kDestinationEnv1Decay         = "ENV 1 DECAY";
    constexpr const char* kDestinationEnv1Release       = "ENV 1 RELEASE";
    constexpr const char* kDestinationEnv1Amplitude     = "ENV 1 AMPLITUDE";
    constexpr const char* kDestinationEnv2Delay         = "ENV 2 DELAY";
    constexpr const char* kDestinationEnv2Attack        = "ENV 2 ATTACK";
    constexpr const char* kDestinationEnv2Decay         = "ENV 2 DECAY";
    constexpr const char* kDestinationEnv2Release       = "ENV 2 RELEASE";
    constexpr const char* kDestinationEnv2Amplitude     = "ENV 2 AMPLITUDE";
    constexpr const char* kDestinationEnv3Delay         = "ENV 3 DELAY";
    constexpr const char* kDestinationEnv3Attack        = "ENV 3 ATTACK";
    constexpr const char* kDestinationEnv3Decay         = "ENV 3 DECAY";
    constexpr const char* kDestinationEnv3Release       = "ENV 3 RELEASE";
    constexpr const char* kDestinationEnv3Amplitude     = "ENV 3 AMPLITUDE";
    constexpr const char* kDestinationLfo1Speed         = "LFO 1 SPEED";
    constexpr const char* kDestinationLfo1Amplitude     = "LFO 1 AMPLITUDE";
    constexpr const char* kDestinationLfo2Speed         = "LFO 2 SPEED";
    constexpr const char* kDestinationLfo2Amplitude     = "LFO 2 AMPLITUDE";
    constexpr const char* kDestinationPortamentoRate    = "PORTAMENTO RATE";
}

// ============================================================================
// Standalone Widget IDs
// ============================================================================

namespace WidgetIds
{
    // DCO widgets
    constexpr const char* kDco1Init                     = "dco1Init";
    constexpr const char* kDco1Copy                     = "dco1Copy";
    constexpr const char* kDco1Paste                    = "dco1Paste";
    constexpr const char* kDco2Init                     = "dco2Init";
    constexpr const char* kDco2Copy                     = "dco2Copy";
    constexpr const char* kDco2Paste                    = "dco2Paste";

    // ENV widgets
    constexpr const char* kEnv1Init                     = "env1Init";
    constexpr const char* kEnv1Copy                     = "env1Copy";
    constexpr const char* kEnv1Paste                    = "env1Paste";
    constexpr const char* kEnv2Init                     = "env2Init";
    constexpr const char* kEnv2Copy                     = "env2Copy";
    constexpr const char* kEnv2Paste                    = "env2Paste";
    constexpr const char* kEnv3Init                     = "env3Init";
    constexpr const char* kEnv3Copy                     = "env3Copy";
    constexpr const char* kEnv3Paste                    = "env3Paste";

    // LFO widgets
    constexpr const char* kLfo1Init                     = "lfo1Init";
    constexpr const char* kLfo1Copy                     = "lfo1Copy";
    constexpr const char* kLfo1Paste                    = "lfo1Paste";
    constexpr const char* kLfo2Init                     = "lfo2Init";
    constexpr const char* kLfo2Copy                     = "lfo2Copy";
    constexpr const char* kLfo2Paste                    = "lfo2Paste";

    // Patch Manager widgets
    constexpr const char* kUnlockBank                   = "unlockBank";
    constexpr const char* kSelectBank0                  = "selectBank0";
    constexpr const char* kSelectBank1                  = "selectBank1";
    constexpr const char* kSelectBank2                  = "selectBank2";
    constexpr const char* kSelectBank3                  = "selectBank3";
    constexpr const char* kSelectBank4                  = "selectBank4";
    constexpr const char* kSelectBank5                  = "selectBank5";
    constexpr const char* kSelectBank6                  = "selectBank6";
    constexpr const char* kSelectBank7                  = "selectBank7";
    constexpr const char* kSelectBank8                  = "selectBank8";
    constexpr const char* kSelectBank9                  = "selectBank9";
    constexpr const char* kLoadPrevInternalPatch        = "loadPrevInternalPatch";
    constexpr const char* kLoadNextInternalPatch        = "loadNextInternalPatch";
    constexpr const char* kCurrentBank                  = "currentBank";
    constexpr const char* kCurrentPatch                 = "currentPatch";
    constexpr const char* kInitPatch                    = "initPatch";
    constexpr const char* kCopyPatch                    = "copyPatch";
    constexpr const char* kPastePatch                   = "pastePatch";
    constexpr const char* kStorePatch                   = "storePatch";
    constexpr const char* kLoadPrevComputerPatch        = "loadPrevComputerPatch";
    constexpr const char* kLoadNextComputerPatch        = "loadNextComputerPatch";
    constexpr const char* kSelectComputerPatch          = "selectComputerPatch";
    constexpr const char* kOpenComputerPatchFolder      = "openComputerPatchFolder";
    constexpr const char* kSaveComputerPatchAs          = "savePatchAs";
    constexpr const char* kSaveComputerPatch            = "saveComputerPatch";
}

// ============================================================================
// Standalone Widget Display Names
// ============================================================================

namespace WidgetDisplayNames
{
    // Action labels
    constexpr const char* kActionLabelInit              = "I";
    constexpr const char* kActionLabelCopy              = "C";
    constexpr const char* kActionLabelPaste             = "P";

    // DCO widgets
    constexpr const char* kDco1Init                     = kActionLabelInit;
    constexpr const char* kDco1Copy                     = kActionLabelCopy;
    constexpr const char* kDco1Paste                    = kActionLabelPaste;
    constexpr const char* kDco2Init                     = kActionLabelInit;
    constexpr const char* kDco2Copy                     = kActionLabelCopy;
    constexpr const char* kDco2Paste                    = kActionLabelPaste;

    // ENV widgets
    constexpr const char* kEnv1Init                     = kActionLabelInit;
    constexpr const char* kEnv1Copy                     = kActionLabelCopy;
    constexpr const char* kEnv1Paste                    = kActionLabelPaste;
    constexpr const char* kEnv2Init                     = kActionLabelInit;
    constexpr const char* kEnv2Copy                     = kActionLabelCopy;
    constexpr const char* kEnv2Paste                    = kActionLabelPaste;
    constexpr const char* kEnv3Init                     = kActionLabelInit;
    constexpr const char* kEnv3Copy                     = kActionLabelCopy;
    constexpr const char* kEnv3Paste                    = kActionLabelPaste;

    // LFO widgets
    constexpr const char* kLfo1Init                     = kActionLabelInit;
    constexpr const char* kLfo1Copy                     = kActionLabelCopy;
    constexpr const char* kLfo1Paste                    = kActionLabelPaste;
    constexpr const char* kLfo2Init                     = kActionLabelInit;
    constexpr const char* kLfo2Copy                     = kActionLabelCopy;
    constexpr const char* kLfo2Paste                    = kActionLabelPaste;

    // Patch Manager widgets
    constexpr const char* kUnlockBank                   = "UNLOCK";
    constexpr const char* kSelectBank0                  = "0";
    constexpr const char* kSelectBank1                  = "1";
    constexpr const char* kSelectBank2                  = "2";
    constexpr const char* kSelectBank3                  = "3";
    constexpr const char* kSelectBank4                  = "4";
    constexpr const char* kSelectBank5                  = "5";
    constexpr const char* kSelectBank6                  = "6";
    constexpr const char* kSelectBank7                  = "7";
    constexpr const char* kSelectBank8                  = "8";
    constexpr const char* kSelectBank9                  = "9";
    constexpr const char* kLoadPrevInternalPatch        = "<";
    constexpr const char* kLoadNextInternalPatch        = ">";
    constexpr const char* kCurrentBank                  = ""; // TODO: approche valide ? Comment gérer la valeur Int ?
    constexpr const char* kCurrentPatch                 = ""; // TODO: approche valide ? Comment gérer la valeur Int ?
    constexpr const char* kInitPatch                    = "INIT";
    constexpr const char* kCopyPatch                    = "COPY";
    constexpr const char* kPastePatch                   = "PASTE";
    constexpr const char* kStorePatch                   = "STORE";
    constexpr const char* kLoadPrevComputerPatch        = "<";
    constexpr const char* kLoadNextComputerPatch        = ">";
    constexpr const char* kSelectComputerPatch          = "";
    constexpr const char* kOpenComputerPatchFolder      = "OPEN";
    constexpr const char* kSaveComputerPatchAs          = "SAVE AS";
    constexpr const char* kSaveComputerPatch            = "SAVE";
}

// ============================================================================
// Data Declarations (definitions in SynthDescriptors.cpp)
// ============================================================================

// Synth
extern const std::vector<GroupDescriptor> kAllGroups;

// Master Edit
extern const std::vector<IntParameterDescriptor> kMasterEditIntParameters;
extern const std::vector<ChoiceParameterDescriptor> kMasterEditChoiceParameters;
extern const std::vector<StandaloneWidgetDescriptor> kMasterEditStandaloneWidgets;


// Patch Edit

// DCO 1
extern const std::vector<IntParameterDescriptor> kDco1IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kDco1ChoiceParameters;

// DCO 2
extern const std::vector<IntParameterDescriptor> kDco2IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kDco2ChoiceParameters;

// VCF/VCA
extern const std::vector<IntParameterDescriptor> kVcfVcaIntParameters;
extern const std::vector<ChoiceParameterDescriptor> kVcfVcaChoiceParameters;

// FM/TRACK
extern const std::vector<IntParameterDescriptor> kFmTrackIntParameters;
extern const std::vector<ChoiceParameterDescriptor> kFmTrackChoiceParameters;

// RAMP/PORTAMENTO
extern const std::vector<IntParameterDescriptor> kRampPortamentoIntParameters;
extern const std::vector<ChoiceParameterDescriptor> kRampPortamentoChoiceParameters;

// ENV 1
extern const std::vector<IntParameterDescriptor> kEnv1IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kEnv1ChoiceParameters;

// ENV 2
extern const std::vector<IntParameterDescriptor> kEnv2IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kEnv2ChoiceParameters;

// ENV 3
extern const std::vector<IntParameterDescriptor> kEnv3IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kEnv3ChoiceParameters;

// LFO 1
extern const std::vector<IntParameterDescriptor> kLfo1IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kLfo1ChoiceParameters;

// LFO 2
extern const std::vector<IntParameterDescriptor> kLfo2IntParameters;
extern const std::vector<ChoiceParameterDescriptor> kLfo2ChoiceParameters;

// Standalone Widgets
extern const std::vector<StandaloneWidgetDescriptor> kDcoStandaloneWidgets;
extern const std::vector<StandaloneWidgetDescriptor> kEnvStandaloneWidgets;
extern const std::vector<StandaloneWidgetDescriptor> kLfoStandaloneWidgets;
extern const std::vector<StandaloneWidgetDescriptor> kBankUtilityWidgets;
extern const std::vector<StandaloneWidgetDescriptor> kInternalPatchesWidgets;
extern const std::vector<StandaloneWidgetDescriptor> kComputerPatchesWidgets;

// Matrix Modulation
extern const std::array<std::vector<IntParameterDescriptor>, kMatrixModBusCount> kMatrixModBusIntParameters;
extern const std::array<std::vector<ChoiceParameterDescriptor>, kMatrixModBusCount> kMatrixModBusChoiceParameters;

// ============================================================================
// Helper Functions
// ============================================================================

juce::String getGroupDisplayName(const juce::String& groupId);
juce::String getSectionDisplayName(const char* sectionId);

} // namespace SynthDescriptors
