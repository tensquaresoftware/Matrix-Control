#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class SectionHeader : public juce::Component
    {
    public:
        enum class ColourVariant
        {
            Blue,
            Orange
        };

        explicit SectionHeader(Theme& theme, int width, int height, const juce::String& text = juce::String(), ColourVariant variant = ColourVariant::Blue);
        ~SectionHeader() override = default;

        void setTheme(Theme& theme);

        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static float kContentHeight_ = 20.0f;
        inline constexpr static float kLineHeight_ = 12.0f;
        inline constexpr static float kLeftLineWidth_ = 20.0f;
        inline constexpr static float kTextSpacing_ = 8.0f;
        inline constexpr static float kTextFontHeight_ = 20.0f;

        int width_;
        int height_;
        Theme* theme_ = nullptr;
        juce::String text_;
        ColourVariant colourVariant_;

        void drawText(juce::Graphics& g, const juce::Rectangle<float>& contentArea);
        void drawLines(juce::Graphics& g, const juce::Rectangle<float>& contentArea);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& contentArea);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& contentArea);
        
        juce::Colour getLineColour() const;
        float calculateTextWidth() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionHeader)
    };
}

