#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

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
    VibratoPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~VibratoPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::MasterEditPanel::VibratoPanel::kWidth; }
    static int getHeight() { return kHeight_; }

private:
    inline constexpr static int kHeight_ = 220;
    
    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> vibratoModuleHeader_;

    // Labels
    std::unique_ptr<tss::Label> vibratoSpeedLabel_;
    std::unique_ptr<tss::Label> vibratoSpeedModSourceLabel_;
    std::unique_ptr<tss::Label> vibratoSpeedModAmountLabel_;
    std::unique_ptr<tss::Label> vibratoWaveformLabel_;
    std::unique_ptr<tss::Label> vibratoAmplitudeLabel_;
    std::unique_ptr<tss::Label> vibratoAmpModSourceLabel_;
    std::unique_ptr<tss::Label> vibratoAmpModAmountLabel_;

    // Sliders
    std::unique_ptr<tss::Slider> vibratoSpeedSlider_;
    std::unique_ptr<tss::Slider> vibratoSpeedModAmountSlider_;
    std::unique_ptr<tss::Slider> vibratoAmplitudeSlider_;
    std::unique_ptr<tss::Slider> vibratoAmpModAmountSlider_;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> vibratoSpeedModSourceComboBox_;
    std::unique_ptr<tss::ComboBox> vibratoWaveformComboBox_;
    std::unique_ptr<tss::ComboBox> vibratoAmpModSourceComboBox_;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSpeedAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoSpeedModSourceAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoSpeedModAmountAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoWaveformAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAmplitudeAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> vibratoAmpModSourceAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAmpModAmountAttachment_;

    void setupModuleHeader(tss::Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupIntParameterWithSlider(tss::Theme& theme, WidgetFactory& widgetFactory,
                                     const juce::String& parameterId,
                                     std::unique_ptr<tss::Label>& label,
                                     std::unique_ptr<tss::Slider>& slider,
                                     std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                     std::unique_ptr<tss::HorizontalSeparator>& separator);
    void setupChoiceParameterWithComboBox(tss::Theme& theme, WidgetFactory& widgetFactory,
                                          const juce::String& parameterId,
                                          std::unique_ptr<tss::Label>& label,
                                          std::unique_ptr<tss::ComboBox>& comboBox,
                                          std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                          std::unique_ptr<tss::HorizontalSeparator>& separator);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VibratoPanel)
};
