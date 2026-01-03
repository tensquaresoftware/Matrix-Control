#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class ModuleName;
    class ParameterLabel;
    class Slider;
    class ComboBox;
    class Button;
    class ParameterSeparator;
}

class WidgetFactory;

using tss::Theme;

class Dco1Panel : public juce::Component
{
public:
    Dco1Panel(Theme& newTheme, WidgetFactory& widgetFactory);
    ~Dco1Panel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(Theme& newTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 150;
    inline constexpr static int kHeight = 275;
    Theme* theme;

    std::unique_ptr<tss::ModuleName> dco1ModuleName;

    std::unique_ptr<tss::Button> dco1InitButton;
    std::unique_ptr<tss::Button> dco1CopyButton;
    std::unique_ptr<tss::Button> dco1PasteButton;

    std::unique_ptr<tss::ParameterLabel> dco1FrequencyLabel;
    std::unique_ptr<tss::Slider> dco1FrequencySlider;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator1;

    std::unique_ptr<tss::ParameterLabel> dco1FrequencyModByLfo1Label;
    std::unique_ptr<tss::Slider> dco1FrequencyModByLfo1Slider;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator2;

    std::unique_ptr<tss::ParameterLabel> dco1SyncLabel;
    std::unique_ptr<tss::ComboBox> dco1SyncComboBox;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator3;

    std::unique_ptr<tss::ParameterLabel> dco1PulseWidthLabel;
    std::unique_ptr<tss::Slider> dco1PulseWidthSlider;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator4;

    std::unique_ptr<tss::ParameterLabel> dco1PulseWidthModByLfo2Label;
    std::unique_ptr<tss::Slider> dco1PulseWidthModByLfo2Slider;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator5;

    std::unique_ptr<tss::ParameterLabel> dco1WaveShapeLabel;
    std::unique_ptr<tss::Slider> dco1WaveShapeSlider;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator6;

    std::unique_ptr<tss::ParameterLabel> dco1WaveSelectLabel;
    std::unique_ptr<tss::ComboBox> dco1WaveSelectComboBox;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator7;

    std::unique_ptr<tss::ParameterLabel> dco1LeversLabel;
    std::unique_ptr<tss::ComboBox> dco1LeversComboBox;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator8;

    std::unique_ptr<tss::ParameterLabel> dco1KeyboardPortamentoLabel;
    std::unique_ptr<tss::ComboBox> dco1KeyboardPortamentoComboBox;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator9;
    
    std::unique_ptr<tss::ParameterLabel> dco1KeyClickLabel;
    std::unique_ptr<tss::ComboBox> dco1KeyClickComboBox;
    std::unique_ptr<tss::ParameterSeparator> parameterSeparator10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dco1Panel)
};

