---
title: 'Patch Mutator Flush confirmation'
type: 'feature'
created: '2026-07-26'
status: 'done'
baseline_commit: 'b2656bacc25095ff77b6aff2cb1039834d3bd062'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Clicking Flush (F) in the Patch Mutator immediately clears session history with no confirmation, so a mis-click can wipe mutations the user still wanted.

**Approach:** Before running Flush, show an OS-style confirmation (same native alert path as other gates) with Cancel on the left and Continue on the right; Continue is the default and proceeds with Flush, Cancel (or dismiss) aborts and leaves history unchanged. Title: `Flush mutation history?`.

## Boundaries & Constraints

**Always:**
- Use the existing native alert path (`setUsingNativeAlertWindows` + `showMappedAlert` / `MessageBoxOptions`), not a custom skinned dialog.
- Button labels: `Cancel` and `Continue` (English, matching the rest of the GUI).
- Visual order: Cancel left, Continue right; Continue is the default action and the only path that calls `clearHistory()`; Cancel / Escape / out-of-range dismiss aborts.
- Keep Escape-safe mapping (Continue → proceed / result `1`); do **not** register Cancel first with `showMappedAlert == 1` (that would invert Escape and Confirm). Prefer Continue-then-Cancel `withButton` order so native chrome places default Continue on the right and Cancel on the left (same pattern as unsaved-edit).
- Core ↛ GUI: modal UI stays in `PluginEditor`; handler only invokes an injected `std::function<bool()>` gate (true = proceed).
- Missing gate (headless / unit tests): proceed without a modal (same as other optional gates).
- Skip the modal when history is already empty (Flush button disabled); still no-op-safe if invoked.

**Ask First:**
- Changing Flush semantics (e.g. also clearing the initial snapshot).
- Migrating this gate (or siblings) to async-only modals.
- Using French button labels or a custom non-OS dialog chrome.

**Never:**
- Put `AlertWindow` / `MessageBoxOptions` / GUI includes in Core or the engine.
- Rename wire ids (`patchMutatorClear`) or Core `clearHistory` naming.
- Confirm Delete, Mutate, or other Mutator actions in this change.
- Rewrite all modal gates or change Master Init / About dialogs.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Flush + Continue | History non-empty; user chooses Continue | `clearHistory()` runs as today | N/A |
| Flush + Cancel | History non-empty; user chooses Cancel | No clear; history and selection unchanged | N/A |
| Flush + dismiss | Escape / out-of-range native dismiss | Same as Cancel | Map to abort (not Continue) |
| Empty history | Clear invoked with empty store | No modal; no harmful clear | N/A |
| Headless / no gate | Gate unset | Clear runs without modal | N/A |
| Off message thread | Gate called off message thread | Refuse / abort (match unsaved-edit) | `jassert` / return false |

</frozen-after-approval>

## Code Map

- `Source/GUI/Panels/.../PatchMutatorPanel.cpp` -- Flush button stamps `patchMutatorClear` (read-only)
- `Source/Core/Actions/MutatorActionHandler.*` -- `handleClear()`; inject flush-confirm gate before `clearHistory()`
- `Source/Core/PluginProcessor.*` -- construct handler with gate lambda; `setMutatorFlushConfirmModalGate` + clear on editor teardown
- `Source/GUI/PluginEditor.cpp` -- wire `MessageBoxOptions` + `showMappedAlert` (mirror unsaved-edit)
- `Source/Shared/Definitions/PluginDisplayNames.h` -- `Dialogs::MutatorFlushConfirm` title/body/Cancel/Continue
- `Source/Core/Services/PatchMutator/PatchMutatorEngine.*` -- `clearHistory()` unchanged; enabled mirror already disables F when empty
- `_bmad-output/implementation-artifacts/spec-system-style-confirmation-modals.md` -- native alert conventions

## Tasks & Acceptance

**Execution:**
- [x] `Source/Shared/Definitions/PluginDisplayNames.h` -- add `Dialogs::MutatorFlushConfirm` (title, body, Cancel, Continue) -- SSOT English copy
- [x] `Source/Core/Actions/MutatorActionHandler.*` -- add optional `FlushConfirmModalGate` (`std::function<bool()>`); call before `clearHistory()` in `handleClear` -- gate without GUI in Core
- [x] `Source/Core/PluginProcessor.*` -- hold `setMutatorFlushConfirmModalGate`; pass through to handler ctor like Defrag/Export -- editor-owned modal slot
- [x] `Source/GUI/PluginEditor.cpp` -- wire native alert (`Continue` then `Cancel` in `withButton` → Cancel left / Continue right + default); clear gate on destroy -- OS chrome + Escape-safe mapping
- [x] Unit / handler tests if an existing MutatorActionHandler harness can assert Cancel skips `clearHistory` -- cover I/O matrix where cheap; else manual only

**Acceptance Criteria:**
- Given Mutator history is non-empty and the editor is open, when the user clicks Flush (F), then an OS-style confirmation titled `Flush mutation history?` appears before any history clear, with Cancel left and Continue right (Continue default).
- Given that dialog, when the user chooses Continue, then Flush runs and history is cleared as today (initial snapshot preserved).
- Given that dialog, when the user chooses Cancel or dismisses (Escape), then history is unchanged and no clear SysEx/audition path from Flush runs.
- Given empty history, when Flush would be invoked, then no confirmation is shown (button remains disabled / no-op path).
- Given headless or unset gate, when Clear is dispatched, then behavior matches today’s direct clear (no crash).

## Spec Change Log

## Design Notes

Guillaume confirmed: Cancel left, Continue right, Continue default; title `Flush mutation history?`.

For `showMappedAlert`, register **Continue then Cancel** (same as unsaved-edit): Continue → result `1`, Cancel / Escape / out-of-range → `0`. First `withButton` is JUCE’s default (right on macOS/Windows native). Cancel registers second → typically left. Do not put Cancel first in `withButton` or Escape becomes Continue.

Body: session history cleared, initial snapshot kept; Continue flushes / Cancel keeps history.

`handleClear`: if gate set and returns false, return; else `clearHistory()` as today.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds
- Handler/gate unit tests if added -- expected: pass

**Manual checks (if no CLI):**
- macOS standalone: non-empty history → F → native dialog; Cancel/Escape keep history; Continue flushes; Continue is default.

## Suggested Review Order

**Confirm gate (handler)**

- Ask before `clearHistory` when history enabled mirror is set.
  [`MutatorActionHandler.cpp:95`](../../Source/Core/Actions/MutatorActionHandler.cpp#L95)

- Inject optional `FlushConfirmModalGate` on the handler.
  [`MutatorActionHandler.h:24`](../../Source/Core/Actions/MutatorActionHandler.h#L24)

**Processor slot**

- Pass-through lambda: unset editor gate → proceed without modal.
  [`PluginProcessor.cpp:353`](../../Source/Core/PluginProcessor.cpp#L353)

- Public setter for the editor-owned Flush confirm gate.
  [`PluginProcessor.h:184`](../../Source/Core/PluginProcessor.h#L184)

**Native dialog UI**

- OS alert: Continue then Cancel, Warning icon, Escape-safe `== 1`.
  [`PluginEditor.cpp:311`](../../Source/GUI/PluginEditor.cpp#L311)

- Clear Flush gate on editor teardown.
  [`PluginEditor.cpp:892`](../../Source/GUI/PluginEditor.cpp#L892)

**Copy SSOT**

- Title, body, Cancel / Continue strings.
  [`PluginDisplayNames.h:141`](../../Source/Shared/Definitions/PluginDisplayNames.h#L141)

**Tests**

- Continue / Cancel / empty-history skip coverage.
  [`MutatorActionHandlerTests.cpp:267`](../../Tests/Unit/MutatorActionHandlerTests.cpp#L267)
