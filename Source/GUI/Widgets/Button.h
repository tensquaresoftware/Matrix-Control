#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Button : public juce::Button
    {
    public:
        explicit Button(Theme& theme, int width, int height, const juce::String& text = {});
        ~Button() override = default;

        void setTheme(Theme& theme);
        
        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;

        Theme* theme_ = nullptr;
        int width_;
        int height_;

        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}

