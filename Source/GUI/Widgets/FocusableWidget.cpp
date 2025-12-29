#include "FocusableWidget.h"

#include "../Skin/Skin.h"

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

    void FocusableWidget::drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, Skin& skin) const
    {
        if (hasFocusFlag)
        {
            g.setColour(skin.getSliderFocusBorderColour());
        }
        else
        {
            g.setColour(juce::Colours::transparentBlack);
        }
        auto focusBorderThickness = skin.getFocusBorderThickness();
        g.drawRect(bounds, focusBorderThickness);
    }
}

