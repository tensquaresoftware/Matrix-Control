// Patch Edit Section descriptors — DCO 1, DCO 2, and VCF/VCA modules.
// Sibling translation units cover FM/Track, Ramp, Envelopes, and LFOs.

#include "PluginDescriptors.h"

#include "PluginDisplayNames.h"
#include "PluginHelpers.h"
#include "PluginIDs.h"


namespace PluginDescriptors
{
    // =================================================================================================================
    // Patch Edit Section | DCO 1 Module
    // =================================================================================================================

    // Standalone Widgets ----------------------------------------------------------------------------------------------
    const std::vector<StandaloneWidgetDescriptor> PatchEditSection::Dco1Module::kStandaloneWidgets = {
        {
            .widgetId = PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kInit,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::StandaloneWidgets::kInit,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        },
        {
            .widgetId = PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kCopy,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::StandaloneWidgets::kCopy,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        },
        {
            .widgetId = PluginIDs::PatchEditSection::Dco1Module::StandaloneWidgets::kPaste,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::StandaloneWidgets::kPaste,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        }
    };

    // Int Parameters --------------------------------------------------------------------------------------------------
    const std::vector<IntParameterDescriptor> PatchEditSection::Dco1Module::kIntParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kFrequency,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kFrequency,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 9,
            .sysExId = 0
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kFrequencyModByLfo1,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kFrequencyModByLfo1,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 86,
            .sysExId = 1
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kPulseWidth,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kPulseWidth,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 31,
            .sysExOffset = 11,
            .sysExId = 3
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kPulseWidthModByLfo2,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kPulseWidthModByLfo2,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 87,
            .sysExId = 4
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kWaveShape,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kWaveShape,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 31,
            .sysExOffset = 10,
            .sysExId = 5
        }
    };

    // Choice Parameters -----------------------------------------------------------------------------------------------
    const std::vector<ChoiceParameterDescriptor> PatchEditSection::Dco1Module::kChoiceParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kSync,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kSync,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::Sync::kOff,
                PluginDisplayNames::ChoiceLists::Sync::kSoft,
                PluginDisplayNames::ChoiceLists::Sync::kMedium,
                PluginDisplayNames::ChoiceLists::Sync::kHard }),
            .defaultIndex = 0,
            .sysExOffset = 25,
            .sysExId = 2
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kWaveSelect,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kWaveSelect,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::WaveSelect::kOff,
                PluginDisplayNames::ChoiceLists::WaveSelect::kPulse,
                PluginDisplayNames::ChoiceLists::WaveSelect::kWave,
                PluginDisplayNames::ChoiceLists::WaveSelect::kBoth }),
            .defaultIndex = 2,
            .sysExOffset = 13,
            .sysExId = 6
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kLevers,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kLevers,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::Levers::kOff,
                PluginDisplayNames::ChoiceLists::Levers::kLever1Bend,
                PluginDisplayNames::ChoiceLists::Levers::kLever2Vibrato,
                PluginDisplayNames::ChoiceLists::Levers::kBoth }),
            .defaultIndex = 3,
            .sysExOffset = 12,
            .sysExId = 7
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kKeyboardPortamento,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kKeyboardPortamento,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::KeyboardPortamento::kKeyboard, PluginDisplayNames::ChoiceLists::KeyboardPortamento::kPortamento }),
            .defaultIndex = 0,
            .sysExOffset = 21,
            .sysExId = 8
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kKeyClick,
            .displayName = PluginDisplayNames::PatchEditSection::Dco1Module::ParameterWidgets::kKeyClick,
            .parentGroupId = PluginIDs::PatchEditSection::Dco1Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::OnOff::kOff,
                PluginDisplayNames::ChoiceLists::OnOff::kOn }),
            .defaultIndex = 0,
            .sysExOffset = 22,
            .sysExId = 9
        }
    };


    // =================================================================================================================
    // Patch Edit Section | DCO 2 Module
    // =================================================================================================================

    // Standalone Widgets ----------------------------------------------------------------------------------------------
    const std::vector<StandaloneWidgetDescriptor> PatchEditSection::Dco2Module::kStandaloneWidgets = {
        {
            .widgetId = PluginIDs::PatchEditSection::Dco2Module::StandaloneWidgets::kInit,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::StandaloneWidgets::kInit,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        },
        {
            .widgetId = PluginIDs::PatchEditSection::Dco2Module::StandaloneWidgets::kCopy,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::StandaloneWidgets::kCopy,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        },
        {
            .widgetId = PluginIDs::PatchEditSection::Dco2Module::StandaloneWidgets::kPaste,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::StandaloneWidgets::kPaste,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        }
    };

    // Int Parameters --------------------------------------------------------------------------------------------------
    const std::vector<IntParameterDescriptor> PatchEditSection::Dco2Module::kIntParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kFrequency,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kFrequency,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 14,
            .sysExId = 10
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kFrequencyModByLfo1,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kFrequencyModByLfo1,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 88,
            .sysExId = 11
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kDetune,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kDetune,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = -31,
            .maxValue = 31,
            .defaultValue = 2,
            .sysExOffset = 19,
            .sysExId = 12
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kPulseWidth,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kPulseWidth,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 24,
            .sysExOffset = 16,
            .sysExId = 13
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kPulseWidthModByLfo2,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kPulseWidthModByLfo2,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 89,
            .sysExId = 14
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kWaveShape,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kWaveShape,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 31,
            .sysExOffset = 15,
            .sysExId = 15
        }
    };

    // Choice Parameters -----------------------------------------------------------------------------------------------
    const std::vector<ChoiceParameterDescriptor> PatchEditSection::Dco2Module::kChoiceParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kWaveSelect,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kWaveSelect,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::WaveSelect::kOff,
                PluginDisplayNames::ChoiceLists::WaveSelect::kPulse,
                PluginDisplayNames::ChoiceLists::WaveSelect::kWave,
                PluginDisplayNames::ChoiceLists::WaveSelect::kBoth,
                PluginDisplayNames::ChoiceLists::WaveSelect::kNoise }),
            .defaultIndex = 1,
            .sysExOffset = 18,
            .sysExId = 16
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kLevers,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kLevers,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::Levers::kOff,
                PluginDisplayNames::ChoiceLists::Levers::kLever1Bend,
                PluginDisplayNames::ChoiceLists::Levers::kLever2Vibrato,
                PluginDisplayNames::ChoiceLists::Levers::kBoth }),
            .defaultIndex = 3,
            .sysExOffset = 17,
            .sysExId = 17
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kKeyboardPortamento,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kKeyboardPortamento,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::KeyboardPortamento::kOff, PluginDisplayNames::ChoiceLists::KeyboardPortamento::kPortamento, PluginDisplayNames::ChoiceLists::KeyboardPortamento::kKeyboard }),
            .defaultIndex = 2,
            .sysExOffset = 23,
            .sysExId = 18
        },
        {
            .parameterId = PluginIDs::PatchEditSection::Dco2Module::ParameterWidgets::kKeyClick,
            .displayName = PluginDisplayNames::PatchEditSection::Dco2Module::ParameterWidgets::kKeyClick,
            .parentGroupId = PluginIDs::PatchEditSection::Dco2Module::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::OnOff::kOff,
                PluginDisplayNames::ChoiceLists::OnOff::kOn }),
            .defaultIndex = 0,
            .sysExOffset = 24,
            .sysExId = 19
        }
    };


    // =================================================================================================================
    // Patch Edit Section | VCF/VCA Module
    // =================================================================================================================

    // Standalone Widgets ----------------------------------------------------------------------------------------------
    const std::vector<StandaloneWidgetDescriptor> PatchEditSection::VcfVcaModule::kStandaloneWidgets = {
        {
            .widgetId = PluginIDs::PatchEditSection::VcfVcaModule::StandaloneWidgets::kInit,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::StandaloneWidgets::kInit,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .widgetType = StandaloneWidgetType::kButton,
        }
    };

    // Int Parameters --------------------------------------------------------------------------------------------------
    const std::vector<IntParameterDescriptor> PatchEditSection::VcfVcaModule::kIntParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kBalance,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kBalance,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 31,
            .sysExOffset = 20,
            .sysExId = 20
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequency,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequency,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = 0,
            .maxValue = 127,
            .defaultValue = 55,
            .sysExOffset = 26,
            .sysExId = 21
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByEnv1,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByEnv1,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 42,
            .sysExOffset = 90,
            .sysExId = 22
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByPressure,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kFrequencyModByPressure,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 91,
            .sysExId = 23
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kResonance,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kResonance,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 27,
            .sysExId = 24
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1Volume,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1Volume,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = 0,
            .maxValue = 63,
            .defaultValue = 63,
            .sysExOffset = 31,
            .sysExId = 27
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1ModByVelocity,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1ModByVelocity,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 0,
            .sysExOffset = 92,
            .sysExId = 28
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca2ModByEnv2,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca2ModByEnv2,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .minValue = -63,
            .maxValue = 63,
            .defaultValue = 63,
            .sysExOffset = 93,
            .sysExId = 29
        }
    };

    // Choice Parameters -----------------------------------------------------------------------------------------------
    const std::vector<ChoiceParameterDescriptor> PatchEditSection::VcfVcaModule::kChoiceParameters = {
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kLevers,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kLevers,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::Levers::kOff,
                PluginDisplayNames::ChoiceLists::Levers::kLever1Bend,
                PluginDisplayNames::ChoiceLists::Levers::kLever2Vibrato,
                PluginDisplayNames::ChoiceLists::Levers::kBoth }),
            .defaultIndex = 0,
            .sysExOffset = 28,
            .sysExId = 25
        },
        {
            .parameterId = PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kKeyboardPortamento,
            .displayName = PluginDisplayNames::PatchEditSection::VcfVcaModule::ParameterWidgets::kKeyboardPortamento,
            .parentGroupId = PluginIDs::PatchEditSection::VcfVcaModule::kGroupId,
            .choices = PluginHelpers::makeStringArray({
                PluginDisplayNames::ChoiceLists::KeyboardPortamento::kOff, PluginDisplayNames::ChoiceLists::KeyboardPortamento::kPortamento, PluginDisplayNames::ChoiceLists::KeyboardPortamento::kKeyboard }),
            .defaultIndex = 2,
            .sysExOffset = 29,
            .sysExId = 26
        }
    };
}
