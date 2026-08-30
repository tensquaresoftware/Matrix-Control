---
title: 'Patch Mutator playability unit tests (oracle + corpus fuzz)'
type: 'feature'
created: '2026-08-30'
status: 'done'
baseline_commit: '7e6d9371d37e5565b6bf9c6501d624728d81f189'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/parametres-extinctions-son.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-patch-mutator-playable-calibration.md'
  - '{project-root}/.cursor/rules/core-testing.mdc'
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiate">

## Intent

**Problem:** Anti-silence guards exist in the MUTATE pipeline, but there is no Core suite that proves patches stay structurally playable across real ROM seeds, trap seeds, and many deterministic RNG rolls — without MIDI hardware or listening.

**Approach:** Add test-only playability oracle (classes A/B/C/E, priorities 1–2) plus a complementary fuzz suite that runs `MutationAlgorithm::apply` via existing helpers on a fixed mixed corpus (16 ROM + traps + Init). Keep `MutationAntiSilenceTests` as targeted regressions.

## Boundaries & Constraints

**Always:**
- Tests only under `Tests/` (not linked into AU/VST3). English code/comments. AAA + F.I.R.S.T.
- Oracle aligned on silence taxonomy priorities 1–2 and existing `MutationCalibration` floors/thresholds — not a second policy SSOT in production.
- Drive mutations through `MutationCalibrationTestSupport::mutated` / `MutationAlgorithm::apply` so post-apply + MM policy run.
- Exact ROM list (banks 2–9, two each) under `Tests/Fixtures/Patches/ROM/`; load via `MATRIX_TEST_FIXTURES_DIR` (decode like `PatchModelTests`); do not duplicate ROM bytes elsewhere.
- Default matrix sized for CI (seconds): all seeds × 4 MODE × small PITCH subset × `kTrialCount`-class RNG (~32–64). Named wider constant available, off by default.
- Mostly MUTATE; thinner RETRY slice = second `apply` from same parent with different RNG (no engine/history required).
- Failures report seed path/name, MODE, PITCH, RNG seed, and which oracle rule failed.
- Register new `.cpp` in root `CMakeLists.txt` `Matrix-Control_Tests` sources (same list as AntiSilence). `lint_touched.py` clean on touched C++.

**Ask First:**
- Changing `MutationCalibration` floor numbers (hardware retune).
- Expanding priority-3 (class D) into the oracle.
- Enabling the wide pass by default in CI.

**Never:**
- Change Amount×Random curve math; GUI tests; MIDI/hardware; multi-hour loops.
- Move oracle into plugin runtime.
- Rewrite or weaken `MutationAntiSilenceTests`.
- Touch production code unless oracle proves a real guard hole — then minimal `MutationPostApply` / `MutationMatrixModPolicy` fix + freeze as regression.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| ROM fuzz CI | Each of 16 listed ROM seeds; all MODE; PITCH Preserve+Free+one Consonant; N≈32–64 RNG; MUTATE | Oracle true after apply | Fail message: seed, MODE, PITCH, RNG, rule id |
| Trap + Init fuzz | Synthetic silence-stress seeds + InitDefaults/PatchInit; same matrix | Oracle true after apply | Same reporting |
| RETRY slice | Same parent seed; second apply, distinct RNG | Oracle true | Same reporting |
| Wide pass | `kWideTrialCount` (or flag) enabled locally | Same assertions, higher N; not CI default | Same reporting |
| Dead → playable (Should) | Intentionally dead trap; writable toggles; apply | Oracle true | Skip if modules locked off repair path |
| AntiSilence intact | Existing suite unchanged | Still green | N/A |
| Guard hole found | Oracle fails on reproducible case | Minimal post-apply/MM fix + regression case | Ask First if floors must change |

</frozen-after-approval>

## Code Map

- `Tests/Unit/MutationCalibrationTestSupport.h` — reuse `mutated`, `makeRecipe`, `makeInitPatchModel`, `kTrialCount`, bus/wave helpers; extend only if shared loaders/oracle helpers belong here.
- `Tests/Unit/MutationAntiSilenceTests.cpp` — keep targeted assertions; new suite complementary fuzz only.
- `Tests/Unit/MutationCalibrationTests.cpp` — MODE/PITCH calibration; do not conflate with playability oracle.
- New `Tests/Unit/MutationPlayabilityOracle.h` (+ optional `.cpp`) — pure `PatchModel` → pass/fail + structured rule reason (A/B/C/E prio 1–2).
- New `Tests/Unit/MutationPlayabilityCorpusTests.cpp` — corpus matrix + optional dead→playable; register beside AntiSilence in root `CMakeLists.txt` (~L420–421).
- `Source/Core/Services/PatchMutator/MutationAlgorithm.cpp` (~125–173) — apply pipeline: jitter → `applyPostMutationGuards` → MM policy/openers → role guards (read-only unless hole).
- `MutationPostApply.*` / `MutationMatrixModPolicy.*` / `MutationPolicy.h` `MutationCalibration::*Floor*` — production guards + numeric SSOT for oracle thresholds.
- `Tests/Unit/PatchModelTests.cpp` (~31–67) — fixture load pattern: `MATRIX_TEST_FIXTURES_DIR` + `SysExDecoder::decodePatchSysEx` + `PatchModel::loadFrom`.
- ROM corpus (exact): `Tests/Fixtures/Patches/ROM/BANK {2..9}/` — P00 OBXA-11, P29 AMBIANCE; P03 HALO, P99 SYNCAGE; P34 CS-80, P19 BIGBRA$$; P11 SYNBASS, P68 OSC SYNC; P10 1000STRG, P79 SOLEMN; P04 ARP-2600, P73 SLAP 2; P03 BANJO, P73 NOISE-DN; P01 MIKPIANO, P11 OB8 JUMP.
- Init: `makeInitPatchModel()` and/or `Tests/Fixtures/Init/PatchInit.syx`.
- Taxonomy ref (read-only): `parametres-extinctions-son.md` §2–§6, §9 priorities 1–2.
- `Tests/CMakeLists.txt` — stub warning only; do not link Unit tests into plugin targets.

## Tasks & Acceptance

**Execution:**
- [x] `Tests/Unit/MutationPlayabilityOracle.h` (+ `.cpp` if needed) — implement structural playability check (VCA chain, velocity soft-kill couples, filter+res, dual WaveSelect OFF, mix→OFF DCO, MM risk Amounts to VCA/ENV2 Amp/VCF Freq/Mix) using calibration floors — test-only oracle.
- [x] `Tests/Unit/MutationCalibrationTestSupport.h` (or sibling support) — ROM/Init load helpers + trap seed builders; named `kTrialCount` / `kWideTrialCount` (wide off by default) — corpus plumbing.
- [x] `Tests/Unit/MutationPlayabilityCorpusTests.cpp` — matrix MUTATE (+ thin RETRY) over ROM+traps+Init; rich failure strings; optional dead→playable Should — complementary fuzz.
- [x] `CMakeLists.txt` — `target_sources(Matrix-Control_Tests PRIVATE …)` for new unit sources — linkable runner only.
- [x] `MutationPostApply` / `MutationMatrixModPolicy` — only if oracle finds a hole; minimal fix + freeze case — no curve-math changes.

**Acceptance Criteria:**
- Given the 16 listed ROM fixtures on disk, when the new suite runs, then each file is loaded and exercised in the matrix (not skipped).
- Given CI defaults, when the suite runs under macos-debug-arm64 UnitTest filter, then it finishes in seconds and all oracle checks pass.
- Given a failure, when reported, then seed identity, MODE, PITCH, RNG seed, and oracle rule id are present.
- Given existing AntiSilence/Calibration suites, when the full UnitTest target runs, then they remain green and unchanged in intent.
- Given a intentionally dead writable trap (Should), when apply runs, then oracle is true afterward.

## Design Notes

Oracle is a **checklist**, not a DAW render: compare patch parameters (and active MM buses) against the same floors/thresholds production guards target. Prefer reading `MutationCalibration` constants over hardcoding duplicate magic numbers. Trap seeds should stress known silence classes (VCA chain, dual OFF, closed filter+low res, mix toward OFF DCO, negative MM → VCA/ENV2 Amp/VCF) without requiring hardware.

Default PITCH subset: `kPreserve`, `kFree`, one Consonant (e.g. default octave window). Modes: all four. Actions: MUTATE primary; RETRY = second `apply` from parent with different deterministic RNG.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` — builds test runner with new sources.
- `ctest --preset macos-debug-arm64 -R MutationPlayability` (or UnitTestRunner filter for the new suite name) — green in seconds.
- `python3 Scripts/quality/lint_touched.py` on touched C++ — clean.
- Confirm AntiSilence + Calibration still pass in the same run.

## Suggested Review Order

**Production re-arm (guard hole found by oracle)**

- Second filter pass after MM opener restore — the behavioural fix.
  [`MutationAlgorithm.cpp:124`](../../Source/Core/Services/PatchMutator/MutationAlgorithm.cpp#L124)

- Wired into apply after post-mutation guards.
  [`MutationAlgorithm.cpp:176`](../../Source/Core/Services/PatchMutator/MutationAlgorithm.cpp#L176)

- Shared filter-path entry used by both guard passes.
  [`MutationPostApply.h:44`](../../Source/Core/Services/PatchMutator/MutationPostApply.h#L44)

**Test-only playability oracle**

- Structural checklist API (classes A/B/C/E).
  [`MutationPlayabilityOracle.h:18`](../../Tests/Unit/MutationPlayabilityOracle.h#L18)

- Thresholds read from MutationCalibration floors.
  [`MutationPlayabilityOracle.cpp:156`](../../Tests/Unit/MutationPlayabilityOracle.cpp#L156)

**Corpus fuzz + fixtures**

- Fixed 16 ROM paths + trap builders + wide-pass switch.
  [`MutationCalibrationTestSupport.h:196`](../../Tests/Unit/MutationCalibrationTestSupport.h#L196)

- MUTATE matrix over ROM + traps + Init.
  [`MutationPlayabilityCorpusTests.cpp:146`](../../Tests/Unit/MutationPlayabilityCorpusTests.cpp#L146)

- Dead → playable Should case.
  [`MutationPlayabilityCorpusTests.cpp:189`](../../Tests/Unit/MutationPlayabilityCorpusTests.cpp#L189)

**Pinned re-arm regressions**

- Deterministic opener restore then filter re-arm.
  [`MutationFilterPathRearmTests.cpp:15`](../../Tests/Unit/MutationFilterPathRearmTests.cpp#L15)

**Harness / wiring**

- Optional argv name filter for the test runner.
  [`TestMain.cpp:21`](../../Tests/TestMain.cpp#L21)

- New Unit sources registered on Matrix-Control_Tests only.
  [`CMakeLists.txt:422`](../../CMakeLists.txt#L422)

