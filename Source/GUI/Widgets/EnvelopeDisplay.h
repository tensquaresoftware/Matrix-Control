#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class EnvelopeDisplay : public juce::Component
    {
    public:
        explicit EnvelopeDisplay(Theme& theme, int width, int height);
        ~EnvelopeDisplay() override = default;

        void setTheme(Theme& theme);
        
        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static int kVerticalPadding_ = 15;
        inline constexpr static float kTriangleBase_ = 10.0f;

        Theme* theme_ = nullptr;
        int width_;
        int height_;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeDisplay)
    };
}
