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
    MiscPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~MiscPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 160;
    inline constexpr static int kHeight = 205;
    
    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> miscModuleHeader;

    // Labels
    std::unique_ptr<tss::Label> masterTuneLabel;
    std::unique_ptr<tss::Label> masterTransposeLabel;
    std::unique_ptr<tss::Label> bendRangeLabel;
    std::unique_ptr<tss::Label> unisonEnableLabel;
    std::unique_ptr<tss::Label> volumeInvertEnableLabel;
    std::unique_ptr<tss::Label> bankLockEnableLabel;
    std::unique_ptr<tss::Label> memoryProtectEnableLabel;

    // Sliders
    std::unique_ptr<tss::Slider> masterTuneSlider;
    std::unique_ptr<tss::Slider> masterTransposeSlider;
    std::unique_ptr<tss::Slider> bendRangeSlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> unisonEnableComboBox;
    std::unique_ptr<tss::ComboBox> volumeInvertEnableComboBox;
    std::unique_ptr<tss::ComboBox> bankLockEnableComboBox;
    std::unique_ptr<tss::ComboBox> memoryProtectEnableComboBox;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterTuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterTransposeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bendRangeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> unisonEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> volumeInvertEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> bankLockEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> memoryProtectEnableAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiscPanel)
};
