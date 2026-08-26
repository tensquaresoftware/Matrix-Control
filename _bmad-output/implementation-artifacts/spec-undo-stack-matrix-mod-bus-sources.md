---
title: 'Fix editorial undo stack collapse and MIDI resync (Matrix Mod bus sources)'
type: 'bugfix'
created: '2026-08-26'
status: 'done'
route: 'one-shot'
---

# Fix editorial undo stack collapse and MIDI resync (Matrix Mod bus sources)

Commit: `d8e20d0`

## Intent

### Stack depth collapse

**Problem:** After three sequential Matrix Mod bus source edits in standalone, only one UNDO step was available; the stack collapsed because JUCE undo limits were misconfigured for full APVTS layouts.

**Approach:** Pass the editorial transaction depth (100) as both arguments to `UndoManager::setMaxNumberOfStoredUnits` (`maxStoredUnits` and `minimumTransactionsToKeep`), and add a full-layout unit test mirroring the smoke scenario.

### Parasitic granular SysEx after editorial undo/redo

**Problem:** After Matrix Mod source edits followed by editorial undo (or redo), the synth received a full-patch resync (0x0D) **plus** spurious granular SysEx (0x0B Matrix Mod coalesce, 0x06 patch param, 0x03 master) triggered by deferred APVTS flushes and the coalesce timer firing after suppress flags were cleared.

**Approach:** In `performEditorialUndo` / `performEditorialRedo`:

1. Begin a ~500 ms granular-MIDI quiet window (`kEditorialUndoRedoGranularMidiQuietMs`).
2. Cancel any pending Matrix Mod coalesce timer (`MatrixModSysExCoalesceTimer::cancelPending`).
3. Keep all `suppress*` flags true through `undo()` / `redo()`.
4. Call `resyncSynthAfterEditorialUndoRedo()` (full patch 0x0D via `sendFullPatchForAudition`) **before** clearing suppress.
5. Call `flushDeferredApvtsParameterSync(apvts)` (`copyState()` while suppress still active).
6. Cancel coalesce again, then clear suppress flags.
7. Honour the quiet window in `dispatchPatchOrMatrixModParameterChange` / `dispatchMasterParameterChange` so deferred side effects cannot emit granular opcodes after the resync.

**Smoke validation:** 3 Matrix Mod source edits → 3 undo → 3 redo — MIDI Monitor shows only 0x0D per step (Matrix-1000).

Policy contract: `_bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md` § MIDI resync after undo/redo.

## Suggested Review Order

1. [Source/Core/PluginProcessor.cpp](Source/Core/PluginProcessor.cpp) — production undo limit wiring.
2. [Source/Core/PluginProcessorValueTree.cpp](Source/Core/PluginProcessorValueTree.cpp) — editorial undo/redo sequencing, quiet window, dispatch guards.
3. [Source/Core/PluginProcessor.h](Source/Core/PluginProcessor.h) — `MatrixModSysExCoalesceTimer::cancelPending`.
4. [Source/Core/PluginProcessorInternal.h](Source/Core/PluginProcessorInternal.h) — `kEditorialUndoRedoGranularMidiQuietMs`, `flushDeferredApvtsParameterSync`.
5. [Tests/Unit/UndoManagerEditorialUndoMidiTests.cpp](Tests/Unit/UndoManagerEditorialUndoMidiTests.cpp) — opcode regression (0x0D only on undo/redo; live edit still 0x0B).
6. [Tests/Unit/UndoManagerMatrixModSourceChoiceTests.cpp](Tests/Unit/UndoManagerMatrixModSourceChoiceTests.cpp) — full APVTS stack-depth regression.
7. [_bmad-output/specs/spec-undo-redo-patch-editing/undo-policy.md](../specs/spec-undo-redo-patch-editing/undo-policy.md) — § MIDI resync after undo/redo.

### Review Findings (commit d8e20d0, 2026-08-26)

- [x] [Review][Decision] Spec one-shot étendu pour couvrir stack depth + resync MIDI (option 1).
- [x] [Review][Defer] Tests harness vs PluginProcessor production — deferred, smoke MIDI Monitor validé
- [x] [Review][Defer] Limites undo production non épinglées par test PluginProcessor — deferred
- [x] [Review][Defer] Branche master absente des tests MIDI editorial — deferred
- [x] [Review][Defer] Doc undo-policy (edge matrix MIDI, minTransactionsToKeep, quiet window UX) — deferred
- [x] [Review][Defer] Test éviction 101 transactions — deferred, hors périmètre bugfix
