#include "MiddlePanel.h"

#include "GUI/Themes/Theme.h"

using tss::Theme;

MiddlePanel::~MiddlePanel() = default;

MiddlePanel::MiddlePanel(Theme& theme)
    : theme_(&theme)
    , envelope1Display_(theme, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope2Display_(theme, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope3Display_(theme, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , trackGeneratorDisplay_(theme, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , patchNameDisplay_(theme, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
{
    setOpaque(true);
    setSize(getWidth(), getHeight());

    addAndMakeVisible(envelope1Display_);
    addAndMakeVisible(envelope2Display_);
    addAndMakeVisible(envelope3Display_);
    addAndMakeVisible(trackGeneratorDisplay_);
    addAndMakeVisible(patchNameDisplay_);
}

void MiddlePanel::paint(juce::Graphics& g)
{
    if (theme_ == nullptr)
    {
        return;
    }

    g.fillAll(theme_->getPatchEditPanelBackgroundColour());
}

void MiddlePanel::resized()
{
    const auto childWidth = PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth;
    const auto childHeight = PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight;

    envelope1Display_.setBounds(0, 0, childWidth, childHeight);
    envelope2Display_.setBounds(envelope1Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    envelope3Display_.setBounds(envelope2Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    trackGeneratorDisplay_.setBounds(envelope3Display_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
    patchNameDisplay_.setBounds(trackGeneratorDisplay_.getBounds().getRight() + kSpacing, 0, childWidth, childHeight);
}

void MiddlePanel::setTheme(Theme& theme)
{
    theme_ = &theme;
    envelope1Display_.setTheme(theme);
    envelope2Display_.setTheme(theme);
    envelope3Display_.setTheme(theme);
    trackGeneratorDisplay_.setTheme(theme);
    patchNameDisplay_.setTheme(theme);
}

