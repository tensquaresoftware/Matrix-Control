#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Label : public juce::Component
    {
    public:
        explicit Label(Theme& theme, int width, int height, const juce::String& text = juce::String());
        ~Label() override = default;

        void setTheme(Theme& theme);
        
        void setText(const juce::String& text);
        juce::String getText() const { return labelText_; }

        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBackgroundPadding_ = 1;
        inline constexpr static int kTextLeftPadding_ = 2;

        Theme* theme_ = nullptr;
        int width_;
        int height_;
        juce::String labelText_;

        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Label)
    };
}

