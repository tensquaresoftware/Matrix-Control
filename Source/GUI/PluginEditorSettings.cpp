// Extracted from PluginEditor.cpp for modular maintenance.
// Settings panel restore-from-state + control wiring.

#include "PluginEditor.h"
#include "PluginEditorInternal.h"

#include "Core/Services/PatchNameDisplayMode.h"
#include "GUI/Settings/SettingsPanel.h"
#include "Shared/Definitions/PluginIDs.h"

void PluginEditor::restoreSettingsPanelFromState(SettingsPanel& panel)
{
    if (!pluginProcessor.isStandalone())
        panel.getHardwareLatencySlider().setValue(pluginProcessor.getHardwareLatencyMs(), juce::dontSendNotification);

    const int policy = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kComputerPatchesNameReconciliationPolicy,
        PluginIDs::Settings::NameReconciliationPolicy::kDefault));
    panel.getNameReconciliationPolicyCombo().setSelectedId(policy, juce::dontSendNotification);

    const int unsavedPolicyRaw = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kUnsavedEditWarningPolicy,
        PluginIDs::Settings::UnsavedEditWarningPolicy::kDefault));
    const int unsavedPolicy =
        (unsavedPolicyRaw == PluginIDs::Settings::UnsavedEditWarningPolicy::kWarnAlways
         || unsavedPolicyRaw == PluginIDs::Settings::UnsavedEditWarningPolicy::kNeverWarn)
            ? unsavedPolicyRaw
            : PluginIDs::Settings::UnsavedEditWarningPolicy::kDefault;
    panel.getUnsavedEditWarningPolicyCombo().setSelectedId(unsavedPolicy, juce::dontSendNotification);

    const int mutatorDeletePolicyRaw = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kMutatorDeleteWarningPolicy,
        PluginIDs::Settings::MutatorDeleteWarningPolicy::kDefault));
    const int mutatorDeletePolicy =
        (mutatorDeletePolicyRaw == PluginIDs::Settings::MutatorDeleteWarningPolicy::kWarnAlways
         || mutatorDeletePolicyRaw == PluginIDs::Settings::MutatorDeleteWarningPolicy::kNeverWarn)
            ? mutatorDeletePolicyRaw
            : PluginIDs::Settings::MutatorDeleteWarningPolicy::kDefault;
    if (mutatorDeletePolicy != mutatorDeletePolicyRaw)
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kMutatorDeleteWarningPolicy,
            mutatorDeletePolicy,
            nullptr);
    }
    panel.getMutatorDeleteWarningPolicyCombo().setSelectedId(mutatorDeletePolicy, juce::dontSendNotification);

    const int patchNameDisplayRaw = static_cast<int>(pluginProcessor.getApvts().state.getProperty(
        PluginIDs::Settings::kPatchNameDisplayMode,
        PluginIDs::Settings::PatchNameDisplayMode::kDefault));
    const int patchNameDisplayMode = Core::PatchNameDisplay::normalize(patchNameDisplayRaw);
    if (patchNameDisplayMode != patchNameDisplayRaw)
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kPatchNameDisplayMode,
            patchNameDisplayMode,
            nullptr);
    }
    panel.getPatchNameDisplayModeCombo().setSelectedId(patchNameDisplayMode, juce::dontSendNotification);
}

void PluginEditor::wireSettingsPanel(SettingsPanel& panel)
{
    panel.getHardwareLatencySlider().onValueChange = [this, &panel]
    {
        pluginProcessor.setHardwareLatencyMs(static_cast<float>(panel.getHardwareLatencySlider().getValue()));
    };

    panel.getNameReconciliationPolicyCombo().onChange = [this, &panel]
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kComputerPatchesNameReconciliationPolicy,
            panel.getNameReconciliationPolicyCombo().getSelectedId(),
            nullptr);
    };

    panel.getUnsavedEditWarningPolicyCombo().onChange = [this, &panel]
    {
        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kUnsavedEditWarningPolicy,
            panel.getUnsavedEditWarningPolicyCombo().getSelectedId(),
            nullptr);
    };

    panel.getMutatorDeleteWarningPolicyCombo().onChange = [this, &panel]
    {
        using namespace PluginIDs::Settings::MutatorDeleteWarningPolicy;

        const int selectedId = panel.getMutatorDeleteWarningPolicyCombo().getSelectedId();
        if (selectedId != kWarnAlways && selectedId != kNeverWarn)
            return;

        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kMutatorDeleteWarningPolicy,
            selectedId,
            nullptr);
    };

    panel.getPatchNameDisplayModeCombo().onChange = [this, &panel]
    {
        using namespace PluginIDs::Settings::PatchNameDisplayMode;

        const int selectedId = panel.getPatchNameDisplayModeCombo().getSelectedId();
        if (selectedId != kMusicalNames && selectedId != kHardwareNames)
            return;

        pluginProcessor.getApvts().state.setProperty(
            PluginIDs::Settings::kPatchNameDisplayMode,
            selectedId,
            nullptr);
        pluginProcessor.refreshPatchNameDisplayForSettingsMode();
    };
}
