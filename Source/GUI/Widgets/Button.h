#pragma once

#include <array>

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
        void resized() override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;
        
        enum class ButtonState
        {
            Normal = 0,
            Hover = 1,
            Down = 2,
            Disabled = 3
        };

        Theme* theme_ = nullptr;
        int width_;
        int height_;
        
        std::array<juce::Image, 4> cachedStates_;
        bool cacheValid_ = false;
        
        void regenerateStateCache();
        void invalidateCache();
        ButtonState getCurrentState(bool enabled, bool isHighlighted, bool isDown) const;

        juce::Colour getBackgroundColour(bool enabled, bool isHighlighted, bool isDown) const;
        juce::Colour getBorderColour(bool enabled) const;
        juce::Colour getTextColour(bool enabled, bool isHighlighted, bool isDown) const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}

