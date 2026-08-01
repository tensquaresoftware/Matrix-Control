#include "SettingsPanel.h"

#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinValues.h"

using TSS::SkinColourId;

SettingsPanel::SettingsPanel(TSS::ISkin& skin, bool isPluginMode)
    : skin_(&skin)
    , isPluginMode_(isPluginMode)
{
    setOpaque(true);

    setupGeneralSection(skin);
    setupPolicyControls(skin);
    setupPlaceholderRows(skin);
    setupMatrix1000Section(skin);
    populatePolicyComboItems();
    applyComboPopupLooks(skin);

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

void SettingsPanel::layoutSectionHeader(juce::Rectangle<int>& bounds, const SectionHeaderLayoutArgs& args)
{
    const int titleGap = juce::roundToInt(static_cast<float>(kSectionTitleGap_) * uiScale_);

    auto titleRow = bounds.removeFromTop(args.controlHeight);
    args.title->setBounds(titleRow);
    args.title->setUiScale(uiScale_);
    bounds.removeFromTop(titleGap);

    auto sepRow = bounds.removeFromTop(args.separatorHeight);
    args.separator->setBounds(sepRow);
    args.separator->setUiScale(uiScale_);
    bounds.removeFromTop(args.rowGap);
}

void SettingsPanel::layoutLabeledControlRow(juce::Rectangle<int>& bounds,
                                            const RowLayoutMetrics& metrics,
                                            const LabeledControlRowArgs& args)
{
    auto row = bounds.removeFromTop(metrics.controlHeight);
    args.label->setBounds(row.removeFromLeft(metrics.labelWidth));
    args.label->setUiScale(uiScale_);
    row.removeFromLeft(metrics.gap);
    args.control->setBounds(row.removeFromLeft(args.controlWidth).withHeight(metrics.controlHeight));
    if (auto* slider = dynamic_cast<TSS::Slider*>(args.control))
        slider->setUiScale(uiScale_);
    bounds.removeFromTop(metrics.rowGap);
}

void SettingsPanel::layoutPlaceholderRow(juce::Rectangle<int>& bounds,
                                         const RowLayoutMetrics& metrics,
                                         TSS::Label& label,
                                         TSS::Label& placeholder)
{
    auto row = bounds.removeFromTop(metrics.controlHeight);
    label.setBounds(row.removeFromLeft(metrics.labelWidth));
    label.setUiScale(uiScale_);
    row.removeFromLeft(metrics.gap);
    placeholder.setBounds(row);
    placeholder.setUiScale(uiScale_);
    bounds.removeFromTop(metrics.rowGap);
}

void SettingsPanel::layoutGeneralSection(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics)
{
    layoutSectionHeader(bounds,
                        SectionHeaderLayoutArgs{ generalSectionLabel_.get(),
                                                 generalSectionSeparator_.get(),
                                                 metrics.controlHeight,
                                                 metrics.separatorHeight,
                                                 metrics.rowGap });

    if (isPluginMode_)
    {
        layoutLabeledControlRow(bounds,
                                metrics,
                                LabeledControlRowArgs{ hardwareLatencyLabel_.get(),
                                                       hardwareLatencySlider_.get(),
                                                       metrics.sliderWidth });
        bounds.removeFromTop(metrics.rowGap);
    }

    layoutPlaceholderRow(bounds, metrics, *masterOpsLabel_, *masterOpsPlaceholder_);
    layoutLabeledControlRow(bounds,
                            metrics,
                            LabeledControlRowArgs{ policiesLabel_.get(),
                                                   nameReconciliationPolicyCombo_.get(),
                                                   metrics.comboWidth });
    layoutLabeledControlRow(bounds,
                            metrics,
                            LabeledControlRowArgs{ unsavedEditWarningLabel_.get(),
                                                   unsavedEditWarningPolicyCombo_.get(),
                                                   metrics.comboWidth });
    layoutLabeledControlRow(bounds,
                            metrics,
                            LabeledControlRowArgs{ mutatorDeleteWarningLabel_.get(),
                                                   mutatorDeleteWarningPolicyCombo_.get(),
                                                   metrics.comboWidth });
    layoutPlaceholderRow(bounds, metrics, *defragLabel_, *defragPlaceholder_);
    layoutPlaceholderRow(bounds, metrics, *loggingLabel_, *loggingPlaceholder_);
}

void SettingsPanel::layoutMatrix1000Section(juce::Rectangle<int>& bounds, const RowLayoutMetrics& metrics)
{
    layoutSectionHeader(bounds,
                        SectionHeaderLayoutArgs{ matrix1000SectionLabel_.get(),
                                                 matrix1000SectionSeparator_.get(),
                                                 metrics.controlHeight,
                                                 metrics.separatorHeight,
                                                 metrics.rowGap });
    layoutLabeledControlRow(bounds,
                            metrics,
                            LabeledControlRowArgs{ patchNameDisplayLabel_.get(),
                                                   patchNameDisplayModeCombo_.get(),
                                                   metrics.comboWidth });
}

void SettingsPanel::layoutContent(juce::Rectangle<int> bounds)
{
    RowLayoutMetrics metrics;
    metrics.rowGap = juce::roundToInt(static_cast<float>(kRowGap_) * uiScale_);
    metrics.gap = juce::roundToInt(static_cast<float>(kGap_) * uiScale_);
    metrics.labelWidth = juce::roundToInt(static_cast<float>(kLabelWidth_) * uiScale_);
    metrics.sliderWidth = juce::roundToInt(static_cast<float>(kSliderWidth_) * uiScale_);
    metrics.controlHeight = juce::roundToInt(static_cast<float>(kControlHeight_) * uiScale_);
    metrics.separatorHeight = juce::roundToInt(static_cast<float>(kSeparatorHeight_) * uiScale_);
    metrics.comboWidth = bounds.getWidth() - metrics.labelWidth - metrics.gap;

    layoutGeneralSection(bounds, metrics);
    bounds.removeFromTop(metrics.rowGap);
    layoutMatrix1000Section(bounds, metrics);
}

void SettingsPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    applyChildLooks(skin);
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

    hardwareLatencyLabel_->setVisible(showPluginControls);
    hardwareLatencySlider_->setVisible(showPluginControls);
}
