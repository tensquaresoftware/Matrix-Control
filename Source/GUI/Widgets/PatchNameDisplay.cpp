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
        setOpaque(true);
        setSize(width_, height_);
    }

    void PatchNameDisplay::setTheme(Theme& theme)
    {
        theme_ = &theme;
        repaint();
    }

    void PatchNameDisplay::setPatchName(const juce::String& patchName)
    {
        patchName_ = patchName;
        repaint();
    }

    void PatchNameDisplay::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
        {
            return;
        }

        g.fillAll(theme_->getPatchEditPanelBackgroundColour());

        const auto bounds = getLocalBounds().toFloat();

        drawBackground(g, bounds);
        drawBorder(g, bounds);
        drawText(g, bounds);
    }


    void PatchNameDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto backgroundColour = theme_->getPatchNameDisplayBackgroundColour();
        const auto contentBounds = bounds.reduced(0, kVerticalPadding_);

        g.setColour(backgroundColour);
        g.fillRect(contentBounds);
    }

    void PatchNameDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto borderColour = theme_->getPatchNameDisplayBorderColour();
        const auto contentBounds = bounds.reduced(0, kVerticalPadding_);

        g.setColour(borderColour);
        g.drawRect(contentBounds, kBorderThickness_);
    }

    void PatchNameDisplay::drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        const auto textColour = theme_->getPatchNameDisplayTextColour();
        const auto contentBounds = bounds.reduced(0, kVerticalPadding_);
        auto font = theme_->getBaseFont();
        font = font.withHeight(kFontHeight_);

        g.setColour(textColour);
        g.setFont(font);
        g.drawText(patchName_, contentBounds, juce::Justification::centred, false);
    }
}
