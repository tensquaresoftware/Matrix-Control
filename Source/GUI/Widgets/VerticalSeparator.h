#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class VerticalSeparator : public juce::Component
    {
    public:
        explicit VerticalSeparator(Theme& inTheme);
        ~VerticalSeparator() override = default;

        void setTheme(Theme& theme);

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth() { return kWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kWidth = 30;
        inline constexpr static int kHeight = 730;
        inline constexpr static float kLineWidth = 4.0f;

        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalSeparator)
    };
}

