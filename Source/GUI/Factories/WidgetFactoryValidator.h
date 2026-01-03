#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "../../Shared/SynthDescriptors.h"
#include "../../Core/Exceptions/WidgetFactoryExceptions.h"

class WidgetFactoryValidator
{
public:
    explicit WidgetFactoryValidator(juce::AudioProcessorValueTreeState& inApvts);

    void throwIfParameterIdEmpty(const juce::String& inParameterId);
    void throwIfWidgetIdEmpty(const juce::String& inWidgetId);
    
    const SynthDescriptors::IntParameterDescriptor* getIntParameterDescriptorOrThrow(
        const SynthDescriptors::IntParameterDescriptor* inDescriptor,
        const juce::String& inParameterId) const;
    
    const SynthDescriptors::ChoiceParameterDescriptor* getChoiceParameterDescriptorOrThrow(
        const SynthDescriptors::ChoiceParameterDescriptor* inDescriptor,
        const juce::String& inParameterId) const;
    
    const SynthDescriptors::StandaloneWidgetDescriptor* getStandaloneWidgetDescriptorOrThrow(
        const SynthDescriptors::StandaloneWidgetDescriptor* inDescriptor,
        const juce::String& inWidgetId) const;
    
    void validateIntParameterValues(
        const SynthDescriptors::IntParameterDescriptor* inDescriptor,
        const juce::String& inParameterId) const;
    
    void validateChoiceParameterValues(
        const SynthDescriptors::ChoiceParameterDescriptor* inDescriptor,
        const juce::String& inParameterId) const;
    
    void validateWidgetType(
        const SynthDescriptors::StandaloneWidgetDescriptor* inDescriptor,
        const juce::String& inWidgetId) const;

private:
    juce::AudioProcessorValueTreeState& apvts;
    
    juce::String getWidgetTypeString(SynthDescriptors::StandaloneWidgetType inWidgetType) const;
};

