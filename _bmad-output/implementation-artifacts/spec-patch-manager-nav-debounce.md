---
title: 'Patch Manager Next/Previous navigation debounce'
type: 'feature'
created: '2026-08-23'
status: 'done'
review_loop_iteration: 0
baseline_commit: '57fef1973c88a9a95b31674a2411de6526cb8547'
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Rapid Next/Previous clicks in Patch Manager (Internal and Computer Patches) spam Program Change, dump requests, and file SysEx loads, stressing Matrix-1000/6/6R and flooding unsaved-edit dialogs.

**Approach:** Keep UI selection updates immediate; coalesce heavy MIDI/file work with Core debounce so only the final target runs after settle (~300 ms for buttons). Show the unsaved-edit confirm once at settle; Cancel restores the last committed selection with no MIDI for the abandoned target. Reuse `ComboboxPatchSendDebouncer`; keep Mutator History on 150 ms. Also debounce Computer Patches combobox selection at 150 ms via the shared load settle path.

## Boundaries & Constraints

**Always:**
- Debounce + settle confirm live in Core (`PatchManagerActionHandler`), not GUI.
- Buttons stay enabled; UI numbers/selection may update on every click.
- Button settle delay = dedicated `kPatchNavButtonDebounceMs` (300). Combobox / Mutator stay on `kComboboxPatchSendDebounceMs` (150).
- Reuse `ComboboxPatchSendDebouncer` (ctor override for ms); no second home-grown timer.
- Reuse `cancelPendingSysExRequest` / pending device-load generation / `abortComputerPatchesNavigation` for abandoned or superseded targets.
- Unit tests prove coalescing via `flushPendingSynchronouslyForTests` (no wall-clock sleep). Mutator History debounce must not regress.
- English-only source; `Scripts/quality/lint_touched.py` clean on touched C++.

**Ask First:**
- Changing confirm copy, FR-51 semantics beyond “once at settle”, or SysEx hardware timing.
- Debouncing bank buttons or other Patch Manager actions.
- Look / layout changes.

**Never:**
- Freeze or disable Next/Previous during the debounce window.
- Confirm on every click inside a burst.
- Alter `SysExDelayProfile` / `MidiRequestTiming`.
- Duplicate debouncer logic instead of extending the shared helper.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Internal rapid Next/Prev | N clicks within <300 ms | UI coords follow each click; after settle, one PC + one dump for final coords | Superseded in-flight dump cancelled / ignored via generation |
| Computer rapid Next/Prev | N clicks within <300 ms | Combobox selection follows; after settle, one file load + one patch SysEx for final file | Cancel at settle → revert to committed file, no SysEx |
| Confirm Cancel at settle | Dirty editor; user Cancels | Restore last committed Internal coords or Computer file selection; no MIDI for abandoned target | Stable baseline unchanged |
| Confirm OK at settle | Dirty editor; user Continues | Single load/MIDI for final target; new committed baseline | Existing persist/continue paths |
| Isolated slow click | One click, wait ≥300 ms | Same end state as today after ~300 ms delay | N/A |
| Circular wrap | Next at last / Prev at first during burst | Final wrapped target only is loaded | N/A |
| New burst after settle | Second burst before prior dump finishes | New settle cancels prior pending device load; only latest target applies | Generation / cancelPendingSysExRequest |
| Computer combobox skim | Rapid select changes | UI immediate; one load after 150 ms settle | Same confirm-at-settle rules |
| Mutator History skim | Rapid history select | Still one audition after 150 ms (regression guard) | Existing tests stay green |

</frozen-after-approval>

## Code Map

- `Source/Core/Util/ComboboxPatchSendDebouncer.h` / `.cpp` — `kComboboxPatchSendDebounceMs` (150), `schedule`, `flushPendingSynchronouslyForTests`; add `kPatchNavButtonDebounceMs = 300`.
- `Source/Core/Actions/MutatorActionHandler.h` — 150 ms consumer; keep timing + `flushHistorySelectionDebouncerForTests`.
- `Source/Core/Actions/PatchManagerActionHandler.h` — nav (300) + Computer-select (150) debouncers; committed baselines; flush seams; injectable ms for tests.
- `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` — `tryHandleInternalPatchNavigation` (~L123): today confirm→`applyPatchCoordinates`→`beginPendingDeviceLoad`→`loadCurrentPatchFromDevice` (split UI vs settle MIDI); `tryHandleComputerPatchFileNavigation` (~L148) → `advanceComputerPatchesSelection` / `handleLoadSelectedPatchFile`.
- `Source/Core/Actions/PatchManagerActionHandler.cpp` — `applyPatchCoordinates` (~L362) always MIDI-syncs — need UI-only advance during burst.
- `Source/Core/Actions/PatchManagerActionHandlerDeviceLoad.cpp` — `beginPendingDeviceLoad` / generation / `cancelPendingSysExRequest`; `restoreInternalCoordinates`.
- `Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp` — `handleLoadSelectedPatchFile` (~L80) confirm+load+SysEx (confirm once at settle).
- `Source/Core/Actions/PatchManagerActionHandlerComputerBrowser.cpp` — `advanceComputerPatchesSelection`; `abortComputerPatchesNavigation`.
- `Source/Core/Actions/ActionExecutionHooks.h` — `confirmPatchContextChange` hook (GUI wired in processor).
- Tests: `ComboboxPatchSendDebouncerTests.cpp`, `MutatorActionHandlerTests.cpp`, `PatchManagerActionHandler{ComputerNavigation,Gate,Basic}Tests.cpp`, `PatchManagerActionHandlerTestSupport.*` — coalesce via sync flush.

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Util/ComboboxPatchSendDebouncer.h` — add `kPatchNavButtonDebounceMs = 300` (comment: button nav vs combobox 150) — separate policies
- [x] `Source/Core/Actions/PatchManagerActionHandler.h` (+ `.cpp` as needed) — members: nav debouncer (300), computer-select debouncer (150), committed Internal/Computer baselines, flush-for-tests seams, injectable ms for unit tests
- [x] `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` — Internal Next/Prev: immediate UI coords without MIDI; schedule settle; at settle confirm once then PC+dump for final target; Cancel restores committed baseline
- [x] `Source/Core/Actions/PatchManagerActionHandler.cpp` — support UI-only coordinate apply (no Program Change) used during burst
- [x] `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` + Computer load/browser files — Computer Next/Prev: immediate selection UI; coalesce load/SysEx at 300 ms settle; confirm only then; Cancel uses existing abort/revert
- [x] Computer select/`handleLoadSelectedPatchFile` path — debounce combobox-driven loads at 150 ms with same settle-confirm rules (bonus in scope via shared settle)
- [x] Device-load path — on each new scheduled settle and on Cancel, cancel/abandon superseded pending dumps
- [x] `Tests/Unit/*` (+ CMake if new file) — burst→single send; wrap; Cancel restore; supersede pending; combobox coalesce; Mutator History 150 ms still green; use sync flush only
- [x] Run lint_touched + relevant unit tests

**Acceptance Criteria:**
- Given rapid Internal Next/Prev, when settle completes, then exactly one Program Change and one dump request target the final patch.
- Given rapid Computer Next/Prev, when settle completes, then exactly one file load/SysEx targets the final file.
- Given a dirty editor during a burst, when settle fires, then confirm appears at most once; Cancel restores committed selection and sends no MIDI for the abandoned target.
- Given a single slow click, when 300 ms elapses, then behaviour matches today’s successful load aside from the delay.
- Given rapid Mutator History selection, when flushed, then still a single audition at 150 ms policy.
- Given unit tests, when run, then coalescing is proven without wall-clock sleeps.

## Spec Change Log

- 2026-08-23 — Implemented Patch Manager Next/Previous + Computer select debounce; tests + lint green.

## Design Notes

**Internal settle:** Today confirm runs before UI move. Required: UI-first coords with MIDI suppressed; committed baseline at burst start; settle → `confirmPatchContextChange` once; OK → MIDI `applyPatchCoordinates` + `beginPendingDeviceLoad` + `loadCurrentPatchFromDevice`; Cancel → `restoreInternalCoordinates` + clear schedule.

**Computer:** Selection already moves first. Debounce `handleLoadSelectedPatchFile` (button 300 ms, combobox/select 150 ms) so confirm/SysEx run once; Cancel → `abortComputerPatchesNavigation`. Do not retune Mutator (150 ms). Prefer `kPatchNavButtonDebounceMs = 300`.

## Verification

**Commands:**
- Build + run Patch Manager / Combobox debouncer / Mutator unit tests — expected: green
- `python3 Scripts/quality/lint_touched.py` — expected: exit 0 on touched C++

**Manual checks:**
- Mash Internal/Computer Next/Prev — UI tracks, one MIDI after pause; dirty Cancel restores; Mutator History still ~150 ms

## Suggested Review Order

**Debounce policy**

- Separate 300 ms button settle from existing 150 ms combobox policy.
  [`ComboboxPatchSendDebouncer.h:16`](../../Source/Core/Util/ComboboxPatchSendDebouncer.h#L16)

- `cancel` / `isPending` support cross-path supersession without firing stale settles.
  [`ComboboxPatchSendDebouncer.h:27`](../../Source/Core/Util/ComboboxPatchSendDebouncer.h#L27)

**Internal Next/Previous settle**

- UI advances immediately with MIDI off; settle scheduled on the button debouncer.
  [`PatchManagerActionHandlerInternalPatches.cpp:123`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L123)

- Confirm once at settle, then one PC + dump for the final target.
  [`PatchManagerActionHandlerInternalPatches.cpp:170`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L170)

- Dropped Internal settles restore the committed baseline before Computer work continues.
  [`PatchManagerActionHandlerInternalPatches.cpp:150`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L150)

**Computer Next/Previous + combobox**

- Button nav coalesces load at 300 ms; combobox select at 150 ms on the shared settle.
  [`PatchManagerActionHandlerInternalPatches.cpp:217`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L217)

- Optional MIDI flag on coordinate apply keeps burst UI silent.
  [`PatchManagerActionHandler.cpp:364`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L364)

**Tests**

- Coalescing, Cancel, confirm-at-settle, and Internal→Computer cross-path coverage.
  [`PatchManagerActionHandlerNavDebounceTests.cpp:5`](../../Tests/Unit/PatchManagerActionHandlerNavDebounceTests.cpp#L5)
