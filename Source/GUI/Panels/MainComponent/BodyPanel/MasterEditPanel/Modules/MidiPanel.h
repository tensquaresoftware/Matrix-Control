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

class MidiPanel : public juce::Component
{
public:
    MidiPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MidiPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::MasterEditPanel::MidiPanel::kWidth; }
    static int getHeight() { return kHeight_; }

private:
    inline constexpr static int kHeight_ = 245;
    
    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> midiModuleHeader_;

    // Labels
    std::unique_ptr<tss::Label> midiChannelLabel_;
    std::unique_ptr<tss::Label> midiEchoLabel_;
    std::unique_ptr<tss::Label> midiControllersLabel_;
    std::unique_ptr<tss::Label> midiPatchChangesLabel_;
    std::unique_ptr<tss::Label> midiPedal1SelectLabel_;
    std::unique_ptr<tss::Label> midiPedal2SelectLabel_;
    std::unique_ptr<tss::Label> midiLever2SelectLabel_;
    std::unique_ptr<tss::Label> midiLever3SelectLabel_;

    // Sliders
    std::unique_ptr<tss::Slider> midiPedal1SelectSlider_;
    std::unique_ptr<tss::Slider> midiPedal2SelectSlider_;
    std::unique_ptr<tss::Slider> midiLever2SelectSlider_;
    std::unique_ptr<tss::Slider> midiLever3SelectSlider_;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> midiChannelComboBox_;
    std::unique_ptr<tss::ComboBox> midiEchoComboBox_;
    std::unique_ptr<tss::ComboBox> midiControllersComboBox_;
    std::unique_ptr<tss::ComboBox> midiPatchChangesComboBox_;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7_;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator8_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiChannelAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiEchoAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiControllersAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiPatchChangesAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiPedal1SelectAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiPedal2SelectAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiLever2SelectAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiLever3SelectAttachment_;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiPanel)
};
