#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;

class McModuleTitle : public juce::Component
{
public:
    enum class Size
    {
        Normal,
        Large
    };

    explicit McModuleTitle(const juce::String& text = juce::String(), Size size = Size::Normal);
    ~McModuleTitle() override = default;

    void setTheme(McTheme* theme);
    
    void setText(const juce::String& newText);
    juce::String getText() const { return titleText; }

    void paint(juce::Graphics& g) override;

private:
    static constexpr int kNormalWidth = 150;
    static constexpr int kLargeWidth = 225;
    static constexpr int kHeight = 25;
    static constexpr int kTextLeftPadding = 2;
    static constexpr int kTextAreaHeight = 20;
    static constexpr int kLineThickness = 5;
    static constexpr float kFontSize = 16.0f;

    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    juce::Font getTitleFont() const;

    McTheme* mcTheme = nullptr;
    juce::String titleText;
    Size titleSize;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(McModuleTitle)
};

