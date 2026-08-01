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

void SettingsPanel::setupGeneralSection(TSS::ISkin& skin)
{
    generalSectionLabel_ = makeLabel(skin, kContentWidth_, PluginDisplayNames::Settings::kGeneralSection);
    generalSectionSeparator_ = makeSeparator(skin);
    hardwareLatencyLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kHardwareLatencyLabel);
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
    masterOpsLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kMasterOperationsSection);
    masterOpsPlaceholder_ = makeLabel(
        skin, kContentWidth_ - kLabelWidth_ - kGap_, PluginDisplayNames::Settings::kComingSoon);

    addAndMakeVisible(*generalSectionLabel_);
    addAndMakeVisible(*generalSectionSeparator_);
    addAndMakeVisible(*hardwareLatencyLabel_);
    addAndMakeVisible(*hardwareLatencySlider_);
    addAndMakeVisible(*masterOpsLabel_);
    addAndMakeVisible(*masterOpsPlaceholder_);
}

void SettingsPanel::setupPolicyControls(TSS::ISkin& skin)
{
    const int comboWidth = kContentWidth_ - kLabelWidth_ - kGap_;

    policiesLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kPoliciesSection);
    nameReconciliationPolicyCombo_ = makeCombo(skin, comboWidth);
    unsavedEditWarningLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kUnsavedEditWarningLabel);
    unsavedEditWarningPolicyCombo_ = makeCombo(skin, comboWidth);
    mutatorDeleteWarningLabel_ = makeLabel(
        skin, kLabelWidth_, PluginDisplayNames::Settings::kMutatorDeleteWarningLabel);
    mutatorDeleteWarningPolicyCombo_ = makeCombo(skin, comboWidth);

    addAndMakeVisible(*policiesLabel_);
    addAndMakeVisible(*nameReconciliationPolicyCombo_);
    addAndMakeVisible(*unsavedEditWarningLabel_);
    addAndMakeVisible(*unsavedEditWarningPolicyCombo_);
    addAndMakeVisible(*mutatorDeleteWarningLabel_);
    addAndMakeVisible(*mutatorDeleteWarningPolicyCombo_);
}

void SettingsPanel::setupPlaceholderRows(TSS::ISkin& skin)
{
    const int placeholderWidth = kContentWidth_ - kLabelWidth_ - kGap_;

    defragLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kDefragSection);
    defragPlaceholder_ = makeLabel(skin, placeholderWidth, PluginDisplayNames::Settings::kComingSoon);
    loggingLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kLoggingSection);
    loggingPlaceholder_ = makeLabel(skin, placeholderWidth, PluginDisplayNames::Settings::kComingSoon);

    addAndMakeVisible(*defragLabel_);
    addAndMakeVisible(*defragPlaceholder_);
    addAndMakeVisible(*loggingLabel_);
    addAndMakeVisible(*loggingPlaceholder_);
}

void SettingsPanel::setupMatrix1000Section(TSS::ISkin& skin)
{
    matrix1000SectionLabel_ = makeLabel(skin, kContentWidth_, PluginDisplayNames::Settings::kMatrix1000Section);
    matrix1000SectionSeparator_ = makeSeparator(skin);
    patchNameDisplayLabel_ = makeLabel(skin, kLabelWidth_, PluginDisplayNames::Settings::kPatchNameDisplayLabel);
    patchNameDisplayModeCombo_ = makeCombo(skin, kContentWidth_ - kLabelWidth_ - kGap_);

    addAndMakeVisible(*matrix1000SectionLabel_);
    addAndMakeVisible(*matrix1000SectionSeparator_);
    addAndMakeVisible(*patchNameDisplayLabel_);
    addAndMakeVisible(*patchNameDisplayModeCombo_);
}

void SettingsPanel::populatePolicyComboItems()
{
    using namespace PluginIDs::Settings::NameReconciliationPolicy;
    nameReconciliationPolicyCombo_->addItem(PluginDisplayNames::Settings::kNameReconciliationPreferInternal,
                                            kPreferInternal);
    nameReconciliationPolicyCombo_->addItem(PluginDisplayNames::Settings::kNameReconciliationPreferFilename,
                                            kPreferFilename);
    nameReconciliationPolicyCombo_->addItem(PluginDisplayNames::Settings::kNameReconciliationAskOnce,
                                            kAskOncePerLoad);

    using namespace PluginIDs::Settings::UnsavedEditWarningPolicy;
    unsavedEditWarningPolicyCombo_->addItem(PluginDisplayNames::Settings::kUnsavedEditWarnAlways, kWarnAlways);
    unsavedEditWarningPolicyCombo_->addItem(PluginDisplayNames::Settings::kUnsavedEditNeverWarn, kNeverWarn);

    mutatorDeleteWarningPolicyCombo_->addItem(
        PluginDisplayNames::Settings::kMutatorDeleteWarnAlways,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kWarnAlways);
    mutatorDeleteWarningPolicyCombo_->addItem(
        PluginDisplayNames::Settings::kMutatorDeleteNeverWarn,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kNeverWarn);

    using namespace PluginIDs::Settings::PatchNameDisplayMode;
    patchNameDisplayModeCombo_->addItem(PluginDisplayNames::Settings::kPatchNameDisplayMusicalNames,
                                        kMusicalNames);
    patchNameDisplayModeCombo_->addItem(PluginDisplayNames::Settings::kPatchNameDisplayHardwareNames,
                                        kHardwareNames);
}

void SettingsPanel::applyComboPopupLooks(TSS::ISkin& skin)
{
    const auto popupLook = TSS::popupMenuLookFromSkin(skin);
    nameReconciliationPolicyCombo_->setPopupMenuLook(popupLook);
    unsavedEditWarningPolicyCombo_->setPopupMenuLook(popupLook);
    mutatorDeleteWarningPolicyCombo_->setPopupMenuLook(popupLook);
    patchNameDisplayModeCombo_->setPopupMenuLook(popupLook);
}

void SettingsPanel::applyChildLooks(TSS::ISkin& skin)
{
    const auto labelLook = TSS::labelLookFromSkin(skin);
    const auto separatorLook = TSS::horizontalSeparatorLookFromSkin(skin);
    const auto comboLook = TSS::comboBoxLookFromSkin(skin);

    generalSectionLabel_->setLook(labelLook);
    generalSectionSeparator_->setLook(separatorLook);
    hardwareLatencyLabel_->setLook(labelLook);
    hardwareLatencySlider_->setLook(TSS::sliderLookFromSkin(skin));
    masterOpsLabel_->setLook(labelLook);
    masterOpsPlaceholder_->setLook(labelLook);
    policiesLabel_->setLook(labelLook);
    nameReconciliationPolicyCombo_->setLook(comboLook);
    unsavedEditWarningLabel_->setLook(labelLook);
    unsavedEditWarningPolicyCombo_->setLook(comboLook);
    mutatorDeleteWarningLabel_->setLook(labelLook);
    mutatorDeleteWarningPolicyCombo_->setLook(comboLook);
    defragLabel_->setLook(labelLook);
    defragPlaceholder_->setLook(labelLook);
    loggingLabel_->setLook(labelLook);
    loggingPlaceholder_->setLook(labelLook);
    matrix1000SectionLabel_->setLook(labelLook);
    matrix1000SectionSeparator_->setLook(separatorLook);
    patchNameDisplayLabel_->setLook(labelLook);
    patchNameDisplayModeCombo_->setLook(comboLook);

    applyComboPopupLooks(skin);
}
