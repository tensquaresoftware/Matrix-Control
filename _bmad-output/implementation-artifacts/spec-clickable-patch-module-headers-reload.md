---
title: 'Clickable Internal/Computer module headers reclaim focus and reload'
type: 'feature'
created: '2026-09-05'
status: 'done'
baseline_commit: 'f2f0d743bf8c8b5e66ee3e0bf2fbe1bdb1f49476'
review_loop_iteration: 0
context:
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Navigation focus often sits on the other Patch Manager source, so the GUI shows a synth memory patch while Computer Patches still displays a `.syx` name (or the reverse). There is no honest one-click way to reclaim that module and reload what its UI already shows.

**Approach:** Make **INTERNAL PATCHES** and **COMPUTER PATCHES** titles clickable (hover → focus-red). A click sets that module’s navigation focus and, when a loadable selection exists, reloads through existing load paths. If nothing is loadable, only reclaim focus. Leave Bank Utility, Next/Previous, and combo open-on-click unchanged.

## Boundaries & Constraints

**Always:**
- Only **INTERNAL PATCHES** and **COMPUTER PATCHES** titles are clickable; Bank Utility / Patch Mutator headers stay inert.
- Click → `setNavigationFocus` (`kInternal` / `kComputer`).
- Computer: reload only when selected file id ≥ 1 via existing `handleLoadSelectedPatchFile` settle path (unsaved-edit + Mutator history gates).
- Internal: reload only when patch coordinates are established via existing `loadCurrentPatchFromDevice` (same gates).
- If not loadable: focus only; no error toast for that empty case.
- Hover: title text uses `kNumberBoxTextFocus` (same token as NumberBox / Computer combo focus).
- Fire via new APVTS timestamp action IDs through ActionDispatcher — do not bypass confirmation gates with a panel-only load call.
- English-only in source; existing title strings unchanged.

**Ask First:**
- Changing Next/Previous, closed-combo open behavior, or Bank Utility header interactivity.
- Skipping/weakening unsaved-edit gates on header reload.
- Enabling title click on ModuleHeaders outside these two Patch Manager modules.

**Never:**
- Hijack Previous/Next to mean “reload current”.
- Special-case “click closed combo without opening”.
- Include BANK UTILITY or Patch Mutator headers in this gesture.
- Invent a parallel load pipeline.
- Auto-reload on unrelated focus changes.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Computer reclaim + reload | Focus ≠ Computer; selectedId ≥ 1; click COMPUTER PATCHES | Focus → Computer; `.syx` reloads; combo focus-red | Existing unsaved/history gate; cancel aborts like other Computer loads |
| Computer focus only | selectedId &lt; 1; click title | Focus → Computer; no load | Silent |
| Internal reclaim + reload | Coordinates established; click INTERNAL PATCHES | Focus → Internal; current slot loads from device | Same gates as other internal loads |
| Internal focus only | Coordinates not established; click title | Focus → Internal; no device load | Silent |
| Already focused + loadable | Focus already that module; loadable; click title | Focus unchanged; reload still runs | Same gates |
| Hover / leave | Pointer enter/exit clickable title | Text focus-red on enter; restore on exit | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Widgets/ModuleHeader.{h,cpp}` — title paint-only today; add **opt-in** click + hover (default off; keep `WithActionsSpec` intact).
- `InternalPatchesPanelSetup.cpp` (~41–50) + `InternalPatchesPanel.cpp` — enable header; fire new action; NumberBox focus-red unchanged.
- `ComputerPatchesPanel.cpp` (`setupModuleHeader` ~386–395; focus highlight ~130–207) — same; sentinels id `0` / `<EMPTY!>` / `<SELECT>`.
- `PluginIDs.h` (~550–636) + `ActionPropertyRegistry.cpp` (~81–95) — two new header-click action IDs → PatchManager.
- `PatchManagerActionHandler.cpp` (`handleAction` ~194–214) — route new actions.
- `PatchManagerActionHandlerInternalPatches.cpp` — reuse `setNavigationFocus` + device load when coordinates established.
- `PatchManagerActionHandlerComputerLoadSave.cpp` — reuse `handleLoadSelectedPatchFile`; id &lt; 1 stays silent no-op.
- `PatchManagerActionHandlerDeviceLoad.cpp` (~201–228) — device dump entry for internal reload.
- Colour: `SkinColourId::kNumberBoxTextFocus` (same as Bank Utility current-bank marker / Computer focus combo).

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Widgets/ModuleHeader.{h,cpp}` -- Opt-in title click + hover focus-red; default off -- title was paint-only
- [x] `PluginIDs.h` + `ActionPropertyRegistry.cpp` -- Add/register Internal/Computer header-click action IDs -- dispatcher path
- [x] `PatchManagerActionHandler*.cpp` -- set focus; reload via existing paths when loadable, else focus-only -- single pipeline
- [x] `InternalPatchesPanel*.cpp` + `ComputerPatchesPanel.cpp` -- Enable headers; fire timestamp actions -- UI entry
- [x] Lint + build smoke -- `lint_touched.py` + debug build; walk I/O matrix manually -- gates/sentinels

**Acceptance Criteria:**
- Given a real `.syx` selected but focus is Internal, when user clicks **COMPUTER PATCHES**, then focus becomes Computer, combo is focus-red, and that file reloads (subject to existing confirmation).
- Given Computer empty/select sentinel, when user clicks **COMPUTER PATCHES**, then focus only; no load.
- Given Internal coordinates established but focus is Computer, when user clicks **INTERNAL PATCHES**, then focus becomes Internal and the current slot reloads from the device (same gates).
- Given Internal coordinates not established, when user clicks **INTERNAL PATCHES**, then focus only; no device load.
- Given hover on a clickable title, then text is focus-red; on leave, normal colour returns.
- Given Bank Utility / Patch Mutator headers or Previous/Next / Computer combo, when used, then today’s semantics stay unchanged.

## Spec Change Log

## Design Notes

Header click is the honest “this module owns navigation” control. Prefer APVTS timestamp actions so reload shares `confirmPatchContextChange` with combo/Prev/Next/NumberBox. Bank Utility’s focus-red bank marker remains display-only — its ModuleHeader stays inert.

## Verification

**Commands:**
- `python3 Scripts/quality/lint_touched.py` -- expected: clean on touched C++
- `cmake --build --preset macos-debug-arm64` -- expected: success (or current macOS debug preset)

**Manual checks:**
- Internal focus + Computer `.syx` selected → click **COMPUTER PATCHES** → red combo + reload.
- Computer `<SELECT>`/`<EMPTY!>` → click title → focus only.
- Computer focus + Internal coords set → click **INTERNAL PATCHES** → red NumberBoxes + device reload.
- Hover titles; Bank Utility title unchanged; unsaved policy still prompts on header reload.

## Suggested Review Order

**Handler — focus + reload**

- Entry point: set focus, then reload only when loadable (else silent).
  [`PatchManagerActionHandlerHeaderClick.cpp:8`](../../Source/Core/Actions/PatchManagerActionHandlerHeaderClick.cpp#L8)

- Routes new header actions ahead of bank/file handlers.
  [`PatchManagerActionHandler.cpp:204`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L204)

**UI — clickable titles**

- Opt-in title click; left-button only; hover uses focus-red colour.
  [`ModuleHeader.cpp:199`](../../Source/GUI/Widgets/ModuleHeader.cpp#L199)

- Internal header fires timestamp action on click.
  [`InternalPatchesPanelSetup.cpp:50`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/InternalPatchesPanelSetup.cpp#L50)

- Computer header same wiring.
  [`ComputerPatchesPanel.cpp:395`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/ComputerPatchesPanel.cpp#L395)

- Skin maps title hover to NumberBox focus-red token.
  [`LookBuilders.cpp:243`](../../Source/GUI/Looks/LookBuilders.cpp#L243)

**IDs & registry**

- New Internal/Computer header-click action IDs.
  [`PluginIDs.h:614`](../../Source/Shared/Definitions/PluginIDs.h#L614)

**Tests**

- Handler matrix: focus-only vs reload, already-focused, gate cancel.
  [`PatchManagerActionHandlerHeaderClickTests.cpp:16`](../../Tests/Unit/PatchManagerActionHandlerHeaderClickTests.cpp#L16)

- Dispatcher routes both header-click IDs to Patch Manager.
  [`ActionDispatcherTests.cpp:129`](../../Tests/Unit/ActionDispatcherTests.cpp#L129)
