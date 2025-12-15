#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McLookAndFeel;

class McButton : public juce::Button
{
public:
    explicit McButton(int width, const juce::String& buttonText = {});
    ~McButton() override = default;

    void setLookAndFeel(McLookAndFeel* lookAndFeel);

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    static constexpr int kDefaultHeight = 20;
    static constexpr float kBorderThickness = 2.0f;

    void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isDown);

    McLookAndFeel* mcLookAndFeel = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McButton)
};
