#include "ModulationBusHeader.h"

#include "GUI/Themes/Theme.h"
#include "Shared/PluginDescriptors.h"

namespace tss
{
    ModulationBusHeader::ModulationBusHeader(Theme& theme, int width, int height, ColourVariant variant)
        : theme_(&theme)
        , busNumberText_(PluginDescriptors::ParameterDisplayNames::kModulationBusNumber)
        , busSourceText_(PluginDescriptors::ParameterDisplayNames::kModulationBusSource)
        , busAmountText_(PluginDescriptors::ParameterDisplayNames::kModulationBusAmount)
        , busDestinationText_(PluginDescriptors::ParameterDisplayNames::kModulationBusDestination)
        , colourVariant_(variant)
    {
        setOpaque(true);
        setSize(width, height);
    }

    void ModulationBusHeader::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void ModulationBusHeader::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        g.fillAll(theme_->getMatrixModulationPanelBackgroundColour());

        auto bounds = getLocalBounds().toFloat();

        drawText(g, bounds);
        drawLine(g, bounds);
    }


    void ModulationBusHeader::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto textColour = theme_->getModuleHeaderTextColour();
        auto font = theme_->getBaseFont().withHeight(16.0f).boldened();

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
        g.drawText(busNumberText_, busNumberBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusSourceText(juce::Graphics& g, float x, float y)
    {
        auto busSourceBounds = juce::Rectangle<float>(x, y, kBusSourceTextWidth_, kTextAreaHeight_);
        g.drawText(busSourceText_, busSourceBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusAmountText(juce::Graphics& g, float x, float y)
    {
        auto busAmountBounds = juce::Rectangle<float>(x, y, kBusAmountTextWidth_, kTextAreaHeight_);
        g.drawText(busAmountText_, busAmountBounds, juce::Justification::centredLeft, false);
    }

    void ModulationBusHeader::drawBusDestinationText(juce::Graphics& g, float x, float y)
    {
        auto busDestinationBounds = juce::Rectangle<float>(x, y, kBusDestinationTextWidth_, kTextAreaHeight_);
        g.drawText(busDestinationText_, busDestinationBounds, juce::Justification::centredLeft, false);
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
        return (colourVariant_ == ColourVariant::Blue) 
            ? theme_->getModuleHeaderLineColourBlue() 
            : theme_->getModuleHeaderLineColourOrange();
    }
}

