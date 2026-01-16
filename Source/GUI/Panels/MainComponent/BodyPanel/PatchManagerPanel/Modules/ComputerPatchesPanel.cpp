#include "ComputerPatchesPanel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../Shared/PluginDimensions.h"
#include "../../../../../Factories/WidgetFactory.h"

using tss::Theme;

ComputerPatchesPanel::ComputerPatchesPanel(Theme& theme, WidgetFactory& widgetFactory)
    : theme_(&theme)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kComputerPatches);

    setSize(getWidth(), getHeight());
}

ComputerPatchesPanel::~ComputerPatchesPanel() = default;

void ComputerPatchesPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void ComputerPatchesPanel::resized()
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule;

    int y = kTopPadding_;

    if (auto* header = moduleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);
}

void ComputerPatchesPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeader_.get())
        header->setTheme(theme);

    repaint();
}

void ComputerPatchesPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    moduleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*moduleHeader_);
}
