---
title: 'Plugin modals always in front'
type: 'bugfix'
created: '2026-09-05'
status: 'done'
baseline_commit: 'a2c7d1a04aec1bd5a3fd2e6f39bb2b1b8021df16'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** After native FileChoosers (notably Computer Patches → OPEN folder then load `.syx`), follow-up alerts and sometimes the picker itself can sit behind the plugin UI, so the user thinks nothing happened while a modal is open and blocking.

**Approach:** Keep existing sync modal/gate contracts. Centralize a raise-before/after pattern for every OS FileChooser and every alert helper so the plugin (or standalone) window is brought forward before any modal is shown and after any native picker returns — closing gaps where only some call sites raise today.

## Boundaries & Constraints

**Always:**
- Stay on the message thread for all modal display (existing `jassert` / gate checks).
- Preserve button labels, copy, and semantic return codes for all confirms.
- Keep Core gate callbacks sync (`runModalLoop` / `NativeMessageBox::show` / sync `FileChooser` browse).
- Raise UI before every alert path and before/after every native FileChooser used by the editor.
- UI strings remain English.

**Ask First:**
- Redesigning gates to async (`showAsync` / `FileChooser::launchAsync`) for sheet parenting.
- Making the editor `setAlwaysOnTop(true)` as a primary fix.
- Changing Settings / About / Master Init / Bank progress overlays (in-editor children, not OS dialogs).

**Never:**
- Expand into dirty-patch UX, new dialog features, or Core↔GUI dependency flips.
- Rely on macOS sync `withAssociatedComponent` or FileChooser parent alone for z-order (native sync panels ignore sheet attachment).
- Touch popup-menu `enterModalState` infrastructure or test-only CallOutBox code unless a shared raise helper forces a trivial include.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| OPEN then ASK ONCE / unsaved | Folder picker OK → load `.syx` → confirm gate | Picker and next alert visible in front; click on plugin does not dismiss the modal | Cancel / Escape → existing abort paths |
| Other FileChoosers | Mutator export folder, bank import/export, Save As | Same raise-before/after; no buried follow-up alert | Cancel browse → empty/`false` as today |
| All ordered confirms | Defrag, collision, history, unsaved, flush, name recon, bank gates | Still raise via shared alert helper; codes unchanged | Escape/OOR → cancel (0) |
| Mutator Delete confirm | Checkbox AlertWindow | Raise before `runModalLoop` (gap today) | Cancel → not confirmed |
| Standalone vs plugin-in-host | Same flows | Standalone: raise + foreground process; plugin: best-effort `toFront` (no always-on-top) | If host still buries, do not invent host-specific hacks this pass |

</frozen-after-approval>

## Code Map

- `Source/GUI/PluginEditorAlerts.cpp` / `PluginEditorInternal.h` — `raiseUiBeforeModalDialog`; `browseForDirectorySync` / `browseForFileToSaveSync` (SafePointer + raise before/after); `showOrderedConfirmAlert` + `showMutatorDeleteConfirmAlert` both raise
- `Source/GUI/PluginEditorPatchBindings.cpp` — OPEN, Mutator export folder, Save As via shared helpers; invalid-name `NativeMessageBox` still raises around alert
- `Source/GUI/PluginEditorBankBindings.cpp` — bank export/import via `browseForDirectorySync`
- `Source/GUI/PluginEditorWindows.cpp` — in-editor overlays (read-only unless Ask First)
- `Source/Core/PluginProcessorGates.cpp` — message-thread rule (read-only)
- Prior art: `spec-system-style-confirmation-modals.md` (native alerts; does not fix post-chooser z-order)

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/PluginEditorInternal.h` + `PluginEditorAlerts.cpp` — add sync FileChooser helper: raise, browse (directory or save), raise again, return result
- [x] `Source/GUI/PluginEditorAlerts.cpp` — raise at start of `showMutatorDeleteConfirmAlert`; keep raise in `showOrderedConfirmAlert`
- [x] `Source/GUI/PluginEditorPatchBindings.cpp` — OPEN, Mutator export folder, Save As via shared helper; keep invalid-name alert raise
- [x] `Source/GUI/PluginEditorBankBindings.cpp` — bank import/export folder choosers via same helper
- [x] Build + `Scripts/quality/lint_touched.py` on touched C++ — green

**Acceptance Criteria:**
- Given Computer Patches → OPEN then a follow-up confirm, when the modal is open, then it is in front of the plugin UI and clicking the plugin does not dismiss or hide it.
- Given Mutator export folder, bank import/export, and Save As, when those pickers or immediate follow-up alerts run, then the same front-and-modal behavior holds.
- Given every ordered confirm and Mutator Delete, when shown, then the UI was raised immediately before the modal and return codes / copy are unchanged.
- Given this change ships, then no gate was converted to async and no always-on-top flag was applied to the editor.

## Spec Change Log

## Design Notes

macOS sync native alerts/pickers are app-modal `runModal`, not sheets — `associatedComponent` / FileChooser parent do not fix z-order. After NSOpenPanel, the next NSAlert can open behind. Existing `raiseUiBeforeModalDialog` (`toFront` + macOS foreground) is the right fix; make it universal for all choosers and Mutator Delete.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — expected: build succeeds
- `python3 Scripts/quality/lint_touched.py` — expected: pass on touched `Source/` files

**Manual checks (if no CLI):**
- Standalone: OPEN → folder with `.syx` → ASK ONCE / unsaved in front; click plugin does not dismiss.
- Spot-check Mutator export folder, bank import/export, Save As, Mutator Delete.
- Optional plugin-in-host: same OPEN path; host-specific burial out of scope this pass.

## Suggested Review Order

**Shared raise + FileChooser helpers**

- Raise brings the top-level window forward (macOS also foregrounds the process).
  [`PluginEditorAlerts.cpp:56`](../../Source/GUI/PluginEditorAlerts.cpp#L56)

- Directory picker: SafePointer, raise → browse → raise, empty File on cancel.
  [`PluginEditorAlerts.cpp:71`](../../Source/GUI/PluginEditorAlerts.cpp#L71)

- Save picker: same raise contract; overwrite warn stays `true` as before.
  [`PluginEditorAlerts.cpp:94`](../../Source/GUI/PluginEditorAlerts.cpp#L94)

- Mutator Delete now raises before its checkbox AlertWindow.
  [`PluginEditorAlerts.cpp:178`](../../Source/GUI/PluginEditorAlerts.cpp#L178)

**Call-site adoption**

- Computer Patches OPEN uses the shared directory helper (main repro path).
  [`PluginEditorPatchBindings.cpp:50`](../../Source/GUI/PluginEditorPatchBindings.cpp#L50)

- Mutator export folder picker.
  [`PluginEditorPatchBindings.cpp:64`](../../Source/GUI/PluginEditorPatchBindings.cpp#L64)

- Save As via shared save helper; invalid-name alert still raises around NativeMessageBox.
  [`PluginEditorPatchBindings.cpp:285`](../../Source/GUI/PluginEditorPatchBindings.cpp#L285)

- Bank export / import folder pickers.
  [`PluginEditorBankBindings.cpp:44`](../../Source/GUI/PluginEditorBankBindings.cpp#L44)
