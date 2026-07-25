---
organization: Ten Square Software
project: Matrix-Control
title: Story V1.1 — Unsaved Navigation Consistency
author: BMad Agent
status: done
baseline_commit: 9ff5531
sources:
  - implementation-artifacts/deferred-work.md
  - implementation-artifacts/9-2-unsaved-edit-confirmation-dialog.md
  - implementation-artifacts/spec-9-1-dirtypatchtracker.md
  - implementation-artifacts/spec-computer-patches-open-auto-select-first.md
  - implementation-artifacts/spec-mutator-synth-load-history-export-compare.md
  - implementation-artifacts/epic-9-context.md
  - planning-artifacts/prds/prd-matrix-control-2026-05-25/prd.md
  - planning-artifacts/epics.md
  - project-context.md
created: 2026-07-25
updated: 2026-07-25
---

# Story V1.1: Unsaved Navigation Consistency

Status: done

<!-- Ultimate context engine analysis completed - comprehensive developer guide created -->

## Story

As a sound designer travelling without Matrix hardware,
I want Continue / Cancel on unsaved edits, Open, patch changes, and failed synth dumps to leave the UI and edit buffer coherent,
so that I never see a desynced list, a silent overwrite, or a false dirty flag before V1 ships (FR-51 hardening).

## Brownfield Reality (Read First)

**Epic 9 is done.** Stories `9-1-dirtypatchtracker` and `9-2-unsaved-edit-confirmation-dialog` shipped DirtyPatchTracker, FR-51 Cancel/Continue, Settings always/never warn, snapshot refresh on successful load/save/STORE, and Computer Cancel restore for OPEN + combo. This story is **V1 deferred-work hardening**, not a rewrite of FR-51.

| Area | Status after 9.2 |
|------|------------------|
| FR-51 modal (Cancel / Continue) + Settings policy | **Done** — preserve |
| Gate order: dirty → Mutator history | **Done** — preserve |
| OPEN Cancel restores prior folder + scan + selection | **Done** for the happy path |
| Device dump after Continue when dump unavailable / fails | **Broken UX** — coords advanced, buffer stale |
| Edits between Continue and async dump apply | **Silent overwrite** then clean snapshot |
| `lastCommittedComputerPatchesSelectedId_` bootstrap `0` | **Edge desync** on early Cancel |
| Name-reconciliation Cancel after history Discard | **History unrestorable** — document or harden |
| PatchModel name 7-bit encode vs 6-bit decode | **False dirty risk** after dump/APVTS round-trip |

**Validation constraint (travel):** no Matrix hardware UAT and no audible synth requirement. Prove behaviour with **unit tests + Matrix-Simulator** (Device Inquiry / ports) where useful. Patch dump success/failure must be covered by **injectable MidiManager / async callback fakes** — Matrix-Simulator does **not** emulate full patch dumps today.

## Acceptance Criteria

1. **Given** unsaved edits and Settings **warn always**  
   **When** the user Continues on FR-51 (and optionally Discards Mutator history) then navigates Internal Patches (Prev/Next, NumberBox, or bank) and the device dump is unavailable, times out, returns empty/wrong size, or otherwise fails  
   **Then** the user is **not** left believing the new bank/patch was loaded  
   **And** Internal Patches bank/patch UI coordinates roll back to the pre-navigation values  
   **And** the edit buffer keeps the pre-navigation content (no partial apply)  
   **And** dirty state remains true for those unsaved edits (Continue did not successfully load a replacement)  
   **And** Mutator history is **not** cleared by the failed dump (`onPatchLoaded` only on success — already true; keep)  
   **And** a clear footer warns that the synth dump failed  
   **And** no hardware audible UAT is required — unit tests with fake dump availability / empty callback suffice.

2. **Given** the user Continued on FR-51 and an async device dump is still pending  
   **When** they edit the patch before the dump callback applies  
   **Then** the dump must **not** silently overwrite those edits and mark the patch clean  
   **And** the product policy (locked below) is: treat the pending load as aborted — restore pre-navigation bank/patch coordinates, keep the mid-window edits, keep/reassert dirty, publish a clear footer (or re-prompt FR-51 once if the implementation prefers a second confirm; silent apply is forbidden)  
   **And** the message thread stays non-blocking (no return to synchronous dump waits).

3. **Given** Computer Patches OPEN (new folder auto-selects first file) or combobox / Prev/Next file selection  
   **When** the user Cancels FR-51 **or** Cancels the Mutator history gate  
   **Then** the combo/list (and OPEN folder + scan when applicable) restore to the last **committed** browser state matching the still-loaded edit buffer  
   **And** the list must not remain on a file that was never loaded  
   **And** bootstrap: if no successful load has committed an id yet, Cancel must not invent a wrong revert target (seed `lastCommittedComputerPatchesSelectedId_` from the current non-sentinel selection / scan baseline before navigation, or equivalent).

4. **Given** dirty Continue + Mutator history Discard, then Computer file load reaches name reconciliation (D-025)  
   **When** the user Cancels reconciliation  
   **Then** packed model / APVTS name are restored (already true) and Computer selection / OPEN browser abort as today  
   **And** Mutator history behaviour is **predictable and documented in this story’s Dev Notes**: either (preferred implementation) history Discard is deferred until load fully succeeds (reconcile accept + apply), **or** history remains discarded with an explicit English Dev Note + Completion Note stating the limit is accepted for V1  
   **And** no silent half-applied APVTS state.

5. **Given** a packed patch whose name bytes are Matrix 6-bit wire codes (not display ASCII)  
   **When** the clean snapshot is captured after load/dump/sync (or name is normalized on `loadFrom` / capture path)  
   **Then** a subsequent APVTS name round-trip without user rename must **not** mark the patch dirty  
   **And** full-buffer dirty compare (bytes 0–7 included) remains the FR-51 foundation — do not “ignore name” in DirtyPatchTracker  
   **And** a unit regression locks the 6-bit → display/canonical path.

6. **And** Core ↛ GUI remains: no new `AlertWindow` in Core; reuse existing PluginEditor gate callbacks if a second FR-51 prompt is needed.

7. **And** `Matrix-Control_Tests` pass; macOS Debug build clean; English-only source; no French in code or dialogs.

## Locked Product Decisions (do not re-litigate)

| Topic | Decision | Why |
|-------|----------|-----|
| Dump fail after Continue | **Roll back** Internal bank/patch coordinates; keep buffer; keep dirty; footer | User must not think they advanced when the patch never reloaded |
| Mid-dump edits | **No silent overwrite**; abort pending load + roll back coords + keep edits/dirty (+ footer or second FR-51) | Matches “no silent crush” intent |
| Mutator history on failed dump | Keep today’s rule: clear only via `onPatchLoaded` after **successful** dump | Already correct; do not clear on fail |
| History vs reconcile Cancel | Prefer **defer Discard until load success**; else document unrestorable history as accepted V1 limit | Predictable > surprising empty history |
| False dirty name | **Normalize codec / capture path**; keep full memcmp including name | Fix root cause, don’t weaken tracker |
| Modal style | Match existing FR-51 / history `AlertWindow` + `JUCE_MODAL_LOOPS_PERMITTED` | Async modal migration out of scope |
| Hardware UAT | **Not required** for this story | Travel / Matrix-Simulator + unit fakes |
| PASTE null PatchNameSyncer skew | **Out of scope** (separate 9.2 defer) | Keep this story focused on navigation consistency |
| INIT clean-on-fallback | **Out of scope** (intentional 9.2 baseline) | Not a navigation desync |

## Tasks / Subtasks

- [x] **T1 — Device dump failure / timeout consistency** (AC: #1, #6, #7)
  - [x] Stash pre-navigation bank/patch (and any NumberBox-owned mirror) before `applyPatchCoordinates` / NumberBox commit paths that call `loadCurrentPatchFromDevice`
  - [x] On `!isDeviceDumpAvailable`, empty/wrong-size dump, or async failure: restore stashed coordinates, do not apply dump, do not `captureCleanSnapshot`, do not call `onPatchLoaded`, keep dirty, publish dump-failure footer
  - [x] Cover Prev/Next, bank button, and NumberBox entry points (processor path must participate)
  - [x] Unit tests with fake MidiManager: unavailable dump; async empty/wrong-size callback after Continue

- [x] **T2 — Async settle-window edits** (AC: #2, #6, #7)
  - [x] Introduce a pending-load generation / epoch (handler or MidiManager cancel token + handler flag)
  - [x] On dump callback: if user edited after Continue (dirty vs post-Continue baseline, or generation mismatch), abort apply per locked policy
  - [x] Ensure late callbacks from superseded navigations are ignored
  - [x] Unit test: Continue → mutate param before callback → callback must not leave clean overwritten state

- [x] **T3 — Computer Cancel / OPEN selection integrity** (AC: #3, #7)
  - [x] Seed / maintain `lastCommittedComputerPatchesSelectedId_` so early Cancel revert targets are correct
  - [x] Verify Mutator history Cancel uses the same `abortComputerPatchesNavigation` restore path as FR-51 Cancel (OPEN + combo)
  - [x] Tests: bootstrap Cancel before first successful remember; OPEN Cancel after history Discard/Cancel; regression for existing `unsavedGate_openCancelRestoresPriorBrowser` / `unsavedGate_prevNextCancelDoesNotDoubleLoad`

- [x] **T4 — Reconcile Cancel vs Mutator Discard predictability** (AC: #4, #7)
  - [x] Implement deferred history Discard until load success **or** document accepted limit in Dev Agent Completion Notes + short comment near gate
  - [x] If deferring: move `resetSessionForPatchLoad` from gate Discard/Export success to post-apply success paths (device dump success + Computer apply); Cancel at reconcile must leave history intact
  - [x] Test with multi-stage gate fake when deferring; otherwise document-only + assert model restore still holds

- [x] **T5 — Name encode/decode false dirty** (AC: #5, #7)
  - [x] Normalize name bytes on load/capture (prefer `PatchModel` / `loadFrom` / `captureCleanSnapshot` path) so 6-bit wire form does not diverge after `getName`→`setName`
  - [x] Keep `captureCleanSnapshot` sync-before-capture
  - [x] Unit regression: packed 6-bit name codes → sync/capture → `syncApvtsAndIsDirty == false` without user rename

- [x] **T6 — Verification** (AC: #7)
  - [x] `Matrix-Control_Tests` green
  - [x] Optional Standalone smoke with Matrix-Simulator for inquiry/ports only — not a dump proof
  - [x] Update deferred-work bullets for closed items when story completes (dev-story / review)

## Dev Notes

### Epic / V1 context

Implementation sprint (Epics 0–11, U, T) is complete. This is the first **V1 priority deferred-work** story: harden FR-51 navigation consistency before release. Upstream: Epic 9 (`9-1`, `9-2`), Epic 4 Computer Patches, Epic 7 Patch Manager handlers, Mutator synth-load history gate.

[Source: `deferred-work.md` — 9-2 / 9-1 / computer-patches-open / mutator synth-load sections]

### Current code to extend (UPDATE — do not fork FR-51)

**FR-51 composition** (`PluginProcessor.cpp`):

- `confirmUnsavedEditGateIfNeeded` — dirty + policy + modal callback
- `confirmPatchContextChangeGate` — dirty first, then Mutator history (Cancel / Discard / Export)
- History Discard currently calls `patchMutatorEngine_->resetSessionForPatchLoad()` **immediately** on gate success (~964–966)

**Handler navigation** (`PatchManagerActionHandler.cpp`):

| Path | Flow today |
|------|------------|
| Internal Prev/Next | gate → `applyPatchCoordinates` → `loadCurrentPatchFromDevice` (~134–149) |
| Bank button | gate → bank props + MIDI → `loadCurrentPatchFromDevice` |
| NumberBox | processor gate → MIDI → `loadCurrentPatchFromDevice` |
| OPEN | snapshot prior browser → scan → select id 1 → load (~375–423) |
| Computer select | resolve → gate → reconcile → apply; Cancel → `abortComputerPatchesNavigation` (~486–526) |
| Dump | early exit if `!isDeviceDumpAvailable` (~796–799); async callback size check (~811–814); success → push + `captureCleanSnapshot` + `onPatchLoaded` (~820–830) |

**Computer Cancel helpers:** `pendingBrowserRestoreOnCancel_`, `abortComputerPatchesNavigation`, `rememberComputerPatchesSelection`, `lastCommittedComputerPatchesSelectedId_` (default `0`).

**Name codec** (`PatchModel.cpp`): `getName` uses 6-bit decode + remap; `setName` stores 7-bit ASCII. Asymmetry creates false dirty after hardware-style dumps if snapshot captured on raw wire bytes then APVTS round-trips.

**Capture helper:** `captureCleanSnapshot` already syncs mapper + `PatchNameSyncer` before `captureSnapshot` — keep; extend with name normalization if needed.

### What must be preserved

1. Cancel / Continue only on FR-51 — no SAVE/STORE in modal.
2. Core ↛ GUI — modals only via processor gate callbacks in PluginEditor.
3. Gate order dirty → history unless T4 explicitly defers Discard side effects.
4. Settings never-warn skips FR-51 when dirty.
5. OPEN Cancel restore via `pendingBrowserRestoreOnCancel_`.
6. Computer Prev/Next Cancel must not double-load (`nextId == beforeId` force-load rule only).
7. Failed dump must not call `onPatchLoaded`.
8. Non-blocking async dump (no message-thread wait).
9. STORE captures clean only when outbound allowed.

### Anti-patterns (will fail review)

1. Moving FR-51 after dump request (gate must stay before load intent).
2. Silent dump apply over mid-window edits + `captureCleanSnapshot`.
3. Leaving advanced bank/patch UI after dump failure.
4. Clearing Mutator history on dump failure.
5. Ignoring name in DirtyPatchTracker memcmp.
6. `AlertWindow` / GUI includes in Core.
7. Requiring Matrix hardware audible UAT as AC.
8. Expanding scope to PASTE name-syncer skew or INIT fallback baseline.
9. Rewriting all modals to `showAsync` in this story.
10. French in source or user-visible strings.

### Previous story intelligence (9.2)

- Patched in review: OPEN Cancel browser restore; Prev/Next Cancel double-load; load fail selection revert; STORE outbound guard; Settings policy clamp.
- Explicitly deferred into this V1 story: dump fail after Continue; settle-window edits; reconcile Cancel vs Discard; `lastCommitted` bootstrap; broader harness gaps.
- Tests to extend: `Tests/Unit/PatchManagerActionHandlerTests.cpp` (`unsavedGate_*`), `Tests/Unit/DirtyPatchTrackerTests.cpp`, optionally `PatchModel` name codec tests.

### Git intelligence

- `53c107f` — Add FR-51 unsaved-edit confirmation and close story 9.2
- `984aefd` — Add DirtyPatchTracker Core service
- Recent mainline: Epic U release gate closed (`9ff5531`) — V1 deferred work starts after full implementation sprint

### Latest tech notes (JUCE 8.0.12)

- Prefer `AlertWindow::showAsync` / `showScopedAsync` for **new** designs; this repo already uses modal loops for FR-51 / history / reconciliation with `JUCE_MODAL_LOOPS_PERMITTED=1`. **Match existing gates**; do not migrate all modals here.
- Always assert message thread before modal loops (existing pattern).
- Async dump callbacks: use generation tokens / SafePointer patterns already used elsewhere in MidiManager; ignore stale callbacks.

### Project structure notes

| Action | Path |
|--------|------|
| UPDATE | `Source/Core/Actions/PatchManagerActionHandler.{h,cpp}` |
| UPDATE | `Source/Core/PluginProcessor.{h,cpp}` (NumberBox + optional deferred history Discard) |
| UPDATE | `Source/Core/Models/PatchModel.{h,cpp}` and/or capture/load normalization |
| UPDATE | `Source/Core/MIDI/MidiManager.*` only if pending-load cancel/generation needs a seam |
| KEEP | `Source/Core/Services/DirtyPatchTracker.*` — tiny helpers OK; do not drop name from compare |
| UPDATE | `Tests/Unit/PatchManagerActionHandlerTests.cpp` |
| UPDATE | `Tests/Unit/DirtyPatchTrackerTests.cpp` and/or `PatchModel` tests |
| UPDATE | `CMakeLists.txt` only if new test `.cpp` |
| AVOID | GUI panel logic for dirty gates; Epic 8 device lock rewrite; PASTE handler scope creep |

### Testing requirements

- Prefer Core/unit tests with injectable MidiManager / gate fakes over GUI automation.
- Required coverage matrix:
  - Continue + dump unavailable → coords restored, dirty true, no `onPatchLoaded`
  - Continue + async empty dump → same
  - Continue + edit before callback → no silent clean overwrite
  - OPEN / combo Cancel (FR-51 and history) → browser matches loaded buffer
  - Bootstrap Cancel before first commit id
  - 6-bit name false-dirty regression
  - Existing 9.2 Cancel / capture regressions still green
- Matrix-Simulator: optional inquiry/port smoke only; **not** a substitute for dump unit fakes.

### References

- [Source: `deferred-work.md` — code review of 9-2-unsaved-edit-confirmation-dialog]
- [Source: `deferred-work.md` — code review / quick-dev of spec-9-1-dirtypatchtracker]
- [Source: `deferred-work.md` — computer-patches-open-auto-select-first Cancel desync]
- [Source: `deferred-work.md` — mutator-synth-load dump settle / async notes]
- [Source: `9-2-unsaved-edit-confirmation-dialog.md` — Review Findings deferrals]
- [Source: `spec-9-1-dirtypatchtracker.md` — name encode asymmetry]
- [Source: `prd.md` — FR-51]
- [Source: `epic-9-context.md`]
- [Source: `project-context.md` — Core↛GUI, English source, test expectations]

### Review Findings

- [x] [Review][Patch] NumberBox dump-fail leaves Banks Locked / Mutator load context on the failed target — Stash `banksLocked` (and restore `PatchLoadContext`) from true pre-navigation state before `syncSelection` / `updateDevicePatchLoadContext`; today `captureInternalCoordinates` runs after those side effects so rollback keeps the post-NumberBox lock and export context. [PluginProcessor.cpp:1687-1720] [PatchManagerActionHandler.cpp:904-913,799-807]
- [x] [Review][Patch] Pending Internal dump not cancelled on Computer / INIT / PASTE — `clearPendingDeviceLoad` + `cancelPendingSysExRequest` on those success (and apply) paths so a late dump cannot overwrite the new buffer or `failPendingDeviceLoad`-restore Internal coordinates. [PatchManagerActionHandler.cpp:269-310,313-342,685-691,915-991]
- [x] [Review][Patch] Null model/mapper in dump callback clears pending without coordinate rollback — Use `failPendingDeviceLoad` instead of `clearPendingDeviceLoad` so advanced bank/patch UI is restored with the dump-failure footer. [PatchManagerActionHandler.cpp:955-959]
- [x] [Review][Patch] Computer combo Cancel bootstrap incomplete vs AC3 — OPEN/Prev/Next seed before writing the new id; combo enters `handleLoadSelectedPatchFile` after the id already changed, so Cancel can still revert to committed `0`. Track a stable pre-change selection (or equivalent) for combo Cancel. [PatchManagerActionHandler.cpp:493-533,858-866]
- [x] [Review][Patch] Missing regressions for claimed coverage — Add tests for bank-button dump fail rollback, NumberBox prior overload / lock+context restore, Discard→reconcile Cancel history intact, and Computer/INIT while dump pending. [PatchManagerActionHandlerTests.cpp]
- [x] [Review][Patch] Dump fake defaults `available=true` with full InitDefaults response — Older navigation tests that previously hit real MidiManager dump-unavailable now silently succeed dumps and fire `onPatchLoaded`; make dump success opt-in per test. [PatchManagerActionHandlerTests.cpp:256-365]
- [x] [Review][Defer] Mid-window abort treats any buffer memcmp delta as user edit — deferred, pre-existing strictness; host automation / SysEx echo during settle can false-abort a good dump (safer than silent overwrite for V1)
- [x] [Review][Defer] `normalizeNameEncoding` not applied on PluginProcessor ctor / DAW-restore snapshot paths — deferred, pre-existing residual; AC5 load/capture dump path is covered

## Dev Agent Record

### Agent Model Used

Composer (Cursor agent)

### Debug Log References

### Implementation Plan

- Pending device-load generation + prior Internal coordinates stash on Prev/Next, bank, and NumberBox paths.
- Injectable dump seams on `ActionExecutionHooks` for unit fakes (unavailable / empty / deferred callback).
- Mid-window edit detection via memcmp of buffer-at-request vs current model after APVTS sync.
- Deferred Mutator history clear: gate Discard/Export no longer call `resetSessionForPatchLoad`; `onPatchLoaded` remains the success path.
- `PatchModel::normalizeNameEncoding` + capture/load call sites for 6-bit wire names.

### Completion Notes List

- Dump unavailable / empty / wrong-size after Continue rolls back bank/patch (and NumberBox prior), keeps dirty buffer, skips `onPatchLoaded` / clean snapshot, shows dump-failure footer.
- Edits between Continue and dump callback abort apply, restore coords, keep mid-window edits + dirty, show abort footer.
- Computer Cancel bootstrap seeds `lastCommittedComputerPatchesSelectedId_` before OPEN and Prev/Next file navigation; OPEN Cancel still uses `pendingBrowserRestoreOnCancel_`.
- History Discard deferred until successful load (`onPatchLoaded`); reconcile Cancel leaves Mutator history intact.
- 6-bit wire name false-dirty fixed via `normalizeNameEncoding` (full memcmp including name retained).
- `Matrix-Control_Tests` green; macOS Debug plugin build clean. Deferred-work 9.2/9.1 bullets marked resolved for this story’s scope. PASTE name-syncer skew and INIT clean-on-fallback remain out of scope.

### File List

- Source/Core/Actions/ActionExecutionHooks.h
- Source/Core/Actions/PatchManagerActionHandler.h
- Source/Core/Actions/PatchManagerActionHandler.cpp
- Source/Core/Models/PatchModel.h
- Source/Core/Models/PatchModel.cpp
- Source/Core/PluginProcessor.cpp
- Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/ComputerPatchesPanel.cpp
- Source/Shared/Definitions/PluginDisplayNames.h
- Source/Shared/Definitions/PluginIDs.h
- Tests/Unit/DirtyPatchTrackerTests.cpp
- Tests/Unit/PatchManagerActionHandlerTests.cpp
- Tests/Unit/PatchModelTests.cpp
- _bmad-output/implementation-artifacts/deferred-work.md
- _bmad-output/implementation-artifacts/sprint-status.yaml
- _bmad-output/implementation-artifacts/v1-1-unsaved-navigation-consistency.md

## Change Log

- 2026-07-25: Story created from deferred-work V1 priority lot (unsaved navigation consistency); status ready-for-dev.
- 2026-07-25: Implemented dump rollback, settle-window abort, Computer Cancel bootstrap, deferred Mutator Discard, name normalize; status review.
- 2026-07-25: Code review patches applied (NumberBox lock/context restore, abandon pending dump on Computer/INIT/PASTE, combo Cancel baseline, regressions); status done.
