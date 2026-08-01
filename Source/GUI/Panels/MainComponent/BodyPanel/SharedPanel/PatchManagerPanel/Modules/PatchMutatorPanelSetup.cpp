// Extracted from PatchMutatorPanel.cpp for modular maintenance.
// Widget construction for amount / random / history rows.

#include "PatchMutatorPanel.h"
#include "PatchMutatorPanelInternal.h"

#include "GUI/Factories/WidgetFactory.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/HierarchicalComboBox.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/Slider.h"
#include "GUI/Widgets/Toggle.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

using namespace PatchMutatorPanelInternal;

void PatchMutatorPanel::setupModuleHeader(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    moduleHeader_ = std::make_unique<TSS::ModuleHeader>(
        dims_.moduleHeader.patchManagerTitleBandWidth,
        dims_.moduleHeader.height,
        TSS::moduleHeaderLookFromSkin(skin),
        TSS::ModuleHeader::ColourVariant::Blue,
        widgetFactory.getGroupDisplayName(PluginIDs::PatchManagerSection::PatchMutatorModule::kGroupId),
        dims_.moduleHeader);
    addAndMakeVisible(*moduleHeader_);
}

std::unique_ptr<TSS::Toggle> PatchMutatorPanel::makeRecipeToggle(TSS::ISkin& skin,
                                                                const char* displayName,
                                                                const char* widgetId)
{
    auto toggle = std::make_unique<TSS::Toggle>(
        dims_.toggles.patchMutatorWidth,
        dims_.toggles.height,
        TSS::toggleLookFromSkin(skin),
        displayName);
    connectToggleToApvts(toggle.get(), widgetId);
    addAndMakeVisible(*toggle);
    return toggle;
}

std::unique_ptr<TSS::Slider> PatchMutatorPanel::makePercentRecipeSlider(TSS::ISkin& skin,
                                                                       const char* widgetId,
                                                                       double fallbackDefault)
{
    const auto* desc = findMutatorIntDescriptor(widgetId);
    auto slider = std::make_unique<TSS::Slider>(
        dims_.sliders.patchMutatorWidth,
        dims_.sliders.standardHeight,
        TSS::sliderLookFromSkin(skin),
        TSS::SliderConfig{
            desc != nullptr ? static_cast<double>(desc->minValue) : 1.0,
            desc != nullptr ? static_cast<double>(desc->maxValue) : 100.0,
            desc != nullptr ? static_cast<double>(desc->defaultValue) : fallbackDefault,
            1.0,
            PluginDisplayNames::Units::kPercent,
            {},
            {},
            {}});

    auto* rawSlider = slider.get();
    slider->onValueChange = [this, rawSlider, widgetId]
    {
        if (recipeHydrating_)
            return;

        apvts_.state.setProperty(widgetId, static_cast<int>(rawSlider->getValue()), nullptr);
    };
    addAndMakeVisible(*slider);
    return slider;
}

void PatchMutatorPanel::setupAmountLine(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    amountLabel_ = std::make_unique<TSS::Label>(
        dims_.labels.patchMutatorWidth,
        dims_.labels.height,
        TSS::labelLookFromSkin(skin),
        MutatorDisplayNames::kAmount);
    addAndMakeVisible(*amountLabel_);

    amountSlider_ = makePercentRecipeSlider(skin, MutatorWidgets::kAmount, 50.0);

    mutateButton_ = widgetFactory.createStandaloneButton(
        MutatorWidgets::kMutate, skin, dims_.buttons.height);
    connectButtonToApvts(mutateButton_.get(), MutatorWidgets::kMutate);
    addAndMakeVisible(*mutateButton_);

    dco1Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableDco1, MutatorWidgets::kEnableDco1);
    dco2Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableDco2, MutatorWidgets::kEnableDco2);
    vcfVcaToggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableVcfVca, MutatorWidgets::kEnableVcfVca);
    fmTrackToggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableFmTrack, MutatorWidgets::kEnableFmTrack);
    rampPortamentoToggle_ = makeRecipeToggle(skin,
                                             MutatorDisplayNames::kEnableRampPortamento,
                                             MutatorWidgets::kEnableRampPortamento);
}

void PatchMutatorPanel::setupRandomLine(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    randomLabel_ = std::make_unique<TSS::Label>(
        dims_.labels.patchMutatorWidth,
        dims_.labels.height,
        TSS::labelLookFromSkin(skin),
        MutatorDisplayNames::kRandom);
    addAndMakeVisible(*randomLabel_);

    randomSlider_ = makePercentRecipeSlider(skin, MutatorWidgets::kRandom, 25.0);

    retryButton_ = widgetFactory.createStandaloneButton(
        MutatorWidgets::kRetry, skin, dims_.buttons.height);
    connectButtonToApvts(retryButton_.get(), MutatorWidgets::kRetry);
    addAndMakeVisible(*retryButton_);

    env1Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableEnvelope1, MutatorWidgets::kEnableEnvelope1);
    env2Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableEnvelope2, MutatorWidgets::kEnableEnvelope2);
    env3Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableEnvelope3, MutatorWidgets::kEnableEnvelope3);
    lfo1Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableLfo1, MutatorWidgets::kEnableLfo1);
    lfo2Toggle_ = makeRecipeToggle(skin, MutatorDisplayNames::kEnableLfo2, MutatorWidgets::kEnableLfo2);
}

void PatchMutatorPanel::wireHistoryComboBox(TSS::ISkin& skin)
{
    historyComboBox_ = std::make_unique<TSS::HierarchicalComboBox>(
        dims_.comboBoxes.patchMutatorHistoryWidth,
        dims_.comboBoxes.standardHeight,
        TSS::comboBoxLookFromSkin(skin));
    historyComboBox_->setPopupMenuLook(TSS::popupMenuLookFromSkin(skin));
    historyComboBox_->setPopupVerticalPlacement(TSS::PopupVerticalPlacement::Above);
    historyComboBox_->setTextWhenNothingSelected(MutatorDisplayNames::kEmptyHistorySentinel);
    historyComboBox_->onBeforeShowPopup = [this]
    {
        refreshHistoryComboBox();
    };
    historyComboBox_->onChange = [this]
    {
        handleHistoryComboSelectionChange();
    };
    addAndMakeVisible(*historyComboBox_);
}

void PatchMutatorPanel::setupHistoryActionButtons(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    const auto makeActionButton = [this, &skin, &widgetFactory](const char* widgetId)
    {
        auto button = widgetFactory.createStandaloneButton(widgetId, skin, dims_.buttons.height);
        connectButtonToApvts(button.get(), widgetId);
        addAndMakeVisible(*button);
        return button;
    };

    historyPreviousButton_ = makeActionButton(MutatorWidgets::kHistoryPrevious);
    historyNextButton_ = makeActionButton(MutatorWidgets::kHistoryNext);
    compareButton_ = makeActionButton(MutatorWidgets::kCompare);
    deleteButton_ = makeActionButton(MutatorWidgets::kDelete);
    clearButton_ = makeActionButton(MutatorWidgets::kClear);
    exportButton_ = makeActionButton(MutatorWidgets::kExport);

    enableMatrixModToggle_ = makeRecipeToggle(skin,
                                              MutatorDisplayNames::kEnableMatrixMod,
                                              MutatorWidgets::kEnableMatrixMod);
}

void PatchMutatorPanel::setupHistoryLine(TSS::ISkin& skin, WidgetFactory& widgetFactory)
{
    historyLabel_ = std::make_unique<TSS::Label>(
        dims_.labels.patchMutatorWidth,
        dims_.labels.height,
        TSS::labelLookFromSkin(skin),
        MutatorDisplayNames::kHistory);
    addAndMakeVisible(*historyLabel_);

    wireHistoryComboBox(skin);
    setupHistoryActionButtons(skin, widgetFactory);
}
