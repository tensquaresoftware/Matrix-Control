# Epic 12 Context: Editorial Undo/Redo for Patch Editing

<!-- Compiled from planning artifacts. Edit freely. Regenerate with compile-epic-context if planning docs change. -->

## Goal

Users editing Matrix patches need standard undo/redo that groups natural editing gestures, resyncs the synthesizer after each step, and stays separate from Patch Mutator history and Compare audition. Today the user manual states there is no undo; this epic wires JUCE UndoManager into APVTS, groups gestures (sliders, displays, Init/Paste, Matrix Mod reorder), adds keyboard shortcuts, and keeps the dirty-patch indicator trustworthy after undo/redo.

## Stories

- Story 12.1: Spike UndoManager on APVTS with one slider and MIDI proof
- Story 12.2: Bulk editorial transactions for Init and Paste module
- Story 12.3: Interactive display gesture transactions
- Story 12.4: Matrix Mod reorder undo and keyboard shortcuts
- Story 12.5: Mutator load policy dirty tracker and Core tests

## Requirements & Constraints

- Editorial undo restores prior APVTS patch, master, and Matrix Mod parameter state; redo reapplies undone transactions.
- Related parameter changes from one user gesture form a single undo transaction (not per-parameter micro-steps).
- After undo/redo, the synth reflects restored APVTS state through the existing model and SysEx dispatch pipeline; bulk suppress flags must prevent MIDI storms during grouped writes.
- Patch load, full patch paste, MUTATE, and RETRY clear the editorial stack; loaded/mutated state becomes the baseline.
- Undo/redo is disabled while Patch Mutator Compare is active.
- STORE, SAVE, bank import/export, and navigation are not undoable in v1.
- Stack depth counts transactions (default 100), not raw parameter writes.
- Undo logic lives in Core; GUI handles shortcuts only. Core must not depend on GUI.
- Non-editorial `setProperty(..., nullptr)` pattern remains for navigation, mutator state, clipboard feedback, ports, skin, etc.
- v1 delivers keyboard shortcuts only — no Undo/Redo menu or toolbar buttons.
- English-only source; quality gate via `lint_touched.py`.

## Technical Decisions

- Wire `juce::UndoManager` into APVTS construction (replacing null manager).
- Use `UndoManager::beginNewTransaction` at gesture boundaries; slider/display gestures use `beginChangeGesture`/`endChangeGesture` where applicable.
- Existing `suppressPatchParameterSysEx_`, `suppressMatrixModParameterSysEx_`, and `suppressMasterParameterSysEx_` flags stay authoritative during bulk internal writes.
- `DirtyPatchTracker` coherence and stack-clear policies are defined in `undo-policy.md` and implemented in story 12.5.
- Patch Mutator `MutationHistoryStore` remains separate from editorial undo.

## UX & Interaction Patterns

- macOS Cmd+Z / Cmd+Shift+Z and Windows/Linux Ctrl+Z / Ctrl+Shift+Z when focus rules allow (see `keyboard-shortcuts.md`).
- Slider mouse drag = one undo step; arrow keys group per focus session until focus lost.
- Init module (I) and Paste module (P) each produce one undo step restoring the full target module.
- User manual §7 must document shortcuts and grouping (story 12.4).

## Cross-Story Dependencies

- Depends on Epic 2 (APVTS→SysEx), Epic 5 (module Paste), Epic 7 (Init/Paste handlers), Epic 9 (`DirtyPatchTracker`), Epic 10 (direct display editing).
- Story 12.1 provides UndoManager foundation; stories 12.2–12.4 build gesture grouping; story 12.5 completes policy, dirty state, and Core tests.
- Canonical contract: `_bmad-output/specs/spec-undo-redo-patch-editing/SPEC.md` (CAP-1…CAP-6).
