#include "PatchEditPanel.h"

#include "TopPanel/TopPanel.h"
#include "MiddlePanel/MiddlePanel.h"
#include "BottomPanel/BottomPanel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Widgets/SectionHeader.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"


PatchEditPanel::~PatchEditPanel() = default;

PatchEditPanel::PatchEditPanel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : skin_(&skin)
    , sectionHeader_(std::make_unique<tss::SectionHeader>(
        skin, 
        PluginDimensions::Widgets::Widths::SectionHeader::kPatchEdit,
        PluginDimensions::Widgets::Heights::kSectionHeader,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kPatchEdit)))
    , topPanel_(std::make_unique<TopPanel>(skin, widgetFactory, apvts))
    , middlePanel_(std::make_unique<MiddlePanel>(skin))
    , bottomPanel_(std::make_unique<BottomPanel>(skin, widgetFactory, apvts))
{
    setOpaque(false);
    addAndMakeVisible(*sectionHeader_);
    addAndMakeVisible(*topPanel_);
    addAndMakeVisible(*middlePanel_);
    addAndMakeVisible(*bottomPanel_);

    setSize(getWidth(), getHeight());
}

void PatchEditPanel::resized()
{
    const auto bounds = getLocalBounds();
    
    const auto sectionHeaderY = 0;
    sectionHeader_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + sectionHeaderY,
        bounds.getWidth(),
        PluginDimensions::Widgets::Heights::kSectionHeader
    );
    
    const auto topPanelY = sectionHeaderY + PluginDimensions::Widgets::Heights::kSectionHeader;
    topPanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + topPanelY,
        bounds.getWidth(),
        TopPanel::getHeight()
    );
    
    const auto middlePanelY = topPanelY + TopPanel::getHeight();
    middlePanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + middlePanelY,
        bounds.getWidth(),
        MiddlePanel::getHeight()
    );
    
    const auto bottomPanelY = middlePanelY + MiddlePanel::getHeight();
    bottomPanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + bottomPanelY,
        bounds.getWidth(),
        BottomPanel::getHeight()
    );
}

void PatchEditPanel::setSkin(tss::Skin& skin)
{
    skin_ = &skin;

    if (auto* header = sectionHeader_.get())
        header->setSkin(skin);

    if (auto* panel = topPanel_.get())
        panel->setSkin(skin);

    if (auto* panel = middlePanel_.get())
        panel->setSkin(skin);

    if (auto* panel = bottomPanel_.get())
        panel->setSkin(skin);
}

