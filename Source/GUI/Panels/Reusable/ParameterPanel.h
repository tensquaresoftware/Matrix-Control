#pragma once

#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace tss
{
    class Theme;
    class Label;
    class Slider;
    class ComboBox;
    class HorizontalSeparator;
}

class WidgetFactory;

class ParameterPanel : public juce::Component
{
public:
    enum class ParameterType { Slider, ComboBox, None };
    enum class ModuleType { PatchEdit, MasterEdit };

    ParameterPanel(tss::Theme& theme,
                   WidgetFactory& factory,
                   const juce::String& parameterId,
                   ParameterType type,
                   ModuleType moduleType,
                   juce::AudioProcessorValueTreeState& apvts);
    ~ParameterPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);
    int getTotalHeight() const;

    tss::Slider* getSlider() const { return slider_.get(); }

private:
    void createParameterLabel(tss::Theme& theme, WidgetFactory& factory, const juce::String& parameterId);
    void createParameterWidget(tss::Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts);
    void createSliderWidget(tss::Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts);
    void createComboBoxWidget(tss::Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts);
    void createSeparator(tss::Theme& theme);

    void layoutParameterLabel(int y);
    void layoutParameterWidget(int y);
    void layoutSeparator(int y);

    tss::Theme* theme_;
    ParameterType parameterType_;
    ModuleType moduleType_;

    std::unique_ptr<tss::Label> label_;
    std::unique_ptr<tss::Slider> slider_;
    std::unique_ptr<tss::ComboBox> comboBox_;
    std::unique_ptr<tss::HorizontalSeparator> separator_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachment_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterPanel)
};
