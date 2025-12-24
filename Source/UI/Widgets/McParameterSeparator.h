#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;

class McParameterSeparator : public juce::Component
{
public:
    McParameterSeparator();
    ~McParameterSeparator() override = default;

    void setTheme(McTheme* theme);

    void paint(juce::Graphics& g) override;

private:
    static constexpr int kWidth = 150;
    static constexpr int kHeight = 5;
    static constexpr float kLineThickness = 1.0f;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

    McTheme* mcTheme = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McParameterSeparator)
};
