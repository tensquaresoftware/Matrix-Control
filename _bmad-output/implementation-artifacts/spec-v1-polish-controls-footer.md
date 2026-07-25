---
title: 'V1 polish — controls & footer (ComboBox stuck, ROM footer, Bank Utility lock)'
type: 'bugfix'
created: '2026-07-25'
status: 'done'
baseline_commit: 'a79ba58d72f227de8444ba92f2e339c9fba4072f'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/deferred-work.md'
  - '{project-root}/_bmad-output/implementation-artifacts/8-3-ui-lock-without-synth.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Four deferred UX defects remain without a synth: ComboBox can stay logically open after a failed popup; the ROM paste/store footer can stick after leaving a ROM bank; ROM guidance may miss if the cursor is already over a gated button when the bank becomes ROM; Bank Utility on Matrix-6/6R stays clickable while grayed instead of locking like the body when no device / unsupported device is present.

**Approach:** Fix only these four — ComboBox open notify after popup show guards (Hierarchical order), clear/refresh ROM footer on block transitions, and lock Bank Utility when grayed (dim + no interaction) like body device lock; publish/clear Matrix-1000-only footer on gray enter/leave, not on click.

## Boundaries & Constraints

**Always:**
- ComboBox: `isPopupOpen_` ends false if `ScrollablePopupMenu::show` / `MultiColumnPopupMenu::show` early-returns. Notify open only after those guards (Hierarchical order).
- ROM footer: blocked true→false → exact-string clear of `kRomBankPasteStoreFooterMessage` + empty severity.
- ROM hover: when block turns on (or rewire while blocked), if Init/Paste/Store already mouse-over → show ROM footer without new `mouseEnter`.
- Bank Utility grayed (detected, no bank concept): dim module + block mouse/keyboard on subtree (body device-lock / `CompareLockBinder` `applySectionLock` idea). No bank/unlock while grayed.
- Bank Utility footer: gray false→true publish `kMatrix1000OnlyFooterMessage`; true→false exact-string clear. Not via click handlers.
- Verify Standalone; Matrix-Simulator for device-type / ROM banks. No hardware synth.

**Ask First:**
- Changing ROM / Matrix-1000-only footer wording or severity.
- Broader ComboBox popup refactor beyond open/fail ordering.
- Using Master Edit’s “panel click still explains” hybrid instead of body-style lock + auto footer.

**Never:**
- Epic U layout refactors or U-10 ±1 px work.
- Keeping Bank Utility clickable-while-grayed, or `setInactiveAppearance` while staying clickable (rejected).
- Changing Paste/Store/Init ROM rules, `hasBankConcept` predicate, or whole-body Compare/device lock itself.
- French UI strings.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| ComboBox show fails | 0 items or null top-level | `isPopupOpen_` false; later clicks work | No crash |
| Leave ROM bank | Blocked → allowed, ROM footer present | Exact ROM footer cleared | Leave other footers |
| Enter ROM, cursor on button | ROM while pointer over Init/Paste/Store | ROM footer shown | N/A |
| Bank Utility M-6/6R | Detected, no banks | Dimmed, not interactive; Matrix-1000-only footer on | No bank/unlock |
| Leave Bank Utility gray | Grayed → not grayed | Interactive again; Matrix-1000-only cleared if present | N/A |
| Bank Utility M-1000 | Device with banks | Normal interactive + selection accent | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/Widgets/ComboBox.cpp` -- open notify before `show` today
- `Source/GUI/Widgets/ScrollablePopupMenu.cpp` / `MultiColumnPopupMenu.cpp` -- early returns
- `Source/GUI/Widgets/HierarchicalPopupMenu.cpp` -- guards then `notifyPopupOpened`
- `Source/GUI/Panels/.../InternalPatchesPanel.cpp` -- ROM hover / update / footer
- `Source/GUI/Panels/.../BankUtilityPanel.cpp` -- gray path + click-to-footer today
- `Source/GUI/Helpers/CompareLockBinder.cpp` -- `applySectionLock` golden
- `Source/GUI/Helpers/GrayedControlHelper.*` -- alpha / footer helpers
- `Tests/Unit/PatchManagerActionHandlerTests.cpp` -- ROM footer set (panel GUI separate)

## Tasks & Acceptance

**Execution:**
- [x] `ComboBox.cpp` + `ScrollablePopupMenu.cpp` + `MultiColumnPopupMenu.cpp` -- open notify after show guards
- [x] `InternalPatchesPanel.cpp` -- ROM footer clear on unblock; show ROM footer if mouse already over gated buttons
- [x] `BankUtilityPanel.cpp` -- grayed = module lock (dim + no interaction); remove clickable-gray; sync Matrix-1000-only footer on gray enter/leave
- [x] `deferred-work.md` -- mark four bullets resolved (Bank Utility supersedes old clickable-alpha note)
- [x] Targeted tests and/or Standalone + Matrix-Simulator checks

**Acceptance Criteria:**
- Given ComboBox `show` early-returns, when async open finishes, then `isPopupOpen_` is false and a later click can open.
- Given ROM footer showing and user leaves ROM bank, when gating refreshes, then that exact ROM message clears.
- Given cursor already over Init/Paste/Store when bank becomes ROM-blocked, when gating refreshes, then ROM footer appears without mouse leave/enter.
- Given Matrix-6/6R detected, when Bank Utility is grayed, then module is dimmed and not interactive, and Matrix-1000-only footer shows without a click.
- Given leaving that grayed state (or Matrix-1000 with banks), when gating refreshes, then Bank Utility is interactive again and Matrix-1000-only footer clears if present; Matrix-1000 keeps selected-bank accent.

## Design Notes

ComboBox: notify open inside both `::show` after `canShowPopup()` + null top-level checks.

Bank Utility: child-level dim + no mouse/keyboard (Master Edit pattern). Do not set panel-level alpha/intercepts — `CompareLockBinder` owns those for device/Compare lock on the same root. Skip child gray while section root is locked to avoid stacked alpha. Auto Matrix-1000-only footer on gray enter/leave only.

ROM / Bank Utility footer clears: exact-string ownership only.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` -- expected: green for touched/added tests

**Manual checks:**
- Standalone + Matrix-Simulator: Paste + ROM enter/leave footer; Matrix-6/6R Bank Utility locked + auto footer; Matrix-1000 normal; ComboBox failed-show code-path / test if practical.

## Suggested Review Order

**Bank Utility lock (compose with CompareLockBinder)**

- Skip child gray when section root already locked; gray only Matrix-6/6R while interactive.
  [`BankUtilityPanel.cpp:95`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L95)

- Child dim + block mouse/keyboard; do not touch panel alpha/intercepts.
  [`BankUtilityPanel.cpp:114`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L114)

- Shared child gray helper (alpha + intercepts + keyboard).
  [`BankUtilityPanel.cpp:38`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L38)

- Exact-string clear of Matrix-1000-only footer on leave gray.
  [`BankUtilityPanel.cpp:196`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L196)

**ROM footer clear + stationary hover**

- Clear ROM footer on unblock; show if cursor already over gated buttons.
  [`InternalPatchesPanel.cpp:333`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/InternalPatchesPanel.cpp#L333)

- Exact-string ROM footer clear helper.
  [`InternalPatchesPanel.cpp:32`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/InternalPatchesPanel.cpp#L32)

**ComboBox stuck-open fix**

- Open notify only after show guards (Hierarchical order).
  [`ScrollablePopupMenu.cpp:283`](../../Source/GUI/Widgets/ScrollablePopupMenu.cpp#L283)

- Same guard + notify order for multi-column popups.
  [`MultiColumnPopupMenu.cpp:336`](../../Source/GUI/Widgets/MultiColumnPopupMenu.cpp#L336)

- Async open no longer notifies before show.
  [`ComboBox.cpp:118`](../../Source/GUI/Widgets/ComboBox.cpp#L118)

**Ledger**

- Deferred bullets marked resolved for these four polish items.
  [`deferred-work.md:1`](./deferred-work.md#L1)
