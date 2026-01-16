#include "NumberBox.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    NumberBox::NumberBox(Theme& inTheme, int width, bool inEditable)
        : theme(&inTheme)
        , editable(inEditable)
    {
        setSize(width, kHeight_);
    }

    void NumberBox::setTheme(Theme& inTheme)
    {
        theme = &inTheme;
        repaint();
    }

    void NumberBox::setValue(int newValue)
    {
        if (currentValue != newValue)
        {
            currentValue = newValue;
            repaint();
        }
    }

    void NumberBox::setShowDot(bool show)
    {
        if (showDot != show)
        {
            showDot = show;
            repaint();
        }
    }

    void NumberBox::paint(juce::Graphics& g)
    {
        if (theme == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBase(g, bounds);
        drawBackground(g, bounds);
        drawBorder(g, bounds);
        drawText(g, bounds);
        
        if (showDot)
        {
            drawDot(g, bounds);
        }
    }

    void NumberBox::mouseDoubleClick(const juce::MouseEvent&)
    {
        if (!editable || !isEnabled())
        {
            return;
        }

        // TODO: Implement editing functionality
        // For now, just trigger a repaint to show visual feedback
        repaint();
    }

    void NumberBox::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto baseColour = theme->getButtonBaseColour();
        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void NumberBox::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme->getButtonBackgroundColourOn();
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void NumberBox::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto enabled = isEnabled();
        juce::Colour borderColour;
        
        if (!enabled)
        {
            borderColour = theme->getButtonBorderColourOff();
        }
        else
        {
            borderColour = theme->getButtonBorderColourOn();
        }
        
        g.setColour(borderColour);
        
        const auto borderThickness = kBorderThickness_;
        
        const auto topBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY(), bounds.getWidth(), borderThickness);
        const auto bottomBorder = juce::Rectangle<float>(bounds.getX(), bounds.getBottom() - borderThickness, bounds.getWidth(), borderThickness);
        const auto leftBorder = juce::Rectangle<float>(bounds.getX(), bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
        const auto rightBorder = juce::Rectangle<float>(bounds.getRight() - borderThickness, bounds.getY() + borderThickness, borderThickness, bounds.getHeight() - 2.0f * borderThickness);
        
        g.fillRect(topBorder);
        g.fillRect(bottomBorder);
        g.fillRect(leftBorder);
        g.fillRect(rightBorder);
    }

    void NumberBox::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto textColour = theme->getNumberBoxTextColour();
        const auto font = theme->getBaseFont();

        g.setColour(textColour);
        g.setFont(font);
        
        const auto text = juce::String(currentValue);
        g.drawText(text, bounds, juce::Justification::centred, false);
    }

    void NumberBox::drawDot(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto dotColour = theme->getNumberBoxDotColour();
        const auto font = theme->getBaseFont();
        
        g.setFont(font);
        
        const auto text = juce::String(currentValue);
        
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, text, 0.0f, 0.0f);
        const auto textWidth = glyphArrangement.getBoundingBox(0, -1, true).getWidth();
        
        const auto textRight = bounds.getCentreX() + textWidth / 2.0f;
        const auto baselineY = bounds.getCentreY() + font.getHeight() / 2.0f - font.getDescent();
        
        constexpr float dotRadius = 1.5f;
        constexpr float dotXOffset = 3.0f;
        
        g.setColour(dotColour);
        g.fillEllipse(textRight + dotXOffset, baselineY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);
    }
}
