#include "McButton.h"
#include "../LookAndFeel/McLookAndFeel.h"

McButton::McButton(int width, const juce::String& buttonText)
    : juce::Button(buttonText)
{
    setSize(width, kDefaultHeight);
}

void McButton::setLookAndFeel(McLookAndFeel* lookAndFeel)
{
    mcLookAndFeel = lookAndFeel;
}

void McButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted);
    
    if (mcLookAndFeel == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();

    drawBorder(g, bounds, enabled);
    drawBackground(g, bounds.reduced(kBorderThickness), enabled);
    drawText(g, bounds, enabled, shouldDrawButtonAsDown);
}

void McButton::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    if (enabled)
    {
        auto borderColour = mcLookAndFeel->getButtonBorderColour(true);
        g.setColour(borderColour);
    }
    else
    {
        auto borderColour = mcLookAndFeel->getSliderBackgroundColour(false);
        g.setColour(borderColour);
    }
    g.drawRect(bounds, kBorderThickness);
}

void McButton::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    if (enabled)
    {
        auto backgroundColour = mcLookAndFeel->getButtonBackgroundColour(true);
        g.setColour(backgroundColour);
    }
    else
    {
        auto backgroundColour = mcLookAndFeel->getSliderBackgroundColour(false);
        g.setColour(backgroundColour);
    }
    g.fillRect(bounds);
}

void McButton::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isDown)
{
    auto buttonText = getButtonText();
    if (buttonText.isEmpty())
    {
        return;
    }

    juce::Colour textColour;
    if (enabled)
    {
        textColour = mcLookAndFeel->getButtonTextColour(true, isDown);
    }
    else
    {
        textColour = mcLookAndFeel->getSliderTextColour(false);
    }

    auto font = mcLookAndFeel->getDefaultFont();

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(buttonText, bounds, juce::Justification::centred, false);
}
