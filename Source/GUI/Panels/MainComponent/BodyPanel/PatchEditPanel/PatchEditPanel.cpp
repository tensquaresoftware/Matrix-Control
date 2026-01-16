#include "PatchEditPanel.h"

#include "TopPanel/TopPanel.h"
#include "MiddlePanel/MiddlePanel.h"
#include "BottomPanel/BottomPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/SectionHeader.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

PatchEditPanel::~PatchEditPanel() = default;

PatchEditPanel::PatchEditPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , sectionHeader_(std::make_unique<tss::SectionHeader>(
        theme, 
        PluginDimensions::Widgets::Widths::SectionHeader::kPatchEdit,
        PluginDimensions::Widgets::Heights::kSectionHeader,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kPatchEdit)))
    , topPanel_(std::make_unique<TopPanel>(theme, widgetFactory, apvts))
    , middlePanel_(std::make_unique<MiddlePanel>(theme))
    , bottomPanel_(std::make_unique<BottomPanel>(theme, widgetFactory, apvts))
{
    addAndMakeVisible(*sectionHeader_);
    addAndMakeVisible(*topPanel_);
    addAndMakeVisible(*middlePanel_);
    addAndMakeVisible(*bottomPanel_);

    setSize(getWidth(), getHeight());
}

void PatchEditPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getPatchEditPanelBackgroundColour());
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

void PatchEditPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = sectionHeader_.get())
        header->setTheme(theme);

    if (auto* panel = topPanel_.get())
        panel->setTheme(theme);

    if (auto* panel = middlePanel_.get())
        panel->setTheme(theme);

    if (auto* panel = bottomPanel_.get())
        panel->setTheme(theme);

    repaint();
}

