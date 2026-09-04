#include "BankUtilityPanel.h"

#include "GUI/Helpers/ClipboardFeedbackButtonBinding.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Factories/WidgetFactory.h"
#include "Shared/Definitions/PluginIDs.h"

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

    void fireBankAction(juce::AudioProcessorValueTreeState& apvts, const char* propertyId)
    {
        apvts.state.setProperty(propertyId, juce::Time::getCurrentTime().toMilliseconds(), nullptr);
    }
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

void BankUtilityPanel::setupSelectBankButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    const auto makeBankAction = [this](const char* propertyId)
    {
        return [this, propertyId]
        {
            if (bankUtilityGrayed_)
                return;

            fireBankAction(apvts_, propertyId);
        };
    };

    const auto look = TSS::buttonLookFromSkin(skin);

    for (int i = 0; i < kBankCount; ++i)
    {
        const auto* widgetId = kSelectBankIds[i];
        auto& button = selectBankButtons_[static_cast<size_t>(i)];
        button = std::make_unique<TSS::Button>(
            dims_.buttons.patchManagerBankSelectWidth,
            dims_.buttons.height,
            look,
            widgetFactory.getStandaloneWidgetDisplayName(widgetId).value_or(""));
        button->onClick = makeBankAction(widgetId);
        addAndMakeVisible(*button);
    }
}

void BankUtilityPanel::setupUtilityButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    using namespace PluginIDs::PatchManagerSection::BankUtilityModule::StandaloneWidgets;

    const auto look = TSS::buttonLookFromSkin(skin);

    copyBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerCopyBankWidth,
        dims_.buttons.height,
        look,
        widgetFactory.getStandaloneWidgetDisplayName(kCopyBank).value_or(""));
    copyBankButton_->onClick = [this]
    {
        if (bankUtilityGrayed_)
            return;

        fireBankAction(apvts_, kCopyBank);
    };
    addAndMakeVisible(*copyBankButton_);

    pasteBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerPasteBankWidth,
        dims_.buttons.height,
        look,
        widgetFactory.getStandaloneWidgetDisplayName(kPasteBank).value_or(""));
    pasteBankButton_->onClick = [this]
    {
        if (bankUtilityGrayed_)
            return;

        fireBankAction(apvts_, kPasteBank);
    };
    addAndMakeVisible(*pasteBankButton_);

    exportBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerExportBankWidth,
        dims_.buttons.height,
        look,
        widgetFactory.getStandaloneWidgetDisplayName(kExportBank).value_or(""));
    exportBankButton_->onClick = [this] { fireBankAction(apvts_, kExportBank); };
    addAndMakeVisible(*exportBankButton_);

    importBankButton_ = std::make_unique<TSS::Button>(
        dims_.buttons.patchManagerImportBankWidth,
        dims_.buttons.height,
        look,
        widgetFactory.getStandaloneWidgetDisplayName(kImportBank).value_or(""));
    importBankButton_->onClick = [this] { fireBankAction(apvts_, kImportBank); };
    addAndMakeVisible(*importBankButton_);
}

void BankUtilityPanel::setupClipboardFeedbackBindings()
{
    if (copyBankButton_ == nullptr || pasteBankButton_ == nullptr)
        return;

    copyFeedbackBinding_ = std::make_unique<TSS::ClipboardFeedbackButtonBinding>(
        apvts_,
        *copyBankButton_,
        PluginIDs::ClipboardFeedback::kBankUtilityCopy,
        true);
    pasteFeedbackBinding_ = std::make_unique<TSS::ClipboardFeedbackButtonBinding>(
        apvts_,
        *pasteBankButton_,
        PluginIDs::ClipboardFeedback::kBankUtilityPaste,
        false);
}
