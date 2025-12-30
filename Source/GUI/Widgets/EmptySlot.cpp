#include "EmptySlot.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    EmptySlot::EmptySlot(Skin& newSkin)
        : skin(&newSkin)
    {
        setSize(SkinDimensions::Widget::EmptySlot::kWidth, SkinDimensions::Widget::EmptySlot::kHeight);
    }

    void EmptySlot::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void EmptySlot::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
    }

    void EmptySlot::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = skin->getEmptySlotBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }
}

