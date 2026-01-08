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

class Dco2Panel : public juce::Component
{
public:
    Dco2Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Dco2Panel() override;

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
    std::unique_ptr<tss::ModuleHeader> dco2ModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> dco2InitButton;
    std::unique_ptr<tss::Button> dco2CopyButton;
    std::unique_ptr<tss::Button> dco2PasteButton;

    // Labels
    std::unique_ptr<tss::Label> dco2FrequencyLabel;
    std::unique_ptr<tss::Label> dco2FrequencyModByLfo1Label;
    std::unique_ptr<tss::Label> dco2DetuneLabel;
    std::unique_ptr<tss::Label> dco2PulseWidthLabel;
    std::unique_ptr<tss::Label> dco2PulseWidthModByLfo2Label;
    std::unique_ptr<tss::Label> dco2WaveShapeLabel;
    std::unique_ptr<tss::Label> dco2WaveSelectLabel;
    std::unique_ptr<tss::Label> dco2LeversLabel;
    std::unique_ptr<tss::Label> dco2KeyboardPortamentoLabel;
    std::unique_ptr<tss::Label> dco2KeyClickLabel;

    // Sliders
    std::unique_ptr<tss::Slider> dco2FrequencySlider;
    std::unique_ptr<tss::Slider> dco2FrequencyModByLfo1Slider;
    std::unique_ptr<tss::Slider> dco2DetuneSlider;
    std::unique_ptr<tss::Slider> dco2PulseWidthSlider;
    std::unique_ptr<tss::Slider> dco2PulseWidthModByLfo2Slider;
    std::unique_ptr<tss::Slider> dco2WaveShapeSlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> dco2WaveSelectComboBox;
    std::unique_ptr<tss::ComboBox> dco2LeversComboBox;
    std::unique_ptr<tss::ComboBox> dco2KeyboardPortamentoComboBox;
    std::unique_ptr<tss::ComboBox> dco2KeyClickComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2FrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2FrequencyModByLfo1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2DetuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2PulseWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2PulseWidthModByLfo2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco2WaveShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco2WaveSelectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco2LeversAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco2KeyboardPortamentoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco2KeyClickAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dco2Panel)
};
