---
organization: Ten Square Software
project: Matrix-Control
title: Spec — Patch Name Display Settings
author: BMad Agent
type: feature
created: '2026-07-31'
status: done
baseline_commit: 'dd160306926e5fbb47b779fb66c2d07ffe386e23'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/implementation-artifacts/passation-patch-name-display-ram-rom-overlay.md'
updated: '2026-07-31'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Matrix-1000 firmware often rewrites SysEx name bytes to `BNK…`, so Patch Name can show either honest machine labels or musical names (factory ROM + editor overlay). Users need an explicit Settings choice, and Settings needs clear GENERAL / MATRIX-1000 sections.

**Approach:** Add always-visible **Patch Name Display** (Hardware Names | Musical Names, default Musical). Stack Settings as GENERAL then MATRIX-1000 (title + horizontal rule + options). Display + editability follow the setting; bank/Mutator **exports stay musical** (no Patch Name Export in this pass).

## Boundaries & Constraints

**Always:**
- Default Musical Names = today’s display resolution (ROM factory → overlay on BNK/empty → else dump → else `Bx-Pyy`).
- Hardware Names display: skip factory + overlay; keep usable dump name (including `BNK…`); `Bx-Pyy` only if name truly empty.
- Keep populating overlay on IMPORT/STORE even in Hardware Names (display must not use it).
- Hardware Names + device-memory origin + banked device (Matrix-1000 limits): disable Patch Name double-click edit. Computer `.syx` and Matrix-6/6R remain editable (existing Compare/ROM gates unchanged).
- Persist like other Settings APVTS int policies. Option always editable without synth connected.
- English UI strings only in source.

**Ask First:**
- Adding Patch Name Export or making exports follow Display.
- Manual synth-type selector or tabs / two-column Settings.

**Never:**
- Remove overlay store; invent MATRIX-6/6R Settings content; global AffineTransform UI scale; GUI includes from Core.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Musical display (default) | Device dump BNK + overlay / ROM factory | Patch Name shows musical resolution as today | N/A |
| Hardware display | Same dump | Patch Name shows dump/`BNK…` (or `Bx-Pyy` if empty) | N/A |
| Export bank | Either Display mode | Export still applies musical resolution before write | N/A |
| Hardware + device M-1000 | Double-click Patch Name | Edit disabled | N/A |
| Hardware + `.syx` origin | Double-click | Edit allowed (other gates apply) | N/A |
| Mode toggle live | Device origin loaded | Name re-resolves; editable refreshes | If no raw dump cached, synthesize `BNKx: yy` when banked |
| 6/6R | Either mode | No notable name/edit regression | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Settings/SettingsPanel.{h,cpp}` / `SettingsWindow.*` — section stack + combo; slight size bump
- `Source/Shared/Definitions/PluginDisplayNames.h` / `PluginIDs.h` — labels, IDs, enum
- `Source/Core/Services/PatchNameDisplayMode.h` — read defaulted policy from APVTS int
- `Source/Core/Services/PatchNameResolver.{h,cpp}` — pure musical vs hardware resolve (unit-tested)
- `Source/Core/Services/PatchFileNameSanitizer.*` — `formatOberheimBankPlaceholderName` for live Hardware refresh fallback
- `Source/Core/Actions/PatchManagerActionHandler.*` — display resolve uses mode; export always musical; cache raw dump name; reapply helper
- `Source/Core/PluginProcessor.*` — init property; `canEditPatchName`; refresh on mode change
- `Source/GUI/PluginEditor.*` — restore/wire combo; call refresh
- `Source/GUI/Panels/.../PatchNameDisplayPanel.*` — track Settings property for editable sync
- `Tests/Unit/PatchNameResolverTests.cpp` — musical vs hardware matrix
- `CMakeLists.txt` — register new sources/tests

## Tasks & Acceptance

**Execution:**
- [x] `PluginIDs.h` / `PluginDisplayNames.h` — add `settingsPatchNameDisplayMode`, enum Musical=1/Hardware=2 default Musical, GENERAL/MATRIX-1000/combo strings
- [x] `PatchNameDisplayMode.h` + `PatchNameResolver.*` + sanitizer placeholder formatter — shared resolve + tests
- [x] `PatchManagerActionHandler.*` — wire display vs export; raw-name cache; `reapplyDisplayedPatchName`
- [x] `SettingsPanel.*` — GENERAL / MATRIX-1000 headers + separators; combo; bump design size
- [x] `PluginProcessor.*` / `PluginEditor.*` / `PatchNameDisplayPanel.*` — persist, edit gate, live refresh
- [x] `CMakeLists.txt` + unit tests — register and cover resolver
- [x] Build preset `macos-debug-arm64` — compile green

**Acceptance Criteria:**
- Given Settings open, when viewing the panel, then GENERAL then MATRIX-1000 appear with titles and horizontal separators.
- Given MATRIX-1000 section, when choosing Patch Name Display, then Hardware Names | Musical Names work, default Musical, persisted, always enabled offline.
- Given Musical Names, when loading device patches, then Patch Name matches prior musical behavior.
- Given Hardware Names on Matrix-1000 device memory, when viewing Patch Name, then no overlay/factory substitution for `BNK…`, and double-click edit is off.
- Given `.syx` or Matrix-6/6R, when Hardware Names, then edit remains allowed under existing gates.
- Given bank export, when either Display mode, then exported names still use musical resolution.

## Spec Change Log

## Design Notes

Display mode is UI/edit only. Exports keep calling musical resolve so Bank Utility / stems stay readable. Overlay continues to be written on IMPORT/STORE so switching back to Musical restores names.

Suggested review order: IDs/strings → resolver + tests → handler display/export split → Settings layout → processor edit gate + editor wire → live mode toggle.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: success
- `ctest --preset macos-debug-arm64 -R PatchNameResolver` (or project unit-test target) -- expected: pass

**Manual checks:**
- Settings sections + combo persistence across reopen
- M-1000 RAM with overlay: toggle Musical ↔ Hardware on Patch Name and editability
- Export one bank slot in Hardware Display — file name still musical

## Suggested Review Order

**Display resolve (musical vs hardware)**

- Pure resolve rules — entry point for the dual mode.
  [`PatchNameResolver.cpp:8`](../../Source/Core/Services/PatchNameResolver.cpp#L8)

- Display path reads Settings; export path always musical.
  [`PatchManagerActionHandler.cpp:167`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L167)

- Unit coverage for both modes.
  [`PatchNameResolverTests.cpp:10`](../../Tests/Unit/PatchNameResolverTests.cpp#L10)

**Live toggle + overlay safety**

- Safe reapply: cache keyed, skip pending/missing dump, preserve dirty.
  [`PatchManagerActionHandler.cpp:223`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L223)

- Never remember BNK placeholders into overlay.
  [`PatchManagerActionHandler.cpp:160`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L160)

- Hardware Names blocks device-memory rename on banked synths.
  [`PluginProcessor.cpp:2105`](../../Source/Core/PluginProcessor.cpp#L2105)

- Mode change refreshes name + marks host state dirty.
  [`PluginProcessor.cpp:2135`](../../Source/Core/PluginProcessor.cpp#L2135)

**Settings UI**

- GENERAL then MATRIX-1000 section headers + separators.
  [`SettingsPanel.cpp:180`](../../Source/GUI/Settings/SettingsPanel.cpp#L180)

- Persist combo and trigger refresh.
  [`PluginEditor.cpp:1486`](../../Source/GUI/PluginEditor.cpp#L1486)

- IDs and display strings.
  [`PluginIDs.h:26`](../../Source/Shared/Definitions/PluginIDs.h#L26)
