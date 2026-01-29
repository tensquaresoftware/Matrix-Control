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
        setOpaque(false);
        setSize(width_, height_);
    }

    void Label::setTheme(Theme& theme)
    {
        theme_ = &theme;
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
        if (theme_ == nullptr || labelText_.isEmpty())
            return;

        const auto textColour = theme_->getLabelTextColour();
        const auto font = theme_->getBaseFont();

        auto textBounds = getLocalBounds().toFloat();
        textBounds.removeFromLeft(kTextLeftPadding_);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(labelText_, textBounds, juce::Justification::centredLeft, false);
    }
}

