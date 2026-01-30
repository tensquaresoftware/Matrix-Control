#include "PatchNameDisplay.h"

#include "GUI/Themes/Theme.h"
#include "Shared/PluginDisplayNames.h"

namespace tss
{
    PatchNameDisplay::PatchNameDisplay(Theme& theme, int width, int height)
        : theme_(&theme)
        , width_(width)
        , height_(height)
        , patchName_(PluginDescriptors::StandaloneWidgetDisplayNames::kDefaultPatchName)
    {
        setOpaque(false);
        setSize(width_, height_);
    }

    void PatchNameDisplay::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void PatchNameDisplay::setPatchName(const juce::String& patchName)
    {
        patchName_ = patchName;
        repaint();
    }

    void PatchNameDisplay::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto contentBounds = bounds.reduced(0.0f, static_cast<float>(kVerticalPadding_));

        drawBackground(g, contentBounds);
        drawBorder(g, contentBounds);
        drawText(g, contentBounds);
    }


    void PatchNameDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme_->getPatchNameDisplayBackgroundColour();
        g.setColour(backgroundColour);
        g.fillRect(bounds);
    }

    void PatchNameDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getPatchNameDisplayBorderColour();
        g.setColour(borderColour);
        g.drawRect(bounds, static_cast<float>(kBorderThickness_));
    }

    void PatchNameDisplay::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto textColour = theme_->getPatchNameDisplayTextColour();
        const auto font = theme_->getBaseFont().withHeight(kFontHeight_);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(patchName_, bounds, juce::Justification::centred, false);
    }
}
