#include "TrackGeneratorDisplay.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    TrackGeneratorDisplay::TrackGeneratorDisplay(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(true);
        setBufferedToImage(false);
        setSize(width_, height_);
    }

    void TrackGeneratorDisplay::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void TrackGeneratorDisplay::paint(juce::Graphics& g)
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


    void TrackGeneratorDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme_->getTrackGeneratorDisplayBackgroundColour();
        
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void TrackGeneratorDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getTrackGeneratorDisplayBorderColour();
        const auto contentBounds = bounds.reduced(0, kVerticalPadding_);

        g.setColour(borderColour);
        g.drawRect(contentBounds, kBorderThickness_);
    }

    void TrackGeneratorDisplay::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getTrackGeneratorDisplayBorderColour();
        const auto triangleHeight = kTriangleBase_ * std::sqrt(3.0f) / 2.0f;
        const auto centreX = bounds.getCentreX();
        const auto baseY = static_cast<float>(kVerticalPadding_);

        juce::Path triangle;
        triangle.addTriangle(centreX - kTriangleBase_ / 2.0f, baseY,
                             centreX + kTriangleBase_ / 2.0f, baseY,
                             centreX, baseY - triangleHeight);

        g.setColour(borderColour);
        g.fillPath(triangle);
    }
}
