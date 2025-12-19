#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme;

class McFocusableWidget
{
public:
    McFocusableWidget();
    ~McFocusableWidget() = default;

    void handleFocusGained(juce::Component* component);
    void handleFocusLost(juce::Component* component);
    
    void drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, McTheme* theme) const;
    
    bool hasFocus() const { return hasFocusFlag; }

private:
    static constexpr float kFocusBorderThickness = 1.0f;
    
    bool hasFocusFlag = false;
};
