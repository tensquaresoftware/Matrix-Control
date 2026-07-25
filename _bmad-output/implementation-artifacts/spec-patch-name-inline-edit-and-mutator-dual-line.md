---
title: 'Patch Name inline edit + Mutator dual-line'
type: 'feature'
created: '2026-07-26'
status: 'done'
review_loop_iteration: 0
baseline_commit: '2a93dbf15da036e770137d9fc61a8ff8b8cc64a8'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/passation-patch-name-inline-edit-and-mutator-dual-line.md'
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** FR-13’s editable half is still missing (story 1-6 deferred it), and Mutator overwrites the musical patch name in bytes 0–7 with `Mxx` / `Mxx-Ryy`, so display and exported SysEx lose the real name.

**Approach:** One change-set: double-click inline rename on Patch Name (custom caret), dual-line display (user name + Mutator/`INITIAL` label), and Mutator/export keep `Mxx` for stems/History only while bytes 0–7 stay the user name; refresh the frozen export basename on rename.

## Boundaries & Constraints

**Always:**
- Affordance = double-click primary name line; no EDIT button.
- Editable when RAM bank or `.syx` (`kComputerFile`); ROM (`DeviceMemoryLimits::isRomBank`) → double-click no-op.
- Enter: `PatchModel::setName` + APVTS `patchEditPatchName` + dirty (buffer memcmp) + `MidiManager::sendFullPatchForAudition` (0x0D edit buffer). No bank STORE. Never `0x06` for name.
- Escape / focus loss / interrupt (Mutate, load, nav, Compare, History audition…) = abandon; no dirty.
- Charset SSOT: `PatchFileNameSanitizer::isAllowedMatrixChar`; uppercase; 8 slots space-padded. Illegal char → ignore + footer `uiMessageSeverity=error`; clear on valid input or exit.
- Empty/all-spaces commit → keep previous name (unit-test).
- Dual-line inside existing 72 px (`kPatchNameDisplay`); no module height growth.
- No history / no Compare: single vertically centred name. History: primary user name + secondary `Mxx`/`Mxx-Ryy` (~75% opacity, smaller font). Compare (`kCompareActive`): secondary literal `INITIAL`.
- Mutate/Retry/Defrag/audition: do not write `Mxx` into bytes 0–7. History labels + on-disk stems stay `formatPatchName` / `formatExportStem`.
- User-name SSOT = live model + APVTS. Stamp that name onto packed buffers at export write and at Mutator audition/push so history snapshots cannot resurrect a stale or `Mxx` name.
- On successful rename: recompute and store frozen export basename via `computeExportBasename(newName)`. Old export folders are not deleted. Existing collision modal unchanged.
- `kComputerFile` basename: if sanitized current patch name non-empty → `{name} @ SyxFile`; else keep `{fileStem} @ SyxFile`.
- SAVE/SAVE AS filename injection still wins (D-025). English UI strings only.

**Ask First:**
- Any need to grow Patch Name height / reopen Epic U layout asserts.
- Any change to History combo labels away from `Mxx`/`Mxx-Ryy`.

**Never:**
- EDIT button; per-character `0x06`; auto-delete old export folders; DAW-automate name as AudioParameter; French source strings.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Rename RAM / .syx | Double-click, type, Enter | Name + dirty + 0x0D; secondary/History unchanged | N/A |
| ROM bank | Double-click | No-op | N/A |
| Cancel | Escape / blur / interrupt | Previous name; not dirty | N/A |
| Illegal char | e.g. `@` | Ignored; footer error | Clear on valid char / exit |
| Empty commit | All spaces + Enter | Keep previous name | N/A |
| Mutate then export | User name `WARMPAD` | Stems `M00…`; .syx bytes 0–7 = `WARMPAD` | N/A |
| Rename then re-export | `WARMPAD`→`COLDPAD` @ B1P73 | New folder `COLDPAD @ B1P73`; old folder kept | Collision modal if target exists |
| .syx origin rename | Rename after file load | Frozen basename uses `{name} @ SyxFile` | Empty name → fileStem fallback |

</frozen-after-approval>

## Code Map

- `Source/GUI/Widgets/PatchNameDisplay.{h,cpp}` -- paint, dual-line, custom caret editor
- `Source/GUI/Modules/PatchNameDisplayPanel.{h,cpp}` -- APVTS listen, editability, secondary label, interrupt cancel
- `Source/GUI/Looks/WidgetLooks.h` + `LookBuilders.cpp` + skin colour tables -- secondary colour/font
- `Source/Core/Models/PatchNameSyncer.{h,cpp}` -- property ↔ model on commit
- `Source/Core/Services/PatchMutator/MutationNaming.{h,cpp}` -- stop buffer clobber; keep format helpers
- `Source/Core/Services/PatchMutator/PatchMutatorEngine.{h,cpp}` -- drop applyPatchName on mutate/retry; stamp name on push; refresh basename API
- `Source/Core/Services/PatchMutator/HistoryDefragService.*` -- stop applyPatchName renumber writes
- `Source/Core/Services/PatchMutator/PatchLoadContext.{h,cpp}` -- computer-file basename uses current name
- `Source/Core/Services/PatchMutator/MutationHistoryStore.*` -- frozen basename set/get
- `Source/Core/Services/PatchFileService.*` -- export stems vs stamped name bytes
- `Source/Core/MIDI/MidiManager.*` -- `sendFullPatchForAudition` reuse
- `Source/Shared/Helpers/PatchFileNameSanitizer.*` -- charset SSOT
- `Source/Shared/Definitions/DesignAtoms.h` -- height 72 unchanged
- `Tests/Unit/MutationNamingTests.cpp`, `HistoryDefragServiceTests.cpp`, `PatchFileServiceTests.cpp`, `MutatorRecipePersistenceTests.cpp`, `PatchMutatorEngineTests.cpp` -- drop `getName()==Mxx` where obsolete; assert stems vs name bytes; basename refresh; empty-commit rule

## Tasks & Acceptance

**Execution:**
- [x] `MutationNaming` + Mutator/Defrag callers -- stop writing `Mxx` into bytes 0–7; keep format helpers for UI/stems
- [x] `PatchMutatorEngine` / `PatchFileService` -- stamp live user name on audition push and export write; add rename → refresh `frozenExportBasename`
- [x] `PatchLoadContext::computeExportBasename` -- `kComputerFile` uses sanitized current name + ` @ SyxFile` when non-empty
- [x] `PatchNameDisplay` (+ Look/skins) -- dual-line layout in 72 px; Compare `INITIAL`; custom red blink caret (~500 ms); keys; charset; empty keep-previous
- [x] `PatchNameDisplayPanel` -- ROM/.syx/RAM gate; secondary from History selection / Compare; footer error; commit via Syncer + dirty + `sendFullPatchForAudition`; cancel on interrupt
- [x] Unit tests -- matrix above + update obsolete `getName()==M00` / export-name assertions

**Acceptance Criteria:**
- Given RAM or `.syx` patch, when user double-clicks name, edits, and presses Enter, then model + APVTS update, patch is dirty, and full-patch 0x0D is sent (no STORE).
- Given ROM bank, when user double-clicks, then nothing changes.
- Given edit in progress, when Escape, focus loss, or Mutate/load/nav interrupts, then edit abandons with no dirty.
- Given history present, when not Compare, then primary shows user name and secondary shows `Mxx`/`Mxx-Ryy`; when Compare active, secondary shows `INITIAL`.
- Given mutate/retry/export, when buffers are written or pushed, then bytes 0–7 equal current user name while file/folder stems and History labels remain `Mxx`/`Mxx-Ryy`.
- Given rename after mutate, when EXPORT runs again, then session folder uses the new basename and the previous folder is left intact.

## Design Notes

**Name ownership:** Live `PatchModel` + APVTS `patchEditPatchName` are SSOT for the musical name. `Mxx` exists only as format-helper strings (History + stems). Because history stores full packed buffers, stamp the live name onto copies at export and at `pushResultToEditorAndSynth` (and equivalent audition paths) so rename cannot leave stale names in `.syx` or resurrect old names on History select.

**Caret:** Custom paint (not JUCE `TextEditor`). Red rect around focused slot; caret-on → black glyph in red; blink ≈ 500 ms on/off. Secondary line read-only while editing.

**Golden export scenario:** `WARMPAD` @ B1P73 → mutate+retry → export `WARMPAD @ B1P73` → rename `COLDPAD` → export → `COLDPAD @ B1P73` created; old folder kept; collision modal if target exists.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- plugin + tests build
- `ctest --preset macos-debug-arm64 -R 'MutationNaming|HistoryDefrag|PatchFileService|MutatorRecipe|PatchMutatorEngine|PatchName|PatchLoadContext' --output-on-failure` -- naming/export/basename/related suites green (adjust filter if target names differ)

**Manual checks:**
- Double-click rename on RAM vs ROM; caret blink + illegal char footer error
- History dual-line + Compare `INITIAL` inside 72 px
- Mutate does not change primary name to `M00`; export `.syx` bytes keep user name; rename then re-export new folder name

## Suggested Review Order

**Name ownership (Mutator / export)**

- Mutate inserts history without writing Mxx into bytes 0–7
  [`PatchMutatorEngine.cpp:249`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L249)

- Audition/push stamps the live user name so History cannot resurrect a stale name
  [`PatchMutatorEngine.cpp:258`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L258)

- Export `.syx` stamps non-empty user name; filenames stay Mxx
  [`PatchFileService.cpp:153`](../../Source/Core/Services/PatchFileService.cpp#L153)

- Rename refreshes the frozen export session basename
  [`PatchMutatorEngine.cpp:604`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L604)

- Computer-file basename prefers current patch name + ` @ SyxFile`
  [`PatchLoadContext.cpp:34`](../../Source/Core/Services/PatchMutator/PatchLoadContext.cpp#L34)

**Inline editor + dual-line UI**

- Custom 8-slot caret editor (not JUCE TextEditor)
  [`PatchNameDisplay.cpp:85`](../../Source/GUI/Widgets/PatchNameDisplay.cpp#L85)

- Keyboard: commit/cancel, arrows, LCD backspace/delete, charset gate
  [`PatchNameDisplay.cpp:187`](../../Source/GUI/Widgets/PatchNameDisplay.cpp#L187)

- Dual-line layout inside the fixed 72 px box
  [`PatchNameDisplay.cpp:269`](../../Source/GUI/Widgets/PatchNameDisplay.cpp#L269)

- Secondary label: History Mxx / Compare `INITIAL`
  [`PatchNameDisplayPanel.cpp:176`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/PatchNameDisplayPanel.cpp#L176)

- Interrupt properties cancel an open edit
  [`PatchNameDisplayPanel.cpp:143`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/PatchNameDisplayPanel.cpp#L143)

**Commit path**

- ROM / `.syx` / Compare gate for double-click
  [`PluginProcessor.cpp:2027`](../../Source/Core/PluginProcessor.cpp#L2027)

- Enter: model + APVTS + basename refresh + full-patch audition
  [`PluginProcessor.cpp:2047`](../../Source/Core/PluginProcessor.cpp#L2047)

- Editor wires can-edit + commit into the panel
  [`PluginEditor.cpp:393`](../../Source/GUI/PluginEditor.cpp#L393)

**Tests / helpers**

- Empty/all-spaces commit keeps the previous name
  [`PatchNameEditRules.cpp:6`](../../Source/Core/Services/PatchNameEditRules.cpp#L6)
