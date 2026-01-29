#include "TrackGeneratorDisplay.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    TrackGeneratorDisplay::TrackGeneratorDisplay(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void TrackGeneratorDisplay::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void TrackGeneratorDisplay::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto contentBounds = bounds.reduced(0.0f, static_cast<float>(kVerticalPadding_));

        drawBackground(g, contentBounds);
        drawBorder(g, contentBounds);
        drawTriangle(g, contentBounds);
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
        g.setColour(borderColour);
        g.drawRect(bounds, static_cast<float>(kBorderThickness_));
    }

    void TrackGeneratorDisplay::drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto triangleColour = theme_->getTrackGeneratorDisplayBorderColour();
        const auto triangleHeight = kTriangleBase_ * std::sqrt(3.0f) * 0.5f;
        const auto centreX = bounds.getCentreX();
        const auto baseY = bounds.getY();

        juce::Path triangle;
        triangle.addTriangle(centreX - kTriangleBase_ * 0.5f, baseY,
                             centreX + kTriangleBase_ * 0.5f, baseY,
                             centreX, baseY - triangleHeight);

        g.setColour(triangleColour);
        g.fillPath(triangle);
    }
}
