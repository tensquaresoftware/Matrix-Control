#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class GroupLabel : public juce::Component
    {
    public:
        explicit GroupLabel(Theme& newTheme, int width, const juce::String& text = juce::String());
        ~GroupLabel() override = default;

        void setTheme(Theme& newTheme);

        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

    private:
        inline constexpr static int kHeight = 40;
        inline constexpr static float kTopAreaHeight = 15.0f;
        inline constexpr static float kContentHeight = 20.0f;
        inline constexpr static float kTextSpacing = 20.0f;
        inline constexpr static float kLineThickness = 1.0f;

        Theme* theme = nullptr;
        juce::String labelText;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        juce::Rectangle<float> getContentArea(const juce::Rectangle<float>& bounds) const;
        float calculateTextWidth() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroupLabel)
    };
}

