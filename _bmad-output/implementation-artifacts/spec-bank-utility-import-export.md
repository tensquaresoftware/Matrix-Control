---
title: 'Bank Utility IMPORT / EXPORT'
type: 'feature'
created: '2026-07-26'
status: 'done'
baseline_commit: 'a6b19b4b0369025d193c011e2e321947927b3429'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/reference-docs/oberheim/oberheim-matrix-1000-midi-sysex-implementation.md'
  - '{project-root}/_bmad-output/reference-docs/oberheim/oberheim-matrix-6-6r-midi-sysex-implementation.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Users cannot back up or restore a full bank as a folder of `.syx` files from Bank Utility; Matrix-6/6R has no path for the same 100-patch memory.

**Approach:** Add IMPORT and EXPORT on Bank Utility that talk to the synth over MIDI (OS folder pickers, confirm, progress + cancel, footer tallies), with the approved Figma layout.

## Boundaries & Constraints

**Always:**
- **EXPORT** — all Matrix-1000 banks; on Matrix-6/6R the single 100-patch memory. OS parent-folder picker. Child folder `BANK N` (selected bank) on M-1000, or `PATCHES` on 6/6R. Live-dump slots 0–99 to one `.syx` each. Name: `Pxx - {Name}.syx` (`xx` = slot; `{Name}` = `sanitizeOsPathSegmentOrEmpty` from payload); empty/unusable → `Pxx.syx`. Keep payload name bytes; encode SysEx with real slot number; honor inter-SysEx delay.
- **IMPORT** — M-1000: only RAM banks 0–1. M-6/6R: always (whole memory). OS picker → Cancel/Continue confirm (Cancel left) → non-recursive `*.syx`, alpha sort, first 100. Valid patches overwrite 0…N−1 only. Snapshot device bank first; buffer validated packs; write only if not cancelled. Busy UI: linear % + waiting text + Cancel. Cancel restores snapshot. Success footer: found / valid / imported counts.
- **Layout @ 100%:** SELECT BANK + UNLOCK stay **68**; banks 0–9 → **24**; gaps **4**; IMPORT **56×20** right of bank 4; EXPORT **56×20** right of bank 9.
- **M-6/6R gray-out:** bank buttons + UNLOCK + SELECT BANK stay grayed; IMPORT/EXPORT remain usable under normal device/Compare gates.
- Reuse dirty/Compare/device gates, `PatchFileService`, sanitizer, `PluginEditor` folder chooser + ordered confirm patterns.

**Ask First:** Rename export folders; IMPORT on ROM 2–9; whole-device 1000-patch export.

**Never:** Offline-only (no MIDI); bulk 10-bank export; Mutator `Name @ BxPyy` filenames here; recursive scan; >100 writes; persistent 100-patch cache beyond op snapshot; French UI strings; change Computer Patches / Mutator export conventions.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Export M1000 | Bank 3 selected | `BANK 3` with `P00…P99` `.syx` | Fail → footer error; cancel mid-write cleans files created this run |
| Export M6/6R | Device ready | Folder `PATCHES` | Same |
| Export blank name | Empty name bytes | `P47.syx` | N/A |
| Import RAM | Bank 0, 80 valid | Slots 0–79 written; footer tallies | Invalid skipped in valid count |
| Import >100 | 150 `.syx` | First 100 alpha; found=150 | N/A |
| Confirm Cancel | User Cancel | No MIDI writes | N/A |
| Progress Cancel | Mid-import | Restore snapshot | Snapshot fail → do not start write |
| Import ROM | Bank 5 | IMPORT disabled | N/A |
| No device | Undetected | Both inactive via existing gates | N/A |

</frozen-after-approval>

## Code Map

- `BankUtilityPanel.{h,cpp}` — buttons, layout, partial M6 gray-out
- `DesignAtoms.h` / `DimensionFactory.cpp` / `PanelDimensions.h` — widths 24 / 56×20
- `PluginIDs.h` / `PluginDisplayNames.h` / descriptors — IMPORT/EXPORT widgets
- `PatchManagerActionHandler.*` — orchestration, gates, footers
- `MidiManager.*` / `SysExEncoder.*` / `SysExConstants.h` — dump loop or `kRequestAllBank`; `sendPatch`; cancel
- `PatchFileService.*` / `PatchFileNameSanitizer.*` — `.syx` I/O + `Pxx` naming helper
- `DeviceMemoryLimits.*` — RAM/ROM / `hasBankConcept`
- `PluginEditor.cpp` + `Dialogs/*` — pickers, confirm, progress overlay (MasterInit-style)
- `Tests/Unit/*` — naming, sort/cap, tally logic

## Tasks & Acceptance

**Execution:**
- [x] `DesignAtoms.h` + BankUtility dims — bank 24; IMPORT/EXPORT 56×20 — layout
- [x] `PluginIDs.h` / `PluginDisplayNames.h` / descriptors (+ CMake if new) — register widgets
- [x] `BankUtilityPanel.*` — place buttons; M6 gray bank row only — UI
- [x] Naming helper (`PatchFileNameSanitizer` or adjacent) — `Pxx - Name` / `Pxx` — SSOT
- [x] `MidiManager` (+ encoder if needed) — cancellable sequential dump + progress — device read
- [x] `PatchManagerActionHandler` — EXPORT/IMPORT flows, snapshot/restore, gates, footers — orchestration
- [x] `PluginEditor` + progress dialog — % bar, wait copy, Cancel — UX
- [x] `Tests/Unit/…` — I/O matrix pure-logic cases — regression

**Acceptance Criteria:**
- Given M-1000 bank 0, when user Continues IMPORT of valid `.syx`, then slots fill in order and footer reports found/valid/imported.
- Given M-1000 bank 5, then IMPORT inactive and EXPORT active.
- Given M-6/6R, then bank/UNLOCK stay grayed, IMPORT/EXPORT work, export folder is `PATCHES`.
- Given EXPORT bank 2, then `BANK 2` holds `Pxx…` files per naming rule.
- Given IMPORT in progress, when Cancel, then device matches pre-import snapshot.
- Given 100% scale, then Bank Utility widths/gaps match Always layout.

## Design Notes

Dump via sequential `requestSinglePatchAsync(0…99)` after Set Bank when needed, unless `kRequestAllBank` (`0x04` type 0) is clearly smaller. Import: snapshot → buffer → write; cancel rewrites snapshot. Example: `Warm Pad` → `P03 - WARM PAD.syx`; empty → `P03.syx`.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — build success
- Targeted unit tests for new naming/sort helpers — pass

**Manual checks:**
- M-1000: EXPORT RAM + ROM; IMPORT only 0/1; cancel mid-import restores
- M-6/6R: IMPORT/EXPORT → `PATCHES`; banks stay grayed
- Visual: Figma widths at 100% scale

## Suggested Review Order

**UI entry and gates**

- IMPORT/EXPORT placement and M-6 gray-out that keeps transfer buttons usable
  [`BankUtilityPanel.cpp:249`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L249)

- ROM vs RAM enablement uses highlighted selected bank
  [`BankUtilityPanel.cpp:155`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L155)

**Transfer orchestration**

- Export uses selected bank, dirty confirm, then live dump to folder
  [`PatchManagerActionHandler.cpp:1371`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1371)

- Import: confirm → buffer → snapshot → write → cancel restores device
  [`PatchManagerActionHandler.cpp:1545`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1545)

- Cancel only deletes files created this run; removes empty new folders
  [`PatchManagerActionHandler.cpp:1520`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1520)

**Progress UX**

- Modal progress overlay with Cancel wired from PluginEditor
  [`BankTransferProgressDialog.h:15`](../../Source/GUI/Dialogs/BankTransferProgressDialog.h#L15)

- Presenter hooks show/update/hide during transfers
  [`PluginEditor.cpp:1217`](../../Source/GUI/PluginEditor.cpp#L1217)

**Pure helpers and tests**

- `Pxx - Name` / `Pxx` export stem SSOT
  [`PatchFileNameSanitizer.cpp:78`](../../Source/Core/Services/PatchFileNameSanitizer.cpp#L78)

- Alpha sort, 100-cap, found/valid plan
  [`BankImportPlanner.cpp:5`](../../Source/Core/Services/BankImportPlanner.cpp#L5)
