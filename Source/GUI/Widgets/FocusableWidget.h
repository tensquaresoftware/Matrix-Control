#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class FocusableWidget
    {
    public:
        FocusableWidget();
        ~FocusableWidget() = default;

        void handleFocusGained(juce::Component* component);
        void handleFocusLost(juce::Component* component);
        
        void drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, Theme& theme) const;
        
        bool hasFocus() const { return hasFocusFlag_; }

    private:
        inline constexpr static int kBorderThickness_ = 1;
        bool hasFocusFlag_ = false;
    };
}

