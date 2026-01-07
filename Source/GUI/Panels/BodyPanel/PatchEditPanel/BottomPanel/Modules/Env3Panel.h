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

class Env3Panel : public juce::Component
{
public:
    Env3Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Env3Panel() override;

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
    std::unique_ptr<tss::ModuleName> env3ModuleName;

    // Buttons
    std::unique_ptr<tss::Button> env3InitButton;
    std::unique_ptr<tss::Button> env3CopyButton;
    std::unique_ptr<tss::Button> env3PasteButton;

    // Labels
    std::unique_ptr<tss::ParameterLabel> env3DelayLabel;
    std::unique_ptr<tss::ParameterLabel> env3AttackLabel;
    std::unique_ptr<tss::ParameterLabel> env3DecayLabel;
    std::unique_ptr<tss::ParameterLabel> env3SustainLabel;
    std::unique_ptr<tss::ParameterLabel> env3ReleaseLabel;
    std::unique_ptr<tss::ParameterLabel> env3AmplitudeLabel;
    std::unique_ptr<tss::ParameterLabel> env3AmplitudeModByVelocityLabel;
    std::unique_ptr<tss::ParameterLabel> env3TriggerModeLabel;
    std::unique_ptr<tss::ParameterLabel> env3EnvelopeModeLabel;
    std::unique_ptr<tss::ParameterLabel> env3Lfo1TriggerLabel;

    // Sliders
    std::unique_ptr<tss::Slider> env3DelaySlider;
    std::unique_ptr<tss::Slider> env3AttackSlider;
    std::unique_ptr<tss::Slider> env3DecaySlider;
    std::unique_ptr<tss::Slider> env3SustainSlider;
    std::unique_ptr<tss::Slider> env3ReleaseSlider;
    std::unique_ptr<tss::Slider> env3AmplitudeSlider;
    std::unique_ptr<tss::Slider> env3AmplitudeModByVelocitySlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> env3TriggerModeComboBox;
    std::unique_ptr<tss::ComboBox> env3EnvelopeModeComboBox;
    std::unique_ptr<tss::ComboBox> env3Lfo1TriggerComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3DelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3AttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3DecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3SustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3ReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3AmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env3AmplitudeModByVelocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env3TriggerModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env3EnvelopeModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env3Lfo1TriggerAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Env3Panel)
};
