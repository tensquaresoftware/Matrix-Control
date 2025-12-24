#include "McModuleTitle.h"
#include "../Themes/McTheme.h"

McModuleTitle::McModuleTitle(const juce::String& text, Size size)
    : titleText(text)
    , titleSize(size)
{
    auto width = (size == Size::Normal) ? kNormalWidth : kLargeWidth;
    setSize(width, kHeight);
}

void McModuleTitle::setTheme(McTheme* theme)
{
    mcTheme = theme;
}

void McModuleTitle::setText(const juce::String& newText)
{
    if (titleText != newText)
    {
        titleText = newText;
        repaint();
    }
}

void McModuleTitle::paint(juce::Graphics& g)
{
    if (mcTheme == nullptr)
    {
        return;
    }

    auto bounds = getLocalBounds().toFloat();

    drawBase(g, bounds);
    drawText(g, bounds);
    drawLine(g, bounds);
}

void McModuleTitle::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto baseColour = mcTheme->getModuleTitleBaseColour();
    g.setColour(baseColour);
    g.fillRect(bounds);
}

void McModuleTitle::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    if (titleText.isEmpty())
    {
        return;
    }

    auto textColour = mcTheme->getModuleTitleTextColour();
    auto font = getTitleFont();

    auto textBounds = bounds;
    textBounds.setHeight(static_cast<float>(kTextAreaHeight));
    textBounds.removeFromLeft(static_cast<float>(kTextLeftPadding));

    g.setColour(textColour);
    g.setFont(font);
    g.drawText(titleText, textBounds, juce::Justification::centredLeft, false);
}

void McModuleTitle::drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto lineColour = mcTheme->getModuleTitleLineColour();
    
    auto lineBounds = bounds;
    lineBounds.removeFromTop(bounds.getHeight() - static_cast<float>(kLineThickness));

    g.setColour(lineColour);
    g.fillRect(lineBounds);
}

juce::Font McModuleTitle::getTitleFont() const
{
    juce::Font font(juce::FontOptions().withHeight(kFontSize).withStyle(juce::Font::bold));
    
    auto availableTypefaces = juce::Font::findAllTypefaceNames();
    
    for (const auto& name : availableTypefaces)
    {
        if (name.containsIgnoreCase("PT Sans Narrow"))
        {
            font = juce::Font(juce::FontOptions(name, kFontSize, juce::Font::bold));
            break;
        }
    }
    
    return font;
}

