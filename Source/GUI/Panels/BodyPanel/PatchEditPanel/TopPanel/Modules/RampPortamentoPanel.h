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
    class HorizontalSeparator;
}

class WidgetFactory;

class RampPortamentoPanel : public juce::Component
{
public:
    RampPortamentoPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~RampPortamentoPanel() override;

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
    std::unique_ptr<tss::ModuleHeader> rampPortamentoModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> rampPortamentoInitButton;

    // Labels
    std::unique_ptr<tss::Label> ramp1RateLabel;
    std::unique_ptr<tss::Label> ramp1TriggerLabel;
    std::unique_ptr<tss::Label> ramp2RateLabel;
    std::unique_ptr<tss::Label> ramp2TriggerLabel;
    std::unique_ptr<tss::Label> portamentoRateLabel;
    std::unique_ptr<tss::Label> portamentoModByVelocityLabel;
    std::unique_ptr<tss::Label> portamentoModeLabel;
    std::unique_ptr<tss::Label> portamentoLegatoLabel;
    std::unique_ptr<tss::Label> portamentoKeyboardModeLabel;

    // Sliders
    std::unique_ptr<tss::Slider> ramp1RateSlider;
    std::unique_ptr<tss::Slider> ramp2RateSlider;
    std::unique_ptr<tss::Slider> portamentoRateSlider;
    std::unique_ptr<tss::Slider> portamentoModByVelocitySlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> ramp1TriggerComboBox;
    std::unique_ptr<tss::ComboBox> ramp2TriggerComboBox;
    std::unique_ptr<tss::ComboBox> portamentoModeComboBox;
    std::unique_ptr<tss::ComboBox> portamentoLegatoComboBox;
    std::unique_ptr<tss::ComboBox> portamentoKeyboardModeComboBox;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator8;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator9;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator10;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ramp1RateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ramp1TriggerAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ramp2RateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ramp2TriggerAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> portamentoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> portamentoModByVelocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> portamentoModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> portamentoLegatoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> portamentoKeyboardModeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RampPortamentoPanel)
};
