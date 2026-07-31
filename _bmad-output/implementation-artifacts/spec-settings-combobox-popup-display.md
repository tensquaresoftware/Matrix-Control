---
organization: Ten Square Software
project: Matrix-Control
title: Spec — Settings ComboBox Popup Display
author: BMad Agent
type: bugfix
created: '2026-07-31'
status: done
baseline_commit: 'cdfaf538d920c62d6a7fc5f05ab54461b7a3ea01'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Opening any ComboBox in the Settings overlay shows a muddy brown rectangle instead of a readable item list, so Settings policies and Patch Name Display cannot be chosen reliably.

**Approach:** Wire Settings ComboBoxes with the same skin-derived popup menu look used elsewhere (closed combo look alone is not enough), including on skin refresh, so Standard popups paint with correct background and text colours.

## Boundaries & Constraints

**Always:**
- Apply `popupMenuLookFromSkin` to all four Settings ComboBoxes (name reconciliation, unsaved-edit warning, mutator-delete warning, Patch Name Display).
- Refresh popup look in `setSkin` whenever combo look is refreshed.
- Keep Standard popup styling consistent with other Standard ComboBoxes (e.g. Header MIDI).

**Ask First:**
- Changing shared popup infrastructure (`PopupMenuRenderer`, `PopupMenuBase`, default `PopupMenuLook` construction) instead of wiring Settings like other panels.

**Never:**
- Redesign Settings layout, policies, or Patch Name Display behaviour.
- Switch Settings ComboBoxes to ButtonLike style or WidgetFactory solely for this bug.
- Change cream/black skin colour tokens unless the wired look still fails after the Settings fix.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Open policy combo | Settings open; click Name Reconciliation / Unsaved Edit / Mutator Delete combo | Popup lists all items with readable text on Standard background; hover highlight works | Escape closes popup; Settings stays open |
| Open Patch Name Display | Settings open; MATRIX-1000 section; click Patch Name Display combo | Same readable Standard popup with both modes | Escape closes popup |
| Skin switch | Settings open; switch Black ↔ Cream; reopen a combo | Popup colours match active skin | N/A |
| Compare Header | Settings closed; open a Header MIDI ComboBox | Still works as today (no regression) | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Settings/SettingsPanel.cpp` -- constructs four `TSS::ComboBox` with `comboBoxLookFromSkin` only; `setSkin` refreshes combo look but never `setPopupMenuLook`
- `Source/GUI/Settings/SettingsPanel.h` -- ComboBox members
- `Source/GUI/Factories/WidgetFactory.cpp` -- reference pattern: `setPopupMenuLook(popupMenuLookFromSkin(skin))` after combo creation
- `Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp` -- same pattern in ctor + `setSkin`
- `Source/GUI/Looks/LookBuilders.cpp` / `LookBuilders.h` -- `popupMenuLookFromSkin`
- `Source/GUI/Widgets/ComboBox.{h,cpp}` -- `setPopupMenuLook`; Standard style opens `MultiColumnPopupMenu`
- `Source/GUI/Widgets/PopupMenuRenderer.cpp` / `PopupMenuBase.cpp` -- paint path (read-only unless Ask First triggers)
- `Source/GUI/Settings/SettingsWindow.cpp` -- full-bleed overlay + dim scrim (context for muddy “brown” when popup colours are wrong)

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Settings/SettingsPanel.cpp` -- after each Settings ComboBox is constructed (or in a small helper), call `setPopupMenuLook(TSS::popupMenuLookFromSkin(skin))`; in `setSkin`, build `popupLook` once and apply to all four ComboBoxes alongside combo look -- Settings is the only panel that builds Standard ComboBoxes without popup look wiring
- [x] Manual verify Black + Cream: all four Settings popups show readable items; Header MIDI popup unchanged

**Acceptance Criteria:**
- Given Settings is open, when any of the four ComboBoxes is opened, then the popup shows all items with readable Standard colours (not a blank/brown rectangle).
- Given Settings is open and the skin changes, when a ComboBox is opened again, then the popup colours match the active skin.
- Given a Header MIDI ComboBox, when its popup is opened, then behaviour and look remain unchanged.

## Spec Change Log

## Design Notes

Settings ComboBoxes are built directly (not via `WidgetFactory`), so they missed the u-13 `setPopupMenuLook` convention. Default `PopupMenuLook{}` leaves transparent/unset colours; with `PopupMenuBase` opaque over the cream/scrim, that reads as a brown empty rect.

Golden pattern (match Header / WidgetFactory):

```cpp
combo.setPopupMenuLook(TSS::popupMenuLookFromSkin(skin));
```

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds

**Manual checks (if no CLI):**
- Settings → open each of the four ComboBoxes (Black and Cream): green Standard menu, readable items, hover.
- Header MIDI ComboBox popup still correct.
- Escape dismisses popup then Settings as today.

## Suggested Review Order

- Ctor wires skin-derived popup look on all four Settings ComboBoxes after items are added.
  [`SettingsPanel.cpp:112`](../../Source/GUI/Settings/SettingsPanel.cpp#L112)

- setSkin refreshes the same popup look whenever combo looks are refreshed.
  [`SettingsPanel.cpp:216`](../../Source/GUI/Settings/SettingsPanel.cpp#L216)
