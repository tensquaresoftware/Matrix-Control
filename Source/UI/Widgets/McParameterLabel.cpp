#include "McParameterLabel.h"
#include "../Themes/McTheme.h"

McParameterLabel::McParameterLabel(const juce::String& text)
    : labelText(text)
{
    setSize(kDefaultWidth, kDefaultHeight);
}

void McParameterLabel::setTheme(McTheme* theme)
{
    mcTheme = theme;
}

void McParameterLabel::setText(const juce::String& newText)
{
    if (labelText != newText)
    {
        labelText = newText;
        repaint();
    }
}

void McParameterLabel::paint(juce::Graphics& g)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();

    drawBase(g, bounds);
    drawBackground(g, bounds);
    drawText(g, bounds);
}

void McParameterLabel::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getParameterLabelBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McParameterLabel::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto backgroundColour = mcTheme->getParameterLabelBackgroundColour();
    auto backgroundBounds = bounds.reduced(kBackgroundMargin);
    
    g.setColour(backgroundColour);
    g.fillRect(backgroundBounds);
}

void McParameterLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    if (labelText.isEmpty())
    {
        return;
    }

    auto textColour = mcTheme->getParameterLabelTextColour();
    auto font = mcTheme->getDefaultFont();

    auto textBounds = bounds.reduced(kBackgroundMargin);
    textBounds.removeFromLeft(kTextLeftPadding);

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(labelText, textBounds, juce::Justification::centredLeft, false);
}
