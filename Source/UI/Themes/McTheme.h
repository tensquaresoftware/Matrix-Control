#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class McTheme
{
public:
    enum class Theme
    {
        Black,
        Cream,
        Debug
    };

    struct ColourPalette
    {
        static constexpr juce::uint32 kTransparent   = 0x00112233;
        static constexpr juce::uint32 kBlack         = 0xFF000000;
        static constexpr juce::uint32 kWhite         = 0xFFFFFFFF;
        static constexpr juce::uint32 kCream         = 0xFFE9E8D5;
        static constexpr juce::uint32 kRed           = 0xFFFF0000;
        static constexpr juce::uint32 kOrange        = 0xFFFF9900;
        static constexpr juce::uint32 kYellow        = 0xFFFFFF00;
        
        static constexpr juce::uint32 kDarkGrey1     = 0xFF101010;
        static constexpr juce::uint32 kDarkGrey2     = 0xFF202020;
        static constexpr juce::uint32 kDarkGrey3     = 0xFF303030;
        static constexpr juce::uint32 kDarkGrey4     = 0xFF404040;
        
        static constexpr juce::uint32 kLightGrey1    = 0xFFC4C4C4;
        
        static constexpr juce::uint32 kGreen1        = 0xFF002D0F;
        static constexpr juce::uint32 kGreen2        = 0xFF06471C;
        static constexpr juce::uint32 kGreen3        = 0xFF00785E;
        static constexpr juce::uint32 kGreen4        = 0xFF00DDAD;

        static constexpr juce::uint32 kBackgroundOff = 0xFF303030;
        static constexpr juce::uint32 kBorderOff     = 0xFF404040;
        static constexpr juce::uint32 kContentOff    = 0xFF505050;
    };

    struct BlackTheme
    {
        // GUI
        static constexpr juce::uint32 kGuiBackgroundColour            = ColourPalette::kDarkGrey2;
        
        // McButton
        static constexpr juce::uint32 kButtonBaseColour               = ColourPalette::kTransparent;
        
        static constexpr juce::uint32 kButtonBackgroundColourOff      = ColourPalette::kBackgroundOff;
        static constexpr juce::uint32 kButtonBorderColourOff          = ColourPalette::kBorderOff;
        static constexpr juce::uint32 kButtonTextColourOff            = ColourPalette::kContentOff;
        
        static constexpr juce::uint32 kButtonBackgroundColourOn       = ColourPalette::kBlack;
        static constexpr juce::uint32 kButtonBorderColourOn           = ColourPalette::kDarkGrey3;
        static constexpr juce::uint32 kButtonTextColourOn             = ColourPalette::kLightGrey1;
        
        static constexpr juce::uint32 kButtonBackgroundColourHoover   = ColourPalette::kDarkGrey1;
        static constexpr juce::uint32 kButtonTextColourHoover         = ColourPalette::kLightGrey1;
        
        static constexpr juce::uint32 kButtonBackgroundColourClicked  = ColourPalette::kLightGrey1;
        static constexpr juce::uint32 kButtonTextColourClicked        = ColourPalette::kDarkGrey1;

        // McParameterLabel
        static constexpr juce::uint32 kParameterLabelBaseColour       = ColourPalette::kTransparent;
        static constexpr juce::uint32 kParameterLabelBackgroundColour = ColourPalette::kDarkGrey3;
        static constexpr juce::uint32 kParameterLabelTextColour       = ColourPalette::kLightGrey1;
        
        // McSlider
        static constexpr juce::uint32 kSliderFocusBorderColour        = 0xFF06471C;
        static constexpr juce::uint32 kSliderBackgroundColour         = 0xFF002D0F;
        static constexpr juce::uint32 kSliderBackgroundColourDisabled = 0xFF303030;
        static constexpr juce::uint32 kSliderTrackColour              = 0xFF00785E;
        static constexpr juce::uint32 kSliderTrackColourDisabled      = 0xFF505050;
        static constexpr juce::uint32 kSliderTextColour               = 0xFF00DDAD;
        static constexpr juce::uint32 kSliderTextColourDisabled       = 0xFF808080;
        static constexpr juce::uint32 kSliderBaseColour               = 0xFFFFFFFF;
        
        // McComboBox
        static constexpr juce::uint32 kComboBoxBackgroundColour       = 0xFF002D0F;
        static constexpr juce::uint32 kComboBoxTextColour             = 0xFF00DDAD;
        static constexpr juce::uint32 kComboBoxTriangleColour         = 0xFF06471C;
        static constexpr juce::uint32 kComboBoxBaseColour             = 0xFFFFFFFF;
        
        // McPopupMenu
        static constexpr juce::uint32 kPopupMenuBackgroundColour      = 0xFF002D0F;
        static constexpr juce::uint32 kPopupMenuTextColour            = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuTextColourHighlighted = 0xFF151515;
        static constexpr juce::uint32 kPopupMenuHighlightColour       = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuBorderColour          = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuSeparatorColour       = 0xFF00DDAD;
    };

    struct CreamTheme
    {
        // GUI
        static constexpr juce::uint32 kGuiBackgroundColour            = ColourPalette::kCream;
        
        // McButton
        static constexpr juce::uint32 kButtonBaseColour               = ColourPalette::kTransparent;
        
        static constexpr juce::uint32 kButtonBackgroundColourOff      = ColourPalette::kBackgroundOff;
        static constexpr juce::uint32 kButtonBorderColourOff          = ColourPalette::kBorderOff;
        static constexpr juce::uint32 kButtonTextColourOff            = ColourPalette::kContentOff;
        
        static constexpr juce::uint32 kButtonBackgroundColourOn       = ColourPalette::kLightGrey1;
        static constexpr juce::uint32 kButtonBorderColourOn           = ColourPalette::kDarkGrey4;
        static constexpr juce::uint32 kButtonTextColourOn             = ColourPalette::kDarkGrey1;
        
        static constexpr juce::uint32 kButtonBackgroundColourHoover   = ColourPalette::kDarkGrey1;
        static constexpr juce::uint32 kButtonTextColourHoover         = ColourPalette::kLightGrey1;
        
        static constexpr juce::uint32 kButtonBackgroundColourClicked  = ColourPalette::kLightGrey1;
        static constexpr juce::uint32 kButtonTextColourClicked        = ColourPalette::kBlack;

        // McParameterLabel
        static constexpr juce::uint32 kParameterLabelBaseColour       = ColourPalette::kTransparent;
        static constexpr juce::uint32 kParameterLabelBackgroundColour = ColourPalette::kTransparent;
        static constexpr juce::uint32 kParameterLabelTextColour       = ColourPalette::kDarkGrey1;
        
        // McSlider
        static constexpr juce::uint32 kSliderFocusBorderColour        = 0xFF06471C;
        static constexpr juce::uint32 kSliderBackgroundColour         = 0xFF002D0F;
        static constexpr juce::uint32 kSliderBackgroundColourDisabled = 0xFF303030;
        static constexpr juce::uint32 kSliderTrackColour              = 0xFF00785E;
        static constexpr juce::uint32 kSliderTrackColourDisabled      = 0xFF505050;
        static constexpr juce::uint32 kSliderTextColour               = 0xFF00DDAD;
        static constexpr juce::uint32 kSliderTextColourDisabled       = 0xFF808080;
        static constexpr juce::uint32 kSliderBaseColour               = 0xFF000000;
        
        // McComboBox
        static constexpr juce::uint32 kComboBoxBackgroundColour       = 0xFF002D0F;
        static constexpr juce::uint32 kComboBoxTextColour             = 0xFF00DDAD;
        static constexpr juce::uint32 kComboBoxTriangleColour         = 0xFF06471C;
        static constexpr juce::uint32 kComboBoxBaseColour             = 0xFF000000;
        
        // McPopupMenu
        static constexpr juce::uint32 kPopupMenuBackgroundColour      = 0xFF002D0F;
        static constexpr juce::uint32 kPopupMenuTextColour            = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuTextColourHighlighted = 0xFF151515;
        static constexpr juce::uint32 kPopupMenuHighlightColour       = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuBorderColour          = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuSeparatorColour       = 0xFF00DDAD;
    };

    struct DebugTheme
    {
        // GUI
        static constexpr juce::uint32 kGuiBackgroundColour            = 0xFF202020;
        
        // McButton
        static constexpr juce::uint32 kButtonBaseColour               = ColourPalette::kRed;
        
        static constexpr juce::uint32 kButtonBackgroundColourOff      = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonBorderColourOff          = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonTextColourOff            = ColourPalette::kTransparent;
        
        static constexpr juce::uint32 kButtonBackgroundColourOn       = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonBorderColourOn           = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonTextColourOn             = ColourPalette::kTransparent;
        
        static constexpr juce::uint32 kButtonBackgroundColourHoover   = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonTextColourHoover         = ColourPalette::kTransparent;
        
        static constexpr juce::uint32 kButtonBackgroundColourClicked  = ColourPalette::kTransparent;
        static constexpr juce::uint32 kButtonTextColourClicked        = ColourPalette::kTransparent;

        // McParameterLabel
        static constexpr juce::uint32 kParameterLabelBaseColour       = ColourPalette::kRed;
        static constexpr juce::uint32 kParameterLabelBackgroundColour = ColourPalette::kOrange;
        static constexpr juce::uint32 kParameterLabelTextColour       = ColourPalette::kTransparent;
        
        // McSlider
        static constexpr juce::uint32 kSliderFocusBorderColour        = 0xFF06471C;
        static constexpr juce::uint32 kSliderBackgroundColour         = 0xFF002D0F;
        static constexpr juce::uint32 kSliderBackgroundColourDisabled = 0xFF303030;
        static constexpr juce::uint32 kSliderTrackColour              = 0xFF00785E;
        static constexpr juce::uint32 kSliderTrackColourDisabled      = 0xFF505050;
        static constexpr juce::uint32 kSliderTextColour               = 0xFF00DDAD;
        static constexpr juce::uint32 kSliderTextColourDisabled       = 0xFF808080;
        static constexpr juce::uint32 kSliderBaseColour               = 0xFFFF0000;
        
        // McComboBox
        static constexpr juce::uint32 kComboBoxBackgroundColour       = 0xFF002D0F;
        static constexpr juce::uint32 kComboBoxTextColour             = 0xFF00DDAD;
        static constexpr juce::uint32 kComboBoxTriangleColour         = 0xFF06471C;
        static constexpr juce::uint32 kComboBoxBaseColour             = 0xFFFF0000;
        
        // McPopupMenu
        static constexpr juce::uint32 kPopupMenuBackgroundColour      = 0xFF002D0F;
        static constexpr juce::uint32 kPopupMenuTextColour            = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuTextColourHighlighted = 0xFF151515;
        static constexpr juce::uint32 kPopupMenuHighlightColour       = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuBorderColour          = 0xFF00DDAD;
        static constexpr juce::uint32 kPopupMenuSeparatorColour       = 0xFF06471C;
    };

    McTheme();
    ~McTheme() = default;

    void setTheme(Theme newTheme);
    Theme getTheme() const { return currentTheme; }

    juce::Font getDefaultFont() const;

    juce::Colour getGuiBackgroundColour() const;
    
    juce::Colour getParameterLabelBackgroundColour() const;
    juce::Colour getParameterLabelTextColour() const;
    juce::Colour getParameterLabelBaseColour() const;

    juce::Colour getButtonBaseColour() const;
    
    juce::Colour getButtonBackgroundColourOff() const;
    juce::Colour getButtonBorderColourOff() const;
    juce::Colour getButtonTextColourOff() const;
    
    juce::Colour getButtonBackgroundColourOn() const;
    juce::Colour getButtonBorderColourOn() const;
    juce::Colour getButtonTextColourOn() const;
    
    juce::Colour getButtonBackgroundColourHoover() const;
    juce::Colour getButtonTextColourHoover() const;
    
    juce::Colour getButtonBackgroundColourClicked() const;
    juce::Colour getButtonTextColourClicked() const;

    juce::Colour getSliderBackgroundColour(bool isEnabled = true) const;
    juce::Colour getSliderTrackColour(bool isEnabled = true) const;
    juce::Colour getSliderTextColour(bool isEnabled = true) const;
    juce::Colour getSliderFocusBorderColour() const;
    juce::Colour getSliderBaseColour() const;

    juce::Colour getComboBoxBackgroundColour(bool isEnabled = true) const;
    juce::Colour getComboBoxBaseColour() const;
    juce::Colour getComboBoxTextColour(bool isEnabled = true) const;
    juce::Colour getComboBoxTriangleColour(bool isEnabled = true) const;

    juce::Colour getPopupMenuBackgroundColour() const;
    juce::Colour getPopupMenuTextColour() const;
    juce::Colour getPopupMenuTextColourHighlighted() const;
    juce::Colour getPopupMenuHighlightColour() const;
    juce::Colour getPopupMenuBorderColour() const;
    juce::Colour getPopupMenuSeparatorColour() const;

private:
    Theme currentTheme = Theme::Black;

    juce::Colour makeDisabledColour(const juce::Colour& activeColour) const;
};

