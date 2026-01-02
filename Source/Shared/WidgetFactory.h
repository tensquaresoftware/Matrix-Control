#pragma once

#include <memory>

#include <juce_audio_processors/juce_audio_processors.h>

#include "SynthDescriptors.h"

namespace tss
{
    class Skin;
    class Slider;
    class ComboBox;
    class Button;
}

class WidgetFactory
{
public:
    explicit WidgetFactory(juce::AudioProcessorValueTreeState& inApvts);

    // ============================================================================
    // Widget Creation Methods
    // ============================================================================
    
    std::unique_ptr<tss::Slider> createIntParameterSlider(
        const juce::String& parameterId,
        tss::Skin& skin);
    
    std::unique_ptr<tss::ComboBox> createChoiceParameterComboBox(
        const juce::String& parameterId,
        tss::Skin& skin);
    
    std::unique_ptr<tss::Button> createStandaloneButton(
        const juce::String& widgetId,
        tss::Skin& skin);

    // ============================================================================
    // Display Name Helper Methods
    // ============================================================================
    
    juce::String getParameterDisplayName(const juce::String& parameterId);
    juce::String getGroupDisplayName(const juce::String& groupId);
    juce::String getStandaloneWidgetDisplayName(const juce::String& widgetId);

private:
    juce::AudioProcessorValueTreeState& apvts;
    
    // Helper methods to query SynthDescriptors
    const SynthDescriptors::IntParameterDescriptor* findIntParameter(const juce::String& parameterId);
    const SynthDescriptors::ChoiceParameterDescriptor* findChoiceParameter(const juce::String& parameterId);
    const SynthDescriptors::StandaloneWidgetDescriptor* findStandaloneWidget(const juce::String& widgetId);
    const SynthDescriptors::GroupDescriptor* findGroup(const juce::String& groupId);
    
    const SynthDescriptors::IntParameterDescriptor* searchInPatchEditModuleIntParameters(const juce::String& parameterId);
    const SynthDescriptors::IntParameterDescriptor* searchInMatrixModulationBusIntParameters(const juce::String& parameterId);
    const SynthDescriptors::ChoiceParameterDescriptor* searchInPatchEditModuleChoiceParameters(const juce::String& parameterId);
    const SynthDescriptors::ChoiceParameterDescriptor* searchInMatrixModulationBusChoiceParameters(const juce::String& parameterId);
    const SynthDescriptors::StandaloneWidgetDescriptor* searchInPatchEditStandaloneWidgets(const juce::String& widgetId);
    const SynthDescriptors::StandaloneWidgetDescriptor* searchInPatchManagerStandaloneWidgets(const juce::String& widgetId);
};

