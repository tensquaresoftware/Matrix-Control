#include "Label.h"

#include "GUI/Themes/Theme.h"

namespace tss
{
    Label::Label(Theme& theme, int width, int height, const juce::String& text)
        : theme_(&theme)
        , width_(width)
        , height_(height)
        , labelText_(text)
    {
        setOpaque(true);
        setSize(width_, height_);
    }

    void Label::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void Label::setText(const juce::String& text)
    {
        if (labelText_ != text)
        {
            labelText_ = text;
            repaint();
        }
    }

    void Label::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        g.fillAll(theme_->getGuiBackgroundColour());

        const auto bounds = getLocalBounds().toFloat();

        drawBackground(g, bounds);
        drawText(g, bounds);
    }


    void Label::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme_->getLabelBackgroundColour();
        const auto backgroundBounds = bounds.reduced(kBackgroundPadding_);
        
        g.setColour(backgroundColour);
        g.fillRect(backgroundBounds);
    }

    void Label::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        if (labelText_.isEmpty())
        {
            return;
        }

        const auto textColour = theme_->getLabelTextColour();
        const auto font = theme_->getBaseFont();

        auto textBounds = bounds.reduced(kBackgroundPadding_);
        textBounds.removeFromLeft(kTextLeftPadding_);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText_, textBounds, juce::Justification::centredLeft, false);
    }
}

