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

class VcfVcaPanel : public juce::Component
{
public:
    VcfVcaPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~VcfVcaPanel() override;

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
    std::unique_ptr<tss::ModuleHeader> vcfVcaModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> vcfVcaInitButton;

    // Labels
    std::unique_ptr<tss::Label> vcfBalanceLabel;
    std::unique_ptr<tss::Label> vcfFrequencyLabel;
    std::unique_ptr<tss::Label> vcfFrequencyModByEnv1Label;
    std::unique_ptr<tss::Label> vcfFrequencyModByPressureLabel;
    std::unique_ptr<tss::Label> vcfResonanceLabel;
    std::unique_ptr<tss::Label> vca1VolumeLabel;
    std::unique_ptr<tss::Label> vca1ModByVelocityLabel;
    std::unique_ptr<tss::Label> vca2ModByEnv2Label;
    std::unique_ptr<tss::Label> vcfLeversLabel;
    std::unique_ptr<tss::Label> vcfKeyboardPortamentoLabel;

    // Sliders
    std::unique_ptr<tss::Slider> vcfBalanceSlider;
    std::unique_ptr<tss::Slider> vcfFrequencySlider;
    std::unique_ptr<tss::Slider> vcfFrequencyModByEnv1Slider;
    std::unique_ptr<tss::Slider> vcfFrequencyModByPressureSlider;
    std::unique_ptr<tss::Slider> vcfResonanceSlider;
    std::unique_ptr<tss::Slider> vca1VolumeSlider;
    std::unique_ptr<tss::Slider> vca1ModByVelocitySlider;
    std::unique_ptr<tss::Slider> vca2ModByEnv2Slider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> vcfLeversComboBox;
    std::unique_ptr<tss::ComboBox> vcfKeyboardPortamentoComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vcfBalanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vcfFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vcfFrequencyModByEnv1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vcfFrequencyModByPressureAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vcfResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vca1VolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vca1ModByVelocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vca2ModByEnv2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vcfLeversAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vcfKeyboardPortamentoAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VcfVcaPanel)
};
