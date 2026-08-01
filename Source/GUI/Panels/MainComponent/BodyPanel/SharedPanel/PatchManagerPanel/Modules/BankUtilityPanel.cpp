#include "BankUtilityPanel.h"

#include "../PatchManagerEqualWidthStrip.h"

#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/Services/DeviceMemoryLimits.h"
#include "Core/Services/DeviceTypeRegistry.h"
#include "GUI/Helpers/GrayedControlHelper.h"
#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Skins/ColourChart.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/Button.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "GUI/Factories/WidgetFactory.h"
#include <juce_core/juce_core.h>

namespace
{
    namespace SelectBankIds = PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets;

    constexpr const char* kSelectBankIds[BankUtilityPanel::kBankCount] = {
        SelectBankIds::kSelectBank0,
        SelectBankIds::kSelectBank1,
        SelectBankIds::kSelectBank2,
        SelectBankIds::kSelectBank3,
        SelectBankIds::kSelectBank4,
        SelectBankIds::kSelectBank5,
        SelectBankIds::kSelectBank6,
        SelectBankIds::kSelectBank7,
        SelectBankIds::kSelectBank8,
        SelectBankIds::kSelectBank9,
    };

    void setSubtreeKeyboardInteractionEnabled(juce::Component& root, bool enabled)
    {
        root.setWantsKeyboardFocus(enabled);
        root.setMouseClickGrabsKeyboardFocus(enabled);

        for (int i = 0; i < root.getNumChildComponents(); ++i)
        {
            if (auto* child = root.getChildComponent(i))
                setSubtreeKeyboardInteractionEnabled(*child, enabled);
        }
    }

    void applyGrayedToChild(juce::Component* child, bool grayed)
    {
        if (child == nullptr)
            return;

        TSS::GrayedControlHelper::applyGrayedAppearance(*child, grayed);
        child->setInterceptsMouseClicks(! grayed, ! grayed);
        setSubtreeKeyboardInteractionEnabled(*child, ! grayed);
    }

    void setOptionalUiScale(TSS::Label* label, float sf)
    {
        if (label != nullptr)
            label->setUiScale(sf);
    }

    void setOptionalUiScale(TSS::ModuleHeader* header, float sf)
    {
        if (header != nullptr)
            header->setUiScale(sf);
    }

    void setOptionalUiScale(TSS::Button* button, float sf)
    {
        if (button != nullptr)
            button->setUiScale(sf);
    }

    void setOptionalLook(TSS::Button* button, const TSS::ButtonLook& look)
    {
        if (button != nullptr)
            button->setLook(look);
    }
}


BankUtilityPanel::BankUtilityPanel(TSS::ISkin& skin, const BankUtilityPanelDimensions& dims, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : dims_(dims)
    , skin_(&skin)
    , apvts_(apvts)
{
    setOpaque(false);
    setupModuleHeader(skin, widgetFactory, PluginIDs::PatchManagerSection::BankUtilityModule::kGroupId);
    setupBankSelectorLabel(skin);
    setupSelectBankButtons(skin, widgetFactory);
    setupImportExportButtons(skin, widgetFactory);

    normalBankLook_ = TSS::buttonLookFromSkin(skin);
    apvts_.state.addListener(this);
    refreshDeviceGating();
    refreshSelectedBankHighlight();
    refreshImportExportEnabled();

    setSize(dims_.width, dims_.height);
}

BankUtilityPanel::~BankUtilityPanel()
{
    apvts_.state.removeListener(this);
}

void BankUtilityPanel::valueTreePropertyChanged(juce::ValueTree&,
                                                const juce::Identifier& property)
{
    const auto propertyName = property.toString();
    if (propertyName == MatrixDeviceTypes::kApvtsPropertyName
        || propertyName == "deviceDetected"
        || propertyName == PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties::kCompareActive)
    {
        refreshDeviceGating();
    }
    else if (propertyName == PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank)
    {
        refreshSelectedBankHighlight();
        refreshImportExportEnabled();
    }
}

void BankUtilityPanel::valueTreeRedirected(juce::ValueTree&)
{
    refreshDeviceGating();
    refreshSelectedBankHighlight();
    refreshImportExportEnabled();
}

void BankUtilityPanel::refreshDeviceGating()
{
    const bool deviceDetected = static_cast<bool>(apvts_.state.getProperty("deviceDetected"));
    const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
        apvts_.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));
    const auto limits = Core::DeviceMemoryLimits::resolve(deviceType);
    const bool compareActive = static_cast<bool>(apvts_.state.getProperty(
        PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties::kCompareActive,
        false));

    // Root Compare/device lock already dims this panel via CompareLockBinder — skip child gray
    // so we do not fight panel-level alpha/intercepts or stack to ~0.25 alpha.
    const bool rootLocked = Core::isSectionLocked(deviceDetected, deviceType, compareActive);
    const bool shouldGray = ! rootLocked
        && deviceDetected
        && ! limits.hasBankConcept();
    setBankUtilityGrayed(shouldGray);
    refreshImportExportEnabled();
}

void BankUtilityPanel::setBankUtilityGrayed(bool grayed)
{
    bankUtilityGrayed_ = grayed;

    // Child-level lock (Master Edit pattern). Do not touch this panel's alpha/intercepts —
    // CompareLockBinder owns those for device/Compare section lock.
    applyGrayedToChild(bankUtilityModuleHeader_.get(), grayed);
    applyGrayedToChild(bankSelectorLabel_.get(), grayed);
    applyGrayedToChild(unlockBankButton_.get(), grayed);

    for (auto& button : selectBankButtons_)
        applyGrayedToChild(button.get(), grayed);

    if (grayed)
        giveAwayKeyboardFocus();

    refreshSelectedBankHighlight();
    repaint();
}

void BankUtilityPanel::refreshImportExportEnabled()
{
    const bool deviceDetected = static_cast<bool>(apvts_.state.getProperty("deviceDetected"));
    const auto deviceType = Core::DeviceTypeRegistry::fromApvtsProperty(
        apvts_.state.getProperty(MatrixDeviceTypes::kApvtsPropertyName));
    const auto limits = Core::DeviceMemoryLimits::resolve(deviceType);
    const bool compareActive = static_cast<bool>(apvts_.state.getProperty(
        PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties::kCompareActive,
        false));
    const bool rootLocked = Core::isSectionLocked(deviceDetected, deviceType, compareActive);

    if (exportBankButton_)
        exportBankButton_->setEnabled(! rootLocked);

    if (importBankButton_ == nullptr)
        return;

    if (rootLocked)
    {
        importBankButton_->setEnabled(false);
        return;
    }

    if (! limits.hasBankConcept())
    {
        // Matrix-6/6R: single 100-patch memory, always importable when the section is unlocked.
        importBankButton_->setEnabled(true);
        return;
    }

    const int selectedBank = static_cast<int>(apvts_.state.getProperty(
        PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank,
        0));
    importBankButton_->setEnabled(! limits.isRomBank(selectedBank));
}

void BankUtilityPanel::refreshSelectedBankHighlight()
{
    const int selected = static_cast<int>(apvts_.state.getProperty(
        PluginIDs::PatchManagerSection::BankUtilityModule::StateProperties::kSelectedBank,
        0));

    for (auto& button : selectBankButtons_)
        setOptionalLook(button.get(), normalBankLook_);

    if (!bankUtilityGrayed_ && selected >= 0 && selected < kBankCount)
    {
        if (auto* button = selectBankButtons_[static_cast<size_t>(selected)].get())
        {
            auto accentLook = normalBankLook_;
            const auto selectedRed = juce::Colour(ColourChart::kRed);
            accentLook.textOff = selectedRed;
            accentLook.textHover = selectedRed;
            accentLook.textClicked = selectedRed;
            button->setLook(accentLook);
        }
    }
}

void BankUtilityPanel::layoutContentRows(float sf)
{
    const int moduleHeaderHeight = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.moduleHeader.height), sf);
    const int moduleHeaderWidth = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.moduleHeader.patchManagerTitleBandWidth), sf);
    const int labelWidth = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.bankSelectorLabel.patchManagerSelectBankWidth), sf);
    const int labelHeight = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.bankSelectorLabel.height), sf);
    const int buttonHeight = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.buttons.height), sf);
    const int lockButtonWidth = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.buttons.patchManagerUnlockBankWidth), sf);
    const int importButtonWidth = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.buttons.patchManagerImportBankWidth), sf);
    const int exportButtonWidth = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.buttons.patchManagerExportBankWidth), sf);
    const int rowGap = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.layout.interControlGap), sf);
    const int rowH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.layout.contentRowHeight), sf);

    if (auto* header = bankUtilityModuleHeader_.get())
        header->setBounds(0, 0, moduleHeaderWidth, moduleHeaderHeight);

    // Row 1 — label + banks 0–4 (Recipes::BankUtilityModule::kHeight = 76 @ 100 %)
    const int row1Y = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.moduleHeader.height), sf);
    const int shortControlY = row1Y + (rowH - labelHeight) / 2;

    if (auto* label = bankSelectorLabel_.get())
        label->setBounds(0, shortControlY, labelWidth, labelHeight);

    juce::Component* row1Banks[5] = {};
    for (int i = 0; i < 5; ++i)
        row1Banks[i] = selectBankButtons_[static_cast<size_t>(i)].get();

    const int row1AfterBanksX = TSS::placeEqualWidthStrip(labelWidth + rowGap, row1Y, sf,
                              dims_.buttons.patchManagerBankSelectWidth, dims_.buttons.height,
                              dims_.layout.interControlGap, row1Banks, 5);

    if (auto* button = importBankButton_.get())
        button->setBounds(row1AfterBanksX + rowGap, row1Y, importButtonWidth, buttonHeight);

    // Row 2 — unlock + banks 5–9 (successive integer stack: header + row + gap)
    const int row2Y = row1Y + rowH + rowGap;

    if (auto* button = unlockBankButton_.get())
        button->setBounds(0, row2Y, lockButtonWidth, buttonHeight);

    juce::Component* row2Banks[5] = {};
    for (int i = 0; i < 5; ++i)
        row2Banks[i] = selectBankButtons_[static_cast<size_t>(i + 5)].get();

    const int row2AfterBanksX = TSS::placeEqualWidthStrip(lockButtonWidth + rowGap, row2Y, sf,
                              dims_.buttons.patchManagerBankSelectWidth, dims_.buttons.height,
                              dims_.layout.interControlGap, row2Banks, 5);

    if (auto* button = exportBankButton_.get())
        button->setBounds(row2AfterBanksX + rowGap, row2Y, exportButtonWidth, buttonHeight);
}

void BankUtilityPanel::applyChildUiScales(float sf)
{
    setOptionalUiScale(bankSelectorLabel_.get(), sf);
    setOptionalUiScale(bankUtilityModuleHeader_.get(), sf);
    setOptionalUiScale(unlockBankButton_.get(), sf);
    setOptionalUiScale(importBankButton_.get(), sf);
    setOptionalUiScale(exportBankButton_.get(), sf);

    for (auto& button : selectBankButtons_)
        setOptionalUiScale(button.get(), sf);
}

void BankUtilityPanel::resized()
{
    layoutContentRows(uiScale_);
    applyChildUiScales(uiScale_);
}

void BankUtilityPanel::applyNormalLookToActionButtons()
{
    setOptionalLook(unlockBankButton_.get(), normalBankLook_);
    setOptionalLook(importBankButton_.get(), normalBankLook_);
    setOptionalLook(exportBankButton_.get(), normalBankLook_);

    for (auto& button : selectBankButtons_)
        setOptionalLook(button.get(), normalBankLook_);
}

void BankUtilityPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    normalBankLook_ = TSS::buttonLookFromSkin(skin);

    if (bankUtilityModuleHeader_)
        bankUtilityModuleHeader_->setLook(TSS::moduleHeaderLookFromSkin(skin));

    if (bankSelectorLabel_)
        bankSelectorLabel_->setLook(TSS::labelLookFromSkin(skin));

    applyNormalLookToActionButtons();
    refreshSelectedBankHighlight();
}

void BankUtilityPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;
    repaint();
}

void BankUtilityPanel::setupModuleHeader(TSS::ISkin& skin, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    bankUtilityModuleHeader_ = std::make_unique<TSS::ModuleHeader>(
        dims_.moduleHeader.patchManagerTitleBandWidth,
        dims_.moduleHeader.height,
        TSS::moduleHeaderLookFromSkin(skin),
        TSS::ModuleHeader::ColourVariant::Blue,
        widgetFactory.getGroupDisplayName(moduleId),
        dims_.moduleHeader);
    addAndMakeVisible(*bankUtilityModuleHeader_);
}

void BankUtilityPanel::setupBankSelectorLabel(TSS::ISkin& skin)
{
    bankSelectorLabel_ = std::make_unique<TSS::Label>(
        dims_.bankSelectorLabel.patchManagerSelectBankWidth,
        dims_.bankSelectorLabel.height,
        TSS::labelLookFromSkin(skin),
        PluginDisplayNames::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kBankSelector,
        TSS::LabelStyle::Centered);
    addAndMakeVisible(*bankSelectorLabel_);
}

void BankUtilityPanel::setupUnlockBankButton(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    unlockBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerUnlockBankWidth,
        dims_.buttons.height,
        TSS::buttonLookFromSkin(skin),
        widgetFactory.getStandaloneWidgetDisplayName(
            PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kUnlockBank).value_or(""));
    unlockBankButton_->onClick = [this]
    {
        if (bankUtilityGrayed_)
            return;

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kUnlockBank,
            juce::Time::getCurrentTime().toMilliseconds(),
            nullptr);
        TSS::GrayedControlHelper::setFooterInfoMessage(
            apvts_,
            PluginDisplayNames::PatchManagerSection::BankUtilityModule::kUnlockBankFooterMessage);
    };
    addAndMakeVisible(*unlockBankButton_);
}

void BankUtilityPanel::setupSelectBankButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    const auto makeBankAction = [this](const char* propertyId)
    {
        return [this, propertyId]
        {
            if (bankUtilityGrayed_)
                return;

            apvts_.state.setProperty(propertyId,
                                    juce::Time::getCurrentTime().toMilliseconds(),
                                    nullptr);
        };
    };

    const auto look = TSS::buttonLookFromSkin(skin);
    const auto createBankButton = [&](int bankIndex)
    {
        const auto* widgetId = kSelectBankIds[bankIndex];
        auto& button = selectBankButtons_[static_cast<size_t>(bankIndex)];
        button = std::make_unique<TSS::Button>(
            dims_.buttons.patchManagerBankSelectWidth,
            dims_.buttons.height,
            look,
            widgetFactory.getStandaloneWidgetDisplayName(widgetId).value_or(""));
        button->onClick = makeBankAction(widgetId);
        addAndMakeVisible(*button);
    };

    for (int i = 0; i < 5; ++i)
        createBankButton(i);

    setupUnlockBankButton(skin, widgetFactory);

    for (int i = 5; i < kBankCount; ++i)
        createBankButton(i);
}

void BankUtilityPanel::setupImportExportButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    // IMPORT/EXPORT stay usable on Matrix-6/6R (and on M1000 ROM banks for EXPORT) even while
    // the bank row / UNLOCK are grayed — do not gate their onClick on bankUtilityGrayed_.
    importBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerImportBankWidth,
        dims_.buttons.height,
        TSS::buttonLookFromSkin(skin),
        widgetFactory.getStandaloneWidgetDisplayName(PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kImportBank).value_or(""));
    importBankButton_->onClick = [this]
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kImportBank,
            juce::Time::getCurrentTime().toMilliseconds(),
            nullptr);
    };
    addAndMakeVisible(*importBankButton_);

    exportBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerExportBankWidth,
        dims_.buttons.height,
        TSS::buttonLookFromSkin(skin),
        widgetFactory.getStandaloneWidgetDisplayName(PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kExportBank).value_or(""));
    exportBankButton_->onClick = [this]
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets::kExportBank,
            juce::Time::getCurrentTime().toMilliseconds(),
            nullptr);
    };
    addAndMakeVisible(*exportBankButton_);
}
