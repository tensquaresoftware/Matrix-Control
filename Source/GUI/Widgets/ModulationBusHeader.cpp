#include "ModulationBusHeader.h"

#include "GUI/Themes/Theme.h"
#include "Shared/PluginDescriptors.h"

namespace tss
{
    ModulationBusHeader::ModulationBusHeader(Theme& inTheme, int width, int height, ColourVariant variant)
        : theme(&inTheme)
        , busNumberText(PluginDescriptors::ParameterDisplayNames::kModulationBusNumber)
        , busSourceText(PluginDescriptors::ParameterDisplayNames::kModulationBusSource)
        , busAmountText(PluginDescriptors::ParameterDisplayNames::kModulationBusAmount)
        , busDestinationText(PluginDescriptors::ParameterDisplayNames::kModulationBusDestination)
        , colourVariant(variant)
    {
        setSize(width, height);
    }

    void ModulationBusHeader::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void ModulationBusHeader::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawText(g, bounds);
        drawLine(g, bounds);
    }

    void ModulationBusHeader::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto baseColour = theme->getModuleHeaderBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void ModulationBusHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto textColour = theme->getModuleHeaderTextColour();
        auto font = theme->getBaseFont().withHeight(16.0f).boldened();

        auto textArea = bounds;
        textArea.setHeight(kTextAreaHeight_);
        textArea.removeFromLeft(kTextLeftPadding_);

        auto x = textArea.getX();
        auto y = textArea.getY();

        g.setColour(textColour);
        g.setFont(font);

        drawBusNumberText(g, x, y);
        x += kBusNumberTextWidth_;

        drawBusSourceText(g, x, y);
        x += kBusSourceTextWidth_;

        drawBusAmountText(g, x, y);
        x += kBusAmountTextWidth_;

        drawBusDestinationText(g, x, y);
    }

    void ModulationBusHeader::drawBusNumberText(juce::Graphics& g, float x, float y)
    {
        auto busNumberBounds = juce::Rectangle<float>(x, y, kBusNumberTextWidth_, kTextAreaHeight_);
        g.drawText(busNumberText, busNumberBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusSourceText(juce::Graphics& g, float x, float y)
    {
        auto busSourceBounds = juce::Rectangle<float>(x, y, kBusSourceTextWidth_, kTextAreaHeight_);
        g.drawText(busSourceText, busSourceBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusAmountText(juce::Graphics& g, float x, float y)
    {
        auto busAmountBounds = juce::Rectangle<float>(x, y, kBusAmountTextWidth_, kTextAreaHeight_);
        g.drawText(busAmountText, busAmountBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusDestinationText(juce::Graphics& g, float x, float y)
    {
        auto busDestinationBounds = juce::Rectangle<float>(x, y, kBusDestinationTextWidth_, kTextAreaHeight_);
        g.drawText(busDestinationText, busDestinationBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = getLineColour();
        
        auto lineThickness = kLineThickness_;
        auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight_;
        auto verticalOffset = kTextAreaHeight_ + (lineAreaHeight - lineThickness) / 2.0f;
        
        auto lineBounds = bounds;
        lineBounds.setHeight(lineThickness);
        lineBounds.translate(0.0f, verticalOffset);

        g.setColour(lineColour);
        g.fillRect(lineBounds);
    }

    juce::Colour ModulationBusHeader::getLineColour() const
    {
        return (colourVariant == ColourVariant::Blue) 
            ? theme->getModuleHeaderLineColourBlue() 
            : theme->getModuleHeaderLineColourOrange();
    }
}

