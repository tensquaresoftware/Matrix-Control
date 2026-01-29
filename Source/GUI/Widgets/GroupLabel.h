#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class GroupLabel : public juce::Component
    {
    public:
        explicit GroupLabel(Theme& theme, int width, int height, const juce::String& text = juce::String());
        ~GroupLabel() override = default;

        void setTheme(Theme& theme);

        void setText(const juce::String& text);
        juce::String getText() const { return labelText_; }

        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kTopAreaHeight_ = 5;
        inline constexpr static int kContentHeight_ = 20;
        inline constexpr static int kTextSpacing_ = 8;
        inline constexpr static int kLineThickness_ = 1;
        
        Theme* theme_ = nullptr;
        int width_;
        int height_;
        juce::String labelText_;
        
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& area);
        void drawLines(juce::Graphics& g, const juce::Rectangle<float>& area, float textWidth);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& area, float centreX, float halfTextWidth, float centreY);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& area, float centreX, float halfTextWidth, float centreY);

        juce::Rectangle<float> calculateContentArea() const;
        float calculateTextWidth() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroupLabel)
    };
}

