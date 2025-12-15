#include "McLookAndFeel.h"

McLookAndFeel::McLookAndFeel()
{
    setTheme(Theme::Black);
}

void McLookAndFeel::setTheme(Theme newTheme)
{
    currentTheme = newTheme;
}

juce::Colour McLookAndFeel::makeDisabledColour(const juce::Colour& activeColour) const
{
    float h, s, l;
    activeColour.getHSL(h, s, l);
    
    l = juce::jlimit(0.0f, 1.0f, l * 0.35f);
    s = juce::jlimit(0.0f, 1.0f, s * 0.2f);
    
    auto alpha = activeColour.getFloatAlpha() * 0.7f;
    
    return juce::Colour::fromHSL(h, s, l, alpha);
}

juce::Font McLookAndFeel::getDefaultFont() const
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

juce::Colour McLookAndFeel::getGuiBackgroundColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kGuiBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kGuiBackgroundColour;
    }
    else
    {
        colourValue = ThemeDebug::kGuiBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getParameterLabelBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kParameterLabelBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kParameterLabelBackgroundColour;
    }
    else
    {
        colourValue = ThemeDebug::kParameterLabelBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getParameterLabelTextColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kParameterLabelTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kParameterLabelTextColour;
    }
    else
    {
        colourValue = ThemeDebug::kParameterLabelTextColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getButtonBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kButtonBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kButtonBackgroundColour;
    }
    else
    {
        colourValue = ThemeDebug::kButtonBackgroundColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getButtonBorderColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kButtonBorderColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kButtonBorderColour;
    }
    else
    {
        colourValue = ThemeDebug::kButtonBorderColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getButtonTextColour(bool isEnabled, bool isClicked) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isClicked ? ThemeBlack::kButtonTextColourClicked : ThemeBlack::kButtonTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isClicked ? ThemeCream::kButtonTextColourClicked : ThemeCream::kButtonTextColour;
    }
    else
    {
        colourValue = isClicked ? ThemeDebug::kButtonTextColourClicked : ThemeDebug::kButtonTextColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getSliderBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? ThemeBlack::kSliderBackgroundColour : ThemeBlack::kSliderBackgroundColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? ThemeCream::kSliderBackgroundColour : ThemeCream::kSliderBackgroundColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? ThemeDebug::kSliderBackgroundColour : ThemeDebug::kSliderBackgroundColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getSliderTrackColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? ThemeBlack::kSliderTrackColour : ThemeBlack::kSliderTrackColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? ThemeCream::kSliderTrackColour : ThemeCream::kSliderTrackColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? ThemeDebug::kSliderTrackColour : ThemeDebug::kSliderTrackColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getSliderTextColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = isEnabled ? ThemeBlack::kSliderTextColour : ThemeBlack::kSliderTextColourDisabled;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = isEnabled ? ThemeCream::kSliderTextColour : ThemeCream::kSliderTextColourDisabled;
    }
    else
    {
        colourValue = isEnabled ? ThemeDebug::kSliderTextColour : ThemeDebug::kSliderTextColourDisabled;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getSliderFocusBorderColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kSliderFocusBorderColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kSliderFocusBorderColour;
    }
    else
    {
        colourValue = ThemeDebug::kSliderFocusBorderColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getComboBoxBackgroundColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kComboBoxBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kComboBoxBackgroundColour;
    }
    else
    {
        colourValue = ThemeDebug::kComboBoxBackgroundColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getComboBoxTextColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kComboBoxTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kComboBoxTextColour;
    }
    else
    {
        colourValue = ThemeDebug::kComboBoxTextColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getComboBoxTriangleColour(bool isEnabled) const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kComboBoxTriangleColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kComboBoxTriangleColour;
    }
    else
    {
        colourValue = ThemeDebug::kComboBoxTriangleColour;
    }
    
    auto colour = juce::Colour(colourValue);
    return isEnabled ? colour : makeDisabledColour(colour);
}

juce::Colour McLookAndFeel::getPopupMenuBackgroundColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kPopupMenuBackgroundColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kPopupMenuBackgroundColour;
    }
    else
    {
        colourValue = ThemeDebug::kPopupMenuBackgroundColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getPopupMenuTextColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kPopupMenuTextColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kPopupMenuTextColour;
    }
    else
    {
        colourValue = ThemeDebug::kPopupMenuTextColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getPopupMenuTextColourHighlighted() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kPopupMenuTextColourHighlighted;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kPopupMenuTextColourHighlighted;
    }
    else
    {
        colourValue = ThemeDebug::kPopupMenuTextColourHighlighted;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getPopupMenuHighlightColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kPopupMenuHighlightColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kPopupMenuHighlightColour;
    }
    else
    {
        colourValue = ThemeDebug::kPopupMenuHighlightColour;
    }
    
    return juce::Colour(colourValue);
}

juce::Colour McLookAndFeel::getPopupMenuBorderColour() const
{
    juce::uint32 colourValue;
    
    if (currentTheme == Theme::Black)
    {
        colourValue = ThemeBlack::kPopupMenuBorderColour;
    }
    else if (currentTheme == Theme::Cream)
    {
        colourValue = ThemeCream::kPopupMenuBorderColour;
    }
    else
    {
        colourValue = ThemeDebug::kPopupMenuBorderColour;
    }
    
    return juce::Colour(colourValue);
}
