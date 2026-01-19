#include "ComputerPatchesPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/GroupLabel.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/ComboBox.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"
#include <juce_core/juce_core.h>

using tss::Theme;

ComputerPatchesPanel::ComputerPatchesPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kComputerPatches);

    setupBrowserGroupLabel(theme);
    setupLoadPreviousPatchFileButton(theme, widgetFactory);
    setupLoadNextPatchFileButton(theme, widgetFactory);
    setupSelectPatchFileComboBox(theme);

    setupUtilityGroupLabel(theme);
    setupOpenPatchFolderButton(theme, widgetFactory);
    setupSavePatchFileAsButton(theme, widgetFactory);
    setupSavePatchFileButton(theme, widgetFactory);

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
    int x = 0;
    int y = kTopPadding_;

    layoutModuleHeader(x, y);
    y += PluginDimensions::Widgets::Heights::kModuleHeader;

    const auto browserGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kComputerPatchesBrowser;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    layoutBrowserGroupLabel(x, y);
    x = browserGroupWidth + kGroupLabelSpacing_;

    layoutUtilityGroupLabel(x, y);
    y += groupLabelHeight + kTopPadding_;

    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;
    x = 0;

    layoutLoadPreviousPatchFileButton(x, y);
    x += navigationButtonWidth + kSpacing_;

    layoutLoadNextPatchFileButton(x, y);
    x += navigationButtonWidth + kSpacing_;

    layoutSelectPatchFileComboBox(x, y);

    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kComputerPatchesUtility;
    const auto saveAsButtonWidth = PluginDimensions::Widgets::Widths::Button::kComputerPatchesSaveAs;
    x = browserGroupWidth + kGroupLabelSpacing_;

    layoutOpenPatchFolderButton(x, y);
    x += utilityButtonWidth + kSpacing_;

    layoutSavePatchFileAsButton(x, y);
    x += saveAsButtonWidth + kSpacing_;

    layoutSavePatchFileButton(x, y);
}

void ComputerPatchesPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeader_.get())
        header->setTheme(theme);

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setTheme(theme);

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setTheme(theme);

    if (auto* comboBox = selectPatchFileComboBox_.get())
        comboBox->setTheme(theme);

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

void ComputerPatchesPanel::setupBrowserGroupLabel(Theme& theme)
{
    browserGroupLabel = std::make_unique<tss::GroupLabel>(
        theme,
        PluginDimensions::Widgets::Widths::GroupLabel::kComputerPatchesBrowser,
        PluginDimensions::Widgets::Heights::kGroupLabel,
        PluginDescriptors::StandaloneWidgetDisplayNames::kComputerPatchesBrowser);
    addAndMakeVisible(*browserGroupLabel);
}

void ComputerPatchesPanel::setupLoadPreviousPatchFileButton(Theme& theme, WidgetFactory& widgetFactory)
{
    loadPreviousPatchFileButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadPreviousPatchFile,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    loadPreviousPatchFileButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kLoadPreviousPatchFile,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*loadPreviousPatchFileButton_);
}

void ComputerPatchesPanel::setupLoadNextPatchFileButton(Theme& theme, WidgetFactory& widgetFactory)
{
    loadNextPatchFileButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kLoadNextPatchFile,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    loadNextPatchFileButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kLoadNextPatchFile,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*loadNextPatchFileButton_);
}

void ComputerPatchesPanel::setupSelectPatchFileComboBox(Theme& theme)
{
    selectPatchFileComboBox_ = std::make_unique<tss::ComboBox>(
        theme,
        PluginDimensions::Widgets::Widths::ComboBox::kPatchManagerComputerPatches,
        PluginDimensions::Widgets::Heights::kButton,
        tss::ComboBox::Style::ButtonLike);
    selectPatchFileComboBox_->addItem(
        PluginDescriptors::StandaloneWidgetDisplayNames::kSelectPatchFile,
        kSelectPatchFileEmptyId_);
    selectPatchFileComboBox_->setSelectedId(kSelectPatchFileEmptyId_);
    selectPatchFileComboBox_->setEnabled(false);
    selectPatchFileComboBox_->onChange = [this]
    {
        if (auto* comboBox = selectPatchFileComboBox_.get())
        {
            apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSelectPatchFile,
                                    comboBox->getSelectedId(),
                                    nullptr);
        }
    };
    addAndMakeVisible(*selectPatchFileComboBox_);
}

void ComputerPatchesPanel::setupUtilityGroupLabel(Theme& theme)
{
    utilityGroupLabel = std::make_unique<tss::GroupLabel>(
        theme,
        PluginDimensions::Widgets::Widths::GroupLabel::kComputerPatchesUtility,
        PluginDimensions::Widgets::Heights::kGroupLabel,
        PluginDescriptors::StandaloneWidgetDisplayNames::kComputerPatchesUtility);
    addAndMakeVisible(*utilityGroupLabel);
}

void ComputerPatchesPanel::setupOpenPatchFolderButton(Theme& theme, WidgetFactory& widgetFactory)
{
    openPatchFolderButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kOpenPatchFolder,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    openPatchFolderButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kOpenPatchFolder,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*openPatchFolderButton_);
}

void ComputerPatchesPanel::setupSavePatchFileAsButton(Theme& theme, WidgetFactory& widgetFactory)
{
    savePatchFileAsButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kSavePatchAs,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    savePatchFileAsButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSavePatchAs,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*savePatchFileAsButton_);
}

void ComputerPatchesPanel::setupSavePatchFileButton(Theme& theme, WidgetFactory& widgetFactory)
{
    savePatchFileButton_ = widgetFactory.createStandaloneButton(
        PluginDescriptors::StandaloneWidgetIds::kSavePatchFile,
        theme,
        PluginDimensions::Widgets::Heights::kButton);
    savePatchFileButton_->onClick = [this]
    {
        apvts_.state.setProperty(PluginDescriptors::StandaloneWidgetIds::kSavePatchFile,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*savePatchFileButton_);
}

void ComputerPatchesPanel::layoutModuleHeader(int x, int y)
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kPatchManagerModule;

    if (auto* header = moduleHeader_.get())
        header->setBounds(x, y, moduleHeaderWidth, moduleHeaderHeight);
}

void ComputerPatchesPanel::layoutBrowserGroupLabel(int x, int y)
{
    const auto browserGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kComputerPatchesBrowser;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    if (auto* browserLabel = browserGroupLabel.get())
        browserLabel->setBounds(x, y, browserGroupWidth, groupLabelHeight);
}

void ComputerPatchesPanel::layoutLoadPreviousPatchFileButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;

    if (auto* prevButton = loadPreviousPatchFileButton_.get())
        prevButton->setBounds(x, y, navigationButtonWidth, buttonHeight);
}

void ComputerPatchesPanel::layoutLoadNextPatchFileButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto navigationButtonWidth = PluginDimensions::Widgets::Widths::Button::kInit;

    if (auto* nextButton = loadNextPatchFileButton_.get())
        nextButton->setBounds(x, y, navigationButtonWidth, buttonHeight);
}

void ComputerPatchesPanel::layoutSelectPatchFileComboBox(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto comboBoxWidth = PluginDimensions::Widgets::Widths::ComboBox::kPatchManagerComputerPatches;

    if (auto* comboBox = selectPatchFileComboBox_.get())
        comboBox->setBounds(x, y, comboBoxWidth, buttonHeight);
}

void ComputerPatchesPanel::layoutUtilityGroupLabel(int x, int y)
{
    const auto utilityGroupWidth = PluginDimensions::Widgets::Widths::GroupLabel::kComputerPatchesUtility;
    const auto groupLabelHeight = PluginDimensions::Widgets::Heights::kGroupLabel;

    if (auto* utilityLabel = utilityGroupLabel.get())
        utilityLabel->setBounds(x, y, utilityGroupWidth, groupLabelHeight);
}

void ComputerPatchesPanel::layoutOpenPatchFolderButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kComputerPatchesUtility;

    if (auto* openButton = openPatchFolderButton_.get())
        openButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}

void ComputerPatchesPanel::layoutSavePatchFileAsButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto saveAsButtonWidth = PluginDimensions::Widgets::Widths::Button::kComputerPatchesSaveAs;

    if (auto* saveAsButton = savePatchFileAsButton_.get())
        saveAsButton->setBounds(x, y, saveAsButtonWidth, buttonHeight);
}

void ComputerPatchesPanel::layoutSavePatchFileButton(int x, int y)
{
    const auto buttonHeight = PluginDimensions::Widgets::Heights::kButton;
    const auto utilityButtonWidth = PluginDimensions::Widgets::Widths::Button::kComputerPatchesUtility;

    if (auto* saveButton = savePatchFileButton_.get())
        saveButton->setBounds(x, y, utilityButtonWidth, buttonHeight);
}
