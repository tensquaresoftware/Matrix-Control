#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;

class McParameterLabel : public juce::Component
{
public:
    explicit McParameterLabel(const juce::String& text = juce::String());
    ~McParameterLabel() override = default;

    void setTheme(McTheme* theme);
    
    void setText(const juce::String& newText);
    juce::String getText() const { return labelText; }

    void paint(juce::Graphics& g) override;

private:
    static constexpr int kDefaultWidth = 90;
    static constexpr int kDefaultHeight = 20;
    static constexpr int kBackgroundMargin = 1;
    static constexpr int kTextLeftPadding = 2;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

    McTheme* mcTheme = nullptr;
    juce::String labelText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McParameterLabel)
};
