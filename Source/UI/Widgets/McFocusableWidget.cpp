#include "McFocusableWidget.h"
#include "../LookAndFeel/McLookAndFeel.h"

McFocusableWidget::McFocusableWidget()
{
}

void McFocusableWidget::handleFocusGained(juce::Component* component)
{
    if (component != nullptr && component->isEnabled())
    {
        hasFocusFlag = true;
        component->repaint();
    }
}

void McFocusableWidget::handleFocusLost(juce::Component* component)
{
    hasFocusFlag = false;
    if (component != nullptr)
    {
        component->repaint();
    }
}

void McFocusableWidget::drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, McLookAndFeel* lookAndFeel) const
{
    if (hasFocusFlag && lookAndFeel != nullptr)
    {
        g.setColour(lookAndFeel->getSliderFocusBorderColour());
    }
    else
    {
        g.setColour(juce::Colours::transparentBlack);
    }
    g.drawRect(bounds, kFocusBorderThickness);
}
