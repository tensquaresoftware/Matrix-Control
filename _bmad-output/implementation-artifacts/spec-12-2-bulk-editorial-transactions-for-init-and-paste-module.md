---
title: '12-2 Bulk editorial transactions for Init and Paste module'
type: 'feature'
created: '2026-08-24'
status: 'done'
baseline_commit: '278681ef3219aa91afe9f88b9539b094b4ca5131'
review_loop_iteration: 0
context:
  - _bmad-output/specs/spec-undo-redo-patch-editing/SPEC.md
  - _bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md
  - _bmad-output/implementation-artifacts/spec-12-1-spike-undomanager-on-apvts-with-one-slider-and-midi-proof.md
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Init (I) and Paste (P) on a patch module write many APVTS parameters at once. Without a grouped undo transaction, each parameter write would become a separate undo step — or nothing would be recorded — so users could not revert a whole module reset or paste in one gesture.

**Approach:** Call `UndoManager::beginNewTransaction` immediately before bulk APVTS writes in the existing `ModuleActionHandler` patch-module Init and Paste paths, wired from `PluginProcessor` via `ActionExecutionHooks`. Keep existing `suppressPatchParameterSysEx_` hooks around bulk writes; after undo, rely on the existing listener → `apvtsToBuffer` → SysEx path (CAP-5).

## Boundaries & Constraints

**Always:**
- Undo wiring stays in Core; `ModuleActionHandler` receives a hook (or equivalent seam) from `PluginProcessorConstruction`, not GUI.
- Scope is **patch-module** Init and Paste only (`handlePatchModuleInit`, `pastePatchModule` / `pushModuleToApvtsAndSysEx`).
- One `beginNewTransaction` per successful Init or Paste that performs APVTS writes.
- Existing `setSuppressPatchSysEx(true/false)` boundaries in `ModuleActionHandler` remain unchanged.
- Proof via programmatic `undo()` / `redo()` in Core unit tests — no keyboard shortcuts in this story.
- English-only source; `lint_touched.py` on touched C++ under `Source/` and `Tests/`.

**Ask First:**
- If grouping Init requires refactoring `PatchModuleInitService` (which currently calls `pushModuleToApvts` internally) beyond adding a pre-write transaction hook at the handler boundary, confirm the seam location before moving APVTS push out of the service.

**Never:**
- Master module Init, Matrix Mod Init/Paste, or full-patch paste (other stories or non-editorial checkpoints).
- Keyboard shortcuts, menu items, or toolbar undo UI (story 12-4).
- `DirtyPatchTracker` or stack-clear on load/mutate (story 12-5).
- Envelope, Track Generator, or Matrix Mod reorder undo (stories 12-3, 12-4).
- User manual updates (story 12-4).
- Changing clipboard compatibility rules or init template loading behavior.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Paste module success | Clipboard holds compatible DCO1 module; target DCO1 params differ | One undo transaction; `getUndoDescriptions().size()` increases by 1; one `undo()` restores all module APVTS params and model buffer to pre-paste values | N/A |
| Paste blocked | Clipboard empty or incompatible | No APVTS writes; undo stack unchanged | Footer warning only (existing) |
| Patch module Init success | User triggers I on e.g. Envelope 1 module | One undo transaction covering all module APVTS params written by init; one `undo()` restores pre-init module state | Init template fallback footer unchanged |
| SysEx after undo | Device path enabled; module paste then `undo()` | Per-param 0x06 messages via existing dispatch (or module burst if coalesced path applies); no regression vs pre-undo suppress during bulk forward write | N/A |
| Non-editorial property | Footer `uiMessageText` after Init/Paste | `setProperty(..., nullptr)` — undo stack action count unchanged | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/Actions/ActionExecutionHooks.h` — add optional `beginEditorialTransaction` callback (or named equivalent) invoked before bulk editorial APVTS writes.
- `Source/Core/PluginProcessorConstruction.cpp:116-119` — wire hook to `undoManager_.beginNewTransaction(...)` alongside existing suppress hooks.
- `Source/Core/Actions/ModuleActionHandler.cpp:419-437` — `handlePatchModuleInit`: call editorial transaction hook after suppress-on, before `patchModuleInitService_->initModule` (service pushes APVTS inside).
- `Source/Core/Actions/ModuleActionHandler.cpp:352-396` — `pastePatchModule`: call hook after successful `clipboardService_->pasteModule`, before `pushModuleToApvtsAndSysEx`.
- `Source/Core/Actions/ModuleActionHandler.cpp:309-321` — `pushModuleToApvtsAndSysEx`: read-only; already wraps `pushModuleToApvts` with `suppressPatchSysEx`.
- `Source/Core/Init/PatchModuleInitService.cpp:26-38` — read-only; init flow: model copy → `pushModuleToApvts` → `dispatchModule`; transaction must start in handler before this runs.
- `Source/Core/Models/ApvtsPatchMapper.cpp:50-63` — `pushModuleToApvts` iterates module descriptors; each `setValueNotifyingHost` records undo when manager wired.
- `Source/Core/PluginProcessorValueTree.cpp:150-154` — suppress skips per-param SysEx during bulk forward write; undo path uses normal dispatch when suppress off.
- `Source/Core/PluginProcessor.h:96-97,402` — `getUndoManager()` / `undoManager_` (story 12-1).
- `Tests/Unit/ModuleActionHandlerTests.cpp:64-116` — `HandlerHarness` uses `nullptr` UndoManager today; extend or mirror with `&undoManager` for paste proof.
- `Tests/Unit/UndoManagerApvtsSpikeTests.cpp:81-95,324-353` — reference `beginNewTransaction` + multi-write single-transaction pattern.
- `_bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md:12-13,90-91` — one transaction per Init/Paste module.
- `_bmad-output/specs/spec-undo-redo-patch-editing/stories.yaml` — story 12-2 canonical scope.

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Actions/ActionExecutionHooks.h` — add `std::function<void(const juce::String&)> beginEditorialTransaction` (nullable) — Core seam without GUI dependency.
- [x] `Source/Core/PluginProcessorConstruction.cpp` — wire hook to `undoManager_.beginNewTransaction(name)` — connects processor UndoManager to handlers.
- [x] `Source/Core/Actions/ModuleActionHandler.cpp` — invoke hook in `handlePatchModuleInit` and `pastePatchModule` before bulk APVTS writes — CAP-4 Init/Paste grouping.
- [x] `Source/Core/Models/ApvtsPatchMapper.cpp` — push int/choice via `getParameterAsValue().setValue` so bulk writes record undo under `beginNewTransaction`.
- [x] `Tests/Unit/UndoManagerModuleBulkTests.cpp` — harness with UndoManager wired APVTS; paste/init/redo undo proofs — CAP-4/CAP-5 without full PluginProcessor.
- [x] `CMakeLists.txt` — register `UndoManagerModuleBulkTests.cpp`.
- [x] `python3 Scripts/quality/lint_touched.py` — zero findings on touched C++.

**Acceptance Criteria:**
- Given APVTS wired with UndoManager and a patch module with known non-default values, when Paste module succeeds via `ModuleActionHandler`, then `undoManager.getUndoDescriptions().size()` increases by exactly one and one `undo()` restores all APVTS parameters in that module to pre-paste values with model buffer matching.
- Given the same baseline, when Patch module Init succeeds via `ModuleActionHandler`, then one undo transaction is recorded and one `undo()` restores the entire target module to pre-init APVTS and model state.
- Given Paste blocked (empty clipboard), when Paste is triggered, then undo stack depth is unchanged.
- Given bulk forward write with `suppressPatchSysEx` true, when Init or Paste completes, then SysEx is dispatched via existing module burst path (not per-param during suppress); when `undo()` runs with suppress false, restored parameters emit SysEx through existing listener dispatch.
- Given footer `uiMessageText` update after Init/Paste, when property is set with null undo manager, then `undoManager.getNumActions()` is unchanged.

## Spec Change Log

- Review loop 1 (patch): `ApvtsPatchMapper::pushIntToApvts` / `pushChoiceToApvts` switched from `setValueNotifyingHost` to `getParameterAsValue().setValue` so bulk Init/Paste writes record undo when wrapped by `beginNewTransaction`. KEEP: handler-level `beginEditorialTransaction` hook and suppress boundaries unchanged.
## Design Notes

`PatchModuleInitService::initModule` performs model copy and APVTS push in one call. The editorial transaction boundary belongs in `handlePatchModuleInit` immediately before that call so all `pushModuleToApvts` writes inside the service share one undo step without restructuring the service.

Paste already separates model write (`clipboardService_->pasteModule`) from APVTS push (`pushModuleToApvtsAndSysEx`). Start the transaction after successful paste, before APVTS push — only APVTS editorial state is undoable per SPEC.

Bulk APVTS push must use `getParameterAsValue().setValue` (not `setValueNotifyingHost`) so changes enter the UndoManager while `beginNewTransaction` is active.

Test harness pattern: copy `ModuleActionHandlerTests::HandlerHarness` but construct `apvts(*this, &undoManager, …)` and assert module param count > 1 for a meaningful bulk proof (e.g. Envelope module paste layout already in tests).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` — expected: clean build.
- Run `Matrix-Control_Tests` — expected: new/extended module bulk undo tests pass; existing tests green.
- `python3 Scripts/quality/lint_touched.py` — expected: zero findings on touched C++ under `Source/` and `Tests/`.

**Manual checks (if no CLI):**
- Standalone: tweak DCO1, Paste another module onto DCO1, trigger programmatic undo (debugger/temporary hook); all DCO1 params revert together.

## Suggested Review Order

**Editorial transaction seam**

- Hook opens one undo transaction before bulk module writes.
  [`ActionExecutionHooks.h:36`](../../Source/Core/Actions/ActionExecutionHooks.h#L36)

- Processor connects the hook to its UndoManager.
  [`PluginProcessorConstruction.cpp:127`](../../Source/Core/PluginProcessorConstruction.cpp#L127)

**Handler boundaries**

- Init starts a transaction before the init service bulk APVTS push.
  [`ModuleActionHandler.cpp:434`](../../Source/Core/Actions/ModuleActionHandler.cpp#L434)

- Paste starts a transaction after model paste, before APVTS push.
  [`ModuleActionHandler.cpp:395`](../../Source/Core/Actions/ModuleActionHandler.cpp#L395)

**APVTS push path**

- Bulk push uses ParameterAsValue so undo records under beginNewTransaction.
  [`ApvtsPatchMapper.cpp:79`](../../Source/Core/Models/ApvtsPatchMapper.cpp#L79)

**Tests**

- Bulk paste/init/redo undo proofs in unit harness.
  [`UndoManagerModuleBulkTests.cpp:302`](../../Tests/Unit/UndoManagerModuleBulkTests.cpp#L302)
