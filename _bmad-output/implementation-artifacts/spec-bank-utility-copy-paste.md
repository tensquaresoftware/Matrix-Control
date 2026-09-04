---
title: 'Bank Utility layout + bank COPY/PASTE'
type: 'feature'
created: '2026-09-04'
status: 'done'
baseline_commit: 'e9d5afc697ace0984113b3d8eb93b33dd2945127'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-bank-utility-import-export.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Bank Utility still shows SELECT BANK + UNLOCK (UNLOCK is front-panel-only on the Matrix-1000 and is undone on bank select), and users cannot duplicate a full bank in-plugin without a disk round-trip.

**Approach:** Rebuild Bank Utility as Selector | Utility columns (Figma), remove SELECT BANK + UNLOCK UI/actions, keep the BANK UTILITY header, and add type-aware bank COPY/PASTE (MIDI dump → clipboard → confirm → MIDI write) with the same progress overlay pattern as import/export.

## Boundaries & Constraints

**Always:**
- Keep module header **"BANK UTILITY"**; module height stays **76** (header + 2×20 + gap 4).
- **Layout @100%:** Selector **156×44** — banks 0–9 in two rows 0–4 / 5–9, button **W=28**, gap **4**. Gap Selector→Utility **16**. Utility **96×44** — subcol1 COPY then PASTE (**W=44**, gap 4); subcol2 EXPORT then IMPORT (**W=48**, gap 4); subcol gap **4**. Total content **268×44**.
- **Remove** SELECT BANK label and UNLOCK button end-to-end (IDs, descriptors, widths, action dispatch, GUI). **Keep** `kBanksLocked` + `markBanksLockedInApvts()` on bank select (Internal Patches lock dot). Do **not** auto-send Unlock SysEx on COPY/PASTE/IMPORT/EXPORT.
- **COPY:** active on any selected bank (M-1000 0–9) when device gates allow. On click: **COPY starts blinking immediately**, then dump slots 0–99 into `ClipboardMode::Bank` (100 packed + source bank) with export-family progress. Dump fail/cancel → disarm blink; clipboard unchanged. Until PASTE is eligible, **only COPY** blinks.
- **PASTE enable / blink:** when clipboard holds a bank, selected bank is M-1000 RAM **0 or 1**, and **selected ≠ source**. As soon as that target is compatible, **PASTE joins** and blinks **in alternation** with COPY. ROM or same-as-source → PASTE grayed (COPY keeps blinking). After successful paste → disarm blink / normal labels.
- **PASTE flow:** click PASTE → confirm modal (Cancel left / Continue) warning overwrite of **all 100 patches**, naming source→target; on Continue → progress dialog; snapshot+restore cancel safety like import; block other bank actions while busy (overlay + shared `isBankTransferBusy`). Post-success sync like import.

- **EXPORT/IMPORT:** keep existing semantics; new widths/positions only. M-6/6R: bank buttons + COPY/PASTE grayed; IMPORT/EXPORT remain as today.
- **Escape:** same as other type-aware clipboards — while a bank COPY/PASTE session is armed (blink active, no blocking overlay), Escape clears the bank clipboard and disarms blink via existing `clearClipboardFeedbackFromEscape`. During progress/confirm overlays, Escape stays blocked like import/export (use dialog Cancel).
- English UI strings only. Four utility buttons only (no INIT / SWAP / CLEAR).

**Ask First:** Change module height / Patch Manager stack; enable bank COPY/PASTE on Matrix-6/6R; auto Unlock SysEx on write; add more Utility primitives.

**Never:** Same-bank PASTE (0→0 / 1→1); PASTE to ROM 2–9; offline bank copy without MIDI dump; leave UNLOCK GUI or SELECT BANK label; French source/UI strings; redesign Internal/Computer Patches in this spec.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| COPY ROM/RAM | Bank N selected, device OK | On click COPY blinks; dump 100 → Bank clipboard source=N; COPY alone until PASTE eligible | Cancel/fail mid-dump → disarm blink; clipboard unchanged; footer warning |
| PASTE 0→1 | Clipboard source 0, select 1 | Confirm → write 100 to bank 1; blink stops | Modal Cancel → no write, blink stays |
| PASTE same bank | Source 0, select 0 | PASTE disabled | N/A |
| PASTE ROM | Source any, select 5 | PASTE disabled; COPY still blinks | N/A |
| Re-COPY on target | Source 0, on bank 1, click COPY | Clipboard source becomes 1; PASTE disables until other RAM | N/A |
| Busy | Transfer in progress | Overlay; other bank C/P/I/E/select refused | Cancel → import-like restore when writes started |
| M-6/6R | No bank concept | COPY/PASTE grayed; I/E as today | N/A |
| Other clipboard | User copies a module | Bank mode replaced; Bank Utility blink off | N/A |
| Escape (armed blink) | Bank clipboard + no overlay | Clipboard cleared; COPY/PASTE blink off | N/A |
| Escape (progress) | Transfer overlay visible | Escape ignored; Cancel on dialog aborts | Same as import/export |

</frozen-after-approval>

## Code Map

- `BankUtilityPanel.{h,cpp}` — two-column layout; drop label/UNLOCK; COPY/PASTE + `ClipboardFeedbackButtonBinding`
- `DesignAtoms.h` / `DesignRecipes.h` — banks **28**; COPY/PASTE **44**; EXPORT/IMPORT **48**; Selector↔Utility gap **16**; drop unlock + SELECT BANK widths; height 76
- `DimensionFactoryBuilders.cpp`, `PanelDimensions.h`, `WidgetDimensionRegistry.cpp`, `WidgetDimensions.h`, `DesignChecks.h` — dims wiring
- `PluginIDs.h`, `PluginDisplayNames.h`, `PluginDescriptorsPatchManager.cpp`, `ActionPropertyRegistry.cpp` — drop unlock; add copy/paste + BankPasteConfirm + progress titles + ClipboardFeedback bank roles
- `PatchManagerActionHandler*` — `BankTransferState` kinds copy/paste; reuse export dump + import snapshot/write; busy-gate bank select; remove unlock handler
- `ClipboardService.*` — `ClipboardMode::Bank` (100 packed + sourceBank); `canPasteBank(target)`
- `ClipboardFeedbackResolver` / paste-enabled resolver / `PluginProcessorClipboard.cpp` — copy-only blink until eligible, then alternate
- `PluginEditorBankBindings.cpp`, `PluginEditorAlerts.cpp`, `BankTransferProgressDialog` — confirm gate + COPYING/PASTING titles
- `DeviceMemoryLimits.*` — reuse `isPasteStoreAllowed` / `isRomBank`
- `Tests/` — bank clipboard eligibility (same/ROM/cross) + mode replace
- Keep Internal Patches `kBanksLocked` consumers; GUI must not call Unlock SysEx

## Tasks & Acceptance

**Execution:**
- [x] Design atoms/recipes + DimensionFactory/registry — widths/gap 16; drop unlock/select-bank dims
- [x] PluginIDs / DisplayNames / Descriptors / ActionPropertyRegistry — copy/paste IDs; drop UNLOCK
- [x] ClipboardService + feedback/paste resolvers — Bank mode + blink rules
- [x] PatchManagerActionHandler + editor bank bindings — COPY dump / PASTE confirm+write; shared busy; remove unlock
- [x] BankUtilityPanel — layout, gating, feedback bindings
- [x] Tests — I/O matrix eligibility cases
- [x] `Scripts/quality/lint_touched.py` — pass on touched C++

**Acceptance Criteria:**
- [x] Given Bank Utility @100%, when laid out, then Selector 156×44, Utility 96×44, gap 16, header present, no SELECT BANK/UNLOCK.
- [x] Given bank N and device OK, when user clicks COPY, then COPY blinks at once; after dump OK, clipboard source=N; PASTE still off until a compatible other RAM is selected.
- [x] Given source 0 and selected 1, when PASTE runs, then confirm states 100-patch overwrite, write succeeds, blink stops.
- [x] Given source 0 and selected 0 or ROM, when viewing PASTE, then it stays disabled.
- [x] Given busy transfer, when other bank actions fire, then Core refuses them; cancel restores like import if writes began.
- [x] Given Matrix-6/6R, when panel shows, then COPY/PASTE grayed; IMPORT/EXPORT still gated as today.

## Design Notes

COPY blinks on click (before dump finishes); PASTE joins blink only when target RAM is compatible. Dump fail/cancel keeps prior bank blink if clipboard still holds a bank. Paste uses a single overwrite confirm (no second unsaved-edit gate). Paste cancel after writes → import-like snapshot restore. Distinct progress titles; same dialog. No Unlock SysEx in this flow.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — success (2026-09-04)
- Unit tests: `ClipboardBankService`, `ClipboardFeedbackResolver`, `ClipboardPasteEnabledResolver`, `WidgetDimensionRegistry` — pass
- `python3 Scripts/quality/lint_touched.py` — clean on touched files

**Manual checks:**
- M-1000: COPY ROM→PASTE RAM; same-bank PASTE gray; cross-RAM alternate blink; mid-paste Cancel restores; UNLOCK gone. (pending device session)

## Suggested Review Order

**Layout (Selector | Utility)**

- Two-column Bank Utility layout matching Figma widths/gaps
  [`BankUtilityPanel.cpp:255`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L255)

- Design atoms for bank 28 / COPY-PASTE 44 / EXPORT-IMPORT 48
  [`DesignAtoms.h:53`](../../Source/GUI/Layout/Design/DesignAtoms.h#L53)

- Selector/Utility widths and 16 px column gap recipe
  [`DesignRecipes.h:75`](../../Source/GUI/Layout/Design/DesignRecipes.h#L75)

**Bank COPY / PASTE transfer**

- COPY arms blink on click, then dumps 100 patches to clipboard
  [`PatchManagerActionHandlerBankCopyPaste.cpp:110`](../../Source/Core/Actions/PatchManagerActionHandlerBankCopyPaste.cpp#L110)

- PASTE: single overwrite confirm, then snapshot/write like import
  [`PatchManagerActionHandlerBankCopyPaste.cpp:373`](../../Source/Core/Actions/PatchManagerActionHandlerBankCopyPaste.cpp#L373)

- Confirm dialog wiring (Cancel left / Continue)
  [`PluginEditorBankBindings.cpp:154`](../../Source/GUI/PluginEditorBankBindings.cpp#L154)

**Type-aware clipboard**

- New `ClipboardMode::Bank` with source bank + 100 packed patches
  [`ClipboardService.h:54`](../../Source/Core/Services/ClipboardService.h#L54)

- Blink: pending COPY-only, then alternate when target RAM eligible
  [`ClipboardFeedbackResolver.cpp:132`](../../Source/Core/Services/ClipboardFeedbackResolver.cpp#L132)

- Paste overwrite copy (100 patches, source→target)
  [`PluginDisplayNames.h:215`](../../Source/Shared/Definitions/PluginDisplayNames.h#L215)

**Tests**

- Same-bank / cross-RAM eligibility and mode replace
  [`ClipboardBankServiceTests.cpp:1`](../../Tests/Unit/ClipboardBankServiceTests.cpp#L1)
