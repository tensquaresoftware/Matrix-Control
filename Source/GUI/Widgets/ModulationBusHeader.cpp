#include "ModulationBusHeader.h"

#include "../Themes/Theme.h"
#include "../../Shared/SynthDescriptors.h"

namespace tss
{
    ModulationBusHeader::ModulationBusHeader(Theme& inTheme, ColourVariant variant)
        : theme(&inTheme)
        , busNumberText(SynthDescriptors::ParameterDisplayNames::kModulationBusNumber)
        , busSourceText(SynthDescriptors::ParameterDisplayNames::kModulationBusSource)
        , busAmountText(SynthDescriptors::ParameterDisplayNames::kModulationBusAmount)
        , busDestinationText(SynthDescriptors::ParameterDisplayNames::kModulationBusDestination)
        , colourVariant(variant)
    {
        setSize(kWidth, kHeight);
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
        textArea.setHeight(kTextAreaHeight);
        textArea.removeFromLeft(kTextLeftPadding);

        auto x = textArea.getX();
        auto y = textArea.getY();

        g.setColour(textColour);
        g.setFont(font);

        drawBusNumberText(g, x, y);
        x += kBusNumberTextWidth;

        drawBusSourceText(g, x, y);
        x += kBusSourceTextWidth;

        drawBusAmountText(g, x, y);
        x += kBusAmountTextWidth;

        drawBusDestinationText(g, x, y);
    }

    void ModulationBusHeader::drawBusNumberText(juce::Graphics& g, float x, float y)
    {
        auto busNumberBounds = juce::Rectangle<float>(x, y, kBusNumberTextWidth, kTextAreaHeight);
        g.drawText(busNumberText, busNumberBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusSourceText(juce::Graphics& g, float x, float y)
    {
        auto busSourceBounds = juce::Rectangle<float>(x, y, kBusSourceTextWidth, kTextAreaHeight);
        g.drawText(busSourceText, busSourceBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusAmountText(juce::Graphics& g, float x, float y)
    {
        auto busAmountBounds = juce::Rectangle<float>(x, y, kBusAmountTextWidth, kTextAreaHeight);
        g.drawText(busAmountText, busAmountBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusDestinationText(juce::Graphics& g, float x, float y)
    {
        auto busDestinationBounds = juce::Rectangle<float>(x, y, kBusDestinationTextWidth, kTextAreaHeight);
        g.drawText(busDestinationText, busDestinationBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto lineColour = getLineColour();
        
        auto lineThickness = kLineThickness;
        auto lineAreaHeight = bounds.getHeight() - kTextAreaHeight;
        auto verticalOffset = kTextAreaHeight + (lineAreaHeight - lineThickness) / 2.0f;
        
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

