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

juce::Colour McLookAndFeel::getParameterLabelBackgroundColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF000000);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::transparentBlack;
}

juce::Colour McLookAndFeel::getParameterLabelTextColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFFC4C4C4);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::white;
}

juce::Colour McLookAndFeel::getButtonBackgroundColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF101010);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::grey;
}

juce::Colour McLookAndFeel::getButtonBorderColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF303030);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::darkgrey;
}

juce::Colour McLookAndFeel::getButtonTextColour(bool isEnabled, bool isClicked) const
{
    if (currentTheme == Theme::Black)
    {
        if (isClicked)
        {
            auto colour = juce::Colour(0xFFD80000);
            return isEnabled ? colour : makeDisabledColour(colour);
        }
        
        auto colour = juce::Colour(0xFFC4C4C4);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::white;
}

juce::Colour McLookAndFeel::getSliderBackgroundColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF002D0F);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::darkgreen;
}

juce::Colour McLookAndFeel::getSliderTrackColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF00785E);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::green;
}

juce::Colour McLookAndFeel::getSliderTextColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF00DDAD);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::lightgreen;
}

juce::Colour McLookAndFeel::getComboBoxBackgroundColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF002D0F);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::darkgreen;
}

juce::Colour McLookAndFeel::getComboBoxTextColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF00DDAD);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::lightgreen;
}

juce::Colour McLookAndFeel::getComboBoxTriangleColour(bool isEnabled) const
{
    if (currentTheme == Theme::Black)
    {
        auto colour = juce::Colour(0xFF06471C);
        return isEnabled ? colour : makeDisabledColour(colour);
    }
    
    return juce::Colours::green;
}

juce::Colour McLookAndFeel::getPopupMenuBackgroundColour() const
{
    if (currentTheme == Theme::Black)
    {
        return juce::Colour(0xFF002D0F);
    }
    
    return juce::Colours::darkgrey;
}

juce::Colour McLookAndFeel::getPopupMenuTextColour() const
{
    if (currentTheme == Theme::Black)
    {
        return juce::Colour(0xFF00DDAD);
    }
    
    return juce::Colours::white;
}

juce::Colour McLookAndFeel::getPopupMenuHighlightColour() const
{
    if (currentTheme == Theme::Black)
    {
        return juce::Colour(0xFF00785E);
    }
    
    return juce::Colours::lightblue;
}

juce::Colour McLookAndFeel::getPopupMenuBorderColour() const
{
    if (currentTheme == Theme::Black)
    {
        return juce::Colour(0xFF06471C);
    }
    
    return juce::Colours::darkgrey;
}
