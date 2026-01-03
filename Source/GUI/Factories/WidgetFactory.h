#pragma once

#include <memory>
#include <map>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "../../Shared/SynthDescriptors.h"
#include "WidgetFactoryValidator.h"

namespace tss
{
    class Theme;
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
        tss::Theme& theme);
    
    std::unique_ptr<tss::ComboBox> createChoiceParameterComboBox(
        const juce::String& parameterId,
        tss::Theme& theme);
    
    std::unique_ptr<tss::Button> createStandaloneButton(
        const juce::String& widgetId,
        tss::Theme& theme);

    // ============================================================================
    // Display Name Helper Methods
    // ============================================================================
    
    juce::String getParameterDisplayName(const juce::String& parameterId) const;
    juce::String getGroupDisplayName(const juce::String& groupId) const;
    juce::String getStandaloneWidgetDisplayName(const juce::String& widgetId) const;

private:
    WidgetFactoryValidator validator;
    
    std::map<juce::String, const SynthDescriptors::IntParameterDescriptor*> intParameterMap;
    std::map<juce::String, const SynthDescriptors::ChoiceParameterDescriptor*> choiceParameterMap;
    std::map<juce::String, const SynthDescriptors::StandaloneWidgetDescriptor*> standaloneWidgetMap;
    std::map<juce::String, const SynthDescriptors::GroupDescriptor*> groupMap;
    
    void buildSearchMaps();
    void buildIntParameterMap();
    void buildChoiceParameterMap();
    void buildStandaloneWidgetMap();
    void buildGroupMap();
    void addPatchEditModuleIntParametersToMap();
    void addMatrixModulationBusIntParametersToMap();
    void addMasterEditIntParametersToMap();
    void addPatchEditModuleChoiceParametersToMap();
    void addMatrixModulationBusChoiceParametersToMap();
    void addMasterEditChoiceParametersToMap();
    void addPatchEditStandaloneWidgetsToMap();
    void addPatchManagerStandaloneWidgetsToMap();
    void addMasterEditStandaloneWidgetsToMap();
    
    const SynthDescriptors::IntParameterDescriptor* findIntParameter(const juce::String& parameterId) const;
    const SynthDescriptors::ChoiceParameterDescriptor* findChoiceParameter(const juce::String& parameterId) const;
    const SynthDescriptors::StandaloneWidgetDescriptor* findStandaloneWidget(const juce::String& widgetId) const;
    const SynthDescriptors::GroupDescriptor* findGroup(const juce::String& groupId) const;
    
    void addIntParametersToMap(const std::vector<SynthDescriptors::IntParameterDescriptor>& parameters);
    void addChoiceParametersToMap(const std::vector<SynthDescriptors::ChoiceParameterDescriptor>& parameters);
    void addStandaloneWidgetsToMap(const std::vector<SynthDescriptors::StandaloneWidgetDescriptor>& widgets);
    
    std::unique_ptr<tss::Slider> createSliderFromDescriptor(const SynthDescriptors::IntParameterDescriptor* desc, tss::Theme& theme);
    std::unique_ptr<tss::ComboBox> createComboBoxFromDescriptor(const SynthDescriptors::ChoiceParameterDescriptor* desc, tss::Theme& theme);
};

