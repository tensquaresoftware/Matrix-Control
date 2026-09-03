// Extracted from SettingsPanel.cpp for modular maintenance.
// Widget construction and initial combo wiring for the settings dialog.

#include "SettingsPanel.h"

#include "Core/Audio/HardwareLatency.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Skins/ISkin.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

std::unique_ptr<TSS::Label> SettingsPanel::makeLabel(TSS::ISkin& skin, int width, const juce::String& text)
{
    return std::make_unique<TSS::Label>(width, kControlHeight_, TSS::labelLookFromSkin(skin), text);
}

std::unique_ptr<TSS::ComboBox> SettingsPanel::makeCombo(TSS::ISkin& skin, int width)
{
    return std::make_unique<TSS::ComboBox>(width, kControlHeight_, TSS::comboBoxLookFromSkin(skin));
}

std::unique_ptr<TSS::HorizontalSeparator> SettingsPanel::makeSeparator(TSS::ISkin& skin)
{
    return std::make_unique<TSS::HorizontalSeparator>(
        kContentWidth_, kSeparatorHeight_, TSS::horizontalSeparatorLookFromSkin(skin));
}

void SettingsPanel::setupPatchSection(TSS::ISkin& skin)
{
    const int comboWidth = kContentWidth_ - kLabelWidth_ - kGap_;

    patchSectionLabel_ = makeLabel(skin, kContentWidth_, PluginDisplayNames::Settings::kPatchSection);
    patchSectionSeparator_ = makeSeparator(skin);
    matrix1000PatchesLabel_ =
        makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kMatrix1000PatchesLabel);
    matrix1000PatchesCombo_ = makeCombo(skin, comboWidth);
    computerPatchesLabel_ =
        makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kComputerPatchesLabel);
    computerPatchesCombo_ = makeCombo(skin, comboWidth);
    unsavedStateLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kUnsavedStateLabel);
    unsavedStateCombo_ = makeCombo(skin, comboWidth);

    addAndMakeVisible(*patchSectionLabel_);
    addAndMakeVisible(*patchSectionSeparator_);
    addAndMakeVisible(*matrix1000PatchesLabel_);
    addAndMakeVisible(*matrix1000PatchesCombo_);
    addAndMakeVisible(*computerPatchesLabel_);
    addAndMakeVisible(*computerPatchesCombo_);
    addAndMakeVisible(*unsavedStateLabel_);
    addAndMakeVisible(*unsavedStateCombo_);
}

void SettingsPanel::setupPatchMutatorSection(TSS::ISkin& skin)
{
    const int comboWidth = kContentWidth_ - kLabelWidth_ - kGap_;
    const int placeholderWidth = comboWidth;

    patchMutatorSectionLabel_ =
        makeLabel(skin, kContentWidth_, PluginDisplayNames::Settings::kPatchMutatorSection);
    patchMutatorSectionSeparator_ = makeSeparator(skin);
    deleteWarningLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kDeleteWarningLabel);
    deleteWarningCombo_ = makeCombo(skin, comboWidth);
    defragHistoryLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kDefragHistoryLabel);
    defragHistoryPlaceholder_ =
        makeLabel(skin, placeholderWidth, PluginDisplayNames::Settings::kComingSoon);

    addAndMakeVisible(*patchMutatorSectionLabel_);
    addAndMakeVisible(*patchMutatorSectionSeparator_);
    addAndMakeVisible(*deleteWarningLabel_);
    addAndMakeVisible(*deleteWarningCombo_);
    addAndMakeVisible(*defragHistoryLabel_);
    addAndMakeVisible(*defragHistoryPlaceholder_);
}

void SettingsPanel::setupMasterSection(TSS::ISkin& skin)
{
    masterSectionLabel_ = makeLabel(skin, kContentWidth_, PluginDisplayNames::Settings::kMasterSection);
    masterSectionSeparator_ = makeSeparator(skin);
    hardwareLatencyLabel_ =
        makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kHardwareLatencyLabel);
    hardwareLatencySlider_ = std::make_unique<TSS::Slider>(
        kSliderWidth_,
        kControlHeight_,
        TSS::sliderLookFromSkin(skin),
        TSS::SliderConfig{
            Core::HardwareLatency::kMinMs,
            Core::HardwareLatency::kMaxMs,
            Core::HardwareLatency::kMinMs,
            Core::HardwareLatency::kStepMs,
            "ms",
            {},
            {},
            {}});
    masterOperationsLabel_ =
        makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kMasterOperationsLabel);
    masterOperationsPlaceholder_ = makeLabel(
        skin, kContentWidth_ - kLabelWidth_ - kGap_, PluginDisplayNames::Settings::kComingSoon);

    addAndMakeVisible(*masterSectionLabel_);
    addAndMakeVisible(*masterSectionSeparator_);
    addAndMakeVisible(*hardwareLatencyLabel_);
    addAndMakeVisible(*hardwareLatencySlider_);
    addAndMakeVisible(*masterOperationsLabel_);
    addAndMakeVisible(*masterOperationsPlaceholder_);
}

void SettingsPanel::populateComboItems()
{
    using namespace PluginIDs::Settings::Matrix1000PatchesNamesMode;
    matrix1000PatchesCombo_->addItem(PluginDisplayNames::Settings::kDisplayMusicalNames,
                                     kDisplayMusicalNames);
    matrix1000PatchesCombo_->addItem(PluginDisplayNames::Settings::kDisplayHardwareNames,
                                     kDisplayHardwareNames);

    using namespace PluginIDs::Settings::ComputerPatchesNamesPolicy;
    computerPatchesCombo_->addItem(PluginDisplayNames::Settings::kDisplaySysexNames, kDisplaySysexNames);
    computerPatchesCombo_->addItem(PluginDisplayNames::Settings::kDisplayFileNames, kDisplayFileNames);
    computerPatchesCombo_->addItem(PluginDisplayNames::Settings::kAskOncePerLoad, kAskOncePerLoad);

    using namespace PluginIDs::Settings::UnsavedStatePolicy;
    unsavedStateCombo_->addItem(PluginDisplayNames::Settings::kAlwaysWarn, kAlwaysWarn);
    unsavedStateCombo_->addItem(PluginDisplayNames::Settings::kNeverWarn, kNeverWarn);

    deleteWarningCombo_->addItem(PluginDisplayNames::Settings::kAlwaysWarn,
                                 PluginIDs::Settings::DeleteWarningPolicy::kAlwaysWarn);
    deleteWarningCombo_->addItem(PluginDisplayNames::Settings::kNeverWarn,
                                 PluginIDs::Settings::DeleteWarningPolicy::kNeverWarn);
}

void SettingsPanel::applyComboPopupLooks(TSS::ISkin& skin)
{
    const auto popupLook = TSS::popupMenuLookFromSkin(skin);
    matrix1000PatchesCombo_->setPopupMenuLook(popupLook);
    computerPatchesCombo_->setPopupMenuLook(popupLook);
    unsavedStateCombo_->setPopupMenuLook(popupLook);
    deleteWarningCombo_->setPopupMenuLook(popupLook);
}

void SettingsPanel::applyChildLooks(TSS::ISkin& skin)
{
    const auto labelLook = TSS::labelLookFromSkin(skin);
    const auto separatorLook = TSS::horizontalSeparatorLookFromSkin(skin);
    const auto comboLook = TSS::comboBoxLookFromSkin(skin);

    patchSectionLabel_->setLook(labelLook);
    patchSectionSeparator_->setLook(separatorLook);
    matrix1000PatchesLabel_->setLook(labelLook);
    matrix1000PatchesCombo_->setLook(comboLook);
    computerPatchesLabel_->setLook(labelLook);
    computerPatchesCombo_->setLook(comboLook);
    unsavedStateLabel_->setLook(labelLook);
    unsavedStateCombo_->setLook(comboLook);

    patchMutatorSectionLabel_->setLook(labelLook);
    patchMutatorSectionSeparator_->setLook(separatorLook);
    deleteWarningLabel_->setLook(labelLook);
    deleteWarningCombo_->setLook(comboLook);
    defragHistoryLabel_->setLook(labelLook);
    defragHistoryPlaceholder_->setLook(labelLook);

    masterSectionLabel_->setLook(labelLook);
    masterSectionSeparator_->setLook(separatorLook);
    hardwareLatencyLabel_->setLook(labelLook);
    hardwareLatencySlider_->setLook(TSS::sliderLookFromSkin(skin));
    masterOperationsLabel_->setLook(labelLook);
    masterOperationsPlaceholder_->setLook(labelLook);

    applyComboPopupLooks(skin);
}
