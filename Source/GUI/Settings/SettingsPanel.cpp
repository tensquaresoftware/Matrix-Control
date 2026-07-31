#include "SettingsPanel.h"

#include "Core/Audio/HardwareLatency.h"
#include "GUI/Looks/LookBuilders.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinValues.h"
#include "Shared/Definitions/PluginDisplayNames.h"
#include "Shared/Definitions/PluginIDs.h"

using TSS::SkinColourId;

SettingsPanel::SettingsPanel(TSS::ISkin& skin, bool isPluginMode)
    : skin_(&skin)
    , isPluginMode_(isPluginMode)
    , generalSectionLabel_(kContentWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                           PluginDisplayNames::Settings::kGeneralSection)
    , generalSectionSeparator_(kContentWidth_, kSeparatorHeight_,
                               TSS::horizontalSeparatorLookFromSkin(skin))
    , hardwareLatencyLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                            PluginDisplayNames::Settings::kHardwareLatencyLabel)
    , hardwareLatencySlider_(kSliderWidth_, kControlHeight_, TSS::sliderLookFromSkin(skin),
                             TSS::SliderConfig{
                                 Core::HardwareLatency::kMinMs,
                                 Core::HardwareLatency::kMaxMs,
                                 Core::HardwareLatency::kMinMs,
                                 Core::HardwareLatency::kStepMs,
                                 "ms",
                                 {},
                                 {},
                                 {}})
    , masterOpsLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                      PluginDisplayNames::Settings::kMasterOperationsSection)
    , masterOpsPlaceholder_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                            TSS::labelLookFromSkin(skin), PluginDisplayNames::Settings::kComingSoon)
    , policiesLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                     PluginDisplayNames::Settings::kPoliciesSection)
    , nameReconciliationPolicyCombo_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                                     TSS::comboBoxLookFromSkin(skin))
    , unsavedEditWarningLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                               PluginDisplayNames::Settings::kUnsavedEditWarningLabel)
    , unsavedEditWarningPolicyCombo_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                                     TSS::comboBoxLookFromSkin(skin))
    , mutatorDeleteWarningLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                                 PluginDisplayNames::Settings::kMutatorDeleteWarningLabel)
    , mutatorDeleteWarningPolicyCombo_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                                       TSS::comboBoxLookFromSkin(skin))
    , defragLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                   PluginDisplayNames::Settings::kDefragSection)
    , defragPlaceholder_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                         TSS::labelLookFromSkin(skin), PluginDisplayNames::Settings::kComingSoon)
    , loggingLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                    PluginDisplayNames::Settings::kLoggingSection)
    , loggingPlaceholder_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                          TSS::labelLookFromSkin(skin), PluginDisplayNames::Settings::kComingSoon)
    , matrix1000SectionLabel_(kContentWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                              PluginDisplayNames::Settings::kMatrix1000Section)
    , matrix1000SectionSeparator_(kContentWidth_, kSeparatorHeight_,
                                  TSS::horizontalSeparatorLookFromSkin(skin))
    , patchNameDisplayLabel_(kLabelWidth_, kControlHeight_, TSS::labelLookFromSkin(skin),
                             PluginDisplayNames::Settings::kPatchNameDisplayLabel)
    , patchNameDisplayModeCombo_(kContentWidth_ - kLabelWidth_ - kGap_, kControlHeight_,
                                 TSS::comboBoxLookFromSkin(skin))
{
    setOpaque(true);

    addAndMakeVisible(generalSectionLabel_);
    addAndMakeVisible(generalSectionSeparator_);
    addAndMakeVisible(hardwareLatencyLabel_);
    addAndMakeVisible(hardwareLatencySlider_);
    addAndMakeVisible(masterOpsLabel_);
    addAndMakeVisible(masterOpsPlaceholder_);
    addAndMakeVisible(policiesLabel_);
    addAndMakeVisible(nameReconciliationPolicyCombo_);
    addAndMakeVisible(unsavedEditWarningLabel_);
    addAndMakeVisible(unsavedEditWarningPolicyCombo_);
    addAndMakeVisible(mutatorDeleteWarningLabel_);
    addAndMakeVisible(mutatorDeleteWarningPolicyCombo_);
    addAndMakeVisible(defragLabel_);
    addAndMakeVisible(defragPlaceholder_);
    addAndMakeVisible(loggingLabel_);
    addAndMakeVisible(loggingPlaceholder_);
    addAndMakeVisible(matrix1000SectionLabel_);
    addAndMakeVisible(matrix1000SectionSeparator_);
    addAndMakeVisible(patchNameDisplayLabel_);
    addAndMakeVisible(patchNameDisplayModeCombo_);

    using namespace PluginIDs::Settings::NameReconciliationPolicy;
    nameReconciliationPolicyCombo_.addItem(PluginDisplayNames::Settings::kNameReconciliationPreferInternal,
                                         kPreferInternal);
    nameReconciliationPolicyCombo_.addItem(PluginDisplayNames::Settings::kNameReconciliationPreferFilename,
                                         kPreferFilename);
    nameReconciliationPolicyCombo_.addItem(PluginDisplayNames::Settings::kNameReconciliationAskOnce,
                                         kAskOncePerLoad);

    using namespace PluginIDs::Settings::UnsavedEditWarningPolicy;
    unsavedEditWarningPolicyCombo_.addItem(PluginDisplayNames::Settings::kUnsavedEditWarnAlways, kWarnAlways);
    unsavedEditWarningPolicyCombo_.addItem(PluginDisplayNames::Settings::kUnsavedEditNeverWarn, kNeverWarn);

    mutatorDeleteWarningPolicyCombo_.addItem(
        PluginDisplayNames::Settings::kMutatorDeleteWarnAlways,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kWarnAlways);
    mutatorDeleteWarningPolicyCombo_.addItem(
        PluginDisplayNames::Settings::kMutatorDeleteNeverWarn,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kNeverWarn);

    using namespace PluginIDs::Settings::PatchNameDisplayMode;
    patchNameDisplayModeCombo_.addItem(PluginDisplayNames::Settings::kPatchNameDisplayMusicalNames,
                                       kMusicalNames);
    patchNameDisplayModeCombo_.addItem(PluginDisplayNames::Settings::kPatchNameDisplayHardwareNames,
                                       kHardwareNames);

    const auto popupLook = TSS::popupMenuLookFromSkin(skin);
    nameReconciliationPolicyCombo_.setPopupMenuLook(popupLook);
    unsavedEditWarningPolicyCombo_.setPopupMenuLook(popupLook);
    mutatorDeleteWarningPolicyCombo_.setPopupMenuLook(popupLook);
    patchNameDisplayModeCombo_.setPopupMenuLook(popupLook);

    setPluginMode(isPluginMode);
}

void SettingsPanel::paint(juce::Graphics& g)
{
    g.fillAll(skin_->getColour(SkinColourId::kBodyPanelBackground));
}

void SettingsPanel::resized()
{
    const float sf = uiScale_;
    const int padding = juce::roundToInt(static_cast<float>(kPadding_) * sf);
    layoutContent(getLocalBounds().reduced(padding));
}

void SettingsPanel::layoutSectionHeader(juce::Rectangle<int>& bounds,
                                        TSS::Label& title,
                                        TSS::HorizontalSeparator& separator,
                                        int controlHeight,
                                        int separatorHeight,
                                        int rowGap)
{
    const int titleGap = juce::roundToInt(static_cast<float>(kSectionTitleGap_) * uiScale_);

    auto titleRow = bounds.removeFromTop(controlHeight);
    title.setBounds(titleRow);
    title.setUiScale(uiScale_);
    bounds.removeFromTop(titleGap);

    auto sepRow = bounds.removeFromTop(separatorHeight);
    separator.setBounds(sepRow);
    separator.setUiScale(uiScale_);
    bounds.removeFromTop(rowGap);
}

void SettingsPanel::layoutContent(juce::Rectangle<int> bounds)
{
    const int rowGap = juce::roundToInt(static_cast<float>(kRowGap_) * uiScale_);
    const int gap = juce::roundToInt(static_cast<float>(kGap_) * uiScale_);
    const int labelWidth = juce::roundToInt(static_cast<float>(kLabelWidth_) * uiScale_);
    const int sliderWidth = juce::roundToInt(static_cast<float>(kSliderWidth_) * uiScale_);
    const int controlHeight = juce::roundToInt(static_cast<float>(kControlHeight_) * uiScale_);
    const int separatorHeight = juce::roundToInt(static_cast<float>(kSeparatorHeight_) * uiScale_);
    const int comboWidth = bounds.getWidth() - labelWidth - gap;

    auto layoutRow = [&](TSS::Label& label, juce::Component& control, int controlWidth)
    {
        auto row = bounds.removeFromTop(controlHeight);
        label.setBounds(row.removeFromLeft(labelWidth));
        label.setUiScale(uiScale_);
        row.removeFromLeft(gap);
        control.setBounds(row.removeFromLeft(controlWidth).withHeight(controlHeight));
        if (auto* slider = dynamic_cast<TSS::Slider*>(&control))
            slider->setUiScale(uiScale_);
        bounds.removeFromTop(rowGap);
    };

    auto layoutPlaceholderRow = [&](TSS::Label& label, TSS::Label& placeholder)
    {
        auto row = bounds.removeFromTop(controlHeight);
        label.setBounds(row.removeFromLeft(labelWidth));
        label.setUiScale(uiScale_);
        row.removeFromLeft(gap);
        placeholder.setBounds(row);
        placeholder.setUiScale(uiScale_);
        bounds.removeFromTop(rowGap);
    };

    layoutSectionHeader(bounds, generalSectionLabel_, generalSectionSeparator_,
                        controlHeight, separatorHeight, rowGap);

    if (isPluginMode_)
    {
        layoutRow(hardwareLatencyLabel_, hardwareLatencySlider_, sliderWidth);
        bounds.removeFromTop(rowGap);
    }

    layoutPlaceholderRow(masterOpsLabel_, masterOpsPlaceholder_);
    layoutRow(policiesLabel_, nameReconciliationPolicyCombo_, comboWidth);
    layoutRow(unsavedEditWarningLabel_, unsavedEditWarningPolicyCombo_, comboWidth);
    layoutRow(mutatorDeleteWarningLabel_, mutatorDeleteWarningPolicyCombo_, comboWidth);
    layoutPlaceholderRow(defragLabel_, defragPlaceholder_);
    layoutPlaceholderRow(loggingLabel_, loggingPlaceholder_);

    bounds.removeFromTop(rowGap);

    layoutSectionHeader(bounds, matrix1000SectionLabel_, matrix1000SectionSeparator_,
                        controlHeight, separatorHeight, rowGap);
    layoutRow(patchNameDisplayLabel_, patchNameDisplayModeCombo_, comboWidth);
}

void SettingsPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;

    const auto labelLook = TSS::labelLookFromSkin(skin);
    const auto separatorLook = TSS::horizontalSeparatorLookFromSkin(skin);
    const auto comboLook = TSS::comboBoxLookFromSkin(skin);
    const auto popupLook = TSS::popupMenuLookFromSkin(skin);

    generalSectionLabel_.setLook(labelLook);
    generalSectionSeparator_.setLook(separatorLook);
    hardwareLatencyLabel_.setLook(labelLook);
    masterOpsLabel_.setLook(labelLook);
    masterOpsPlaceholder_.setLook(labelLook);
    policiesLabel_.setLook(labelLook);
    nameReconciliationPolicyCombo_.setLook(comboLook);
    nameReconciliationPolicyCombo_.setPopupMenuLook(popupLook);
    unsavedEditWarningLabel_.setLook(labelLook);
    unsavedEditWarningPolicyCombo_.setLook(comboLook);
    unsavedEditWarningPolicyCombo_.setPopupMenuLook(popupLook);
    mutatorDeleteWarningLabel_.setLook(labelLook);
    mutatorDeleteWarningPolicyCombo_.setLook(comboLook);
    mutatorDeleteWarningPolicyCombo_.setPopupMenuLook(popupLook);
    defragLabel_.setLook(labelLook);
    defragPlaceholder_.setLook(labelLook);
    loggingLabel_.setLook(labelLook);
    loggingPlaceholder_.setLook(labelLook);
    matrix1000SectionLabel_.setLook(labelLook);
    matrix1000SectionSeparator_.setLook(separatorLook);
    patchNameDisplayLabel_.setLook(labelLook);
    patchNameDisplayModeCombo_.setLook(comboLook);
    patchNameDisplayModeCombo_.setPopupMenuLook(popupLook);

    hardwareLatencySlider_.setLook(TSS::sliderLookFromSkin(skin));

    repaint();
}

void SettingsPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;
    resized();
    repaint();
}

void SettingsPanel::setPluginMode(bool isPluginMode)
{
    isPluginMode_ = isPluginMode;
    updateModeSpecificVisibility();
    resized();
}

void SettingsPanel::updateModeSpecificVisibility()
{
    const bool showPluginControls = isPluginMode_;

    hardwareLatencyLabel_.setVisible(showPluginControls);
    hardwareLatencySlider_.setVisible(showPluginControls);
}
