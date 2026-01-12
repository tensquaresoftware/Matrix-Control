#include "ComputerPatchesPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

ComputerPatchesPanel::ComputerPatchesPanel(Theme& inTheme, WidgetFactory& widgetFactory)
    : theme(&inTheme)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kComputerPatches);

    setSize(getWidth(), getHeight());
}

ComputerPatchesPanel::~ComputerPatchesPanel() = default;

void ComputerPatchesPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void ComputerPatchesPanel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::PatchManager);

    int y = kTopPadding;

    if (auto* header = computerPatchesModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);
}

void ComputerPatchesPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = computerPatchesModuleHeader.get())
        header->setTheme(inTheme);

    repaint();
}

void ComputerPatchesPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    computerPatchesModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme,
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::PatchManager,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*computerPatchesModuleHeader);
}
