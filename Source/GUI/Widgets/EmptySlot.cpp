#include "EmptySlot.h"

#include "../Themes/Theme.h"

namespace tss
{
    EmptySlot::EmptySlot(Theme& newTheme)
        : theme(&newTheme)
    {
        setSize(kWidth, kHeight);
    }

    void EmptySlot::setTheme(Theme& newTheme)
    {
        theme = &newTheme;
        repaint();
    }

    void EmptySlot::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
    }

    void EmptySlot::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getEmptySlotBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }
}

