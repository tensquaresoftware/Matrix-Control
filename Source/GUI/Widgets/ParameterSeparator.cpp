#include "ParameterSeparator.h"

#include "../Skin/Skin.h"
#include "../Skin/SkinDimensions.h"

namespace tss
{
    ParameterSeparator::ParameterSeparator(Skin& newSkin)
        : skin(&newSkin)
    {
        setSize(SkinDimensions::Widget::ParameterSeparator::kWidth, SkinDimensions::Widget::ParameterSeparator::kHeight);
    }

    void ParameterSeparator::setSkin(Skin& newSkin)
    {
        skin = &newSkin;
        repaint();
    }

    void ParameterSeparator::paint(juce::Graphics& g)
    {
        if (skin == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawLine(g, bounds);
    }

    void ParameterSeparator::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = skin->getParameterSeparatorBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ParameterSeparator::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = skin->getParameterSeparatorLineColour();
        auto lineY = bounds.getCentreY();
        auto lineThickness = skin->getParameterSeparatorLineThickness();
        
        g.setColour(lineColour);
        g.drawLine(bounds.getX(), lineY, bounds.getRight(), lineY, lineThickness);
    }
}

