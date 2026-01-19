#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDimensions.h"

namespace tss
{
    class Theme;
    class Label;
    class ComboBox;
    class Slider;
    class Button;
    class HorizontalSeparator;
}

class WidgetFactory;

class ModulationBusPanel : public juce::Component
{
public:
    ModulationBusPanel(int busNumber,
                      WidgetFactory& factory,
                      tss::Theme& theme,
                      juce::AudioProcessorValueTreeState& apvts,
                      const juce::String& sourceParamId,
                      const juce::String& amountParamId,
                      const juce::String& destinationParamId,
                      const juce::String& busId);
    ~ModulationBusPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);
    static int getHeight();

private:
    void createBusNumberLabel(int busNumber, tss::Theme& theme);
    void createSourceComboBox(WidgetFactory& factory, tss::Theme& theme, const juce::String& sourceParamId, juce::AudioProcessorValueTreeState& apvts);
    void createAmountSlider(WidgetFactory& factory, tss::Theme& theme, const juce::String& amountParamId, juce::AudioProcessorValueTreeState& apvts);
    void createDestinationComboBox(int busNumber, tss::Theme& theme, const juce::String& destinationParamId, juce::AudioProcessorValueTreeState& apvts);
    void createInitButton(tss::Theme& theme, int busNumber);
    void createSeparator(tss::Theme& theme);

    void layoutWidgetRow();
    void layoutSeparator(int y);

    inline constexpr static int kSpacing_ = 5;

    std::unique_ptr<tss::Label> busNumberLabel_;
    std::unique_ptr<tss::ComboBox> sourceComboBox_;
    std::unique_ptr<tss::Slider> amountSlider_;
    std::unique_ptr<tss::ComboBox> destinationComboBox_;
    std::unique_ptr<tss::Button> initButton_;
    std::unique_ptr<tss::HorizontalSeparator> separator_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sourceAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> destinationAttachment_;

    juce::AudioProcessorValueTreeState& apvts_;
    juce::String busId_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationBusPanel)
};
