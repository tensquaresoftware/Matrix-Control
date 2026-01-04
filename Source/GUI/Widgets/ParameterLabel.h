#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ParameterLabel : public juce::Component
    {
    public:
        explicit ParameterLabel(Theme& inTheme, const juce::String& text = juce::String());
        ~ParameterLabel() override = default;

        void setTheme(Theme& inTheme);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth() { return kWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kWidth = 90;
        inline constexpr static int kHeight = 20;
        inline constexpr static float kBackgroundPadding = 1.0f;
        inline constexpr static float kTextLeftPadding = 2.0f;

        Theme* theme = nullptr;
        juce::String labelText;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterLabel)
    };
}

