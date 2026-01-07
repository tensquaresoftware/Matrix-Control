#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class ModuleHeader : public juce::Component
    {
    public:
        enum class Size
        {
            Normal,
            Large
        };

        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit ModuleHeader(Theme& inTheme, const juce::String& text = juce::String(), Size size = Size::Normal, ColourVariant variant = ColourVariant::Blue);
        ~ModuleHeader() override = default;

        void setTheme(Theme& inTheme);
        
        void setText(const juce::String& newText);
        juce::String getText() const { return text; }

        void paint(juce::Graphics& g) override;

        static constexpr int getNormalWidth() { return kNormalWidth; }
        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kNormalWidth = 150;
        inline constexpr static int kLargeWidth = 225;
        inline constexpr static int kHeight = 30;
        
        inline constexpr static float kTextLeftPadding = 2.0f;
        inline constexpr static float kTextAreaHeight = 20.0f;
        inline constexpr static float kLineThickness = 4.0f;

        Theme* theme = nullptr;
        juce::String text;
        ColourVariant colourVariant;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        
        juce::Colour getLineColour() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleHeader)
    };
}

