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

class FmTrackPanel : public juce::Component
{
public:
    FmTrackPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~FmTrackPanel() override;

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
    std::unique_ptr<tss::ModuleHeader> fmTrackModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> fmTrackInitButton;

    // Labels
    std::unique_ptr<tss::Label> fmAmountLabel;
    std::unique_ptr<tss::Label> fmModByEnv3Label;
    std::unique_ptr<tss::Label> fmModByPressureLabel;
    std::unique_ptr<tss::Label> trackPoint1Label;
    std::unique_ptr<tss::Label> trackPoint2Label;
    std::unique_ptr<tss::Label> trackPoint3Label;
    std::unique_ptr<tss::Label> trackPoint4Label;
    std::unique_ptr<tss::Label> trackPoint5Label;
    std::unique_ptr<tss::Label> trackInputLabel;

    // Sliders
    std::unique_ptr<tss::Slider> fmAmountSlider;
    std::unique_ptr<tss::Slider> fmModByEnv3Slider;
    std::unique_ptr<tss::Slider> fmModByPressureSlider;
    std::unique_ptr<tss::Slider> trackPoint1Slider;
    std::unique_ptr<tss::Slider> trackPoint2Slider;
    std::unique_ptr<tss::Slider> trackPoint3Slider;
    std::unique_ptr<tss::Slider> trackPoint4Slider;
    std::unique_ptr<tss::Slider> trackPoint5Slider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> trackInputComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmModByEnv3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fmModByPressureAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trackPoint1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trackPoint2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trackPoint3Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trackPoint4Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trackPoint5Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> trackInputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmTrackPanel)
};
