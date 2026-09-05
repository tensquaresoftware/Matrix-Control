---
title: 'Bank export Pxx. filename + Computer Patches Filename display'
type: 'bugfix'
created: '2026-09-05'
status: 'done'
baseline_commit: 'ee6e33e989496454bfc69cd63cb8776cf14687b2'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Bank Utility EXPORT uses `Pxx - {Name}.syx`, which exceeds the Matrix 8-character name. Opening those files with DISPLAY FILE NAMES mangles PATCH NAME (e.g. `P10 - NY`). The hyphen separator is also ambiguous with intentional names like `P99 - DJ`.

**Approach:** Export as `Pxx. {Name}.syx` (dot + space; `.` is illegal in Matrix names). Strip that prefix before Computer Patches DISPLAY FILE NAMES reconciliation so PATCH NAME shows the real name. Drop all `Pxx -` emit/parse. Keep OPEN combo on full filenames. No footer tip about 8-character names.

## Boundaries & Constraints

**Always:**
- Export SSOT `bankExportFileStem`: `Pxx. {Name}` (`xx` 00–99; `{Name}` = `sanitizeOsPathSegmentOrEmpty`); empty name → `Pxx` only (never `Pxx.` / `Pxx. `).
- Parse SSOT `nameFromBankExportStem`: only `Pxx.` (+ optional spaces after the dot); empty for slot-only / non-match. No hyphen legacy.
- Computer Patches DISPLAY FILE NAMES (load + settings reapply): strip matching bank-export prefix, then existing reconciler / Matrix 8-char sanitize. Non-matching stems (e.g. artistic `P99 - DJ`) unchanged.
- OPEN combo lists full file stems (no strip/truncate in the combo).
- IMPORT slots follow sorted file order (not `Pxx` as slot index); stem parse remains name-backfill only for empty/`BNK…` payloads.
- English only; unit tests updated for the new format.

**Ask First:** OPEN combo truncation; footer 8-char load warnings; dual-format compatibility; renaming `Tests/Fixtures/Patches/` historical `Pxx -` paths.

**Never:**
- Legacy `Pxx -` / `Pxx-Name` code; IMPORT-by-`Pxx`-number; Finder policing beyond known-prefix strip.
- Changing DISPLAY SYSEX NAMES; footer “max 8 characters” on load (SAVE AS already enforces; bank exports are intentionally longer).
- Unrelated Computer Patches / Mutator naming refactors; French source strings.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Export named | Slot 10, `NYLON 12` | `P10. NYLON 12.syx` | N/A |
| Export blank | Slot 76, empty name | `P76.syx` | N/A |
| Export artistic | Slot 5, `P99 - DJ` | `P05. P99 - DJ.syx` | N/A |
| FILE NAMES load | `P10. NYLON 12.syx` | PATCH NAME `NYLON 12` | N/A |
| FILE NAMES load | `P05. P99 - DJ.syx` | PATCH NAME `P99 - DJ` | N/A |
| FILE NAMES load | `P76. AA.syx` | PATCH NAME `AA` | N/A |
| FILE NAMES load | `P99 - DJ.syx` (no prefix) | PATCH NAME `P99 - DJ` (no false strip) | N/A |
| SYSEX NAMES | Any bank-export file | PATCH NAME from SysEx bytes | N/A |
| OPEN combo | `Pxx. …` folder | Full stems in combo | N/A |
| IMPORT backfill | Empty/`BNK…` + `P10. NYLON 12.syx` | Name → `NYLON 12` | Invalid `.syx` skipped |
| Old hyphen file | `P10 - NYLON 12.syx` | Not a bank-export stem | Sanitize full stem as non-prefix |
| Slot-only parse | `P07` | Parser → empty | N/A |

</frozen-after-approval>

## Code Map

- `PatchFileNameSanitizer.{h,cpp}` — emit `Pxx. {Name}` / `Pxx`; parse only `Pxx.`; remove hyphen tolerance (~L99–135)
- `PatchManagerActionHandlerBankExport.cpp` — already calls `bankExportFileStem` (~L223)
- `PatchManagerActionHandlerBankImport.cpp` — backfill via `nameFromBankExportStem` (~L40)
- `PatchManagerActionHandlerComputerLoadSave.cpp` — strip prefix before `reconcile` in `reconcileLoadedPatchName` + `reapplyComputerPatchDisplayedName`
- `PatchFileNameReconciler.cpp` — keep policy; strip at call site (do not change global `sanitizeFileStem`)
- `ComputerPatchesPanel.cpp` — OPEN full stems (~L199–202); **read-only**
- `Tests/Unit/PatchFileNameSanitizerTests.cpp` + `PatchFileServiceTests.cpp` — update expectations/cases
- `MutationCalibrationTestSupport.h` + `Tests/Fixtures/Patches/**` — leave historical `P00 -` paths

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Services/PatchFileNameSanitizer.{h,cpp}` — `Pxx. {Name}` emit/parse; delete hyphen legacy — SSOT
- [x] `Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp` — strip before Filename reconcile (load + reapply) — PATCH NAME fix
- [x] `Tests/Unit/PatchFileNameSanitizerTests.cpp` + `PatchFileServiceTests.cpp` + `PatchManagerActionHandlerBankExportFilenameTests.cpp` — I/O matrix + load/reapply FILE NAMES — regression
- [x] Targeted unit tests + `python3 Scripts/quality/lint_touched.py` on touched C++ — quality gate

**Acceptance Criteria:**
- Given EXPORT of a named slot, when written, then stem is `Pxx. {Name}` (or `Pxx` if empty).
- Given OPEN of `Pxx. {Name}.syx` with DISPLAY FILE NAMES, when loaded or settings reapplied, then PATCH NAME shows `{Name}` (Matrix-sanitized), not a truncated `Pxx…` prefix.
- Given stem `P99 - DJ.syx`, when DISPLAY FILE NAMES loads it, then artistic name is kept.
- Given OPEN combo on a bank-export folder, then items still show full `Pxx. …` filenames.
- Given sanitizer/export unit tests, when run, then they pass with no `Pxx -` assertions.

## Spec Change Log

## Design Notes

`.` ∉ Matrix charset → `Pxx. ` cannot be a SAVE AS / PATCH NAME string (unlike `Pxx - `). Strip only at Computer Patches reconcile call sites. Footer already reports load/reconciliation and truncates; a standing 8-char tip would false-alarm on bank exports.

Examples: `P03. WARM PAD.syx` → `WARM PAD`; `P05. P99 - DJ.syx` → `P99 - DJ`; `P76. AA.syx` → `AA`; blank → `P07.syx`.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — build success
- `PatchFileNameSanitizer` + `PatchFileService` + `PatchManagerActionHandlerBankExportFilename` unit tests — pass
- `python3 Scripts/quality/lint_touched.py` on touched C++ — pass

**Manual checks:**
- EXPORT → Finder `Pxx. …`; OPEN + FILE NAMES → correct PATCH NAME; combo still full labels; SYSEX NAMES unchanged

## Suggested Review Order

**Export / parse SSOT**

- Emit `Pxx. {Name}` (empty → `Pxx` only).
  [`PatchFileNameSanitizer.cpp:99`](../../Source/Core/Services/PatchFileNameSanitizer.cpp#L99)

- Parse only `Pxx.` (+ optional spaces); no hyphen legacy.
  [`PatchFileNameSanitizer.cpp:117`](../../Source/Core/Services/PatchFileNameSanitizer.cpp#L117)

**Computer Patches FILE NAMES**

- Strip bank-export prefix before reconcile on load and settings reapply.
  [`PatchManagerActionHandlerComputerLoadSave.cpp:186`](../../Source/Core/Actions/PatchManagerActionHandlerComputerLoadSave.cpp#L186)

**Tests**

- Load/reapply coverage for stripped vs artistic stems.
  [`PatchManagerActionHandlerBankExportFilenameTests.cpp:25`](../../Tests/Unit/PatchManagerActionHandlerBankExportFilenameTests.cpp#L25)

- Sanitizer emit/parse matrix cases.
  [`PatchFileNameSanitizerTests.cpp:134`](../../Tests/Unit/PatchFileNameSanitizerTests.cpp#L134)
