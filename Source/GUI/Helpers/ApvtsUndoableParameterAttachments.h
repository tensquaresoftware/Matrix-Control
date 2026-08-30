#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
    /**
        Slider binding that records APVTS edits through getParameterAsValue().setValue()
        so UndoManager entries are committed synchronously on the ValueTree.

        Display sync always reads the live parameter (not a possibly orphaned Value mirror)
        and rebinds after APVTS replaceState / valueTreeRedirected.
    */
    class ApvtsUndoableSliderAttachment final : private juce::Slider::Listener,
                                                private juce::Value::Listener,
                                                private juce::AudioProcessorParameter::Listener,
                                                private juce::ValueTree::Listener,
                                                private juce::AsyncUpdater
    {
    public:
        ApvtsUndoableSliderAttachment(juce::AudioProcessorValueTreeState& apvts,
                                      const juce::String& parameterId,
                                      juce::Slider& slider);
        ~ApvtsUndoableSliderAttachment() override;

    private:
        void rebindParameterValue();
        void syncSliderFromParameter();
        void writeSliderValueToParameter(bool beginNewEditorialStep);
        void beginSliderGesture();
        void endSliderGesture();

        void valueChanged(juce::Value& value) override;
        void sliderValueChanged(juce::Slider* slider) override;
        void sliderDragStarted(juce::Slider* slider) override;
        void sliderDragEnded(juce::Slider* slider) override;
        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
        void valueTreeRedirected(juce::ValueTree& treeWhosePropertyHasChanged) override;
        void handleAsyncUpdate() override;

        juce::AudioProcessorValueTreeState& apvts_;
        juce::String parameterId_;
        juce::Slider& slider_;
        juce::RangedAudioParameter* parameter_ { nullptr };
        juce::Value parameterValue_;
        bool ignoreCallbacks_ { false };
        bool changeGestureActive_ { false };
    };

    /** ComboBox binding with the same undo-recording strategy as ApvtsUndoableSliderAttachment. */
    class ApvtsUndoableComboBoxAttachment final : private juce::ComboBox::Listener,
                                                  private juce::Value::Listener,
                                                  private juce::AudioProcessorParameter::Listener,
                                                  private juce::ValueTree::Listener,
                                                  private juce::AsyncUpdater
    {
    public:
        ApvtsUndoableComboBoxAttachment(juce::AudioProcessorValueTreeState& apvts,
                                        const juce::String& parameterId,
                                        juce::ComboBox& comboBox);
        ~ApvtsUndoableComboBoxAttachment() override;

    private:
        void rebindParameterValue();
        void syncComboBoxFromParameter();
        void writeComboBoxSelectionToParameter();
        int readChoiceIndexFromParameter() const;

        void valueChanged(juce::Value& value) override;
        void comboBoxChanged(juce::ComboBox* comboBox) override;
        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
        void valueTreeRedirected(juce::ValueTree& treeWhosePropertyHasChanged) override;
        void handleAsyncUpdate() override;

        juce::AudioProcessorValueTreeState& apvts_;
        juce::String parameterId_;
        juce::ComboBox& comboBox_;
        juce::Value parameterValue_;
        bool ignoreCallbacks_ { false };
    };
}
