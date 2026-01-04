#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Button : public juce::Button
    {
    public:
        explicit Button(Theme& inTheme, int width, const juce::String& text = {});
        ~Button() override = default;

        void setTheme(Theme& inTheme);
        
        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        static constexpr int getDefaultWidth() { return kDefaultWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kDefaultWidth = 20;
        inline constexpr static int kHeight = 20;
        inline constexpr static float kBorderThickness = 2.0f;

        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}

