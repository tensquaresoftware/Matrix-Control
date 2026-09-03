---
title: 'Computer Patches SAVE AS — Matrix naming homogeneity'
type: 'feature'
created: '2026-09-04'
status: 'done'
baseline_commit: '782f6a3076beaad14b8cc9fad2a268c4db1be41e'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/4-4-save-with-filename-injection.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-rom-factory-names-export-folders.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiate">

## Intent

**Problem:** SAVE AS can write OS-tolerant filenames (lowercase, accents) while Patch Name stays Matrix-constrained, and Save no longer writes the chosen stem into bytes 0–7 — so Computer Patches files reopen with mismatched names depending on Settings.

**Approach:** Treat SAVE AS filenames as Matrix names: validate before write (refuse + re-open dialog, no silent character drop), always write an uppercase Matrix stem with `.syx`, inject that stem into bytes 0–7 on SAVE and SAVE AS, keep the COMPUTER PATCHES Settings policy for foreign files, and leave the combo showing on-disk names unchanged.

## Boundaries & Constraints

**Always:**
- Chosen SAVE AS stem must be Matrix-legal after uppercase: only `PatchFileNameSanitizer::isAllowedMatrixChar` characters, length 1–8. Case-only differences are accepted and normalized to uppercase on disk.
- On invalid stem (accents, exotic punctuation, length > 8, empty after trim): show an English error alert, then immediately re-open the native Save dialog on the same folder with the rejected stem pre-filled; cancel ends the flow. Do not write a rewritten name silently.
- Successful SAVE and SAVE AS: write `{MATRIX_STEM}.syx` in the chosen folder; `PatchModel::setName(MATRIX_STEM)` before encode so bytes 0–7 match; sync Patch Name APVTS from buffer after write.
- Suggested SAVE AS stem comes from current Patch Name via `sanitizeFileStem` (Matrix), not the looser OS-only sanitizer.
- Computer Patches combo continues to display `sortedValidFileNames` stems exactly as on disk (no display-only uppercase).
- Keep Settings COMPUTER PATCHES name policy (DISPLAY SYSEX / FILE / ASK ONCE) — still needed for third-party or Finder-renamed files.
- After dialogs/alerts, raise the plugin UI before the next modal (`raiseUiBeforeModalDialog`).
- This intentionally renegotiates `spec-rom-factory-names-export-folders` “do not inject filename into bytes on Save” for Computer Patches SAVE/SAVE AS only.

**Ask First:**
- Changing device/ROM Save As origin semantics (today Save As from device does not switch to computer-file origin / known `.syx` path).
- Replacing the native Save dialog with an in-plugin Matrix name editor.

**Never:**
- Silent accent/punctuation stripping that produces a different stem than typed (e.g. `réso` → write `RSO.syx` without refusal).
- Display-only uppercase in the Computer Patches combo while the file remains differently cased on disk.
- Removing the COMPUTER PATCHES Settings name policy in this pass.
- Changing Mutator export folder naming, ROM factory table display, or Hardware vs Musical Patch Name Display.
- Custom non-native JUCE file browser as the primary Save As UI.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| SAVE AS Matrix stem | User confirms `TEST` | Writes `TEST.syx`; bytes 0–7 = `TEST`; Patch Name = `TEST`; combo shows `TEST` after rescan | N/A |
| SAVE AS lowercase only | User confirms `test` | Accepts; writes `TEST.syx` + injects `TEST` | N/A |
| SAVE AS accent | User confirms `réso` | No write; alert; Save dialog reopens with `réso` pre-filled | Cancel → no write |
| SAVE AS exotic ROM name | Model `*'CANOPY`, user confirms `FOO` | Writes `FOO.syx`; bytes become `FOO` (injection restored) | If user confirms `*'CANOPY` → refuse + reopen |
| SAVE overwrite | Selected `WARM.syx`, edited params | Overwrites file; injects `WARM` into bytes 0–7 | Existing save failure footer |
| Combo after Finder rename | Disk file `test.syx` | Combo shows `test` (disk truth) | N/A |
| Settings unchanged | Load foreign `.syx` with name ≠ stem | Existing COMPUTER PATCHES policy still applies | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/PluginEditorPatchBindings.cpp` — `setPatchSaveFilePickerBinding`: native `FileChooser`; add validate → alert → `raiseUiBeforeModalDialog` → re-open loop; pre-fill rejected stem
- `Source/GUI/PluginEditorAlerts.cpp` / `PluginEditorInternal.h` — reuse `raiseUiBeforeModalDialog`
- `Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp` — `handleSavePatchAs`, `saveCurrentPatchToFile`, `resolveSuggestedSaveStem` (`sanitizeOsFileStem` → `sanitizeFileStem`); restore inject via `PatchModel::setName` before `savePatchSysExFile`
- `Source/Core/Services/PatchFileNameSanitizer.{h,cpp}` — add pure helper e.g. `isExactMatrixFileStem` / normalize-for-save (case fold OK; refuse otherwise); reuse `sanitizeFileStem`, `isAllowedMatrixChar`
- `Source/Shared/Definitions/PluginDisplayNames.h` — English alert title/body for invalid SAVE AS name (Matrix charset + max 8)
- `Tests/Unit/PatchManagerActionHandlerSaveTests.cpp` — replace `save_preservesPatchEditName` with inject expectations; cover uppercase normalize + inject on SAVE/SAVE AS
- `Tests/Unit/PatchFileNameSanitizerTests.cpp` — validation helper cases (`réso`, `test`→ok normalize, `*'CANOPY` refuse, length)
- Read-only: `PatchFileService` write API; `ComputerPatchesPanel` combo (no display transform); `PatchFileNameReconciler` / Settings policy (keep); `spec-rom-factory-names-export-folders.md` (prior non-inject — superseded here for Save only)

## Tasks & Acceptance

**Execution:**
- [x] `PatchFileNameSanitizer.*` + unit tests — Matrix SAVE stem validate/normalize (case-only OK; accents/exotic/length fail) — shared Core rule for picker + inject
- [x] `PluginDisplayNames.h` + `PluginEditorPatchBindings.cpp` — invalid-name alert + Save As re-open loop with raise-before-modal — no silent rewrite
- [x] `PatchManagerActionHandlerComputerLoadSave.cpp` — Matrix suggested stem; inject sanitized stem into bytes on SAVE/SAVE AS; write `{STEM}.syx` — homogeneity file ↔ Patch Name
- [x] `PatchManagerActionHandlerSaveTests.cpp` — flip preserve-exotic save test to inject; assert written payload name bytes — lock renegotiation

**Acceptance Criteria:**
- Given SAVE AS with a Matrix-legal stem (any case), when confirmed, then disk file is uppercase `.syx`, bytes 0–7 and Patch Name match that stem.
- Given SAVE AS with accents or other illegal Matrix characters, when confirmed, then no file is written, an alert explains the rule, and the Save dialog returns with the stem available to replace.
- Given SAVE of a selected Computer Patches file, when it succeeds, then bytes 0–7 match the Matrix-sanitized uppercase file stem.
- Given a Finder-renamed lowercase `.syx`, when the folder is scanned, then the combo shows the on-disk casing unchanged.
- Given Settings COMPUTER PATCHES policies, when loading mismatched third-party files, then existing reconciliation behavior still works.

## Spec Change Log

## Design Notes

Prior `spec-rom-factory-names-export-folders` removed Save injection so exotic factory spellings (e.g. `*'CANOPY`) survived a round-trip Save. Product now prefers Computer Patches files to be Matrix-homogeneous: choosing Save As is an explicit commit to an A–Z/0–9/space/-/_ name (max 8). ROM table display and Mutator export naming stay unchanged. Native dialogs cannot filter keystrokes; refusal + re-open is the approved substitute. Text selection inside the re-opened OS dialog is best-effort (OS default).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — expected: success
- `ctest --preset macos-debug-arm64 -R 'PatchFileNameSanitizer|PatchManagerActionHandlerSave'` — expected: pass
- `python3 Scripts/quality/lint_touched.py` — expected: pass on touched C++

**Manual checks:**
- SAVE AS `test` → `TEST.syx`, Patch Name `TEST`
- SAVE AS `réso` → alert → dialog returns; cancel leaves disk unchanged
- SAVE AS from ROM patch → file + Patch Name use chosen Matrix stem
- OPEN folder after Finder rename to lowercase → combo shows lowercase stem

## Suggested Review Order

**Validation (Matrix stem)**

- Accept case-only fold; refuse accents / exotic / overlength with empty return.
  [`PatchFileNameSanitizer.cpp:35`](../../Source/Core/Services/PatchFileNameSanitizer.cpp#L35)

**SAVE AS refuse + re-open**

- Native dialog loop: illegal stem → alert → reopen with rejected stem prefilled.
  [`PluginEditorPatchBindings.cpp:297`](../../Source/GUI/PluginEditorPatchBindings.cpp#L297)

**Write boundary + inject**

- Core refuses illegal stems (no silent rewrite); restores name if disk write fails; injects Matrix stem into bytes 0–7.
  [`PatchManagerActionHandlerComputerLoadSave.cpp:311`](../../Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp#L311)

**Copy**

- Alert and footer strings for invalid SAVE AS / SAVE stem.
  [`PluginDisplayNames.h:152`](../../Source/Shared/Definitions/PluginDisplayNames.h#L152)

**Tests**

- Inject, illegal refuse, suggested Matrix stem, overwrite case fold.
  [`PatchManagerActionHandlerSaveTests.cpp:23`](../../Tests/Unit/PatchManagerActionHandlerSaveTests.cpp#L23)
