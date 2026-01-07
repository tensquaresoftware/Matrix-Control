#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
    class ModuleHeader;
    class Label;
    class Slider;
    class ComboBox;
    class Button;
    class ParameterSeparator;
}

class WidgetFactory;

class Lfo2Panel : public juce::Component
{
public:
    Lfo2Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Lfo2Panel() override;

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
    std::unique_ptr<tss::ModuleHeader> lfo2ModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> lfo2InitButton;
    std::unique_ptr<tss::Button> lfo2CopyButton;
    std::unique_ptr<tss::Button> lfo2PasteButton;

    // Labels
    std::unique_ptr<tss::Label> lfo2SpeedLabel;
    std::unique_ptr<tss::Label> lfo2SpeedModByKeyboardLabel;
    std::unique_ptr<tss::Label> lfo2RetriggerPointLabel;
    std::unique_ptr<tss::Label> lfo2AmplitudeLabel;
    std::unique_ptr<tss::Label> lfo2AmplitudeModByRamp2Label;
    std::unique_ptr<tss::Label> lfo2WaveformLabel;
    std::unique_ptr<tss::Label> lfo2TriggerModeLabel;
    std::unique_ptr<tss::Label> lfo2LagLabel;
    std::unique_ptr<tss::Label> lfo2SampleInputLabel;

    // Sliders
    std::unique_ptr<tss::Slider> lfo2SpeedSlider;
    std::unique_ptr<tss::Slider> lfo2SpeedModByKeyboardSlider;
    std::unique_ptr<tss::Slider> lfo2RetriggerPointSlider;
    std::unique_ptr<tss::Slider> lfo2AmplitudeSlider;
    std::unique_ptr<tss::Slider> lfo2AmplitudeModByRamp2Slider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> lfo2WaveformComboBox;
    std::unique_ptr<tss::ComboBox> lfo2TriggerModeComboBox;
    std::unique_ptr<tss::ComboBox> lfo2LagComboBox;
    std::unique_ptr<tss::ComboBox> lfo2SampleInputComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo2SpeedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo2SpeedModByKeyboardAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo2RetriggerPointAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo2AmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo2AmplitudeModByRamp2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo2WaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo2TriggerModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo2LagAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo2SampleInputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lfo2Panel)
};
