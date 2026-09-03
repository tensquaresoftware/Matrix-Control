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

**Approach:** Reorganize into PATCH / PATCH MUTATOR / MASTER with action verbs in combo choices (DISPLAY…, ALWAYS/NEVER WARN), set a compact dialog width (panel design 292 → ~300 px on screen with chrome; columns 120 + 140), scale combo fonts like the main GUI, hide LOGGING, and rename Settings identifiers to match the on-screen copy while keeping persisted property string keys stable.

## Suggested Review Order

**On-screen copy and IDs**

- Section/row/choice strings match the agreed mockup.
  [`PluginDisplayNames.h:90`](../../Source/Shared/Definitions/PluginDisplayNames.h#L90)

- C++ Settings IDs renamed; persistence string values unchanged.
  [`PluginIDs.h:20`](../../Source/Shared/Definitions/PluginIDs.h#L20)

**Modal layout**

- PATCH / PATCH MUTATOR / MASTER structure, compact panel (~300 px on screen), combo `setUiScale`.
  [`SettingsPanel.h:20`](../../Source/GUI/Settings/SettingsPanel.h#L20)

- Widget construction and combo item population.
  [`SettingsPanelSetup.cpp:28`](../../Source/GUI/Settings/SettingsPanelSetup.cpp#L28)

**Persistence wiring**

- Restore/wire with clamped policy IDs for every Settings combo.
  [`PluginEditorSettings.cpp:45`](../../Source/GUI/PluginEditorSettings.cpp#L45)

## Review Findings

- [x] [Review][Decision] Keep Settings width at 292 (120+140) or reopen layout for truncation? — Resolved: keep current compact width (~300 px measured on screen with window chrome). Spec Approach updated; no layout code change.

- [x] [Review][Patch] Restore ASK ONCE PER LOAD alert above Matrix-Control after OPEN folder FileChooser [Source/GUI/PluginEditorPatchBindings.cpp:48]
- [x] [Review][Patch] Refresh PATCH NAME immediately when COMPUTER PATCHES toggles DISPLAY SYSEX NAMES ↔ DISPLAY FILE NAMES [Source/GUI/PluginEditorSettings.cpp:111]
- [x] [Review][Patch] Add Core test for Settings-driven computer-patch name re-reconcile (and cover MATRIX-1000 Settings refresh if cheap) [Tests/Unit/]

- [x] [Review][Defer] Fixed Settings height does not adapt to plugin HARDWARE LATENCY row vs standalone [Source/GUI/Settings/SettingsPanel.h:22] — deferred, pre-existing
