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

    static constexpr juce::uint32 kParameterLabelBackgroundColour = 0xFF000000;
    static constexpr juce::uint32 kParameterLabelTextColour       = 0xFFC4C4C4;
    
    static constexpr juce::uint32 kButtonBackgroundColour         = 0xFF101010;
    static constexpr juce::uint32 kButtonBorderColour             = 0xFF303030;
    static constexpr juce::uint32 kButtonTextColour               = 0xFFC4C4C4;
    static constexpr juce::uint32 kButtonTextColourClicked        = 0xFFD80000;
    
    static constexpr juce::uint32 kSliderFocusBorderColour        = 0xFF06471C;
    static constexpr juce::uint32 kSliderBackgroundColour         = 0xFF002D0F;
    static constexpr juce::uint32 kSliderBackgroundColourDisabled = 0xFF303030;
    static constexpr juce::uint32 kSliderTrackColour              = 0xFF00785E;
    static constexpr juce::uint32 kSliderTrackColourDisabled      = 0xFF505050;
    static constexpr juce::uint32 kSliderTextColour               = 0xFF00DDAD;
    static constexpr juce::uint32 kSliderTextColourDisabled       = 0xFF808080;
    
    static constexpr juce::uint32 kComboBoxBackgroundColour       = 0xFF002D0F;
    static constexpr juce::uint32 kComboBoxTextColour             = 0xFF00DDAD;
    static constexpr juce::uint32 kComboBoxTriangleColour         = 0xFF06471C;
    
    static constexpr juce::uint32 kPopupMenuBackgroundColour      = 0xFF002D0F;
    static constexpr juce::uint32 kPopupMenuTextColour            = 0xFF00DDAD;
    static constexpr juce::uint32 kPopupMenuHighlightColour       = 0xFF00785E;
    static constexpr juce::uint32 kPopupMenuBorderColour          = 0xFF06471C;

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
    juce::Colour getSliderFocusBorderColour() const;

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
