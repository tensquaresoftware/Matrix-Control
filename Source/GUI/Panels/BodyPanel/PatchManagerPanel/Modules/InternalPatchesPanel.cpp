#include "InternalPatchesPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/GroupLabel.h"
#include "../../../../Widgets/Button.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

InternalPatchesPanel::InternalPatchesPanel(Theme& inTheme, WidgetFactory& widgetFactory)
    : theme(&inTheme)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kInternalPatches);

    browserGroupLabel = std::make_unique<tss::GroupLabel>(
        inTheme,
        tss::GroupLabel::GroupWidth::InternalPatchesBrowser,
        PluginDescriptors::StandaloneWidgetDisplayNames::kInternalPatchesBrowser);
    addAndMakeVisible(*browserGroupLabel);

    utilityGroupLabel = std::make_unique<tss::GroupLabel>(
        inTheme,
        tss::GroupLabel::GroupWidth::InternalPatchesUtility,
        PluginDescriptors::StandaloneWidgetDisplayNames::kInternalPatchesUtility);
    addAndMakeVisible(*utilityGroupLabel);

    loadPreviousInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadPreviousPatch,
        inTheme);
    addAndMakeVisible(*loadPreviousInternalPatchButton);

    loadNextInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadNextPatch,
        inTheme);
    addAndMakeVisible(*loadNextInternalPatchButton);

    initInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kInitPatch,
        inTheme);
    addAndMakeVisible(*initInternalPatchButton);

    copyInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kCopyPatch,
        inTheme);
    addAndMakeVisible(*copyInternalPatchButton);

    pasteInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kPastePatch,
        inTheme);
    addAndMakeVisible(*pasteInternalPatchButton);

    storeInternalPatchButton = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kStorePatch,
        inTheme);
    addAndMakeVisible(*storeInternalPatchButton);

    setSize(getWidth(), getHeight());
}

InternalPatchesPanel::~InternalPatchesPanel() = default;

void InternalPatchesPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void InternalPatchesPanel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::PatchManager);

    int y = kTopPadding;

    if (auto* header = internalPatchesModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    const auto browserGroupWidth = tss::GroupLabel::getWidth(tss::GroupLabel::GroupWidth::InternalPatchesBrowser);
    constexpr int browserGroupHeight = 40;
    const auto utilityGroupWidth = tss::GroupLabel::getWidth(tss::GroupLabel::GroupWidth::InternalPatchesUtility);
    constexpr int utilityGroupHeight = 40;

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setBounds(0, y, browserGroupWidth, browserGroupHeight);

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setBounds(browserGroupWidth + kSpacing, y, utilityGroupWidth, utilityGroupHeight);

    y += browserGroupHeight + kTopPadding;

    const auto buttonHeight = tss::Button::getHeight();
    const auto navigationButtonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InitCopyPaste);
    const auto utilityButtonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InternalPatchesUtility);

    if (auto* prevButton = loadPreviousInternalPatchButton.get())
        prevButton->setBounds(0, y, navigationButtonWidth, buttonHeight);

    if (auto* nextButton = loadNextInternalPatchButton.get())
        nextButton->setBounds(navigationButtonWidth + kSpacing, y, navigationButtonWidth, buttonHeight);

    int utilityX = browserGroupWidth + kSpacing;
    if (auto* initButton = initInternalPatchButton.get())
    {
        initButton->setBounds(utilityX, y, utilityButtonWidth, buttonHeight);
        utilityX += utilityButtonWidth + kSpacing;
    }

    if (auto* copyButton = copyInternalPatchButton.get())
    {
        copyButton->setBounds(utilityX, y, utilityButtonWidth, buttonHeight);
        utilityX += utilityButtonWidth + kSpacing;
    }

    if (auto* pasteButton = pasteInternalPatchButton.get())
    {
        pasteButton->setBounds(utilityX, y, utilityButtonWidth, buttonHeight);
        utilityX += utilityButtonWidth + kSpacing;
    }

    if (auto* storeButton = storeInternalPatchButton.get())
        storeButton->setBounds(utilityX, y, utilityButtonWidth, buttonHeight);
}

void InternalPatchesPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = internalPatchesModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setTheme(inTheme);

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setTheme(inTheme);

    repaint();
}

void InternalPatchesPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    internalPatchesModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme,
        widgetFactory.getGroupDisplayName(moduleId),
        tss::ModuleHeader::ModuleWidth::PatchManager,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*internalPatchesModuleHeader);
}
