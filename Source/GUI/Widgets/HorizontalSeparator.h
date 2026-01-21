#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class HorizontalSeparator : public juce::Component
    {
    public:
        HorizontalSeparator(Theme& theme, int width, int height);
        ~HorizontalSeparator() override = default;

        void setTheme(Theme& theme);

        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kLineThickness_ = 1;

        Theme* theme_ = nullptr;
        int width_;
        int height_;
        
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSeparator)
    };
}

