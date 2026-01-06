#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
    class ModuleName;
    class ParameterLabel;
    class Slider;
    class ComboBox;
    class Button;
    class ParameterSeparator;
}

class WidgetFactory;

class Lfo1Panel : public juce::Component
{
public:
    Lfo1Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Lfo1Panel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 150;
    inline constexpr static int kHeight = 280;
    
    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;

    // Module Name
    std::unique_ptr<tss::ModuleName> lfo1ModuleName;

    // Buttons
    std::unique_ptr<tss::Button> lfo1InitButton;
    std::unique_ptr<tss::Button> lfo1CopyButton;
    std::unique_ptr<tss::Button> lfo1PasteButton;

    // Labels
    std::unique_ptr<tss::ParameterLabel> lfo1SpeedLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1SpeedModByPressureLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1RetriggerPointLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1AmplitudeLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1AmplitudeModByRamp1Label;
    std::unique_ptr<tss::ParameterLabel> lfo1WaveformLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1TriggerModeLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1LagLabel;
    std::unique_ptr<tss::ParameterLabel> lfo1SampleInputLabel;

    // Sliders
    std::unique_ptr<tss::Slider> lfo1SpeedSlider;
    std::unique_ptr<tss::Slider> lfo1SpeedModByPressureSlider;
    std::unique_ptr<tss::Slider> lfo1RetriggerPointSlider;
    std::unique_ptr<tss::Slider> lfo1AmplitudeSlider;
    std::unique_ptr<tss::Slider> lfo1AmplitudeModByRamp1Slider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> lfo1WaveformComboBox;
    std::unique_ptr<tss::ComboBox> lfo1TriggerModeComboBox;
    std::unique_ptr<tss::ComboBox> lfo1LagComboBox;
    std::unique_ptr<tss::ComboBox> lfo1SampleInputComboBox;

    // Separators
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator1;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator2;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator3;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator4;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator5;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator6;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator7;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator8;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator9;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator10;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1SpeedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1SpeedModByPressureAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1RetriggerPointAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1AmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1AmplitudeModByRamp1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1WaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1TriggerModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1LagAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1SampleInputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lfo1Panel)
};
