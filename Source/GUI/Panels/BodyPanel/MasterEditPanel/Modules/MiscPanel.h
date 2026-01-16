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

class MiscPanel : public juce::Component
{
public:
    MiscPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MiscPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return kWidth_; }
    static int getHeight() { return kHeight_; }

private:
    inline constexpr static int kWidth_ = 160;
    inline constexpr static int kHeight_ = 205;
    
    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> miscModuleHeader_;

    // Labels
    std::unique_ptr<tss::Label> masterTuneLabel_;
    std::unique_ptr<tss::Label> masterTransposeLabel_;
    std::unique_ptr<tss::Label> bendRangeLabel_;
    std::unique_ptr<tss::Label> unisonEnableLabel_;
    std::unique_ptr<tss::Label> volumeInvertEnableLabel_;
    std::unique_ptr<tss::Label> bankLockEnableLabel_;
    std::unique_ptr<tss::Label> memoryProtectEnableLabel_;

    // Sliders
    std::unique_ptr<tss::Slider> masterTuneSlider_;
    std::unique_ptr<tss::Slider> masterTransposeSlider_;
    std::unique_ptr<tss::Slider> bendRangeSlider_;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> unisonEnableComboBox_;
    std::unique_ptr<tss::ComboBox> volumeInvertEnableComboBox_;
    std::unique_ptr<tss::ComboBox> bankLockEnableComboBox_;
    std::unique_ptr<tss::ComboBox> memoryProtectEnableComboBox_;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterTuneAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterTransposeAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bendRangeAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> unisonEnableAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> volumeInvertEnableAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> bankLockEnableAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> memoryProtectEnableAttachment_;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiscPanel)
};
