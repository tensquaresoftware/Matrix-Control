---
title: 'Footer column bands aligned with Body'
type: 'feature'
created: '2026-07-25'
status: 'done'
baseline_commit: 'bfcb2351843ebe64e4a2f8bab4e3af9add50e922'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** The footer is a flat strip: device identity sits in a narrow right slot that does not line up with the Master Edit column, and status messages are not framed like the Body’s three-column rhythm.

**Approach:** Paint three grey bands in the footer that match the Body columns’ widths, gaps, and panel background colour. Put status messages in the left band, leave the centre empty, and centre device identity (type + firmware) in the right band.

## Boundaries & Constraints

**Always:**
- Band fill colour = `SkinColourId::kBodyPanelBackground` (same as Master Edit / Body column panels).
- Footer shell background remains `kFooterPanelBackground` (visible in the 4 px vertical gutters and the inter-column gaps).
- Horizontal geometry matches Body left→right: Patch Edit panel width → inter-column gap → Shared (Matrix) panel width → inter-column gap → Master Edit panel width, using the same design constants and `ScaledLayout::scaledInt` as `BodyPanel::resized`.
- Each band is 24 design px tall, vertically centred in the 32 px footer (4 px empty above and below).
- Left band: severity icon + user message (existing APVTS footer message behaviour).
- Centre band: painted empty (no content).
- Right band: device identity text centred in the band (`Justification::centred`).
- Keep English-only source strings; no global AffineTransform UI scale.

**Ask First:**
- Changing footer total height away from 32 design px.
- Adding permanent content to the centre band.
- Introducing a new skin colour dedicated to footer bands (instead of reusing `kBodyPanelBackground`).

**Never:**
- Realign or resize Body columns to “fix” the footer.
- Keep driving identity layout from `kIdentityMinWidth` (160) once bands exist — identity uses the full Master Edit band width.
- Put messages or identity outside their respective band bounds.
- French text in source or UI chrome for this change.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Happy path | Device detected + identity string; status message present | Three bands aligned with Body; message in left; centre empty; identity centred in right | N/A |
| No message | Empty message or severity None | Left band still painted; no icon/text | N/A |
| No device | Not detected / empty identity | Right band still painted; no identity text | N/A |
| Long identity | Long type+version string | Fitted/clipped inside right band (single line); stays centred | No overflow into centre gap/band |
| Long message | Long status text | Fitted inside left band (single line) with icon if any | No overflow into centre gap/band |
| UI scale ≠ 1 | `uiScale` 1.25 / 1.5 / 2 | Band widths/gaps/height/gutters scale via same policy as Body columns | Pixel alignment stays consistent with Body |

</frozen-after-approval>

## Code Map

- `Source/GUI/Layout/Design/DesignPanels.h` -- Footer height 32; add/confirm band height 24 + vertical gutter; Body `kPanelWidth` / `kInterColumnGap` SSOT
- `Source/GUI/Layout/PanelDimensions.h` -- Extend `FooterPanelDimensions` with column widths, gap, band height (drop reliance on identity min-width for layout)
- `Source/GUI/Factories/DimensionFactory.cpp` -- Populate new footer dimension fields from Design panels
- `Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp` -- Paint three bands; layout message left / identity centred right
- `Source/GUI/Panels/MainComponent/BodyPanel/BodyPanel.cpp` -- Reference only: column order and scaled width sequence to mirror
- `Source/GUI/Skins/SkinValues.h` -- Reuse `kBodyPanelBackground` / `kFooterPanelBackground` (no new ID unless Ask First)

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Layout/Design/DesignPanels.h` -- Add footer band height (24) and vertical inset (4) as named constants (or derive inset from `kHeight - bandHeight`) -- keep SSOT with Body gap rhythm
- [x] `Source/GUI/Layout/PanelDimensions.h` + `DimensionFactory.cpp` -- Carry Patch Edit / Shared / Master Edit panel widths + inter-column gap + band height into `FooterPanelDimensions`; stop using `identityMinWidth` for band layout (remove or leave unused only if a caller still needs it — prefer remove if unused)
- [x] `Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp` -- Fill footer background; paint three `kBodyPanelBackground` bands with Body-aligned geometry; draw message (+ icon) inside left band with modest inner padding; centre identity in right band; leave centre empty
- [x] Manual visual check -- Confirm at scale 1.0 that band edges line up with Body column edges and that 4 px gutters read like Body column gaps

**Acceptance Criteria:**
- Given the plugin UI at design scale 1.0, when the footer paints, then three bands of 24 px height sit with 4 px empty above and below, coloured like Body panels, with gaps matching Body inter-column gaps.
- Given Body column edges, when comparing footer band edges, then left/centre/right bands align with Patch Edit / Shared / Master Edit panel widths respectively.
- Given a status message, when the footer paints, then icon+text appear only inside the left band.
- Given a detected device identity string, when the footer paints, then the text is horizontally and vertically centred in the right band.
- Given no centre-band content requirement, when the footer paints, then the centre band is filled but empty of widgets/text.
- Given UI scale changes, when layout refreshes, then band geometry scales consistently with Body columns.

## Spec Change Log

## Design Notes

Vertical math is intentional: `Footer::kHeight` (32) = 4 + 24 + 4, mirroring Body’s 4 px inter-column gap language.

Horizontal paint order must mirror `BodyPanel::resized`:

```text
[ PatchEdit kPanelWidth ][ gap 4 ][ Shared kPanelWidth ][ gap 4 ][ MasterEdit kPanelWidth ]
```

Inner text padding inside bands may reuse footer `kPadding` or a smaller inset; keep text fully inside the band. Do not invent a fourth colour — Body panel grey is the band fill.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds

**Manual checks (if no CLI):**
- Open plugin UI: footer shows three grey strips under the three Body columns; messages left; empty centre; centred identity right; 4 px gutters above/below bands and between bands match the Body gap look.

## Suggested Review Order

**Band geometry**

- Design SSOT: 24 px band height and 4 px vertical inset inside the 32 px footer
  [`DesignPanels.h:208`](../../Source/GUI/Layout/Design/DesignPanels.h#L208)

- Paint three Body-aligned bands, then message left and identity centred right
  [`FooterPanel.cpp:33`](../../Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp#L33)

**Dimensions wiring**

- Footer dimensions carry column widths, gap, band height, and vertical inset
  [`PanelDimensions.h:39`](../../Source/GUI/Layout/PanelDimensions.h#L39)

- Factory maps Body/Footer design constants into those fields
  [`DimensionFactory.cpp:435`](../../Source/GUI/Factories/DimensionFactory.cpp#L435)

**Compile-time locks**

- Asserts keep band height, inset, and footer height math honest
  [`DesignChecks.h:19`](../../Source/GUI/Layout/Design/DesignChecks.h#L19)
