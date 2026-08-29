// Extracted from PatchMutatorPanel.cpp for modular maintenance.
// resized / layout strips, UI scale fan-out, and skin propagation.

#include "PatchMutatorPanel.h"
#include "PatchMutatorPanelInternal.h"

#include "../PatchManagerEqualWidthStrip.h"
#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Widgets/Button.h"
#include "GUI/Widgets/ComboBox.h"
#include "GUI/Widgets/HierarchicalComboBox.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/Toggle.h"

using namespace PatchMutatorPanelInternal;

namespace
{
    struct HistoryLineMetrics
    {
        int labelW = 0;
        int labelH = 0;
        int comboBoxW = 0;
        int comboBoxH = 0;
        int rowH = 0;
        int buttonH = 0;
        int navW = 0;
        int compareW = 0;
        int deleteW = 0;
        int clearW = 0;
        int exportW = 0;
        int toggleW = 0;
        int toggleH = 0;
        int interGap = 0;
        int labelY = 0;
        int comboBoxY = 0;

        static HistoryLineMetrics make(const PatchMutatorPanelDimensions& dims, float sf, int y)
        {
            HistoryLineMetrics m;
            m.labelW = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.labels.patchMutatorWidth), sf);
            m.labelH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.labels.height), sf);
            m.comboBoxW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.comboBoxes.patchMutatorHistoryWidth), sf);
            m.comboBoxH = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.comboBoxes.standardHeight), sf);
            m.rowH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.layout.contentRowHeight), sf);
            m.buttonH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.buttons.height), sf);
            m.navW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.buttons.patchMutatorHistoryNavWidth), sf);
            m.compareW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.buttons.patchMutatorCompareWidth), sf);
            m.deleteW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.buttons.patchMutatorDeleteWidth), sf);
            m.clearW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.buttons.patchMutatorClearWidth), sf);
            m.exportW = TSS::ScaledLayout::scaledInt(
                static_cast<float>(dims.buttons.patchMutatorExportWidth), sf);
            m.toggleW = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.toggles.patchMutatorWidth), sf);
            m.toggleH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.toggles.height), sf);
            m.interGap = TSS::ScaledLayout::scaledInt(static_cast<float>(dims.layout.interControlGap), sf);
            m.labelY = y + (m.rowH - m.labelH) / 2;
            m.comboBoxY = y + (m.rowH - m.comboBoxH) / 2;
            return m;
        }
    };
}

void PatchMutatorPanel::resized()
{
    const float sf = uiScale_;

    const int moduleHeaderW = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.moduleHeader.patchManagerTitleBandWidth), sf);
    const int moduleHeaderH = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.moduleHeader.height), sf);

    if (auto* header = moduleHeader_.get())
        header->setBounds(0, 0, moduleHeaderW, moduleHeaderH);

    const int contentRowH = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.layout.contentRowHeight), sf);
    const int rowGap = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.layout.interControlGap), sf);
    const int rowStep = contentRowH + rowGap;

    const int row0Y = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.moduleHeader.height), sf);
    const int row1Y = row0Y + rowStep;
    const int row2Y = row1Y + rowStep;

    TSS::Toggle* modeRowToggles[] = {
        dco1Toggle_.get(), dco2Toggle_.get(), vcfVcaToggle_.get(),
        fmTrackToggle_.get(), rampPortamentoToggle_.get()
    };
    layoutRecipeRow({
        0, row0Y,
        modeLabel_.get(), modeComboBox_.get(), mutateButton_.get(),
        modeRowToggles, 5,
        dims_.buttons.patchMutatorMutateWidth
    });

    TSS::Toggle* pitchRowToggles[] = {
        env1Toggle_.get(), env2Toggle_.get(), env3Toggle_.get(),
        lfo1Toggle_.get(), lfo2Toggle_.get()
    };
    layoutRecipeRow({
        0, row1Y,
        pitchLabel_.get(), pitchComboBox_.get(), retryButton_.get(),
        pitchRowToggles, 5,
        dims_.buttons.patchMutatorRetryWidth
    });

    layoutHistoryLine(0, row2Y);
    applyUiScaleToWidgets(sf);
}

void PatchMutatorPanel::applyUiScaleToWidgets(float sf)
{
    applyUiScale(moduleHeader_, sf);
    applyUiScale(modeLabel_, sf);
    applyUiScale(modeComboBox_, sf);
    applyUiScale(mutateButton_, sf);
    applyUiScale(pitchLabel_, sf);
    applyUiScale(pitchComboBox_, sf);
    applyUiScale(retryButton_, sf);
    applyUiScale(historyLabel_, sf);
    applyUiScale(historyComboBox_, sf);
    applyUiScale(historyPreviousButton_, sf);
    applyUiScale(historyNextButton_, sf);
    applyUiScale(compareButton_, sf);
    applyUiScale(deleteButton_, sf);
    applyUiScale(clearButton_, sf);
    applyUiScale(exportButton_, sf);
    applyUiScale(dco1Toggle_, sf);
    applyUiScale(dco2Toggle_, sf);
    applyUiScale(vcfVcaToggle_, sf);
    applyUiScale(fmTrackToggle_, sf);
    applyUiScale(rampPortamentoToggle_, sf);
    applyUiScale(env1Toggle_, sf);
    applyUiScale(env2Toggle_, sf);
    applyUiScale(env3Toggle_, sf);
    applyUiScale(lfo1Toggle_, sf);
    applyUiScale(lfo2Toggle_, sf);
    applyUiScale(enableMatrixModToggle_, sf);
}

void PatchMutatorPanel::layoutRecipeRow(const RecipeRowLayoutArgs& args)
{
    const float sf = uiScale_;

    const int labelW = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.labels.patchMutatorWidth), sf);
    const int labelH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.labels.height), sf);
    const int controlW = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.comboBoxes.patchMutatorHistoryWidth), sf);
    const int controlH = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dims_.comboBoxes.standardHeight), sf);
    const int buttonW = TSS::ScaledLayout::scaledInt(static_cast<float>(args.actionButtonWidth), sf);
    const int buttonH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.buttons.height), sf);
    const int interGap = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.layout.interControlGap), sf);
    const int rowH = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.layout.contentRowHeight), sf);
    const int labelY = args.y + (rowH - labelH) / 2;
    const int controlY = args.y + (rowH - controlH) / 2;

    // Fixed-width successive strip — no float origin / step × index (U-7 Init precedent)
    int cursorX = args.x;

    if (args.label != nullptr)
        args.label->setBounds(cursorX, labelY, labelW, labelH);
    cursorX += labelW + interGap;

    if (args.control != nullptr)
        args.control->setBounds(cursorX, controlY, controlW, controlH);
    cursorX += controlW + interGap;

    if (args.button != nullptr)
        args.button->setBounds(cursorX, args.y, buttonW, buttonH);
    cursorX += buttonW + interGap;

    juce::Component* toggleControls[5] = {};
    const int toggleCount = juce::jmin(5, args.toggleCount);
    for (int i = 0; i < toggleCount; ++i)
        toggleControls[i] = args.toggles[i];

    TSS::placeEqualWidthStrip({
        .startX = cursorX,
        .y = args.y,
        .uiScale = sf,
        .designWidth = dims_.toggles.patchMutatorWidth,
        .designHeight = dims_.toggles.height,
        .designGap = dims_.layout.interControlGap,
        .controls = toggleControls,
        .count = toggleCount,
    });
}

void PatchMutatorPanel::layoutHistoryLine(int x, int y)
{
    const float sf = uiScale_;
    const auto m = HistoryLineMetrics::make(dims_, sf, y);

    // Fixed-width successive integer cursor — no float origin / step accumulation
    int cursorX = x;

    if (auto* label = historyLabel_.get())
        label->setBounds(cursorX, m.labelY, m.labelW, m.labelH);
    cursorX += m.labelW + m.interGap;

    if (auto* comboBox = historyComboBox_.get())
        comboBox->setBounds(cursorX, m.comboBoxY, m.comboBoxW, m.comboBoxH);
    cursorX += m.comboBoxW + m.interGap;

    if (auto* navPrev = historyPreviousButton_.get())
        navPrev->setBounds(cursorX, y, m.navW, m.buttonH);
    cursorX += m.navW + m.interGap;

    if (auto* navNext = historyNextButton_.get())
        navNext->setBounds(cursorX, y, m.navW, m.buttonH);
    cursorX += m.navW + m.interGap;

    if (auto* compare = compareButton_.get())
        compare->setBounds(cursorX, y, m.compareW, m.buttonH);
    cursorX += m.compareW + m.interGap;

    if (auto* del = deleteButton_.get())
        del->setBounds(cursorX, y, m.deleteW, m.buttonH);
    cursorX += m.deleteW + m.interGap;

    if (auto* clear = clearButton_.get())
        clear->setBounds(cursorX, y, m.clearW, m.buttonH);
    cursorX += m.clearW + m.interGap;

    if (auto* exportBtn = exportButton_.get())
        exportBtn->setBounds(cursorX, y, m.exportW, m.buttonH);
    cursorX += m.exportW + m.interGap;

    if (auto* toggle = enableMatrixModToggle_.get())
        toggle->setBounds(cursorX, y, m.toggleW, m.toggleH);
}

void PatchMutatorPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    propagateSkinsToControlWidgets(skin);
    propagateSkinsToToggleWidgets(skin);
}

void PatchMutatorPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;
    repaint();
}

void PatchMutatorPanel::propagateSkinsToControlWidgets(TSS::ISkin& skin)
{
    applyLook(moduleHeader_, TSS::moduleHeaderLookFromSkin(skin));
    applyLook(modeLabel_, TSS::labelLookFromSkin(skin));
    applyLook(pitchLabel_, TSS::labelLookFromSkin(skin));
    applyLook(historyLabel_, TSS::labelLookFromSkin(skin));

    const auto comboBoxLook = TSS::comboBoxLookFromSkin(skin);
    const auto popupMenuLook = TSS::popupMenuLookFromSkin(skin);

    if (modeComboBox_ != nullptr)
    {
        modeComboBox_->setLook(comboBoxLook);
        modeComboBox_->setPopupMenuLook(popupMenuLook);
    }

    if (pitchComboBox_ != nullptr)
    {
        pitchComboBox_->setLook(comboBoxLook);
        pitchComboBox_->setPopupMenuLook(popupMenuLook);
    }

    if (historyComboBox_ != nullptr)
    {
        historyComboBox_->setLook(comboBoxLook);
        historyComboBox_->setPopupMenuLook(popupMenuLook);
    }

    applyLook(mutateButton_, TSS::buttonLookFromSkin(skin));
    applyLook(retryButton_, TSS::buttonLookFromSkin(skin));
    applyLook(historyPreviousButton_, TSS::buttonLookFromSkin(skin));
    applyLook(historyNextButton_, TSS::buttonLookFromSkin(skin));
    applyLook(compareButton_, TSS::buttonLookFromSkin(skin));
    applyLook(deleteButton_, TSS::buttonLookFromSkin(skin));
    applyLook(clearButton_, TSS::buttonLookFromSkin(skin));
    applyLook(exportButton_, TSS::buttonLookFromSkin(skin));
}

void PatchMutatorPanel::propagateSkinsToToggleWidgets(TSS::ISkin& skin)
{
    const auto toggleLook = TSS::toggleLookFromSkin(skin);
    applyLook(dco1Toggle_, toggleLook);
    applyLook(dco2Toggle_, toggleLook);
    applyLook(vcfVcaToggle_, toggleLook);
    applyLook(fmTrackToggle_, toggleLook);
    applyLook(rampPortamentoToggle_, toggleLook);
    applyLook(env1Toggle_, toggleLook);
    applyLook(env2Toggle_, toggleLook);
    applyLook(env3Toggle_, toggleLook);
    applyLook(lfo1Toggle_, toggleLook);
    applyLook(lfo2Toggle_, toggleLook);
    applyLook(enableMatrixModToggle_, toggleLook);
}
