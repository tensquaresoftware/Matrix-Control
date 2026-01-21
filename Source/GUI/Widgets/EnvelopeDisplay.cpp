#include "EnvelopeDisplay.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    EnvelopeDisplay::EnvelopeDisplay(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(true);
        setBufferedToImage(false);
        setSize(width_, height_);
    }

    void EnvelopeDisplay::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void EnvelopeDisplay::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        const auto bounds = getLocalBounds().toFloat();

        drawBackground(g, bounds);
        drawBorder(g, bounds);
        drawTriangle(g, bounds);
    }


    void EnvelopeDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme_->getEnvelopeDisplayBackgroundColour();
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void EnvelopeDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getEnvelopeDisplayBorderColour();
        const auto contentBounds = bounds.reduced(0, kVerticalPadding_);

        g.setColour(borderColour);
        g.drawRect(contentBounds, kBorderThickness_);
    }

    void EnvelopeDisplay::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getEnvelopeDisplayBorderColour();
        const auto triangleHeight = kTriangleBase_ * std::sqrt(3.0f) / 2.0f;
        const auto centreX = bounds.getCentreX();
        const auto baseY = static_cast<float>(height_ - kVerticalPadding_);

        juce::Path triangle;
        triangle.addTriangle(centreX - kTriangleBase_ / 2.0f, baseY,
                             centreX + kTriangleBase_ / 2.0f, baseY,
                             centreX, baseY + triangleHeight);

        g.setColour(borderColour);
        g.fillPath(triangle);
    }
}
