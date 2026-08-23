---
title: '12-3 Interactive display gesture transactions'
type: 'feature'
created: '2026-08-24'
status: 'done'
baseline_commit: '702dd7352f4808f874b2cef8e7ec9edfbdd1a071'
review_loop_iteration: 1
context:
  - _bmad-output/specs/spec-undo-redo-patch-editing/SPEC.md
  - _bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md
  - _bmad-output/implementation-artifacts/spec-12-1-spike-undomanager-on-apvts-with-one-slider-and-midi-proof.md
  - _bmad-output/implementation-artifacts/spec-12-2-bulk-editorial-transactions-for-init-and-paste-module.md
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Envelope and Track Generator displays already call `beginChangeGesture` / `endChangeGesture` (stories 10.2 / 10.3), but APVTS undo is not grouped into editorial transactions. A curve drag can produce many micro undo steps or none at all because writes use `setValueNotifyingHost` without `beginNewTransaction`, so users cannot revert one display edit with a single undo step.

**Approach:** At each display edit session (mouseDown → mouseUp), open one `UndoManager::beginNewTransaction`, keep the existing per-parameter gesture pairing, and write drag values via `getParameterAsValue().setValue` so intermediate drag samples collapse into one undo step. Mirror the spike slider pattern and story 12-2 bulk recording lessons; prove with Core unit tests that simulate display drag semantics without GUI widgets.

## Boundaries & Constraints

**Always:**
- Scope is `EnvelopeDisplayApvtsBinding` and `TrackGeneratorDisplayApvtsBinding` only — three envelope instances + one Track Generator instance wired from `PatchEditDisplaysPanel`.
- One `beginNewTransaction` per display edit session, opened on gesture begin before `beginChangeGesture`; closed by `endChangeGesture` on gesture end.
- Drag writes during an active session use `apvts.getParameterAsValue(id).setValue(...)` (not `setValueNotifyingHost`) so UndoManager records under the open transaction.
- Preserve existing gesture gate, `callAsync` listener refresh, and display callback wiring from stories 10.2 / 10.3 — no widget geometry or paint changes.
- Proof via programmatic `undo()` / `redo()` in unit tests — no keyboard shortcuts in this story.
- English-only source; `lint_touched.py` on touched C++ under `Source/` and `Tests/`.

**Ask First:**
- If `apvts.getUndoManager()` is null in any production path, confirm no-op behavior is acceptable before adding a Core seam.

**Never:**
- Keyboard shortcuts, menu items, or toolbar undo UI (story 12-4).
- Init/Paste module bulk transactions (story 12-2 — already done).
- Matrix Mod reorder undo (story 12-4).
- `DirtyPatchTracker` or stack-clear on load/mutate (story 12-5).
- Refactoring `EnvelopeDisplay` / `TrackGeneratorDisplay` hit-test or drag math.
- Shared abstraction beyond copy-adapt between the two bindings unless duplication blocks the fix.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Envelope point drag | Env1 Attack 10 → drag through 15, 20, 25 → release at 30 | Exactly one undo transaction; one `undo()` restores 10 | N/A |
| Track Generator point drag | Track point 3 at 5 → drag to 40 | One undo transaction; one `undo()` restores 5 | N/A |
| Two consecutive drags | Attack 10→30, release; Attack 30→45, release | Two undo transactions; first `undo()` restores 10 | N/A |
| SysEx after undo | Device path enabled; envelope drag then `undo()` | Restored parameter emits 0x06 via existing listener → dispatcher path | N/A |
| External sync during drag | APVTS listener update for non-active gesture param | Display refresh via existing gate; no extra undo transaction | N/A |
| Null UndoManager | Harness APVTS constructed with `nullptr` manager | Gesture callbacks no-op safely; writes still update APVTS | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Panels/.../EnvelopeDisplayApvtsBinding.cpp:41-52,99-118` — gesture begin/end callbacks; add `beginNewTransaction` + switch drag writes to `getParameterAsValue().setValue`.
- `Source/GUI/Panels/.../TrackGeneratorDisplayApvtsBinding.cpp:42-53,100-119` — same pattern as envelope binding (copy-adapt from 10.3).
- `Source/GUI/Panels/.../PatchEditDisplaysPanel.cpp:17-47,67-70` — read-only; four bindings already constructed with correct `PluginIDs` parameter tables.
- `Source/GUI/Widgets/EnvelopeDisplay.cpp:393-451` — read-only; fires `onEditGestureBegin(index)` / `onEditGestureEnd()` once per mouse session.
- `Source/GUI/Widgets/TrackGeneratorDisplay.cpp:284-334` — read-only; same single-session gesture contract.
- `Source/Core/PluginProcessor.cpp` — APVTS wired with `&undoManager_` (story 12-1); bindings reach manager via `apvts.getUndoManager()`.
- `Tests/Unit/UndoManagerApvtsSpikeTests.cpp:83-97` — reference `simulateSliderDrag`: `beginNewTransaction` + gesture + `getParameterAsValue().setValue`.
- `Tests/Unit/UndoManagerModuleBulkTests.cpp` — reference harness with UndoManager + patch listener + SysEx dispatch assertions (story 12-2).
- `_bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md:21-22,88-89` — CAP-4 envelope / Track Generator grouping rules.

## Tasks & Acceptance

**Execution:**
- [x] `EnvelopeDisplayApvtsBinding.cpp` — open `beginNewTransaction` on gesture begin; use `getParameterAsValue().setValue` in `writeIntParameter` during active gesture — one undo step per envelope drag (CAP-4).
- [x] `TrackGeneratorDisplayApvtsBinding.cpp` — mirror envelope binding transaction + write path — Track Generator drag grouping.
- [x] `Tests/Unit/UndoManagerDisplayGestureTests.cpp` — harness simulating display drag (multi-sample gesture, undo/redo, SysEx after undo) for one envelope param and one track point — CAP-4/CAP-5 without GUI.
- [x] `Tests/CMakeLists.txt` — register new test file.
- [x] `python3 Scripts/quality/lint_touched.py` — zero findings on touched C++.

**Acceptance Criteria:**
- Given APVTS wired with UndoManager and Env1 Attack at 10, when a simulated display drag sets intermediate values then releases at 30, then `getUndoDescriptions().size()` is 1 and one `undo()` restores 10.
- Given the same harness for `fmTrackPoint3` at 5 dragged to 40, when `undo()` runs, then value is 5 and exactly one transaction was recorded.
- Given two completed display drags on the same parameter, when `undo()` runs twice, then values revert in reverse order (two transactions).
- Given MIDI editor path enabled, when undo restores a display-edited patch parameter, then a 0x06 SysEx is dispatched with the restored packed byte.
- Given `writeIntParameter` called with null UndoManager harness, when drag simulation runs, then no crash and APVTS reflects final value.

## Spec Change Log

- Review loop 1 (patch): Reordered `beginNewTransaction` after null parameter guard in both bindings to avoid empty undo transactions. KEEP: transaction + gesture + `getParameterAsValue().setValue` pattern.
- Review loop 1 (patch): Added redo test, Track Generator SysEx-after-undo test, multi-sample track drag test; updated binding header comments.
## Design Notes

Story 10.2 / 10.3 added gesture pairing for MIDI coalescing and listener echo suppression, not editorial undo. Epic 12 requires `beginNewTransaction` at gesture boundaries (`epic-12-context.md`). Combine both: transaction at session start, `beginChangeGesture` on the active parameter, multiple `setValue` samples during drag, `endChangeGesture` at session end.

Use descriptive transaction names (e.g. `"Envelope edit"`, `"Track Generator edit"`) — exact strings are implementer choice unless product copy dictates otherwise.

Test harness should replicate binding write semantics (`beginNewTransaction` → `beginChangeGesture` → several `setValue` → `endChangeGesture`) with a minimal APVTS layout (one envelope module param + one track point) plus optional listener/dispatcher like `UndoManagerApvtsSpikeTests`.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` — expected: clean build.
- Run `Matrix-Control_Tests` — expected: new display gesture undo tests pass; existing tests green.
- `python3 Scripts/quality/lint_touched.py` — expected: zero findings on touched C++ under `Source/` and `Tests/`.

**Manual checks (if no CLI):**
- Standalone: drag Env1 Attack curve, trigger programmatic undo (debugger/temporary hook); slider and curve both show pre-drag value; synth plays restored level.

## Suggested Review Order

**Display gesture undo transaction**

- Open one editorial transaction only after the target parameter resolves.
  [`EnvelopeDisplayApvtsBinding.cpp:94`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/EnvelopeDisplayApvtsBinding.cpp#L94)

- Mirror the same guard and transaction boundary for Track Generator.
  [`TrackGeneratorDisplayApvtsBinding.cpp:95`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/TrackGeneratorDisplayApvtsBinding.cpp#L95)

**APVTS write path**

- Record undo via ParameterAsValue during display drags instead of setValueNotifyingHost.
  [`EnvelopeDisplayApvtsBinding.cpp:85`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/EnvelopeDisplayApvtsBinding.cpp#L85)

**Tests**

- Multi-sample envelope drag collapses to one undo transaction.
  [`UndoManagerDisplayGestureTests.cpp:268`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L268)

- Track Generator undo restores value and emits 0x06 SysEx.
  [`UndoManagerDisplayGestureTests.cpp:359`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L359)

- Undo then redo restores post-drag display value.
  [`UndoManagerDisplayGestureTests.cpp:395`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L395)

### Review Findings

- [x] [Review][Patch] Reorder beginNewTransaction after parameter null guard — avoids empty undo transactions when parameterId is invalid. [`EnvelopeDisplayApvtsBinding.cpp:94`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/EnvelopeDisplayApvtsBinding.cpp#L94)
- [x] [Review][Patch] Add redo test after display drag — undo-policy requires redo to post-drag values. [`UndoManagerDisplayGestureTests.cpp:395`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L395)
- [x] [Review][Patch] Add Track Generator SysEx-after-undo test — CAP-5 coverage was envelope-only. [`UndoManagerDisplayGestureTests.cpp:359`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L359)
- [x] [Review][Patch] Update binding header comments — still referenced setValueNotifyingHost. [`EnvelopeDisplayApvtsBinding.h:17`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/EnvelopeDisplayApvtsBinding.h#L17)
- [x] [Review][Defer] Harness simulates binding semantics without instantiating GUI binding classes — same pattern as story 12-1 spike; optional future binding-level test. [`UndoManagerDisplayGestureTests.cpp:236`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L236)
- [x] [Review][Defer] Defer beginNewTransaction until first write — click-without-drag could open empty transaction; low product risk today. [`EnvelopeDisplayApvtsBinding.cpp:94`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/EnvelopeDisplayApvtsBinding.cpp#L94)
- [x] [Review][Defer] Extract shared undo test harness from spike/display/module bulk tests — refactor optional. [`UndoManagerDisplayGestureTests.cpp:1`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L1)
- [x] [Review][Patch] Add Track Generator display drag redo test — undo-policy CAP-4 requires symmetric redo coverage; envelope redo exists at `testDisplayDragUndoRedoRestoresValues` but Track Generator has no parallel test. [`UndoManagerDisplayGestureTests.cpp:400`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L400)
- [x] [Review][Defer] I/O matrix “external sync during drag” row not covered — no test asserts listener-only refresh of a non-active parameter leaves undo stack unchanged during an open gesture. [`UndoManagerDisplayGestureTests.cpp:425`](../../Tests/Unit/UndoManagerDisplayGestureTests.cpp#L425)
