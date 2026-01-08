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

class Dco1Panel : public juce::Component
{
public:
    Dco1Panel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts);
    ~Dco1Panel() override;

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
    std::unique_ptr<tss::ModuleHeader> dco1ModuleHeader;

    // Buttons
    std::unique_ptr<tss::Button> dco1InitButton;
    std::unique_ptr<tss::Button> dco1CopyButton;
    std::unique_ptr<tss::Button> dco1PasteButton;

    // Labels
    std::unique_ptr<tss::Label> dco1FrequencyLabel;
    std::unique_ptr<tss::Label> dco1FrequencyModByLfo1Label;
    std::unique_ptr<tss::Label> dco1SyncLabel;
    std::unique_ptr<tss::Label> dco1PulseWidthLabel;
    std::unique_ptr<tss::Label> dco1PulseWidthModByLfo2Label;
    std::unique_ptr<tss::Label> dco1WaveShapeLabel;
    std::unique_ptr<tss::Label> dco1WaveSelectLabel;
    std::unique_ptr<tss::Label> dco1LeversLabel;
    std::unique_ptr<tss::Label> dco1KeyboardPortamentoLabel;
    std::unique_ptr<tss::Label> dco1KeyClickLabel;

    // Sliders
    std::unique_ptr<tss::Slider> dco1FrequencySlider;
    std::unique_ptr<tss::Slider> dco1FrequencyModByLfo1Slider;
    std::unique_ptr<tss::Slider> dco1PulseWidthSlider;
    std::unique_ptr<tss::Slider> dco1PulseWidthModByLfo2Slider;
    std::unique_ptr<tss::Slider> dco1WaveShapeSlider;

    // Combo Boxes
    std::unique_ptr<tss::ComboBox> dco1SyncComboBox;
    std::unique_ptr<tss::ComboBox> dco1WaveSelectComboBox;
    std::unique_ptr<tss::ComboBox> dco1LeversComboBox;
    std::unique_ptr<tss::ComboBox> dco1KeyboardPortamentoComboBox;
    std::unique_ptr<tss::ComboBox> dco1KeyClickComboBox;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco1FrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco1FrequencyModByLfo1Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco1SyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco1PulseWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco1PulseWidthModByLfo2Attachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dco1WaveShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco1WaveSelectAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco1LeversAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco1KeyboardPortamentoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dco1KeyClickAttachment;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dco1Panel)
};

