#include "PanelSeparator.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    PanelSeparator::PanelSeparator(Skin& newSkin)
        : skin(&newSkin)
    {
        setSize(SkinDimensions::Widget::PanelSeparator::kWidth, SkinDimensions::Widget::PanelSeparator::kHeight);
    }

    void PanelSeparator::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void PanelSeparator::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void PanelSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = skin->getPanelSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void PanelSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = skin->getPanelSeparatorLineColour();
        auto lineWidth = skin->getPanelSeparatorLineWidth();
        auto lineX = bounds.getCentreX() - lineWidth / 2.0f;
        
        auto line = bounds;
        line.setX(lineX);
        line.setWidth(lineWidth);
        
        g.setColour(lineColour);
        g.fillRect(line);
    }
}

