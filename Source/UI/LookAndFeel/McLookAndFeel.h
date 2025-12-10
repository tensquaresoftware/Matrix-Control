#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McLookAndFeel
{
public:
    enum class Theme
    {
        Black,
        Cream,
        Debug
    };

    McLookAndFeel();
    ~McLookAndFeel() = default;

    void setTheme(Theme newTheme);
    Theme getTheme() const { return currentTheme; }

    // Colour getters for Black theme
    juce::Colour getParameterLabelBackgroundColour(bool isEnabled = true) const;
    juce::Colour getParameterLabelTextColour(bool isEnabled = true) const;

    juce::Colour getButtonBackgroundColour(bool isEnabled = true) const;
    juce::Colour getButtonBorderColour(bool isEnabled = true) const;
    juce::Colour getButtonTextColour(bool isEnabled = true, bool isClicked = false) const;

    juce::Colour getSliderBackgroundColour(bool isEnabled = true) const;
    juce::Colour getSliderTrackColour(bool isEnabled = true) const;
    juce::Colour getSliderTextColour(bool isEnabled = true) const;

    juce::Colour getComboBoxBackgroundColour(bool isEnabled = true) const;
    juce::Colour getComboBoxTextColour(bool isEnabled = true) const;
    juce::Colour getComboBoxTriangleColour(bool isEnabled = true) const;

    juce::Colour getPopupMenuBackgroundColour() const;
    juce::Colour getPopupMenuTextColour() const;
    juce::Colour getPopupMenuHighlightColour() const;
    juce::Colour getPopupMenuBorderColour() const;

    juce::Font getDefaultFont() const;

private:
    Theme currentTheme = Theme::Black;

    // Utility method for disabled colours
    juce::Colour makeDisabledColour(const juce::Colour& activeColour) const;
};
