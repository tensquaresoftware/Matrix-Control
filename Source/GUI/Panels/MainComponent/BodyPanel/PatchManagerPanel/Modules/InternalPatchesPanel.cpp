#include "InternalPatchesPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/GroupLabel.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/NumberBox.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"
#include <juce_core/juce_core.h>

using tss::Theme;

InternalPatchesPanel::InternalPatchesPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setOpaque(true);
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kInternalPatches);

    setupBrowserGroupLabel(theme);
    setupLoadPreviousPatchButton(theme, widgetFactory);
    setupLoadNextPatchButton(theme, widgetFactory);
    setupCurrentBankNumberBox(theme);
    setupCurrentPatchNumberBox(theme);

    setupUtilityGroupLabel(theme);
    setupInitPatchButton(theme, widgetFactory);
    setupCopyPatchButton(theme, widgetFactory);
    setupPastePatchButton(theme, widgetFactory);
    setupStorePatchButton(theme, widgetFactory);

    setSize(getWidth(), getHeight());
}

InternalPatchesPanel::~InternalPatchesPanel() = default;

void InternalPatchesPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme_)
        g.fillAll(currentTheme->getPatchManagerPanelBackgroundColour());
}

void InternalPatchesPanel::resized()
{
    int x = 0;
    int y = kTopPadding_;

    layoutModuleHeader(x, y);
    y += PluginDimensions::Widgets::Heights::kModuleHeader;

    const auto browserGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kInternalPatchesBrowser;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    layoutBrowserGroupLabel(x, y);
    x = browserGroupWidth + kGroupLabelSpacing_;

    layoutUtilityGroupLabel(x, y);
    y += groupLabelHeight + kTopPadding_;

    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;
    x = 0;

    layoutLoadPreviousPatchButton(x, y);
    x += navigationButtonWidth + kSpacing_;

    layoutLoadNextPatchButton(x, y);
    x += navigationButtonWidth + kSpacing_;

    layoutCurrentBankNumberBox(x, y);
    x += PluginDimensions::Widgets::Widths::NumberBox::kPatchManagerBankNumber + kSpacing_;

    layoutCurrentPatchNumberBox(x, y);

    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kInternalPatchesUtility;
    x = browserGroupWidth + kGroupLabelSpacing_;

    layoutInitPatchButton(x, y);
    x += utilityButtonWidth + kSpacing_;

    layoutCopyPatchButton(x, y);
    x += utilityButtonWidth + kSpacing_;

    layoutPastePatchButton(x, y);
    x += utilityButtonWidth + kSpacing_;

    layoutStorePatchButton(x, y);
}

void InternalPatchesPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeader.get())
        header->setTheme(theme);

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setTheme(theme);

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setTheme(theme);

    if (auto* bankNumber = currentBankNumber.get())
        bankNumber->setTheme(theme);

    if (auto* patchNumber = currentPatchNumber.get())
        patchNumber->setTheme(theme);

    repaint();
}

void InternalPatchesPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    moduleHeader = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Blue);
    addAndMakeVisible(*moduleHeader);
}

void InternalPatchesPanel::setupBrowserGroupLabel(Theme& theme)
{
    browserGroupLabel = std::make_unique<tss::GroupLabel>(
        theme,
        PluginDimensions::Widgets::Widths::GroupLabel::kInternalPatchesBrowser,
        PluginDimensions::Widgets::Heights::kGroupLabel,
        PluginDescriptors::StandaloneWidgetDisplayNames::kInternalPatchesBrowser);
    addAndMakeVisible(*browserGroupLabel);
}

void InternalPatchesPanel::setupLoadPreviousPatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    loadPreviousPatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadPreviousPatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    loadPreviousPatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kLoadPreviousPatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*loadPreviousPatchButton_);
}

void InternalPatchesPanel::setupLoadNextPatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    loadNextPatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadNextPatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    loadNextPatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kLoadNextPatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*loadNextPatchButton_);
}

void InternalPatchesPanel::setupCurrentBankNumberBox(Theme& theme)
{
    currentBankNumber = std::make_unique<tss::NumberBox>(
        theme, 
        PluginDimensions::Widgets::Widths::NumberBox::kPatchManagerBankNumber, 
        false);
    currentBankNumber->setShowDot(true);
    addAndMakeVisible(*currentBankNumber);
}

void InternalPatchesPanel::setupCurrentPatchNumberBox(Theme& theme)
{
    currentPatchNumber = std::make_unique<tss::NumberBox>(
        theme, 
        PluginDimensions::Widgets::Widths::NumberBox::kPatchManagerPatchNumber, 
        true);
    addAndMakeVisible(*currentPatchNumber);
}

void InternalPatchesPanel::setupUtilityGroupLabel(Theme& theme)
{
    utilityGroupLabel = std::make_unique<tss::GroupLabel>(
        theme,
        PluginDimensions::Widgets::Widths::GroupLabel::kInternalPatchesUtility,
        PluginDimensions::Widgets::Heights::kGroupLabel,
        PluginDescriptors::StandaloneWidgetDisplayNames::kInternalPatchesUtility);
    addAndMakeVisible(*utilityGroupLabel);
}

void InternalPatchesPanel::setupInitPatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    initPatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kInitPatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    initPatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kInitPatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*initPatchButton_);
}

void InternalPatchesPanel::setupCopyPatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    copyPatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kCopyPatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    copyPatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kCopyPatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*copyPatchButton_);
}

void InternalPatchesPanel::setupPastePatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    pastePatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kPastePatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    pastePatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kPastePatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*pastePatchButton_);
}

void InternalPatchesPanel::setupStorePatchButton(Theme& theme, WidgetFactory& widgetFactory)
{
    storePatchButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kStorePatch,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    storePatchButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kStorePatch,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*storePatchButton_);
}

void InternalPatchesPanel::layoutModuleHeader(int x, int y)
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule;

    if (auto* header = moduleHeader.get())
        header->setBounds(x, y, moduleHeaderWidth, moduleHeaderHeight);
}

void InternalPatchesPanel::layoutBrowserGroupLabel(int x, int y)
{
    const auto browserGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kInternalPatchesBrowser;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setBounds(x, y, browserGroupWidth, groupLabelHeight);
}

void InternalPatchesPanel::layoutLoadPreviousPatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;

    if (auto* prevButton = loadPreviousPatchButton_.get())
        prevButton->setBounds(x, y, navigationButtonWidth, buttonHeight);
}

void InternalPatchesPanel::layoutLoadNextPatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;

    if (auto* nextButton = loadNextPatchButton_.get())
        nextButton->setBounds(x, y, navigationButtonWidth, buttonHeight);
}

void InternalPatchesPanel::layoutCurrentBankNumberBox(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto bankNumberWidth = PluginDimensions::Widgets::Widths::NumberBox::kPatchManagerBankNumber;

    if (auto* bankNumber = currentBankNumber.get())
        bankNumber->setBounds(x, y, bankNumberWidth, buttonHeight);
}

void InternalPatchesPanel::layoutCurrentPatchNumberBox(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto patchNumberWidth = PluginDimensions::Widgets::Widths::NumberBox::kPatchManagerPatchNumber;

    if (auto* patchNumber = currentPatchNumber.get())
        patchNumber->setBounds(x, y, patchNumberWidth, buttonHeight);
}

void InternalPatchesPanel::layoutUtilityGroupLabel(int x, int y)
{
    const auto utilityGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kInternalPatchesUtility;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setBounds(x, y, utilityGroupWidth, groupLabelHeight);
}

void InternalPatchesPanel::layoutInitPatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kInternalPatchesUtility;

    if (auto* initButton = initPatchButton_.get())
        initButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}

void InternalPatchesPanel::layoutCopyPatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kInternalPatchesUtility;

    if (auto* copyButton = copyPatchButton_.get())
        copyButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}

void InternalPatchesPanel::layoutPastePatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kInternalPatchesUtility;

    if (auto* pasteButton = pastePatchButton_.get())
        pasteButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}

void InternalPatchesPanel::layoutStorePatchButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kInternalPatchesUtility;

    if (auto* storeButton = storePatchButton_.get())
        storeButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}
