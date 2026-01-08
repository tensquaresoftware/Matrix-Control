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

class Env1Panel : public juce::Component
{
public:
    Env1Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Env1Panel() override;

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
    std::unique_ptr<tss::ModuleHeader> env1ModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> env1InitButton;
    std::unique_ptr<tss::Button> env1CopyButton;
    std::unique_ptr<tss::Button> env1PasteButton;

    // Labels
    std::unique_ptr<tss::Label> env1DelayLabel;
    std::unique_ptr<tss::Label> env1AttackLabel;
    std::unique_ptr<tss::Label> env1DecayLabel;
    std::unique_ptr<tss::Label> env1SustainLabel;
    std::unique_ptr<tss::Label> env1ReleaseLabel;
    std::unique_ptr<tss::Label> env1AmplitudeLabel;
    std::unique_ptr<tss::Label> env1AmplitudeModByVelocityLabel;
    std::unique_ptr<tss::Label> env1TriggerModeLabel;
    std::unique_ptr<tss::Label> env1EnvelopeModeLabel;
    std::unique_ptr<tss::Label> env1Lfo1TriggerLabel;

    // Sliders
    std::unique_ptr<tss::Slider> env1DelaySlider;
    std::unique_ptr<tss::Slider> env1AttackSlider;
    std::unique_ptr<tss::Slider> env1DecaySlider;
    std::unique_ptr<tss::Slider> env1SustainSlider;
    std::unique_ptr<tss::Slider> env1ReleaseSlider;
    std::unique_ptr<tss::Slider> env1AmplitudeSlider;
    std::unique_ptr<tss::Slider> env1AmplitudeModByVelocitySlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> env1TriggerModeComboBox;
    std::unique_ptr<tss::ComboBox> env1EnvelopeModeComboBox;
    std::unique_ptr<tss::ComboBox> env1Lfo1TriggerComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1DelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1AttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1DecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1SustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1ReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1AmplitudeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> env1AmplitudeModByVelocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env1TriggerModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env1EnvelopeModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> env1Lfo1TriggerAttachment;

    void setupModuleHeader(tss::Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupInitCopyPasteButtons(tss::Theme& inTheme, WidgetFactory& widgetFactory, 
                                   const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId);
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Env1Panel)
};
