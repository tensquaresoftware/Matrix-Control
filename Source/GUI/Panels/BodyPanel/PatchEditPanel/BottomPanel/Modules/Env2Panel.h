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

class Env2Panel : public juce::Component
{
public:
    Env2Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Env2Panel() override;

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
    std::unique_ptr<tss::ModuleName> env2ModuleName;

    // Buttons
    std::unique_ptr<tss::Button> env2InitButton;
    std::unique_ptr<tss::Button> env2CopyButton;
    std::unique_ptr<tss::Button> env2PasteButton;

    // Labels
    std::unique_ptr<tss::ParameterLabel> env2DelayLabel;
    std::unique_ptr<tss::ParameterLabel> env2AttackLabel;
    std::unique_ptr<tss::ParameterLabel> env2DecayLabel;
    std::unique_ptr<tss::ParameterLabel> env2SustainLabel;
    std::unique_ptr<tss::ParameterLabel> env2ReleaseLabel;
    std::unique_ptr<tss::ParameterLabel> env2AmplitudeLabel;
    std::unique_ptr<tss::ParameterLabel> env2AmplitudeModByVelocityLabel;
    std::unique_ptr<tss::ParameterLabel> env2TriggerModeLabel;
    std::unique_ptr<tss::ParameterLabel> env2EnvelopeModeLabel;
    std::unique_ptr<tss::ParameterLabel> env2Lfo1TriggerLabel;

    // Sliders
    std::unique_ptr<tss::Slider> env2DelaySlider;
    std::unique_ptr<tss::Slider> env2AttackSlider;
    std::unique_ptr<tss::Slider> env2DecaySlider;
    std::unique_ptr<tss::Slider> env2SustainSlider;
    std::unique_ptr<tss::Slider> env2ReleaseSlider;
    std::unique_ptr<tss::Slider> env2AmplitudeSlider;
    std::unique_ptr<tss::Slider> env2AmplitudeModByVelocitySlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> env2TriggerModeComboBox;
    std::unique_ptr<tss::ComboBox> env2EnvelopeModeComboBox;
    std::unique_ptr<tss::ComboBox> env2Lfo1TriggerComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2DelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2AttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2DecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2SustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2ReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2AmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env2AmplitudeModByVelocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env2TriggerModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env2EnvelopeModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env2Lfo1TriggerAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Env2Panel)
};
