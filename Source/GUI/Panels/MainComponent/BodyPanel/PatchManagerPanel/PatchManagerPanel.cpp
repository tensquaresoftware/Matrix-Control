#include "PatchManagerPanel.h"

#include "Modules/BankUtilityPanel.h"
#include "Modules/InternalPatchesPanel.h"
#include "Modules/ComputerPatchesPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/SectionHeader.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

PatchManagerPanel::PatchManagerPanel(Theme& theme, WidgetFactory& widgetFactory)
    : theme_(&theme)
    , sectionHeader_(std::make_unique<tss::SectionHeader>(
        theme,
        PluginDimensions::Widgets::Widths::SectionHeader::kPatchManager,
        PluginDimensions::Widgets::Heights::kSectionHeader,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kPatchManager),
        tss::SectionHeader::ColourVariant::Blue))
    , bankUtilityPanel_(std::make_unique<BankUtilityPanel>(theme, widgetFactory))
    , internalPatchesPanel_(std::make_unique<InternalPatchesPanel>(theme, widgetFactory))
    , computerPatchesPanel_(std::make_unique<ComputerPatchesPanel>(theme, widgetFactory))
{
    addAndMakeVisible(*sectionHeader_);
    addAndMakeVisible(*bankUtilityPanel_);
    addAndMakeVisible(*internalPatchesPanel_);
    addAndMakeVisible(*computerPatchesPanel_);

    setSize(getWidth(), getHeight());
}

PatchManagerPanel::~PatchManagerPanel() = default;

void PatchManagerPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void PatchManagerPanel::resized()
{
    const auto bounds = getLocalBounds();
    int y = 0;

    layoutSectionHeader(bounds, y);
    y += PluginDimensions::Widgets::Heights::kSectionHeader;

    layoutBankUtilityPanel(bounds, y);
    y += BankUtilityPanel::getHeight();

    layoutInternalPatchesPanel(bounds, y);
    y += InternalPatchesPanel::getHeight();

    layoutComputerPatchesPanel(bounds, y);
}

void PatchManagerPanel::layoutSectionHeader(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* header = sectionHeader_.get())
    {
        header->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            bounds.getWidth(),
            PluginDimensions::Widgets::Heights::kSectionHeader
        );
    }
}

void PatchManagerPanel::layoutBankUtilityPanel(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* panel = bankUtilityPanel_.get())
    {
        panel->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            BankUtilityPanel::getWidth(),
            BankUtilityPanel::getHeight()
        );
    }
}

void PatchManagerPanel::layoutInternalPatchesPanel(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* panel = internalPatchesPanel_.get())
    {
        panel->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            InternalPatchesPanel::getWidth(),
            InternalPatchesPanel::getHeight()
        );
    }
}

void PatchManagerPanel::layoutComputerPatchesPanel(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* panel = computerPatchesPanel_.get())
    {
        panel->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            ComputerPatchesPanel::getWidth(),
            ComputerPatchesPanel::getHeight()
        );
    }
}

void PatchManagerPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = sectionHeader_.get())
        header->setTheme(theme);

    if (auto* panel = bankUtilityPanel_.get())
        panel->setTheme(theme);

    if (auto* panel = internalPatchesPanel_.get())
        panel->setTheme(theme);

    if (auto* panel = computerPatchesPanel_.get())
        panel->setTheme(theme);

    repaint();
}

