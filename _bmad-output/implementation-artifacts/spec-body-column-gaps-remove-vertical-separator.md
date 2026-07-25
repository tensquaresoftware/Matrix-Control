---
title: 'Body column gaps — remove VerticalSeparator'
type: 'refactor'
created: '2026-07-25'
status: 'done'
baseline_commit: 'b37e1a1786bee0163e6d9cbea0a8cabade51868c'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** The Body’s two `VerticalSeparator` widgets only paint 4 px dark gutters (GUI `#151515`) inside 24 px slots. They no longer earn a dedicated widget class, and Body-level 12 px padding plus separator slots obscure a simpler three-column layout.

**Approach:** Delete `VerticalSeparator` entirely. Lay out Body as three full-height columns with `Spacing::kStandard` (4 px) gaps only. Move the former Body 12 px padding into each child panel so children keep today’s relative positions inside a padded content band. Accept GUI width **1308**.

## Boundaries & Constraints

**Always:**
- Outer sizes at scale 1.0: PatchEdit **832×728**, Shared **292×728**, MasterEdit **176×728**; Body **1308×728**; GUI width **1308**.
- Body: no edge padding; only two inter-column gaps of `Spacing::kStandard`.
- Each of the three columns: padding `Spacing::kLarge` (12) on all four sides; after that inset, existing child layout and relative positions stay as today (content widths 808 / 268 / 152; PatchEdit/MasterEdit content height 704; Shared stack height remains 700 inside the 704-tall content band).
- Inter-column gap token is `Spacing::kStandard`, not a separator line width atom.
- Remove the widget class files under `Source/GUI/Widgets/VerticalSeparator.*` plus all Body/test/skin/look/CMake wiring for them.
- Keep unrelated popup `drawVerticalSeparators` helpers.

**Ask First:**
- Any change that would stretch Shared’s internal module stack from 700 to fill 704 (beyond empty slack).
- Any change to Header/Footer chrome colours or column backgrounds beyond what the new bounds require.

**Never:**
- Reintroduce decorative vertical separator components or paint fake 4 px lines between columns.
- Change module/cell recipes or HorizontalSeparator behaviour.
- Force-translate or alter product panel names.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Scale 1.0 layout | Default UI scale | Columns 832 / 292 / 176, two 4 px gaps, Body 1308×728; content inset 12 from each column edge | N/A |
| UI scale ≠ 1 | Non-1.0 scale factor | Gaps and paddings scale via existing `ScaledLayout::scaledInt`; relative child layout unchanged | N/A |
| Shared height slack | Shared outer 728 with 12 pad | Content band 704; module stack still 700; 4 px empty at bottom of content band | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Layout/Design/DesignPanels.h` — Body/GUI width formulas; drop separator count; column outer sizes
- `Source/GUI/Layout/Design/DesignChecks.h` — asserts (`1300`→`1308`, separator/padding checks)
- `Source/GUI/Layout/Design/DesignAtoms.h` — delete `Atoms::Widths::VerticalSeparator`
- `Source/GUI/Layout/PanelDimensions.h` / `WidgetDimensions.h` — child `padding`; drop Body separators / unused Body padding fields
- `Source/GUI/Factories/DimensionFactory.cpp` — wire new outer sizes + paddings
- `Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.{h,cpp}` — three-column + gap layout only
- `Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditPanel.cpp` — inset then existing layout
- `Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/SharedPanel.cpp` — inset then existing layout
- `Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp` — inset then existing layout
- `Source/GUI/Widgets/VerticalSeparator.{h,cpp}` — delete
- `Source/GUI/Tests/TestVerticalSeparators.{h,cpp}` + `TestComponent.*` + `TestVisualDebug.h` — remove harness
- `Source/GUI/Looks/*` / `Source/GUI/Skins/*` — remove VerticalSeparator look/colour wiring
- `CMakeLists.txt` — drop deleted sources

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Layout/Design/DesignPanels.h` + `DesignChecks.h` + `DesignAtoms.h` -- Rewrite Body/GUI geometry to 1308 with `2 * Spacing::kStandard` gaps and column outer = content + `2 * Spacing::kLarge`; delete VerticalSeparator atoms/asserts -- SSOT for new sizes
- [x] `Source/GUI/Layout/PanelDimensions.h` + `WidgetDimensions.h` + `DimensionFactory.cpp` -- Add padding on three column dim structs; set outer widths/heights; remove Body `separators` / Body-level padding wiring -- dims match design
- [x] `Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.{h,cpp}` -- Remove separator members; place three panels with scaled `Spacing::kStandard` gaps, no Body edge inset -- Body matches approved layout
- [x] `…/PatchEditPanel.cpp` + `…/SharedPanel.cpp` + `…/MasterEditPanel.cpp` -- At start of `resized()`, inset by scaled column padding; keep existing relative child layout inside remaining bounds; `setSize` uses outer dims -- children keep relative positions
- [x] `VerticalSeparator.*` + looks/skins + `TestVerticalSeparators.*` + `TestComponent.*` + `TestVisualDebug.h` + `CMakeLists.txt` -- Delete widget and purge references -- no dead separator API left

**Acceptance Criteria:**
- Given scale 1.0, when the main window opens, then GUI width is 1308 and Body is three columns 832 / 292 / 176 separated by 4 px gaps with no `VerticalSeparator` instances.
- Given each column panel, when laid out, then content starts 12 px inset from all four edges and child controls keep the same relative arrangement as before (Shared module stack still 700 tall).
- Given a full build, when compiling the plugin target, then `VerticalSeparator` sources are gone and no references remain outside unrelated popup separator helpers.

## Spec Change Log

## Design Notes

Former separator slot was 24 = 10 + 4 + 10 (line centered). New model uses 12 + 4 + 12 per joint (`Spacing::kLarge` + `Spacing::kStandard` + `Spacing::kLarge`), adding +8 to window width (1300 → 1308). Body edge padding moves into columns so content origins shift with the panel, not with a second Body gutter.

Content constants (808 / 268 / 152, heights 704 / 700 / 704) stay the recipes for *inner* layout. Outer panel size = content + 2×12 (height for Shared outer is still 728; 4 px slack remains below the 700 stack inside the padded band).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: plugin target builds; no missing VerticalSeparator symbols
- Compile-time: `DesignChecks.h` static_asserts pass with `GUI::kWidth == 1308`

**Manual checks (if no CLI):**
- Visual: dark 4 px gutters between columns; Header/Footer align to 1308; no light/decorative separator widgets; column contents inset ~12 px from column edges

## Suggested Review Order

**Geometry SSOT**

- Inter-column gap and column padding tokens replace separator slots
  [`DesignPanels.h:17`](../../Source/GUI/Layout/Design/DesignPanels.h#L17)

- GUI width is outer columns plus two gaps (1308)
  [`DesignPanels.h:217`](../../Source/GUI/Layout/Design/DesignPanels.h#L217)

- Compile-time guard for the new width
  [`DesignChecks.h:10`](../../Source/GUI/Layout/Design/DesignChecks.h#L10)

**Body layout**

- Three columns with scaled gaps; no Body edge padding
  [`BodyPanel.cpp:71`](../../Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.cpp#L71)

- Gap strips painted with Header chrome to match former separator line
  [`BodyPanel.cpp:53`](../../Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.cpp#L53)

**Column insets**

- PatchEdit insets then keeps relative child stack
  [`PatchEditPanel.cpp:53`](../../Source/GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditPanel.cpp#L53)

- Shared insets; child width follows reduced area
  [`SharedPanel.cpp:51`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/SharedPanel.cpp#L51)

- MasterEdit same inset-first pattern
  [`MasterEditPanel.cpp:163`](../../Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp#L163)

**Cleanup**

- Widget/test/skin/CMake VerticalSeparator removal (deleted files + purge)
  [`CMakeLists.txt`](../../CMakeLists.txt)

- Test harness IDs preserved after enum gap
  [`TestComponent.h:72`](../../Source/GUI/Tests/TestComponent.h#L72)
