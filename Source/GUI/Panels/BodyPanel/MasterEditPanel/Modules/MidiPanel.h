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

class MidiPanel : public juce::Component
{
public:
    MidiPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~MidiPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 160;
    inline constexpr static int kHeight = 245;
    
    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;

    // Module Name
    std::unique_ptr<tss::ModuleHeader> midiModuleHeader;

    // Labels
    std::unique_ptr<tss::Label> midiChannelLabel;
    std::unique_ptr<tss::Label> midiEchoLabel;
    std::unique_ptr<tss::Label> midiControllersLabel;
    std::unique_ptr<tss::Label> midiPatchChangesLabel;
    std::unique_ptr<tss::Label> midiPedal1SelectLabel;
    std::unique_ptr<tss::Label> midiPedal2SelectLabel;
    std::unique_ptr<tss::Label> midiLever2SelectLabel;
    std::unique_ptr<tss::Label> midiLever3SelectLabel;

    // Sliders
    std::unique_ptr<tss::Slider> midiPedal1SelectSlider;
    std::unique_ptr<tss::Slider> midiPedal2SelectSlider;
    std::unique_ptr<tss::Slider> midiLever2SelectSlider;
    std::unique_ptr<tss::Slider> midiLever3SelectSlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> midiChannelComboBox;
    std::unique_ptr<tss::ComboBox> midiEchoComboBox;
    std::unique_ptr<tss::ComboBox> midiControllersComboBox;
    std::unique_ptr<tss::ComboBox> midiPatchChangesComboBox;

    // Separators
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator1;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator2;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator3;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator4;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator5;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator6;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator7;
    std::unique_ptr<tss::HorizontalSeparator> horizontalSeparator8;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiChannelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiEchoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiControllersAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> midiPatchChangesAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiPedal1SelectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiPedal2SelectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiLever2SelectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midiLever3SelectAttachment;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiPanel)
};
