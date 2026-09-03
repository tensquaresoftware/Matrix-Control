---
title: 'Settings modal sections and naming'
type: 'feature'
created: '2026-09-03'
status: 'done'
route: 'one-shot'
context: []
---

# Settings modal sections and naming

## Intent

**Problem:** Settings mixed unclear section names (GENERAL, POLICIES), mismatched combo fonts, and labels that did not say what the options actually do for patch name display and warnings.

**Approach:** Reorganize into PATCH / PATCH MUTATOR / MASTER with action verbs in combo choices (DISPLAY…, ALWAYS/NEVER WARN), widen the dialog, scale combo fonts like the main GUI, hide LOGGING, and rename Settings identifiers to match the on-screen copy while keeping persisted property string keys stable.

## Suggested Review Order

**On-screen copy and IDs**

- Section/row/choice strings match the agreed mockup.
  [`PluginDisplayNames.h:90`](../../Source/Shared/Definitions/PluginDisplayNames.h#L90)

- C++ Settings IDs renamed; persistence string values unchanged.
  [`PluginIDs.h:20`](../../Source/Shared/Definitions/PluginIDs.h#L20)

**Modal layout**

- PATCH / PATCH MUTATOR / MASTER structure, wider panel, combo `setUiScale`.
  [`SettingsPanel.h:20`](../../Source/GUI/Settings/SettingsPanel.h#L20)

- Widget construction and combo item population.
  [`SettingsPanelSetup.cpp:28`](../../Source/GUI/Settings/SettingsPanelSetup.cpp#L28)

**Persistence wiring**

- Restore/wire with clamped policy IDs for every Settings combo.
  [`PluginEditorSettings.cpp:45`](../../Source/GUI/PluginEditorSettings.cpp#L45)
