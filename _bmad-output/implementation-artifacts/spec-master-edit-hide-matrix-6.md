---
title: 'Hide MASTER EDIT column content on Matrix-6/6R'
type: 'feature'
created: '2026-09-05'
status: 'done'
baseline_commit: '175f3b50e85f49455887a9bb6e723fe507370e6a'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** On Matrix-6/6R, MASTER EDIT stays visible as a semi-transparent gray column. That reads as broken availability and clutters the UI.

**Approach:** Keep the MASTER EDIT column footprint and the opaque panel shell painted with Body background. Hide SectionHeader + Midi / Vibrato / Misc when Matrix-6/6R is detected; show them again on Matrix-1000 (including hot-swap). Do not destroy or recreate widgets.

## Boundaries & Constraints

**Always:**
- Preserve column width and Body/Footer band geometry.
- Keep Core gates unchanged (`isMasterEditAllowed`, MASTER SysEx outbound rules).
- React to existing APVTS properties (`deviceDetected`, device type, unresponsive, Compare).
- Hot-swap Matrix-6/6R ↔ Matrix-1000 toggles visibility without recreating the panel.
- Hidden state shows Body background (`kBodyPanelBackground`), not a gray overlay.

**Ask First:**
- Collapsing the column or changing Body/Footer widths.
- Destroying/recreating Master Edit widgets on device change.
- Updating PRD/epic FR-46 wording beyond this UI behaviour.

**Never:**
- Semi-transparent gray as the Matrix-6/6R MASTER EDIT treatment.
- Changing `GrayedControlHelper` (Bank Utility and others still need it).
- Teaching `CompareLockBinder`, `BodyPanel`, or `FooterPanel` about this hide rule.
- Making `MasterEditPanel` non-opaque without Body-background fill.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| M-6/6R connected | Detected Matrix-6/6R, section unlocked | Header + three modules hidden; shell paints Body background | N/A |
| Hot-swap to M-1000 | Type becomes Matrix-1000, plugin stays open | Four children visible and interactive again | N/A |
| Hot-swap back to M-6 | Type becomes Matrix-6/6R | Content hidden again | N/A |
| No synth / unknown / unresponsive / Compare | Section root-locked | Existing root lock unchanged; no child gray layer | N/A |
| M-1000 connected | Detected Matrix-1000 | Content visible; no hide | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.{h,cpp}` -- Replace gray α path with `setVisible` on `sectionHeader_`, `midiPanel_`, `vibratoPanel_`, `miscPanel_`; keep opaque shell + Body paint; keep `refreshDeviceGating` / ValueTree listeners; re-apply in `setSkin`.
- `Source/Core/MIDI/MasterEditGate.h` -- Pure `shouldHideMasterEditContent` (Matrix-6/6R + unlocked only); keep `isMasterEditAllowed` unchanged for SysEx.
- `Source/Core/MIDI/EditorOutboundGate.h` -- Read-only: `isSectionLocked` (rootLocked skip).
- `Source/GUI/Helpers/GrayedControlHelper.*` -- Do not change.
- `Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.cpp` -- Read-only column owner + CompareLockBinder.
- `Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp` -- Read-only; right band width independent of child visibility.
- `Source/Core/MIDI/MidiManager.cpp` -- Read-only device status writes for hot-swap.
- `Tests/Unit/DeviceAudioInputPreferenceTests.cpp` -- Core gate covered; no new GUI unit test.

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.h` -- Repurpose FR-46 child state from grayed to content-hidden; drop unused gray-only helpers.
- [x] `Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp` -- Hide/show the four children per gate; keep shell paint; give away focus when hiding; restore on hot-swap / `setSkin`.
- [x] `Source/Core/MIDI/MasterEditGate.h` + `Tests/Unit/DeviceAudioInputPreferenceTests.cpp` -- Pure `shouldHideMasterEditContent` + unit coverage for I/O matrix decision rows (GUI smoke remains manual).
- [ ] Manual smoke -- Matrix-6 hide, Matrix-1000 show, hot-swap both directions in Standalone.

**Acceptance Criteria:**
- Given detected Matrix-6/6R with section unlocked, when device gating refreshes, then MASTER EDIT shows only the Body-coloured shell — no header, modules, or semi-transparent controls.
- Given the plugin stays open, when device type hot-swaps to Matrix-1000, then header and modules reappear interactive without recreating the panel.
- Given no synth, unknown, unresponsive, or Compare, when root lock applies, then existing lock behaviour and column geometry remain unchanged.

## Spec Change Log

## Design Notes

Hide children, keep shell: `MasterEditPanel::paint` already fills `kBodyPanelBackground`, so invisible children reveal the correct colour without an overlay rectangle.

Keep today’s rootLocked skip: hide only when the section is unlocked. Predicate is Matrix-6/6R-explicit (not “any non–Matrix-1000”).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds
- Project unit-test run including `MasterEditGate` -- expected: pass

**Manual checks:**
- Standalone: Matrix-6 → blank Body-coloured MASTER EDIT column; Matrix-1000 → full column; reverse hot-swap; footer right band still aligns.

## Suggested Review Order

**Hide decision**

- Matrix-6/6R-only hide when unlocked; Core SysEx gate unchanged
  [`MasterEditGate.h:14`](../../Source/Core/MIDI/MasterEditGate.h#L14)

**UI binding**

- Hide SectionHeader + modules; keep Body-coloured shell; dismiss menus on hide
  [`MasterEditPanel.cpp:114`](../../Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp#L114)

- Same ValueTree props still drive hot-swap refresh
  [`MasterEditPanel.cpp:95`](../../Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp#L95)

**Tests**

- Unit coverage for hide / show / root-locked matrix rows
  [`DeviceAudioInputPreferenceTests.cpp:65`](../../Tests/Unit/DeviceAudioInputPreferenceTests.cpp#L65)
