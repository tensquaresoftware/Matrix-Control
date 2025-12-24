#include "McParameterSeparator.h"
#include "../Themes/McTheme.h"

McParameterSeparator::McParameterSeparator()
{
    setSize(kWidth, kHeight);
}

void McParameterSeparator::setTheme(McTheme* theme)
{
    mcTheme = theme;
}

void McParameterSeparator::paint(juce::Graphics& g)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();

    drawBase(g, bounds);
    drawLine(g, bounds);
}

void McParameterSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getParameterSeparatorBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McParameterSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto lineColour = mcTheme->getParameterSeparatorLineColour();
    auto lineY = bounds.getCentreY();
    
    g.setColour(lineColour);
    g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, kLineThickness);
}
