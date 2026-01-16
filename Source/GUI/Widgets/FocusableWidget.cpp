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
            hasFocusFlag_ = true;
            component->repaint();
        }
    }

    void FocusableWidget::handleFocusLost(juce::Component* component)
    {
        hasFocusFlag_ = false;
        if (auto* widget = component)
            widget->repaint();
    }

    void FocusableWidget::drawFocusBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, Theme& theme) const
    {
        if (hasFocusFlag_)
        {
            g.setColour(theme.getSliderFocusBorderColour());
        }
        else
        {
            g.setColour(juce::Colours::transparentBlack);
        }
        g.drawRect(bounds, kBorderThickness_);
    }
}

