#include "MiddlePanel.h"

#include "GUI/Themes/Skin.h"


MiddlePanel::~MiddlePanel() = default;

MiddlePanel::MiddlePanel(tss::Skin& skin)
    : skin_(&skin)
    , envelope1Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope2Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , envelope3Display_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , trackGeneratorDisplay_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
    , patchNameDisplay_(skin, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kWidth, PluginDimensions::Panels::PatchEditPanel::MiddlePanel::ChildPanels::kHeight)
{
    setOpaque(false);
    setSize(getWidth(), getHeight());

    addAndMakeVisible(envelope1Display_);
    addAndMakeVisible(envelope2Display_);
    addAndMakeVisible(envelope3Display_);
    addAndMakeVisible(trackGeneratorDisplay_);
    addAndMakeVisible(patchNameDisplay_);
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

void MiddlePanel::setSkin(tss::Skin& skin)
{
    skin_ = &skin;
    envelope1Display_.setSkin(skin);
    envelope2Display_.setSkin(skin);
    envelope3Display_.setSkin(skin);
    trackGeneratorDisplay_.setSkin(skin);
    patchNameDisplay_.setSkin(skin);
}

