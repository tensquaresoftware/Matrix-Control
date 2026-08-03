---
organization: Ten Square Software
project: Matrix-Control
title: Patch Mutator HISTORY INITIAL Item
author: BMad Agent
type: feature
created: '2026-08-03'
status: done
baseline_commit: '2fdce5ec519a32be25b6869f8773cf2d62c89d5f'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Compare and Export already use the original patch (`INITIAL` / `Initial.syx`), but HISTORY only lists mutations — users cannot browse the origin the same way.

**Approach:** When history is non-empty and the initial snapshot exists, add a selectable **INITIAL** primary at the top of HISTORY (level 1), with a horizontal separator before `Mxx`. Align selection, Patch Name secondary label, Compare enter/exit, prev/next, and button enablement — without treating manual INITIAL selection as full Compare lock.

## Boundaries & Constraints

**Always:**
- INITIAL is the existing session origin snapshot (Compare/Export), not “previous patch” and not a new `MutationEntry`.
- Show INITIAL + separator only while history has ≥1 entry and `hasInitialSnapshot()`; hide when history is empty; restore after the next Mutate that leaves history non-empty.
- Selecting INITIAL (outside Compare): audition origin; original Patch Name + secondary `INITIAL`; Mutate on; Compare/Retry/Delete off; **no** Compare lock.
- Mutation/retry selection unchanged; Compare enabled. Enter Compare: today’s lock + origin audition **and** select INITIAL in the combo. Exit Compare: restore prior mutation/retry. HISTORY stays locked during Compare.
- Mutate from INITIAL: keep the same origin; new entry becomes selection.
- Prev/next flat list: INITIAL first (when present), then existing M/R order.
- Preserve Compare lock and Export layout (`Initial.syx`); no export redesign.

**Ask First:**
- Changing Clear History so it clears/re-captures the RAM initial snapshot (today Clear keeps the snapshot until patch-load reset).
- HierarchicalComboBox changes beyond minimal selectable INITIAL + separator support.

**Never:**
- Broad Mutator redesign, recipe/algorithm changes, or renaming `Mxx` / retries.
- Reusing empty-combo `isSentinel` for INITIAL (non-selectable).
- French strings in source; keep label `INITIAL` via existing display-name constants.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| First Mutate | Empty → `M00` + snapshot | INITIAL, separator, M00; select M00; Compare on | N/A |
| Select INITIAL | Pick INITIAL | Origin + secondary INITIAL; Compare/Retry/Delete off; Mutate on; no lock | N/A |
| Compare enter | From mutation/retry | Lock as today; combo INITIAL; secondary INITIAL | No-op if no snapshot |
| Compare exit | Prior M/R saved | Restore that M/R + audition; unlock | N/A |
| Mutate from INITIAL | INITIAL selected | Origin unchanged; new entry selected | Existing Mutate failures |
| Clear / empty | Clear or session reset | No INITIAL row; empty sentinel as today | N/A |
| Prev/next | ≥2 flat slots incl. INITIAL | Circular walk includes INITIAL | Off if &lt;2 or Compare |

</frozen-after-approval>

## Code Map

- `Source/GUI/Widgets/HierarchicalComboBox.{h,cpp}` + `HierarchicalPopupMenu*.{h,cpp}` -- selectable INITIAL primary + non-selectable separator (not empty `isSentinel`)
- `Source/GUI/Panels/.../PatchMutatorPanelHistory.cpp` -- populate INITIAL/separator; selection sync; Compare shows INITIAL
- `Source/Core/Services/PatchMutator/PatchMutatorEngine*.{h,cpp}` + `PatchMutatorEngineInternal.h` -- INITIAL selection SSOT; enablement; flat advance; Compare enter/exit
- `Source/Core/Services/PatchMutator/MutationHistoryStore.{h,cpp}` -- reuse `initialSnapshot_`
- `Source/GUI/Panels/.../PatchNameDisplayPanel.cpp` -- secondary INITIAL when Compare **or** INITIAL selected
- `Source/Shared/Definitions/PluginDisplayNames.h` -- reuse `kCompareSecondaryLabel`
- `Tests/Unit/PatchMutatorEngineCompareSnapshotTests.cpp`, `PatchMutatorEngineEnabledResetAdvanceTests.cpp`, related engine tests -- cover matrix edges

## Tasks & Acceptance

**Execution:**
- [x] `HierarchicalComboBox` + `HierarchicalPopupMenu*` -- selectable reserved primary + separator row -- INITIAL + divider without breaking empty sentinel
- [x] `PatchMutatorEngine*` / Internal -- INITIAL selection ≠ Compare lock; flat prev/next; Compare select/restore; gate Compare/Retry/Delete; Mutate stays on -- engine SSOT
- [x] `PatchMutatorPanelHistory.cpp` -- build INITIAL + separator + Mxx; hydrate/Compare display -- UI mirrors engine
- [x] `PatchNameDisplayPanel.cpp` -- secondary INITIAL for Compare **or** INITIAL selection -- dual-line parity
- [x] `Tests/Unit/PatchMutatorEngine*.cpp` -- I/O edges (select, Compare round-trip, Mutate from INITIAL, advance, clear) -- regression safety

**Acceptance Criteria:**
- Given ≥1 mutation + snapshot, when HISTORY level-1 opens, then INITIAL, separator, then `Mxx`.
- Given INITIAL selected outside Compare, when UI updates, then origin loaded, name + secondary INITIAL, Compare/Retry/Delete off, Mutate on, no full Compare lock.
- Given mutation/retry selected, when Compare clicked, then origin auditioned, lock as today, combo shows INITIAL, secondary INITIAL.
- Given Compare active, when Compare clicked again, then prior mutation/retry restored and unlocked.
- Given INITIAL selected, when Mutate succeeds, then same origin snapshot and new item selected.
- Given Clear or empty history, when HISTORY shown, then no INITIAL; it returns after next mutation with snapshot.
- Given existing Compare lock and Export, when this ships, then lock and `Initial.syx` remain intact.

## Design Notes

- Reserved primary id + engine “INITIAL selected” flag; reuse `initialSnapshot_` (no fake `MutationEntry`). Empty `isSentinel` is non-selectable — add an explicit separator kind; INITIAL is a selectable primary. Manual INITIAL ≠ Compare lock (Mutate stays live). Flat nav: INITIAL then existing M/R order.

## Verification

**Commands:**
- Build/run `Matrix-ControlTests` (`PatchMutator|MutationHistory|MutatorAction`) -- expected: pass
- `python3 Scripts/quality/lint_touched.py` on touched C++ -- expected: clean

**Manual checks:**
- Mutate → INITIAL|sep|M00; select INITIAL → name/buttons; Compare round-trip; Mutate from INITIAL; Clear; Export `Initial.syx`.

## Suggested Review Order

**Engine selection SSOT**

- Gate for offering INITIAL and dual-state vs Compare lock.
  [`PatchMutatorEngine.cpp:114`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L114)

- Flat prev/next puts INITIAL first; leaving it restores M/R via preserved indices.
  [`PatchMutatorEngineHistory.cpp:114`](../../Source/Core/Services/PatchMutator/PatchMutatorEngineHistory.cpp#L114)

- Mutate/Retry/Delete/Compare refuse or clear INITIAL appropriately.
  [`PatchMutatorEngineActions.cpp:15`](../../Source/Core/Services/PatchMutator/PatchMutatorEngineActions.cpp#L15)

- Compare enter shows INITIAL; exit restores prior mutation/retry.
  [`PatchMutatorEngineActions.cpp:125`](../../Source/Core/Services/PatchMutator/PatchMutatorEngineActions.cpp#L125)

**HISTORY combo UI**

- Separator item kind (non-selectable) for the divider row.
  [`HierarchicalComboBox.cpp:69`](../../Source/GUI/Widgets/HierarchicalComboBox.cpp#L69)

- Build INITIAL + separator + Mxx; selection sync including Compare.
  [`PatchMutatorPanelHistory.cpp:200`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/PatchMutatorPanelHistory.cpp#L200)

- Compare grayed when INITIAL already selected; nav counts INITIAL.
  [`PatchMutatorPanelHistory.cpp:325`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/PatchMutatorPanelHistory.cpp#L325)

**Patch Name dual-line**

- Secondary `INITIAL` for Compare or manual INITIAL selection.
  [`PatchNameDisplayPanel.cpp:178`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditDisplaysPanel/Modules/PatchNameDisplayPanel.cpp#L178)

**Tests**

- Compare round-trip, Mutate from INITIAL, advance, Clear, button gates.
  [`PatchMutatorEngineCompareSnapshotTests.cpp:203`](../../Tests/Unit/PatchMutatorEngineCompareSnapshotTests.cpp#L203)
