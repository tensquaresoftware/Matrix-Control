#include "FocusableWidget.h"

#include "../Themes/Theme.h"

namespace tss
{
    FocusableWidget::FocusableWidget()
    {
    }

    void FocusableWidget::handleFocusGained(juce::Component* component)
    {
        if (component != nullptr && component->isEnabled())
        {
            hasFocusFlag = true;
            component->repaint();
        }
    }

    void FocusableWidget::handleFocusLost(juce::Component* component)
    {
        hasFocusFlag = false;
        if (component != nullptr)
        {
            component->repaint();
        }
    }

    void FocusableWidget::drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, Theme& theme) const
    {
        if (hasFocusFlag)
        {
            g.setColour(theme.getSliderFocusBorderColour());
        }
        else
        {
            g.setColour(juce::Colours::transparentBlack);
        }
        auto focusBorderThickness = kBorderThickness;
        g.drawRect(bounds, focusBorderThickness);
    }
}

