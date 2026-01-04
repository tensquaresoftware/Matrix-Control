#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ParameterSeparator : public juce::Component
    {
    public:
        explicit ParameterSeparator(Theme& inTheme);
        ~ParameterSeparator() override = default;

        void setTheme(Theme& inTheme);

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth() { return kWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kWidth = 150;
        inline constexpr static int kHeight = 5;
        inline constexpr static float kLineThickness = 1.0f;

        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSeparator)
    };
}

