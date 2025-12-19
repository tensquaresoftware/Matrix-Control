#include "McFocusableWidget.h"
#include "../Themes/McTheme.h"

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

void McFocusableWidget::drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, McTheme* theme) const
{
    if (hasFocusFlag && theme != nullptr)
    {
        g.setColour(theme->getSliderFocusBorderColour());
    }
    else
    {
        g.setColour(juce::Colours::transparentBlack);
    }
    g.drawRect(bounds, kFocusBorderThickness);
}
