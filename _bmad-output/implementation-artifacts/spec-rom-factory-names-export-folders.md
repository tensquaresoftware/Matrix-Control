---
title: 'ROM factory names + Mutator export folder naming'
type: 'feature'
created: '2026-07-25'
status: 'done'
baseline_commit: '22bb501355d0deba3131186e149627de8bae2056'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/reference-docs/oberheim/oberheim-matrix-1000-factory-patch-names.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Matrix-1000 ROM dumps often lack usable name bytes, so PATCH NAME and Mutator export folders fall back to blank/`Bxx-Pyy` even though factory names are known. Save/Save As also overwrites patch name bytes 0–7 with a sanitized filename, destroying exotic factory characters (e.g. `*'CANOPY` → `CANOPY`).

**Approach:** Embed Matrix-1000 ROM factory names (banks 2–9) as Core SSOT; apply them on ROM device load into PatchModel/APVTS; keep Matrix name bytes intact on Save (filename OS-sanitized only); rename Mutator export folders to `Name @ BxPyy` / `BxPyy` / `Stem @ SyxFile`.

## Boundaries & Constraints

**Always:**
- Hardcoded table covers **Matrix-1000 banks 2–9 only** (800 names), sourced from `oberheim-matrix-1000-factory-patch-names.md` (verbatim factory spelling, including spaces and exotic chars). Banks 0–1 are not table-driven.
- On successful device dump load when `DeviceMemoryLimits::isRomBank(bank)`: set patch name from the table for `(bank, patch)` into PatchModel + PATCH NAME APVTS (after `loadFrom` / normalize).
- Matrix name bytes 0–7 preserve the current PatchModel name on Save/Save As — **do not** inject sanitized filename into bytes.
- `.syx` **filename** stem is still OS-sanitized; `.syx` **payload** name bytes match PatchModel (original/factory).
- Mutator export session folder basename:
  - Device / INIT / PASTE with usable OS-sanitized name: `{Name} @ B{b}P{pp:02d}` (single-digit bank, e.g. `BS ETAK @ B2P52`).
  - Device with empty/unsanitizable name: `B{b}P{pp:02d}` only.
  - Computer file: `{Stem} @ SyxFile` (OS-sanitized stem; empty → fallback stem then same pattern, e.g. `PATCH @ SyxFile`).
- OS path sanitize for folder/filename only (`stripOsForbiddenChars` + length/trim as needed); keep spaces in the name segment. Do not run the strict Matrix A–Z/0–9/-/_ filter for export folder name segments.
- Paste ROM→RAM: name already in editor buffer after ROM inject is what gets stored; ensure PATCH NAME APVTS refreshes on paste if bytes include name.
- Matrix-6/6R: no ROM table (`hasRomBanks == false`); export still uses new basename rules with bank `0` → `B0Pyy`.

**Ask First:**
- Hardware proof that M1000 RAM retains written name bytes after STORE + power-cycle (manual UAT only — do not block code on it).

**Never:**
- Change FR-33 inner layout (`Initial.syx`, `Mxx/…`) or MutationNaming `Mxx`/`Mxx-Ryy` stems.
- Auto-export on each MUTATE.
- Apply factory table to Computer `.syx` loads or to RAM banks 0–1.
- Reintroduce Save filename→bytes injection.
- Use `~` or prefix-first `BxPyy - Name` folder shapes (rejected).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| ROM load named | Bank 2 patch 52, table `BS ETAK*` | PATCH NAME shows factory spelling; model bytes hold it | Dump fail → existing footer; no inject |
| ROM load exotic | Bank 9 patch 6 `*'CANOPY` | Display/bytes keep `*'CANOPY` | N/A |
| Save As preserve | Model `*'CANOPY`, file `FOO.syx` | File named OS-safe; payload bytes still `*'CANOPY` | Save fail → restore prior model name only if temporarily mutated (should not mutate) |
| Export device named | Frozen name `BS ETAK*`, bank 2 patch 52 | Folder `BS ETAK @ B2P52` (`*` stripped for OS) | Unwritable folder → existing warning |
| Export blank name | Empty/unsanitizable after OS strip | Folder `B2P52` | N/A |
| Export computer | Stem `WARM-PAD.syx` | Folder `WARM-PAD @ SyxFile` | N/A |
| M6/6R export | Bank concept off, patch 47, name `OB-VOX` | Folder `OB-VOX @ B0P47` | N/A |
| Collision Keep | `BS ETAK @ B2P52` exists | `…-2`, `…-3`, … | Cancel unchanged |

</frozen-after-approval>

## Code Map

- `Source/Core/Services/Matrix1000FactoryPatchNames.{h,cpp}` — **new** SSOT lookup `(bank, patch) → name` for banks 2–9
- `Source/Core/Services/DeviceMemoryLimits.*` — `isRomBank` gate for inject
- `Source/Core/Actions/PatchManagerActionHandler.cpp` — ROM inject after device dump; remove Save filename→`setName` inject; paste name APVTS sync if needed
- `Source/Core/Models/PatchModel.*` / `PatchNameSyncer.*` — name get/set used by inject + display
- `Source/Core/Services/PatchFileNameSanitizer.*` — OS path helper for folder/filename segments (spaces kept; no Matrix-only strip for export names)
- `Source/Core/Services/PatchMutator/PatchLoadContext.{h,cpp}` — `computeExportBasename` / device + computer formats
- `Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp` — `freezeExportBasename` (consumes context API)
- `Tests/Unit/PatchLoadContextTests.cpp`, `PatchMutatorEngineTests.cpp`, `PatchFileServiceTests.cpp`, `PatchFileNameSanitizerTests.cpp`, `PatchManagerActionHandlerTests.cpp` — expectations + new ROM/save cases
- `_bmad-output/reference-docs/oberheim/oberheim-matrix-1000-factory-patch-names.md` — human SSOT to encode (banks 2–9)

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Services/Matrix1000FactoryPatchNames.{h,cpp}` + `CMakeLists.txt` — add 800-name table + lookup; register sources — ROM name SSOT
- [x] `PatchManagerActionHandler.cpp` — after successful ROM dump load, `setName` from table + `patchNameSyncer->bufferToApvts`; stop Save/Save As `setName(sanitizeFileStem(…))` — display + preserve bytes
- [x] `PatchFileNameSanitizer.*` — expose OS-safe stem/folder segment that keeps spaces and does not apply Matrix-only charset filter — path safety without name destruction
- [x] `PatchLoadContext.*` — implement `Name @ BxPyy` / `BxPyy` / `Stem @ SyxFile`; single-digit bank — export naming
- [x] `Tests/Unit/…` — update basename/export expectations; add factory lookup, ROM inject, Save preserves bytes 0–7 — I/O matrix coverage
- [x] Paste path — if PATCH NAME stale after full-patch paste, sync name via `PatchNameSyncer` — UI matches bytes

**Acceptance Criteria:**
- Given M1000 ROM bank/patch with a table entry, when device dump loads, then PATCH NAME equals the factory string (including exotic chars).
- Given a named editor patch, when Save As writes `Other.syx`, then payload bytes 0–7 still match the pre-save patch name and the filename is OS-safe.
- Given first MUTATE after device load at bank 2 patch 52 with factory `BS ETAK*`, when Export runs, then session folder is `BS ETAK @ B2P52`.
- Given Computer stem `WARM-PAD`, when Export runs, then folder is `WARM-PAD @ SyxFile`.
- Given empty name on device origin, when Export runs, then folder is `BxPyy` only.
- Given Matrix-6/6R device origin patch 47 named `OB-VOX`, when Export runs, then folder is `OB-VOX @ B0P47`.

## Spec Change Log

## Design Notes

**Dual naming:** Matrix bytes / PATCH NAME = faithful factory or user/Matrix name. Filesystem folder + `.syx` filename = OS-sanitized derivative only.

**Basename examples:** `BS ETAK @ B2P52`, `B2P52`, `WARM-PAD @ SyxFile`, collision Keep → `BS ETAK @ B2P52-2`.

**ROM inject timing:** after `loadFrom` + `normalizeNameEncoding`, replace name from table so zeroed dumps do not surface as `@@@@@@@@`.

**Paste:** full-buffer paste already copies bytes 0–7; ROM inject before Copy is sufficient if APVTS name sync runs on paste.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` — expected: build succeeds
- Run updated Mutator / PatchLoadContext / PatchFile / PatchManager unit tests — expected: green

**Manual checks:**
- M1000: load ROM 2/52 → PATCH NAME `BS ETAK*`; Export → `BS ETAK @ B2P52`; Save As → filename clean, reopen prefers internal exotic name per Settings policy.
- Optional UAT: STORE named patch to RAM → power-cycle → dump → confirm bytes 0–7 retained (informational).

## Suggested Review Order

**ROM factory names**

- Entry point: 800-name ROM lookup for banks 2–9
  [`Matrix1000FactoryPatchNames.h:12`](../../Source/Core/Services/Matrix1000FactoryPatchNames.h#L12)

- Inject factory spelling after device dump when the bank is ROM
  [`PatchManagerActionHandler.cpp:1051`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1051)

**Preserve name bytes on Save**

- Save keeps PatchModel name; no filename injection into bytes 0–7
  [`PatchManagerActionHandler.cpp:1076`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1076)

- Post-save select must not reload/reconcile and wipe the exotic name
  [`PatchManagerActionHandler.cpp:1119`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L1119)

- Prefer-Internal keeps decoded exotic characters instead of Matrix-filtering them away
  [`PatchFileNameReconciler.cpp:88`](../../Source/Core/Services/PatchFileNameReconciler.cpp#L88)

**Export folder naming**

- `Name @ BxPyy` / location-only / `Stem @ SyxFile`
  [`PatchLoadContext.cpp:42`](../../Source/Core/Services/PatchMutator/PatchLoadContext.cpp#L42)

- OS path segment: strip forbidden chars, trailing dots, keep spaces
  [`PatchFileNameSanitizer.cpp:35`](../../Source/Core/Services/PatchFileNameSanitizer.cpp#L35)

**Tests**

- Basename + factory lookup + Save preserve expectations
  [`PatchLoadContextTests.cpp:19`](../../Tests/Unit/PatchLoadContextTests.cpp#L19)
