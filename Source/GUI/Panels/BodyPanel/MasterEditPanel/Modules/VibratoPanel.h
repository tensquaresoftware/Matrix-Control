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
    class HorizontalSeparator;
}

class WidgetFactory;

class VibratoPanel : public juce::Component
{
public:
    VibratoPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~VibratoPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 160;
    inline constexpr static int kHeight = 220;
    
    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> vibratoModuleHeader;

    // Labels
    std::unique_ptr<tss::Label> vibratoSpeedLabel;
    std::unique_ptr<tss::Label> vibratoSpeedModSourceLabel;
    std::unique_ptr<tss::Label> vibratoSpeedModAmountLabel;
    std::unique_ptr<tss::Label> vibratoWaveformLabel;
    std::unique_ptr<tss::Label> vibratoAmplitudeLabel;
    std::unique_ptr<tss::Label> vibratoAmpModSourceLabel;
    std::unique_ptr<tss::Label> vibratoAmpModAmountLabel;

    // Sliders
    std::unique_ptr<tss::Slider> vibratoSpeedSlider;
    std::unique_ptr<tss::Slider> vibratoSpeedModAmountSlider;
    std::unique_ptr<tss::Slider> vibratoAmplitudeSlider;
    std::unique_ptr<tss::Slider> vibratoAmpModAmountSlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> vibratoSpeedModSourceComboBox;
    std::unique_ptr<tss::ComboBox> vibratoWaveformComboBox;
    std::unique_ptr<tss::ComboBox> vibratoAmpModSourceComboBox;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSpeedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoSpeedModSourceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSpeedModAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoAmpModSourceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAmpModAmountAttachment;

    void setupModuleHeader(tss::Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupIntParameterWithSlider(tss::Theme& inTheme, WidgetFactory& widgetFactory,
                                     const juce::String& parameterId,
                                     std::unique_ptr<tss::Label>& label,
                                     std::unique_ptr<tss::Slider>& slider,
                                     std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                     std::unique_ptr<tss::HorizontalSeparator>& separator);
    void setupChoiceParameterWithComboBox(tss::Theme& inTheme, WidgetFactory& widgetFactory,
                                          const juce::String& parameterId,
                                          std::unique_ptr<tss::Label>& label,
                                          std::unique_ptr<tss::ComboBox>& comboBox,
                                          std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                          std::unique_ptr<tss::HorizontalSeparator>& separator);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibratoPanel)
};
