#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;

class McButton : public juce::Button
{
public:
    explicit McButton(int width, const juce::String& buttonText = {});
    ~McButton() override = default;

    void setTheme(McTheme* theme);
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    static constexpr int kDefaultHeight = 20;
    static constexpr int kBorderThickness = 2;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);
    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);

    McTheme* mcTheme = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McButton)
};
