#include "McParameterLabel.h"
#include "../LookAndFeel/McLookAndFeel.h"

McParameterLabel::McParameterLabel(const juce::String& text)
    : labelText(text)
{
    setSize(kDefaultWidth, kDefaultHeight);
}

void McParameterLabel::setLookAndFeel(McLookAndFeel* lookAndFeel)
{
    mcLookAndFeel = lookAndFeel;
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
    if (mcLookAndFeel == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();
    auto enabled = isEnabled();

    drawBackground(g, bounds, enabled);
    drawText(g, bounds, enabled);
}

void McParameterLabel::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    auto backgroundColour = mcLookAndFeel->getParameterLabelBackgroundColour(enabled);
    g.setColour(backgroundColour);
    g.fillRect(bounds);
}

void McParameterLabel::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled)
{
    if (labelText.isEmpty())
    {
        return;
    }

    auto textColour = mcLookAndFeel->getParameterLabelTextColour(enabled);
    auto font = mcLookAndFeel->getDefaultFont();

    auto textBounds = bounds;
    textBounds.removeFromLeft(kTextLeftMargin);

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(labelText, textBounds, juce::Justification::centredLeft, false);
}
