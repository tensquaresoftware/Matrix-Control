---
title: 'NumberBox disabled background matches greyed ButtonLike'
type: 'bugfix'
created: '2026-09-05'
status: 'done'
route: 'plan-code-review'
baseline_commit: '39a91212beb4bc9c09c100bcd9d7e3f45024785e'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** On Matrix-6/6R, CURRENT BANK is emptied and disabled, but its fill looks darker (~#101010) than other greyed controls such as the Computer Patches OPEN ButtonLike combo (~#151515), because the NumberBox paints black and then gets a 0.5 alpha gray overlay.

**Approach:** Give NumberBox a disabled background token from the shared ColourChart (same Grey1 as Button / ButtonLike disabled), paint that fill when `!isEnabled()`, and stop alpha-graying the bank NumberBox so the palette alone defines the look.

## Boundaries & Constraints

**Always:**
- Disabled NumberBox fill uses `ColourChart::kDarkGrey1` (`0xFF151515`) via SkinColours — same chart entry as `Button::kBackgroundDisabled` / `ComboBox::ButtonLike::kBackgroundDisabled`.
- Enabled NumberBox fill stays black via an explicit NumberBox background ColourElement (no longer borrow ButtonBackgroundOn at LookBuilder time only).
- When NumberBox paints disabled colours from skin tokens, do not also apply `GrayedControlHelper::applyGrayedAppearance` on that same control.
- Keep cream/black variants coherent: disabled Grey1 is identical on both variants.

**Ask First:**
- Changing disabled NumberBox text or border colours beyond the fill (border already switches via `isEnabled()`; empty unavailable state has no text).
- Broad removal of GrayedControlHelper from other widgets unrelated to NumberBox.

**Never:**
- Hardcoding `#151515` / `0xFF151515` outside ColourChart / SkinColours.
- Changing Matrix-6 bank enablement / empty display logic (when the box clears or disables).
- Redesigning enabled NumberBox edit/focus-red behaviour.
- French strings in source; new IDs stay English.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Matrix-6/6R bank unavailable | `hasBankConcept=false` → empty + `setEnabled(false)` | Fill `#151515` (Grey1); no alpha overlay; text empty | N/A |
| Matrix-1000 bank available | Bank NumberBox enabled with value | Fill black; alpha 1.0; value readable | N/A |
| Any NumberBox `setEnabled(false)` | Disabled with or without value | Fill uses NumberBox disabled token (Grey1), not black+alpha | N/A |
| No double gray | Disabled bank box after fix | Skin fill only — not Grey1 + 0.5 alpha | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Skins/ColourChart.h` -- `kDarkGrey1` = `0xFF151515` (SSOT hex)
- `Source/GUI/Skins/SkinColoursWidgetsControls.h` -- `Button::kBackgroundDisabled` already Grey1 (reference)
- `Source/GUI/Skins/SkinColoursWidgetsSelection.h:82-86` -- ButtonLike disabled BG Grey1 (OPEN combo reference)
- `Source/GUI/Skins/SkinColoursWidgetsDisplays.h` -- NumberBox tokens today (text/editor only); add `kBackground` + `kBackgroundDisabled`
- `Source/GUI/Skins/SkinValues.h` / `Skin.cpp` (`initializeDisplayColours`) -- add `kNumberBoxBackground` / `kNumberBoxBackgroundDisabled` and map
- `Source/GUI/Looks/WidgetLooks.h` -- `NumberBoxLook`; add `backgroundDisabled`
- `Source/GUI/Looks/LookBuilders.cpp:214-224` -- today `background` from `kButtonBackgroundOn`; wire NumberBox BG tokens
- `Source/GUI/Widgets/NumberBox.cpp` -- paint fill: disabled → `backgroundDisabled`, else `background` (edit focus red unchanged); `enablementChanged` repaints
- `Source/GUI/Panels/.../InternalPatchesPanel.cpp` -- `kUnavailable` + `setEnabled(false)` only (GrayedControlHelper removed from bank box)
- `Source/Core/Services/DeviceMemoryLimits.cpp:23-33` -- Matrix-6/6R `hasBankConcept=false` (read-only context)
- `Tests/Unit/NumberBoxDisabledBackgroundColourTests.cpp` -- palette parity Grey1 / black (covers matrix colour identity without paint())
- Prior intent (NumberBox was excluded): `spec-disabled-controls-look.md` Never list — this spec extends that convention to NumberBox fill only

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/Skins/SkinColoursWidgetsDisplays.h` -- add NumberBox `kBackground` (black) and `kBackgroundDisabled` (Grey1) ColourElements -- palette SSOT for this widget
- [x] `Source/GUI/Skins/SkinValues.h`, `Skin.cpp` -- register and initialize the new SkinColourIds
- [x] `Source/GUI/Looks/WidgetLooks.h`, `LookBuilders.cpp` -- add `backgroundDisabled`; map both backgrounds from NumberBox skin IDs
- [x] `Source/GUI/Widgets/NumberBox.cpp` -- paint fill from `backgroundDisabled` when `!isEnabled()`, else `background` (keep edit-mode focus fill as today)
- [x] `Source/GUI/Panels/.../InternalPatchesPanel.cpp` -- remove `applyGrayedAppearance` on `currentBankNumber` (keep `setEnabled` / `setDisplayState`)
- [x] `Tests/Unit/NumberBoxDisabledBackgroundColourTests.cpp` (+ `CMakeLists.txt`) -- assert NumberBox disabled BG == Button/ButtonLike Grey1; enabled BG == black
- [ ] Manual GUI check -- Matrix-6/6R CURRENT BANK empty disabled fill matches OPEN ButtonLike grey; Matrix-1000 bank box unchanged when enabled

**Acceptance Criteria:**
- Given Matrix-6/6R connected (no bank concept), when CURRENT BANK is empty and disabled, then its fill matches ButtonLike disabled Grey1 (`#151515`) with no alpha dimming.
- Given Matrix-1000 (or any device with banks), when CURRENT BANK is enabled, then fill remains black and readable.
- Given any disabled NumberBox, when painted, then disabled fill comes from SkinColours NumberBox tokens, not a literal hex and not black+alpha.

## Spec Change Log

## Design Notes

Composite today: black fill × 0.5 alpha over ~Grey2 panel ≈ `#101010`. ButtonLike paints solid Grey1 with no alpha — match that pattern, do not invent a new grey.

Prefer duplicating the Grey1 chart reference on NumberBox ColourElements (same pattern as Button / ButtonLike) over inventing a new Common token, unless an existing Common Grey1 disabled-bg alias already exists (it does not — `Common::kBackgroundDisabled` is Grey3).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- build succeeds
- `python3 Scripts/quality/lint_touched.py` -- clean on touched C++ under `Source/`

**Manual checks (if no CLI):**
- Simulator or device as Matrix-6/6R: CURRENT BANK empty, fill visually matches Computer Patches OPEN combo when that combo is disabled/empty.
- Matrix-1000: CURRENT BANK shows bank digit(s) on black fill as today.

## Suggested Review Order

**Palette SSOT**

- NumberBox disabled fill reuses ColourChart Grey1 like Button / ButtonLike
  [`SkinColoursWidgetsDisplays.h:19`](../../Source/GUI/Skins/SkinColoursWidgetsDisplays.h#L19)

- Register and map the new NumberBox background SkinColourIds
  [`Skin.cpp:318`](../../Source/GUI/Skins/Skin.cpp#L318)

**Paint and look wiring**

- LookBuilder maps NumberBox backgrounds from skin (no Button borrow)
  [`LookBuilders.cpp:217`](../../Source/GUI/Looks/LookBuilders.cpp#L217)

- Disabled fill when `!isEnabled()`; edit focus red unchanged
  [`NumberBox.cpp:156`](../../Source/GUI/Widgets/NumberBox.cpp#L156)

- Dismiss editor and repaint on disable
  [`NumberBox.cpp:179`](../../Source/GUI/Widgets/NumberBox.cpp#L179)

**Bank unavailable path**

- Empty + disabled without alpha gray; force opaque for solid skin fill
  [`InternalPatchesPanel.cpp:344`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/InternalPatchesPanel.cpp#L344)

**Tests**

- Assert NumberBox Grey1 / black parity with Button and ButtonLike
  [`NumberBoxDisabledBackgroundColourTests.cpp:20`](../../Tests/Unit/NumberBoxDisabledBackgroundColourTests.cpp#L20)
