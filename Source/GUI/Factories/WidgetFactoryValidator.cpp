#include "WidgetFactoryValidator.h"

#include "../../Core/Exceptions/ExceptionPropagator.h"

WidgetFactoryValidator::WidgetFactoryValidator(juce::AudioProcessorValueTreeState& inApvts)
    : apvts(inApvts)
{
}

void WidgetFactoryValidator::throwIfParameterIdEmpty(const juce::String& inParameterId)
{
    if (inParameterId.isEmpty())
    {
        InvalidParameterException exception(inParameterId, "Empty parameter ID");
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
}

void WidgetFactoryValidator::throwIfWidgetIdEmpty(const juce::String& inWidgetId)
{
    if (inWidgetId.isEmpty())
    {
        InvalidParameterException exception(inWidgetId, "Empty widget ID");
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
}

const SynthDescriptors::IntParameterDescriptor* WidgetFactoryValidator::getIntParameterDescriptorOrThrow(
    const SynthDescriptors::IntParameterDescriptor* inDescriptor,
    const juce::String& inParameterId) const
{
    if (inDescriptor == nullptr)
    {
        ParameterNotFoundException exception(inParameterId);
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
    return inDescriptor;
}

const SynthDescriptors::ChoiceParameterDescriptor* WidgetFactoryValidator::getChoiceParameterDescriptorOrThrow(
    const SynthDescriptors::ChoiceParameterDescriptor* inDescriptor,
    const juce::String& inParameterId) const
{
    if (inDescriptor == nullptr)
    {
        ParameterNotFoundException exception(inParameterId);
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
    return inDescriptor;
}

const SynthDescriptors::StandaloneWidgetDescriptor* WidgetFactoryValidator::getStandaloneWidgetDescriptorOrThrow(
    const SynthDescriptors::StandaloneWidgetDescriptor* inDescriptor,
    const juce::String& inWidgetId) const
{
    if (inDescriptor == nullptr)
    {
        WidgetNotFoundException exception(inWidgetId);
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
    return inDescriptor;
}

void WidgetFactoryValidator::validateIntParameterValues(
    const SynthDescriptors::IntParameterDescriptor* inDescriptor,
    const juce::String& inParameterId) const
{
    if (inDescriptor->minValue > inDescriptor->maxValue)
    {
        InvalidParameterException exception(
            inParameterId,
            "minValue (" + juce::String(inDescriptor->minValue) + 
            ") > maxValue (" + juce::String(inDescriptor->maxValue) + ")",
            WidgetFactoryErrors::Severity::Warning
        );
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
    
    if (inDescriptor->defaultValue < inDescriptor->minValue || 
        inDescriptor->defaultValue > inDescriptor->maxValue)
    {
        InvalidParameterException exception(
            inParameterId,
            "defaultValue (" + juce::String(inDescriptor->defaultValue) + 
            ") out of range [" + juce::String(inDescriptor->minValue) + 
            "-" + juce::String(inDescriptor->maxValue) + "]",
            WidgetFactoryErrors::Severity::Warning
        );
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
}

void WidgetFactoryValidator::validateChoiceParameterValues(
    const SynthDescriptors::ChoiceParameterDescriptor* inDescriptor,
    const juce::String& inParameterId) const
{
    if (inDescriptor->choices.isEmpty())
    {
        InvalidParameterException exception(
            inParameterId,
            "Empty choices array",
            WidgetFactoryErrors::Severity::Warning
        );
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
    
    if (inDescriptor->defaultIndex < 0 || 
        inDescriptor->defaultIndex >= inDescriptor->choices.size())
    {
        InvalidParameterException exception(
            inParameterId,
            "Invalid defaultIndex: " + juce::String(inDescriptor->defaultIndex) + 
            " (choices count: " + juce::String(inDescriptor->choices.size()) + ")",
            WidgetFactoryErrors::Severity::Warning
        );
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
}

void WidgetFactoryValidator::validateWidgetType(
    const SynthDescriptors::StandaloneWidgetDescriptor* inDescriptor,
    const juce::String& inWidgetId) const
{
    if (inDescriptor->widgetType != SynthDescriptors::StandaloneWidgetType::kButton)
    {
        juce::String actualType = getWidgetTypeString(inDescriptor->widgetType);
        InvalidWidgetTypeException exception(
            inWidgetId,
            "Button",
            actualType
        );
        ExceptionPropagator::propagateToApvts(apvts, exception);
        throw exception;
    }
}

juce::String WidgetFactoryValidator::getWidgetTypeString(SynthDescriptors::StandaloneWidgetType inWidgetType) const
{
    switch (inWidgetType)
    {
        case SynthDescriptors::StandaloneWidgetType::kButton:
            return "Button";
        case SynthDescriptors::StandaloneWidgetType::kComboBox:
            return "ComboBox";
        case SynthDescriptors::StandaloneWidgetType::kNumber:
            return "Number";
        case SynthDescriptors::StandaloneWidgetType::kLabel:
            return "Label";
    }
}

