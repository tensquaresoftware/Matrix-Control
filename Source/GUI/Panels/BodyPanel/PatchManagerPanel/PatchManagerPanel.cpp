#include "PatchManagerPanel.h"

#include "Modules/BankUtilityPanel.h"
#include "Modules/InternalPatchesPanel.h"
#include "Modules/ComputerPatchesPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionHeader.h"
#include "../../../../Shared/PluginDescriptors.h"
#include "../../../Factories/WidgetFactory.h"

using tss::Theme;

PatchManagerPanel::PatchManagerPanel(Theme& inTheme, WidgetFactory& widgetFactory)
    : theme(&inTheme)
    , sectionHeader(std::make_unique<tss::SectionHeader>(
        inTheme,
        tss::SectionHeader::SectionWidth::PatchManager,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kPatchManager),
        tss::SectionHeader::ColourVariant::Blue))
    , bankUtilityPanel(std::make_unique<BankUtilityPanel>(inTheme, widgetFactory))
    , internalPatchesPanel(std::make_unique<InternalPatchesPanel>(inTheme, widgetFactory))
    , computerPatchesPanel(std::make_unique<ComputerPatchesPanel>(inTheme, widgetFactory))
{
    addAndMakeVisible(*sectionHeader);
    addAndMakeVisible(*bankUtilityPanel);
    addAndMakeVisible(*internalPatchesPanel);
    addAndMakeVisible(*computerPatchesPanel);

    setSize(getWidth(), getHeight());
}

PatchManagerPanel::~PatchManagerPanel() = default;

void PatchManagerPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void PatchManagerPanel::resized()
{
    const auto bounds = getLocalBounds();
    int y = 0;

    layoutSectionHeader(bounds, y);
    y += tss::SectionHeader::getHeight();

    layoutBankUtilityPanel(bounds, y);
    y += BankUtilityPanel::getHeight();

    layoutInternalPatchesPanel(bounds, y);
    y += InternalPatchesPanel::getHeight();

    layoutComputerPatchesPanel(bounds, y);
}

void PatchManagerPanel::layoutSectionHeader(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* header = sectionHeader.get())
    {
        header->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            bounds.getWidth(),
            tss::SectionHeader::getHeight()
        );
    }
}

void PatchManagerPanel::layoutBankUtilityPanel(const juce::Rectangle<int>& bounds, int y)
{
    if (auto* panel = bankUtilityPanel.get())
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
    if (auto* panel = internalPatchesPanel.get())
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
    if (auto* panel = computerPatchesPanel.get())
    {
        panel->setBounds(
            bounds.getX(),
            bounds.getY() + y,
            ComputerPatchesPanel::getWidth(),
            ComputerPatchesPanel::getHeight()
        );
    }
}

void PatchManagerPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = sectionHeader.get())
        header->setTheme(inTheme);

    if (auto* panel = bankUtilityPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = internalPatchesPanel.get())
        panel->setTheme(inTheme);

    if (auto* panel = computerPatchesPanel.get())
        panel->setTheme(inTheme);

    repaint();
}

