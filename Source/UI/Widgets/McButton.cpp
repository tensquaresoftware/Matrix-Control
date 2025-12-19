#include "McButton.h"
#include "../Themes/McTheme.h"

McButton::McButton(int width, const juce::String& buttonText)
    : juce::Button(buttonText)
{
    setSize(width, kHeight);
}

void McButton::setTheme(McTheme* theme)
{
    mcTheme = theme;
}

void McButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();

    drawBase(g, bounds);
    drawBackground(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    drawBorder(g, bounds, enabled);
    drawText(g, bounds, enabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

void McButton::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getButtonBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McButton::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
{
    juce::Colour backgroundColour;
    
    if (!enabled)
    {
        backgroundColour = mcTheme->getButtonBackgroundColourOff();
    }
    else if (isDown)
    {
        backgroundColour = mcTheme->getButtonBackgroundColourClicked();
    }
    else if (isHighlighted)
    {
        backgroundColour = mcTheme->getButtonBackgroundColourHoover();
    }
    else
    {
        backgroundColour = mcTheme->getButtonBackgroundColourOn();
    }
    
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McButton::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    juce::Colour borderColour;
    
    if (!enabled)
    {
        borderColour = mcTheme->getButtonBorderColourOff();
    }
    else
    {
        borderColour = mcTheme->getButtonBorderColourOn();
    }
    
    g.setColour(borderColour);
    
    auto borderThickness = static_cast<float>(kBorderThickness);
    
    auto topBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY(), bounds.getWidth(), borderThickness);
    auto bottomBorder = juce::Rectangle<float>(bounds.getX(), bounds.getBottom() - borderThickness, bounds.getWidth(), borderThickness);
    auto leftBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
    auto rightBorder = juce::Rectangle<float>(bounds.getRight() - borderThickness, bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
    
    g.fillRect(topBorder);
    g.fillRect(bottomBorder);
    g.fillRect(leftBorder);
    g.fillRect(rightBorder);
}

void McButton::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown)
{
    auto buttonText = getButtonText();
    if (buttonText.isEmpty())
    {
        return;
    }

    juce::Colour textColour;
    
    if (!enabled)
    {
        textColour = mcTheme->getButtonTextColourOff();
    }
    else if (isDown)
    {
        textColour = mcTheme->getButtonTextColourClicked();
    }
    else if (isHighlighted)
    {
        textColour = mcTheme->getButtonTextColourHoover();
    }
    else
    {
        textColour = mcTheme->getButtonTextColourOn();
    }

    auto font = mcTheme->getDefaultFont();

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(buttonText, bounds, juce::Justification::centred, false);
}
