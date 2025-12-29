#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class FocusableWidget
    {
    public:
        FocusableWidget();
        ~FocusableWidget() = default;

        void handleFocusGained(juce::Component* component);
        void handleFocusLost(juce::Component* component);
        
        void drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, Skin& skin) const;
        
        bool hasFocus() const { return hasFocusFlag; }

    private:
        bool hasFocusFlag = false;
    };
}

