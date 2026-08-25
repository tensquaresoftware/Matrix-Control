#include "ApvtsUndoableParameterAttachments.h"

namespace TSS
{
    namespace
    {
        juce::RangedAudioParameter* getRangedParameterOrNull(juce::AudioProcessorValueTreeState& apvts,
                                                             const juce::String& parameterId)
        {
            return dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter(parameterId));
        }

        void configureSliderRange(juce::Slider& slider, juce::RangedAudioParameter& parameter)
        {
            auto range = parameter.getNormalisableRange();

            auto convertFrom0To1Function = [range](double currentRangeStart,
                                                     double currentRangeEnd,
                                                     double normalisedValue) mutable
            {
                range.start = static_cast<float>(currentRangeStart);
                range.end = static_cast<float>(currentRangeEnd);
                return static_cast<double>(range.convertFrom0to1(static_cast<float>(normalisedValue)));
            };

            auto convertTo0To1Function = [range](double currentRangeStart,
                                                 double currentRangeEnd,
                                                 double mappedValue) mutable
            {
                range.start = static_cast<float>(currentRangeStart);
                range.end = static_cast<float>(currentRangeEnd);
                return static_cast<double>(range.convertTo0to1(static_cast<float>(mappedValue)));
            };

            auto snapToLegalValueFunction = [range](double currentRangeStart,
                                                      double currentRangeEnd,
                                                      double mappedValue) mutable
            {
                range.start = static_cast<float>(currentRangeStart);
                range.end = static_cast<float>(currentRangeEnd);
                return static_cast<double>(range.snapToLegalValue(static_cast<float>(mappedValue)));
            };

            juce::NormalisableRange<double> newRange { static_cast<double>(range.start),
                                                       static_cast<double>(range.end),
                                                       std::move(convertFrom0To1Function),
                                                       std::move(convertTo0To1Function),
                                                       std::move(snapToLegalValueFunction) };
            newRange.interval = range.interval;
            newRange.skew = range.skew;
            newRange.symmetricSkew = range.symmetricSkew;

            slider.setNormalisableRange(newRange);
            slider.setDoubleClickReturnValue(true, parameter.convertTo0to1(parameter.getDefaultValue()));
            slider.valueFromTextFunction = [&parameter](const juce::String& text)
            {
                return static_cast<double>(
                    parameter.convertFrom0to1(parameter.getValueForText(text)));
            };
            slider.textFromValueFunction = [&parameter](double value)
            {
                return parameter.getText(parameter.convertTo0to1(static_cast<float>(value)), 0);
            };
        }

        bool isPerformingEditorialUndoRedo(const juce::AudioProcessorValueTreeState& apvts)
        {
            return apvts.undoManager != nullptr && apvts.undoManager->isPerformingUndoRedo();
        }

        void beginEditorialParameterEdit(juce::AudioProcessorValueTreeState& apvts,
                                         const juce::String& parameterId)
        {
            if (apvts.undoManager != nullptr)
                apvts.undoManager->beginNewTransaction("Parameter edit: " + parameterId);
        }
    }

    ApvtsUndoableSliderAttachment::ApvtsUndoableSliderAttachment(juce::AudioProcessorValueTreeState& apvts,
                                                                 const juce::String& parameterId,
                                                                 juce::Slider& slider)
        : apvts_(apvts)
        , parameterId_(parameterId)
        , slider_(slider)
        , parameter_(getRangedParameterOrNull(apvts_, parameterId_))
        , parameterValue_(apvts.getParameterAsValue(parameterId_))
    {
        jassert(parameter_ != nullptr);
        configureSliderRange(slider_, *parameter_);
        parameterValue_.addListener(this);
        slider_.addListener(this);
        syncSliderFromParameterValue();
    }

    ApvtsUndoableSliderAttachment::~ApvtsUndoableSliderAttachment()
    {
        slider_.removeListener(this);
        parameterValue_.removeListener(this);
    }

    void ApvtsUndoableSliderAttachment::syncSliderFromParameterValue()
    {
        if (parameter_ == nullptr)
            return;

        const juce::ScopedValueSetter<bool> scope(ignoreCallbacks_, true);
        slider_.setValue(static_cast<double>(parameterValue_.getValue()), juce::dontSendNotification);
    }

    void ApvtsUndoableSliderAttachment::writeSliderValueToParameter(bool beginNewEditorialStep)
    {
        if (parameter_ == nullptr || isPerformingEditorialUndoRedo(apvts_))
            return;

        const auto denormalised = static_cast<float>(slider_.getValue());

        if (juce::approximatelyEqual(static_cast<float>(parameterValue_.getValue()), denormalised))
            return;

        if (beginNewEditorialStep)
            beginEditorialParameterEdit(apvts_, parameterId_);

        parameterValue_.setValue(denormalised);
    }

    void ApvtsUndoableSliderAttachment::beginSliderGesture()
    {
        if (parameter_ == nullptr || isPerformingEditorialUndoRedo(apvts_))
            return;

        beginEditorialParameterEdit(apvts_, parameterId_);
        parameter_->beginChangeGesture();
        changeGestureActive_ = true;
    }

    void ApvtsUndoableSliderAttachment::endSliderGesture()
    {
        if (! changeGestureActive_ || parameter_ == nullptr)
            return;

        parameter_->endChangeGesture();
        changeGestureActive_ = false;
    }

    void ApvtsUndoableSliderAttachment::valueChanged(juce::Value& value)
    {
        juce::ignoreUnused(value);

        if (ignoreCallbacks_)
            return;

        syncSliderFromParameterValue();
    }

    void ApvtsUndoableSliderAttachment::sliderDragStarted(juce::Slider*)
    {
        beginSliderGesture();
    }

    void ApvtsUndoableSliderAttachment::sliderValueChanged(juce::Slider*)
    {
        if (ignoreCallbacks_)
            return;

        writeSliderValueToParameter(! changeGestureActive_);
    }

    void ApvtsUndoableSliderAttachment::sliderDragEnded(juce::Slider*)
    {
        endSliderGesture();
    }

    ApvtsUndoableComboBoxAttachment::ApvtsUndoableComboBoxAttachment(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& parameterId,
        juce::ComboBox& comboBox)
        : apvts_(apvts)
        , parameterId_(parameterId)
        , comboBox_(comboBox)
        , parameterValue_(apvts.getParameterAsValue(parameterId_))
    {
        comboBox_.addListener(this);
        parameterValue_.addListener(this);
        syncComboBoxFromParameterValue();
    }

    ApvtsUndoableComboBoxAttachment::~ApvtsUndoableComboBoxAttachment()
    {
        comboBox_.removeListener(this);
        parameterValue_.removeListener(this);
    }

    void ApvtsUndoableComboBoxAttachment::syncComboBoxFromParameterValue()
    {
        const auto selectedIndex = juce::roundToInt(static_cast<float>(parameterValue_.getValue()));
        const juce::ScopedValueSetter<bool> scope(ignoreCallbacks_, true);
        comboBox_.setSelectedItemIndex(selectedIndex, juce::dontSendNotification);
    }

    void ApvtsUndoableComboBoxAttachment::writeComboBoxSelectionToParameter()
    {
        if (isPerformingEditorialUndoRedo(apvts_))
            return;

        const auto selectedIndex = comboBox_.getSelectedItemIndex();
        if (selectedIndex < 0)
            return;

        const auto currentIndex = juce::roundToInt(static_cast<float>(parameterValue_.getValue()));
        if (currentIndex == selectedIndex)
            return;

        beginEditorialParameterEdit(apvts_, parameterId_);
        parameterValue_.setValue(static_cast<float>(selectedIndex));
    }

    void ApvtsUndoableComboBoxAttachment::valueChanged(juce::Value& value)
    {
        juce::ignoreUnused(value);

        if (ignoreCallbacks_)
            return;

        syncComboBoxFromParameterValue();
    }

    void ApvtsUndoableComboBoxAttachment::comboBoxChanged(juce::ComboBox*)
    {
        if (ignoreCallbacks_)
            return;

        writeComboBoxSelectionToParameter();
    }
}
