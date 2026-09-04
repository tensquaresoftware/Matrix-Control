---
title: 'Patch nav focus + bank current marker'
type: 'feature'
created: '2026-09-04'
status: 'done'
baseline_commit: 'e67a7769f13bcacd07471c0979e40a8d3a03185f'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-bank-utility-copy-paste.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-internal-patch-startup-coordinates.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Users cannot tell whether INTERNAL PATCHES or COMPUTER PATCHES currently drives patch navigation; Bank Utility still uses red text for “selected bank,” and startup still shows `0`/`00` as if the synth were synced. The old NumberBox lock-dot (bank unlocked/locked) is obsolete after UNLOCK removal — banks are always within-bank from the plugin.

**Approach:** Introduce an explicit navigation-focus owner (none / internal / computer). Use red text only for that focus (Internal bank+patch NumberBoxes, or Computer combo selection), matching PATCH NAME red. Mark the current Matrix-1000 bank on Bank Utility buttons with a DEVICE-badge-style fill (normal text, never red). Startup shows undefined `-`/`--` in normal colour; first bank select, Internal Prev/Next, or successful OPEN establishes coordinates. On OPEN when undefined (M-1000), set destination `0`/`00` + Set Bank without loading patch 00 into the editor, then send the `.syx`. Keep Internal CurrentBankNumber always laid out; on Matrix-6/6R gray it empty. Remove lock-dot UI and rename lock-centric symbols toward current/selected-bank semantics; leave Master Edit `miscBankLockEnable` and Unlock SysEx opcode alone.

## Boundaries & Constraints

**Always:**
- Red = navigation focus only (Internal NumberBoxes **or** Computer combo text when that owner is active; never Bank Utility button text).
- Bank Utility current bank = DEVICE-badge fill (reuse footer info badge fill / dark text pattern); only one bank marked; none at undefined startup.
- Startup / session restore: Internal shows `-` (M-1000) or gray-empty bank box (6/6R), patch `--`, normal colour, no bank marker, nav focus = none; no MIDI from this reset.
- Bank button click (M-1000): set current bank marker, load that bank’s patch `00`, nav focus = internal (red `N`/`00`).
- First Internal Prev/Next while undefined (M-1000): auto bank `0` + load `00`, marker on `0`, nav focus = internal. 6/6R: establish patch `00` (or advance from undefined) without bank UI.
- Successful OPEN with valid files while coordinates undefined (M-1000): establish `0`/`00` in UI + marker on `0` + Set Bank as needed **without** loading device patch 00 into the editor, then load first `.syx`; nav focus = computer (red combo). If coordinates already defined, keep them. 6/6R: no bank; establish patch destination `00` if still undefined, then load `.syx`.
- Switching focus: last navigation source wins (Internal Prev/Next/bank vs Computer OPEN/Prev/Next/combo select). Non-focus side returns to normal text colour but keeps last coordinates.
- M-1000 patch wrap stays within the current bank (no plugin unlock / cross-bank wrap).
- English UI/code only. `NumberBox` lock-dot API removed if unused elsewhere.

**Ask First:** Changing Patch Manager module heights; making Computer combo red when empty/`<EMPTY!>`; auto-selecting bank on SAVE/STORE instead of OPEN; touching Master Edit BANK LOCK.

**Never:** Restore UNLOCK UI; use red for Bank Utility selection; hide CurrentBankNumber (use gray-empty on 6/6R); load device patch 00 into the editor on OPEN-before-`.syx`; change `miscBankLockEnable` / Master BANK LOCK; French strings; redesign Bank COPY/PASTE/IMPORT/EXPORT semantics.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Cold start M-1000 | No nav yet | Bank `-`, patch `--`, normal colour; no bank marker; focus none | N/A |
| Cold start M-6/6R | No nav yet | Bank box gray empty; patch `--` normal; focus none | N/A |
| Bank button 3 | M-1000 undefined or other | Marker on 3; load 3/00; Internal red `3`/`00`; Computer normal | Unsaved confirm as today |
| Prev/Next first | M-1000 undefined | Marker on 0; load 0/00; Internal red | Unsaved confirm as today |
| OPEN first | M-1000 undefined, valid folder | Marker on 0; coords `0`/`00` normal; Set Bank; no device-00 editor load; `.syx` loads; Computer red | Empty/unusable folder: no coord establish; focus stays none |
| OPEN with bank already set | Bank 2 marked | Keep bank 2 / current patch coords; load `.syx`; Computer red | N/A |
| Nav back Internal | After Computer focus | Internal red; Computer combo normal; bank marker unchanged | N/A |
| Nav to Computer | After Internal focus | Computer red; Internal normal; bank marker unchanged | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Panels/.../BankUtilityPanel.cpp` — replace `refreshSelectedBankHighlight` red text (~251–271) with badge-fill current-bank look; keep `kSelectedBank`
- `Source/GUI/Panels/.../BankUtilityPanelSetup.cpp` — bank button setup; apply/clear fill on selection
- `Source/GUI/Panels/.../InternalPatchesPanel.{h,cpp}` + `InternalPatchesPanelSetup.cpp` — drop `refreshBankLockIndicator` / `setShowDot`; placeholder `-`/`--`; focus colour; M-6 gray-empty bank box (stop hide-only)
- `Source/GUI/Widgets/NumberBox.{h,cpp}` — remove dot paint/`setShowDot`; add undefined/placeholder + normal-vs-focus text colour
- `Source/GUI/Looks/LookBuilders.cpp` + `SkinColoursWidgetsDisplays.h` — NumberBox no longer always-red; optional focus colour; drop `kNumberBoxDot` if unused
- `Source/GUI/Panels/.../ComputerPatchesPanel.cpp` — combo text red when computer owns focus (today green `comboBoxLookFromSkin`)
- `Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp` — DEVICE badge fill/text (~122–142) as visual reference for bank marker
- `Source/Core/PluginProcessorParameters.cpp` — `resetInternalPatchCoordinatesToDefaults` (~408–427): undefined display state, not min bank/patch as “defined”
- `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` — bank select + Prev/Next; first-nav establish `0`/`00`; set nav focus internal
- `Source/Core/Actions/PatchManagerActionHandlerComputerBrowser.cpp` — `handleOpenPatchFolder`: undefined→establish dest without device-00 editor load; focus computer
- `Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp` — file Prev/Next / select → focus computer
- `Source/Core/Actions/PatchManagerActionHandler.cpp` — `markBanksLockedInApvts` / wrap: rename or always within-bank; sync naming with `kBanksLocked` consumers
- `Source/Core/Services/DeviceMemoryLimits.cpp` — `advancePatch` locked wrap remains the M-1000 plugin policy
- `Source/Shared/Definitions/PluginIDs.h` — add nav-focus state id; rename `kBanksLocked` if still needed for “coords established” / wrap, else simplify
- `Source/Core/MIDI/PatchSelectionMidiSync.*` — Set Bank on OPEN establish; do not dump/load patch 00 into editor
- `Tests/Unit/PatchManagerActionHandlerBasicTests.cpp` (+ related) — startup undefined; first nav; OPEN establish; rename lock tests; leave SysEx unlock opcode tests if present
- Read-only: Master Edit `miscBankLockEnable`; Bank COPY/PASTE transfer semantics (`spec-bank-utility-copy-paste.md`)

## Tasks & Acceptance

**Execution:**
- [x] `PluginIDs.h` (+ DisplayNames if needed) -- add nav-focus state; rename/retire lock-centric patch-manager bank lock symbols to match “current/selected bank / within-bank nav” -- clear product language in code
- [x] `NumberBox.{h,cpp}` + looks/skins -- remove lock-dot; support placeholder `-`/`--` and normal vs focus (red) text -- display undefined + focus
- [x] `InternalPatchesPanel*` -- wire placeholders, focus colours, M-6 gray-empty bank box; delete lock-dot refresh -- Internal side of UX
- [x] `BankUtilityPanel*` -- DEVICE-badge current-bank marker; remove red selected text -- bank of work always readable
- [x] `ComputerPatchesPanel.cpp` -- red combo text when computer focus -- Computer side of UX
- [x] `PluginProcessorParameters.cpp` (+ ctor/state callers) -- startup/session reset to undefined nav/coords -- no false `0`/`00`
- [x] `PatchManagerActionHandler*` + `PatchSelectionMidiSync` -- bank/first-PrevNext/OPEN establish rules + focus owner updates -- Core behaviour
- [x] `DeviceMemoryLimits` / handler wrap paths -- M-1000 always within-bank from plugin; clean lock naming -- match abandoned UNLOCK
- [x] `Tests/Unit/PatchManagerActionHandler*.cpp` -- cover I/O matrix establish/focus/OPEN-no-device-00-load; update lock-named tests -- prevent regression
- [x] `Scripts/quality/lint_touched.py` -- pass on touched C++ -- quality gate (2 pre-existing LIGHT ctor-param findings on NumberBox / TestNumberBoxes; no new gate blockers)

**Acceptance Criteria:**
- Given cold start on Matrix-1000, when UI shows, then bank `-`, patch `--`, normal colour, no bank marker, no nav-focus red.
- Given cold start on Matrix-6/6R, when UI shows, then bank NumberBox is gray and empty (no layout hole), patch `--` normal.
- Given undefined M-1000, when user selects bank N, then badge marker is on N, patch `00` loads from device, Internal NumberBoxes are red with `N`/`00`.
- Given undefined M-1000, when user presses Internal Next/Prev, then bank `0` is marked, `00` loads, Internal is red.
- Given undefined M-1000 and a valid OPEN, when first `.syx` loads, then destination is `0`/`00` with marker on `0`, device patch 00 was not loaded into the editor, Computer combo text is red, Internal text is normal.
- Given Computer focus, when user navigates Internal (or selects a bank), then red moves to Internal and Computer returns to normal colour (bank marker follows last bank of work).
- Given Bank Utility, when a bank is current, then that button uses badge fill + dark/normal text — not red label text.
- Given codebase search, when looking for Internal lock-dot / UNLOCK GUI / `setShowDot` usage for bank lock, then those UX paths are gone; Master Edit BANK LOCK untouched.

## Design Notes

Nav focus is a single owner (none/internal/computer). Bank marker is independent: it answers “where would STORE / bank COPY write,” not “who is browsing.” NumberBoxes today are int + always-red skin — placeholder and dual colours are required GUI work. OPEN establish must not call the device-load path used by bank select; only Set Bank + APVTS coords + marker, then existing `.syx` apply.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: success
- `ctest --test-dir Builds/macOS/...` (or project unit-test target for PatchManagerActionHandler*) -- expected: pass
- `python3 Scripts/quality/lint_touched.py` -- expected: clean on touched C++

**Manual checks:**
- M-1000: cold start `-`/`--` → bank select → Internal red → OPEN folder → Computer red + badge stays → Internal Next → red returns Internal
- M-1000: cold start → OPEN first → marker on 0, no audible/device patch-00 takeover before `.syx`
- M-6/6R: bank box gray empty; patch nav focus still works; Bank Utility gray as today

## Suggested Review Order

**Navigation focus state**

- Single owner of red focus (none / internal / computer)
  [`PluginIDs.h:551`](../../Source/Shared/Definitions/PluginIDs.h#L551)

- First Internal Prev/Next establishes slot + bank marker + Internal focus
  [`PatchManagerActionHandlerInternalPatches.cpp:136`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L136)

- OPEN pins Set Bank without establishing until `.syx` commits
  [`PatchManagerActionHandlerComputerBrowser.cpp:10`](../../Source/Core/Actions/PatchManagerActionHandlerComputerBrowser.cpp#L10)

- Successful computer load marks established and Computer focus
  [`PatchManagerActionHandlerComputerLoadSave.cpp:73`](../../Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp#L73)

- Startup/session reset leaves coordinates undefined and focus none
  [`PluginProcessorParameters.cpp:414`](../../Source/Core/PluginProcessorParameters.cpp#L414)

**GUI: placeholders, badge, red**

- NumberBox undefined/unavailable display + focus colour
  [`NumberBox.cpp:133`](../../Source/GUI/Widgets/NumberBox.cpp#L133)

- Internal NumberBoxes follow established + focus
  [`InternalPatchesPanel.cpp:327`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/InternalPatchesPanel.cpp#L327)

- Bank Utility DEVICE-badge current-bank marker
  [`BankUtilityPanel.cpp:251`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L251)

- Computer combo red when Computer owns focus
  [`ComputerPatchesPanel.cpp:130`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/ComputerPatchesPanel.cpp#L130)

**Tests**

- Undefined first Next/Prev, bank select, focus switch, OPEN establish
  [`PatchManagerActionHandlerBasicTests.cpp:391`](../../Tests/Unit/PatchManagerActionHandlerBasicTests.cpp#L391)
