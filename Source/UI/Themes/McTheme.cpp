#include "McTheme.h"

McTheme::McTheme()
{
    setTheme(Theme::Black);
}

void McTheme::setTheme(Theme newTheme)
{
    currentTheme = newTheme;
}

juce::Font McTheme::getDefaultFont() const
{
    juce::Font font(juce::FontOptions().withHeight(14.0f));
    
    auto availableTypefaces = juce::Font::findAllTypefaceNames();
    
    for (const auto& name : availableTypefaces)
    {
        if (name.containsIgnoreCase("PT Sans Narrow"))
        {
            font = juce::Font(juce::FontOptions(name, 14.0f, juce::Font::plain));
            break;
        }
    }
    
    return font;
}

juce::Colour McTheme::getGuiBackgroundColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kGuiBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kGuiBackgroundColour;
    }
    else
    {
        colourValue = DebugTheme::kGuiBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::makeDisabledColour(const juce::Colour& activeColour) const
{
    float h, s, l;
    activeColour.getHSL(h, s, l);
    
    l = juce::jlimit(0.0f, 1.0f, l * 0.35f);
    s = juce::jlimit(0.0f, 1.0f, s * 0.2f);
    
    auto alpha = activeColour.getFloatAlpha() * 0.7f;
    
    return juce::Colour::fromHSL(h, s, l, alpha);
}

juce::Colour McTheme::getParameterLabelBackgroundColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kParameterLabelBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kParameterLabelBackgroundColour;
    }
    else
    {
        colourValue = DebugTheme::kParameterLabelBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getParameterLabelTextColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kParameterLabelTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kParameterLabelTextColour;
    }
    else
    {
        colourValue = DebugTheme::kParameterLabelTextColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getParameterLabelBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kParameterLabelBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kParameterLabelBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kParameterLabelBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kButtonBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBackgroundColourOff() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBackgroundColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBackgroundColourOff;
    }
    else
    {
        colourValue = DebugTheme::kButtonBackgroundColourOff;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBorderColourOff() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBorderColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBorderColourOff;
    }
    else
    {
        colourValue = DebugTheme::kButtonBorderColourOff;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonTextColourOff() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonTextColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonTextColourOff;
    }
    else
    {
        colourValue = DebugTheme::kButtonTextColourOff;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBackgroundColourOn() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBackgroundColourOn;
    }
    else
    {
        colourValue = DebugTheme::kButtonBackgroundColourOn;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBorderColourOn() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBorderColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBorderColourOn;
    }
    else
    {
        colourValue = DebugTheme::kButtonBorderColourOn;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonTextColourOn() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonTextColourOn;
    }
    else
    {
        colourValue = DebugTheme::kButtonTextColourOn;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBackgroundColourHoover() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBackgroundColourHoover;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBackgroundColourHoover;
    }
    else
    {
        colourValue = DebugTheme::kButtonBackgroundColourHoover;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonTextColourHoover() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonTextColourHoover;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonTextColourHoover;
    }
    else
    {
        colourValue = DebugTheme::kButtonTextColourHoover;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonBackgroundColourClicked() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonBackgroundColourClicked;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonBackgroundColourClicked;
    }
    else
    {
        colourValue = DebugTheme::kButtonBackgroundColourClicked;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getButtonTextColourClicked() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kButtonTextColourClicked;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kButtonTextColourClicked;
    }
    else
    {
        colourValue = DebugTheme::kButtonTextColourClicked;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kSliderBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kSliderBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kSliderBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kSliderBackgroundColour : BlackTheme::kSliderBackgroundColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kSliderBackgroundColour : CreamTheme::kSliderBackgroundColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kSliderBackgroundColour : DebugTheme::kSliderBackgroundColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderBorderColour(bool isEnabled, bool hasFocus) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        if (hasFocus)
        {
            colourValue = BlackTheme::kSliderBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? BlackTheme::kSliderBorderColour : BlackTheme::kSliderBorderColourDisabled;
        }
    }
    else if (currentTheme == Theme::Cream)
    {
        if (hasFocus)
        {
            colourValue = CreamTheme::kSliderBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? CreamTheme::kSliderBorderColour : CreamTheme::kSliderBorderColourDisabled;
        }
    }
    else
    {
        if (hasFocus)
        {
            colourValue = DebugTheme::kSliderBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? DebugTheme::kSliderBorderColour : DebugTheme::kSliderBorderColourDisabled;
        }
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderTrackColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kSliderTrackColour : BlackTheme::kSliderTrackColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kSliderTrackColour : CreamTheme::kSliderTrackColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kSliderTrackColour : DebugTheme::kSliderTrackColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderTextColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kSliderTextColour : BlackTheme::kSliderTextColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kSliderTextColour : CreamTheme::kSliderTextColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kSliderTextColour : DebugTheme::kSliderTextColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getSliderFocusBorderColour() const
{
    return getSliderBorderColour(true, true);
}

juce::Colour McTheme::getComboBoxBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kComboBoxBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kComboBoxBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kComboBoxBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getComboBoxBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kComboBoxBackgroundColour : BlackTheme::kComboBoxBackgroundColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kComboBoxBackgroundColour : CreamTheme::kComboBoxBackgroundColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kComboBoxBackgroundColour : DebugTheme::kComboBoxBackgroundColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getComboBoxBorderColour(bool isEnabled, bool hasFocus) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        if (hasFocus)
        {
            colourValue = BlackTheme::kComboBoxBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? BlackTheme::kComboBoxBorderColour : BlackTheme::kComboBoxBorderColourDisabled;
        }
    }
    else if (currentTheme == Theme::Cream)
    {
        if (hasFocus)
        {
            colourValue = CreamTheme::kComboBoxBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? CreamTheme::kComboBoxBorderColour : CreamTheme::kComboBoxBorderColourDisabled;
        }
    }
    else
    {
        if (hasFocus)
        {
            colourValue = DebugTheme::kComboBoxBorderColourFocus;
        }
        else
        {
            colourValue = isEnabled ? DebugTheme::kComboBoxBorderColour : DebugTheme::kComboBoxBorderColourDisabled;
        }
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getComboBoxTriangleColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kComboBoxTriangleColour : BlackTheme::kComboBoxTriangleColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kComboBoxTriangleColour : CreamTheme::kComboBoxTriangleColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kComboBoxTriangleColour : DebugTheme::kComboBoxTriangleColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getComboBoxTextColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? BlackTheme::kComboBoxTextColour : BlackTheme::kComboBoxTextColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? CreamTheme::kComboBoxTextColour : CreamTheme::kComboBoxTextColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? DebugTheme::kComboBoxTextColour : DebugTheme::kComboBoxTextColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuBackgroundColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuBackgroundColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuBorderColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuBorderColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuBorderColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuBorderColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuSeparatorColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuSeparatorColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuSeparatorColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuSeparatorColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuTextColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuTextColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuTextColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuBackgroundHooverColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuBackgroundHooverColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuBackgroundHooverColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuBackgroundHooverColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getPopupMenuTextHooverColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kPopupMenuTextHooverColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kPopupMenuTextHooverColour;
    }
    else
    {
        colourValue = DebugTheme::kPopupMenuTextHooverColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getParameterSeparatorBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kParameterSeparatorBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kParameterSeparatorBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kParameterSeparatorBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getParameterSeparatorLineColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kParameterSeparatorLineColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kParameterSeparatorLineColour;
    }
    else
    {
        colourValue = DebugTheme::kParameterSeparatorLineColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getModuleTitleBaseColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kModuleTitleBaseColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kModuleTitleBaseColour;
    }
    else
    {
        colourValue = DebugTheme::kModuleTitleBaseColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getModuleTitleTextColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kModuleTitleTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kModuleTitleTextColour;
    }
    else
    {
        colourValue = DebugTheme::kModuleTitleTextColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McTheme::getModuleTitleLineColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = BlackTheme::kModuleTitleLineColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = CreamTheme::kModuleTitleLineColour;
    }
    else
    {
        colourValue = DebugTheme::kModuleTitleLineColour;
    }
    
    return juce::Colour(colourValue);
}
