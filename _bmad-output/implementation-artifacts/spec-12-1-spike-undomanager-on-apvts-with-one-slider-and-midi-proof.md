---
title: '12-1 Spike UndoManager on APVTS with one slider and MIDI proof'
type: 'feature'
created: '2026-08-24'
status: 'done'
baseline_commit: '333bbb7f800653d9c4aeb9266bce1e9e6ef8d62b'
review_loop_iteration: 0
context:
  - _bmad-output/specs/spec-undo-redo-patch-editing/SPEC.md
  - _bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** APVTS is constructed with a null UndoManager, so editorial changes cannot be reverted. The undo/redo epic has no technical foundation and users still rely on STORE or Patch Mutator Compare to recover from mistakes.

**Approach:** Wire `juce::UndoManager` into `PluginProcessor` APVTS construction, ensure one `ParameterCell` slider mouse drag forms a single undo transaction, and prove programmatic undo restores the prior value and emits correct SysEx 0x06. Keyboard shortcuts, stack-clear policies, and dirty-tracker coherence are deferred to later stories.

## Boundaries & Constraints

**Always:**
- Undo logic lives in Core (`PluginProcessor`); GUI changes only if gesture hooks are required for one-transaction slider drag grouping.
- Non-editorial `setProperty(..., nullptr)` pattern stays unchanged for navigation, mutator state, ports, skin, clipboard feedback.
- Spike parameter: `dco1Frequency` (`PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kFrequency`).
- Proof via programmatic `undoManager.undo()` / `redo()` — no Cmd/Ctrl+Z in this story.
- English-only source; quality gate via `lint_touched.py` on touched C++.

**Ask First:**
- If wiring gestures on `TSS::Slider` affects all sliders globally before keyboard-arrow grouping (story 12-4), confirm scope stays mouse-drag only.

**Never:**
- Keyboard shortcuts, menu items, or toolbar buttons (story 12-4).
- Stack clear on load, paste, mutate, or compare-disable logic (story 12-5).
- `DirtyPatchTracker` changes (story 12-5).
- Init/Paste module bulk transactions (story 12-2).
- Envelope, Track Generator, or Matrix Mod reorder undo (stories 12-3, 12-4).
- User manual update (story 12-4).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Undo after slider drag | `dco1Frequency` dragged 50→75 | One `undo()` restores 50; patch model buffer matches; one 0x06 SysEx with restored packed byte | N/A |
| Redo after undo | `undo()` then `redo()` | Value returns to 75; 0x06 with post-drag packed byte | N/A |
| Non-editorial property | Navigation property via `setProperty(..., nullptr)` | Undo stack action count unchanged | N/A |
| MIDI gate off | `deviceDetected=false` | Undo still restores APVTS and model; no SysEx enqueued (existing gate) | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/PluginProcessor.h` — add `juce::UndoManager undoManager_` **before** `apvts` (member init order); optional `getUndoManager()` near `getApvts()`.
- `Source/Core/PluginProcessor.cpp:63` — `apvts(*this, nullptr, …)` → pass `&undoManager_`.
- `Source/Core/PluginProcessorValueTree.cpp` — `valueTreePropertyChanged` → `dispatchPatchOrMatrixModParameterChange`; must fire on undo-driven restores (read-only constraint).
- `Source/Core/MIDI/PatchParameterSysExDispatcher.cpp` — `dispatch(parameterId)` enqueues opcode `0x06` via `MidiManager::enqueueRemoteParameterEdit`.
- `Source/GUI/Widgets/Slider.cpp:191-218` — custom `mouseDown`/`mouseDrag`/`mouseUp`; does not call JUCE drag callbacks — likely needs `beginChangeGesture`/`endChangeGesture` for one-transaction grouping.
- `Source/GUI/Widgets/ParameterCell.cpp:52-63` — `SliderAttachment` on spike slider; read-only unless gesture hook lands here instead.
- `Source/GUI/Panels/Envelope/EnvelopeDisplayApvtsBinding.cpp:99-118` — reference pattern for `beginChangeGesture`/`endChangeGesture`.
- `Tests/Unit/PatchParameterSysExDispatcherTests.cpp` — `sysExMatchesRemoteEdit()` helper for 0x06 byte validation.
- `Tests/Unit/MatrixModInitServiceTests.cpp:305-395` — `ProcessorPathHarness` pattern to replicate with patch SysEx path + UndoManager.
- `_bmad-output/specs/spec-undo-redo-patch-editing/stories.yaml` — story 12-1 canonical scope.

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/PluginProcessor.h` — add `undoManager_` before `apvts`; add `getUndoManager()` accessor — member init order requires UndoManager before APVTS ctor.
- [x] `Source/Core/PluginProcessor.cpp` — pass `&undoManager_` to APVTS constructor — enables ValueTree undo recording.
- [x] `Source/GUI/Widgets/Slider.cpp` — `ScopedDragNotification` on mouseDown/mouseUp — notifies `SliderParameterAttachment` (`sliderDragStarted`/`Ended`) for one undo transaction per drag.
- [x] `Tests/Unit/UndoManagerApvtsSpikeTests.cpp` — Core harness: UndoManager + APVTS listener + `PatchParameterSysExDispatcher`; simulate drag, `undo()`/`redo()`, assert value + 0x06 — proves CAP-1/CAP-5 foundation without full PluginProcessor.
- [x] `Tests/CMakeLists.txt` — register `UndoManagerApvtsSpikeTests.cpp`.
- [x] Completion note — one `dco1Frequency` drag → 1 undo transaction; `getNumberOfUnitsTakenUpByStoredCommands()` &lt; 1024 units in spike harness (minimal ValueTree diff).

**Acceptance Criteria:**
- Given APVTS wired with UndoManager and `dco1Frequency` at 50, when a simulated drag sets 75 and `undoManager.undo()` runs, then APVTS value is 50 and patch model buffer matches.
- Given the state after undo, when `undoManager.redo()` runs, then value is 75.
- Given MIDI editor path enabled with device detected, when undo restores a changed patch parameter, then exactly one 0x06 SysEx is enqueued with the restored packed byte.
- Given a non-editorial `setProperty` with null undo manager, when the property changes, then `undoManager.getNumActions()` is unchanged.

## Spec Change Log

## Design Notes

`TSS::Slider` bypasses JUCE `startedDragging`/`endedDragging` by using custom mouse handlers. `EnvelopeDisplayApvtsBinding` already groups gestures via `param->beginChangeGesture()` / `endChangeGesture()`. Apply the same boundary on slider mouseDown/mouseUp so one mouse drag maps to one `UndoManager` transaction once APVTS is wired.

Test strategy: replicate `ProcessorPathHarness` with `juce::UndoManager`, `patchParameterIds_`, `suppressPatchParameterSysEx_`, and `PatchParameterSysExDispatcher` — avoids instantiating full `PluginProcessor` while mirroring the production listener → mapper → dispatcher chain.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` — expected: clean build.
- Run `Matrix-Control_Tests` — expected: new undo spike tests pass.
- `python3 Scripts/quality/lint_touched.py` — expected: zero findings on touched C++ under `Source/` and `Tests/`.

**Manual checks (if no CLI):**
- Standalone: drag DCO1 Frequency slider, call undo via debugger or temporary test hook; synth plays restored value.

## Suggested Review Order

**Undo foundation (Core)**

- APVTS now shares a dedicated UndoManager owned by the processor.
  [`PluginProcessor.h:402`](../../Source/Core/PluginProcessor.h#L402)

- Constructor passes that manager into APVTS instead of nullptr.
  [`PluginProcessor.cpp:63`](../../Source/Core/PluginProcessor.cpp#L63)

**Slider gesture grouping (GUI)**

- Mouse drag opens a ScopedDragNotification so SliderAttachment receives drag-start/end.
  [`Slider.cpp:199`](../../Source/GUI/Widgets/Slider.cpp#L199)

- Drag notification member lives for the press–release interval only.
  [`Slider.h:68`](../../Source/GUI/Widgets/Slider.h#L68)

**Proof tests**

- Harness proves undo/redo restores APVTS, model buffer, and 0x06 SysEx.
  [`UndoManagerApvtsSpikeTests.cpp:222`](../../Tests/Unit/UndoManagerApvtsSpikeTests.cpp#L222)

- Multi-write-in-one-gesture test guards one-transaction-per-drag policy.
  [`UndoManagerApvtsSpikeTests.cpp:324`](../../Tests/Unit/UndoManagerApvtsSpikeTests.cpp#L324)

- Test target registered in root CMake test list.
  [`CMakeLists.txt:473`](../../CMakeLists.txt#L473)

### Review Findings

- [x] [Review][Decision] Faut-il ajouter un test widget (`TSS::Slider` + `SliderAttachment`) ou la preuve harness suffit pour cette spike ? — **Résolu : harness seul** (preuve Core suffisante pour cette spike ; vérification manuelle slider acceptée).

- [x] [Review][Patch] Gate MIDI `deviceDetected` non testé avec le mécanisme réel [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Patch] AC#4 exige pile undo inchangée ; le test vérifie `getUndoDescriptions().size()` (équivalent JUCE de `getNumActions()`) [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Patch] Test multi-écriture incomplet : pas de vérif modèle ni SysEx après undo [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Patch] Test principal sans assertion `canRedo()` avant redo [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Patch] Propriété non éditoriale fictive remplacée par `deviceDetected` avec `nullptr` [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Patch] Structure AAA ajoutée dans les tests spike [`Tests/Unit/UndoManagerApvtsSpikeTests.cpp`] — fixed

- [x] [Review][Defer] Gestes clavier / double-clic sans `ScopedDragNotification` [`Source/GUI/Widgets/Slider.cpp:222`] — deferred, story 12-4

- [x] [Review][Defer] Pas de test deux drags consécutifs ni invalidation redo — deferred, hors périmètre spike

- [x] [Review][Defer] Risque pollution undo par écritures programmatiques APVTS — deferred, story 12-5

- [x] [Review][Defer] Helper `sysExMatchesRemoteEdit` dupliqué — deferred, dette test mineure
