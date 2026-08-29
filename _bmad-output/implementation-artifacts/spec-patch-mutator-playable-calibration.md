---
title: 'Patch Mutator playable calibration (MODE + PITCH)'
type: 'feature'
created: '2026-08-29'
status: 'done'
baseline_commit: '0618096c63063013c051c7d248fbd9d21ae2347f'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/planning-artifacts/architecture/architecture-matrix-control-2026-05-25/mutation-algorithm-spec-addendum-playable-calibration.md'
  - '{project-root}/_bmad-output/planning-artifacts/architecture/architecture-matrix-control-2026-05-25/mutation-algorithm-spec.md'
  - '{project-root}/_bmad-output/brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/intention-patch-mutator-jouable.md'
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** MUTATE / RETRY jitter Amount × Random without musical guards, so results can be silent or smothered — including with Matrix Modulation — while the UI still exposes Amount / Random instead of artistic distance.

**Approach:** Keep parent §4 curve math and Option A′ MM gating; drive A/R from named MODE constants; add policy table + post-apply anti-silence; replace visible Amount/Random with MODE + PITCH (ALL CAPS labels and items); ship addendum Must v1 including the seven musical tracks. Playable ≠ timid.

## Boundaries & Constraints

**Always:**
- Anti-silence on every MODE; write only enabled Mutator toggles; full-patch read OK.
- MODE → named `kModeAmountPercent` / `kModeRandomPercent` (placeholders Kindred 20/15, Drift 45/30, Warp 70/55, Wild 95/85); A>0 and R>0 so MODE never early-exits alone.
- UI labels `MODE` / `PITCH`; combo items ALL CAPS (`KINDRED`…`WILD`; `PRESERVE`, `CONSONANT`/`DISSONANT` ±N OCT children, `FREE`); 56 px slots; Amount/Random not primary.
- Pitch Preserve blocks DCO FREQUENCY + MM pitch dests; Kindred/Drift preserve DCO1–DCO2 interval when both mutable; Consonant/Dissonant = discrete deltas; Free = MODE-scaled §4.1 neighborhood.
- Kindred MM = Amounts on live buses only; no inventing MM→VCA2 relay in v1; English-only strings.
- Legacy Amount/Random MAY persist for migration but MUST NOT override MODE at MUTATE/RETRY.

**Ask First:**
- Retune MODE A/R placeholders away from the addendum table after hardware audition.
- Any Should item that would delay Must ship (guard indicator, dead→playable golden, Unison/Keyboard LOCK, measured Noise inject).

**Never:**
- Change Amount×Random curve math; footer help; Patch Edit padlocks; invent MM→VCA2 relay; UI beyond MODE+PITCH swap; French in source.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Kindred MUTATE | Audible seed; D1+VCF on; MM off | Sparse cousin; VCA1 locked; no dual WaveSelect OFF; Kindred A/R | N/A |
| Pitch Preserve + MM | D1/D2 + MM; pitch dests | FREQUENCY + MM pitch dests unchanged | N/A |
| Anti-silence POST | Low VCA/filter or dual-OFF risk after jitter | Post-pass restores playable path within enabled toggles | Silence only in disabled modules stays unrepaired |
| Kindred MM ladder | MM on; live buses | Amounts on active buses only; risk Amount caps | N/A |
| Option A′ | enableMatrixMod false | Bytes 104–133 unchanged | N/A |
| Early exit | A=0 or R=0 (non-MODE) | apply false; buffer unchanged | Engine footer as today |
| RETRY diversity | Second RETRY same parent | Not trivially identical (distance heuristic) | Rare RNG equality OK |

</frozen-after-approval>

## Code Map

- `MutationAlgorithm.h` (~11–69) — extend `MutationRecipe` with MODE + PITCH; A/R as resolved internals.
- `MutationAlgorithm.cpp` (~38–186, Option A′ ~80–114) — wrap §4 with LOCK/BORNE/CHOIX; post-apply + MM motion guarantee (split files if size).
- New policy/post-apply units under `Source/Core/Services/PatchMutator/` — named constants; register in `CMakeLists.txt`.
- `PatchMutatorEngine.cpp` (~169–228) + `MutatorSessionPersistence.h` (~49–77) — MODE→A/R + PITCH from state; Amount/Random do not override.
- `PatchMutatorEngineActions.cpp` (~15–108) — MUTATE/RETRY; RETRY diversity.
- `PluginIDs.h` (~651–675) + `PluginDisplayNames.h` (~1107+) — MODE/PITCH IDs + ALL CAPS ChoiceLists.
- `PatchMutatorPanelSetup.cpp` (~77–142), `PanelLayout.cpp` (~93–194), `Panel.h` — swap sliders for MODE combo + PITCH `HierarchicalComboBox` at 56 px (reuse History).
- `Tests/Unit/MutationAlgorithmTests.cpp` (+ new focused tests) — matrix edges; keep gv03 Option A′; register in CMake.
- Read-only: addendum §A1–A10; parent §3–§7; `MutationNaming` History M/R unchanged.

## Tasks & Acceptance

**Execution:**
- [x] `MutationAlgorithm.h` (+ policy companions) — MODE/PITCH recipe; named MODE A/R; LOCK/BORNE/CHOIX/POST table — table-driven guards without §4 math change.
- [x] `MutationAlgorithm.cpp` (+ post-apply) — pipeline A9 + Pitch + MM ladder + seven tracks — Must calibration.
- [x] `PatchMutatorEngine.cpp` / `MutatorSessionPersistence.h` / `PluginIDs.h` — state MODE+PITCH drive recipe — source of truth.
- [x] `PluginDisplayNames.h` + `PatchMutatorPanel*.{h,cpp}` — MODE/PITCH ALL CAPS @ 56 px; hide Amount/Random — validated surface.
- [x] `Tests/Unit/MutationAlgorithmTests.cpp` (+ extras) — I/O matrix; Option A′ green — no silent regressions.
- [x] `CMakeLists.txt` / `Tests/CMakeLists.txt` — register new sources — linkable build.

**Acceptance Criteria:**
- Given any MODE with a module on, when MUTATE/RETRY runs, then Must anti-silence guards hold within writable toggles (VCA chain, filter couple, dual WaveSelect OFF, Mix vs OFF, MM risk Amount caps).
- Given a MODE, when recipe builds, then A/R come from that MODE’s named constants and §4 math is unchanged.
- Given Pitch PRESERVE, when D1/D2 and/or MM pitch dests are in scope, then those fields are unchanged.
- Given Kindred + MM on, when mutate runs, then MM changes Amounts on live buses only (no free lethal Source/Dest birth).
- Given enableMatrixMod false, when mutate runs, then bytes 104–133 unchanged.
- Given Mutator panel open, when inspecting recipe row, then MODE and PITCH (ALL CAPS) occupy former Amount/Random slots and Amount/Random are not primary.
- Given Kindred/Drift, both DCO freqs mutable, Pitch ≠ PRESERVE, when pitch jumps, then DCO1–DCO2 interval is preserved.

## Design Notes

Pipeline (addendum A9): recipe → early-exit → per-descriptor policies wrapping §4 → post-apply → MM motion guarantee → changed-bytes.

PITCH UI: `HierarchicalComboBox` — primaries PRESERVE / CONSONANT / DISSONANT / FREE; ±1…±5 OCT children under Consonant/Dissonant.

Should items only if Must is green and capacity remains; else follow-on.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — expected: success.
- Filter MutationAlgorithm / PatchMutatorEngine Mutate|Retry / MutatorRecipe tests — expected: pass.
- `python3 Scripts/quality/lint_touched.py` — expected: clean on touched C++.

**Manual checks:**
- Standalone: MODE×PITCH audible Kindred→Wild; Amount/Random hidden; History M/R OK; MM off leaves MM bytes alone.

## Suggested Review Order

**Pipeline entry**

- MODE resolves A/R, then jitter, post-apply, MM motion guarantee.
  [`MutationAlgorithm.cpp:64`](../../Source/Core/Services/PatchMutator/MutationAlgorithm.cpp#L64)

**MODE / PITCH constants and recipe**

- Named MODE placeholders and policy kinds.
  [`MutationPolicy.h:13`](../../Source/Core/Services/PatchMutator/MutationPolicy.h#L13)

- Recipe carries MODE + PITCH; `applyModeCalibration` fills A/R.
  [`MutationRecipe.h:46`](../../Source/Core/Services/PatchMutator/MutationRecipe.h#L46)

- APVTS/state builds recipe from MODE + PITCH.
  [`PatchMutatorEngine.cpp:246`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L246)

**Pitch and Matrix Modulation**

- Preserve / Consonant / Dissonant / Free planning.
  [`MutationPitchPolicy.cpp:188`](../../Source/Core/Services/PatchMutator/MutationPitchPolicy.cpp#L188)

- Kindred→Wild MM ladder and motion guarantee.
  [`MutationMatrixModPolicy.cpp:158`](../../Source/Core/Services/PatchMutator/MutationMatrixModPolicy.cpp#L158)

- Amount mutates only when ladder grants Amount.
  [`MutationAlgorithm.cpp:297`](../../Source/Core/Services/PatchMutator/MutationAlgorithm.cpp#L297)

**Anti-silence post-apply**

- Coupled guards after jitter (VCA, filter, WaveSelect, Mix, FM nudge).
  [`MutationPostApply.cpp:283`](../../Source/Core/Services/PatchMutator/MutationPostApply.cpp#L283)

**RETRY diversity**

- Re-roll until enough mutable bytes differ.
  [`PatchMutatorEngine.cpp:209`](../../Source/Core/Services/PatchMutator/PatchMutatorEngine.cpp#L209)

**Mutator UI**

- MODE combo + PITCH hierarchical combo (ALL CAPS).
  [`PatchMutatorPanelSetup.cpp:56`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/PatchMutatorPanelSetup.cpp#L56)

- Display strings `MODE` / `KINDRED` / …
  [`PluginDisplayNames.h:1109`](../../Source/Shared/Definitions/PluginDisplayNames.h#L1109)

**Tests**

- MODE / PITCH / MM ladder coverage.
  [`MutationCalibrationTests.cpp:8`](../../Tests/Unit/MutationCalibrationTests.cpp#L8)

- Anti-silence through full `apply` pipeline.
  [`MutationAntiSilenceTests.cpp:8`](../../Tests/Unit/MutationAntiSilenceTests.cpp#L8)

### Review Findings

_Chunk 1 — pipeline core (MutationAlgorithm + policy / pitch / Matrix Mod / post-apply). Remaining chunks: engine/state, UI, tests._

- [x] [Review][Decision] Re-run amplitude/escalation guards after MM motion? — resolved → patch (option 2: re-run volume/envelope guards after motion revive)
- [x] [Review][Decision] Ship without family-gesture coupling? — resolved → patch (option 2: implement family gestures now for A4 #2 Must)

- [x] [Review][Patch] After MM motion/cap, re-run amplitude path + escalated envelope role guards [`MutationAlgorithm.cpp:98`]
- [x] [Review][Patch] Family gestures: couple related moves during jitter (e.g. envelope contour neighborhood; Frequency+Resonance as a family) [`MutationAlgorithm.cpp` / `MutationPolicy`]
- [x] [Review][Patch] Pitch Preserve: MM motion must not revive amount on a pitch destination bus [`MutationMatrixModPolicy.cpp:269`]
- [x] [Review][Patch] Dual WaveSelect OFF: do not early-return when seed activeDcoCount is 0 — restore a writable oscillator [`MutationPostApply.cpp:204`]
- [x] [Review][Patch] Drift growth bus: when routing is granted on a dead bus but another live bus already has amount, still seed the growth bus amount [`MutationMatrixModPolicy.cpp:262`]
- [x] [Review][Patch] WaveSelect CHOIX during choice jitter: refuse a pick that would leave both oscillators OFF before POST [`MutationAlgorithm.cpp:259`]
- [x] [Review][Patch] Preserve destination pick: if redirected pitch pick and timbre list empty, fail closed to None [`MutationMatrixModPolicy.cpp:224`]

- [x] [Review][Defer] Motion / Preserve-dest / choice-step / interval-quality / cardinality / escalated-floor tests incomplete — deferred, cover in tests chunk
- [x] [Review][Defer] Kindred LFO “rates that breathe” (A4 #4 SHOULD) — deferred, pre-existing follow-on / Should
- [x] [Review][Defer] Unused `amplitudeDestinations` catalog field — deferred, dead data cleanup after Must patches

### Review Findings — Chunk 2 (engine / state / IDs)

- [x] [Review][Decision] When RETRY never finds a “different enough” sibling, warn or keep the first usable roll? — resolved → patch (option 2: warn / no History entry; footer must not truncate)
- [x] [Review][Decision] Keep the short “Try a wider MODE” footer, or expand recovery guidance? — resolved → patch (option 2: mention modules too; keep copy short for left-band footer)

- [x] [Review][Patch] RETRY diversity exhaustion: return failure + dedicated short footer instead of accepting a near-clone [`PatchMutatorEngine.cpp:236`]
- [x] [Review][Patch] Expand no-change footer to mention MODE and modules, keep under truncation risk [`PatchMutatorEngineInternal.h:34`]
- [x] [Review][Patch] Clamp `formatOctaveWindow` inputs to the legal octave range [`PluginDisplayNames.h:1162`]
- [x] [Review][Patch] `static_assert` ChoiceLists MODE/PITCH lengths match Core enum counts [`PluginDisplayNames.h:1149`]

- [x] [Review][Defer] Engine APVTS→recipe / diversity re-roll / footer-string tests incomplete — deferred, cover in tests chunk
- [x] [Review][Defer] Legacy Amount/Random descriptors and session defaults remain for migration — deferred, cleanup follow-on
- [x] [Review][Defer] MODE-aware RETRY diversity threshold (Kindred vs Wild) — deferred, A4 #5 SHOULD tuning

### Review Findings — Chunk 3 (Mutator UI panel)

- [x] [Review][Decision] Closed PITCH label in the 56px slot — resolved → patch (Figma: closed face `CONS ±5` / `DIS ±5`; menu open keeps full ALL CAPS + ± N OCT; prefer ±, ASCII `+/-` fallback if typeface lacks the glyph)

- [x] [Review][Patch] Compact closed-face PITCH text (`CONS ±5` / `DIS ±5`; PRESERVE/FREE unchanged) without changing open-menu labels [`HierarchicalComboBox` / `PatchMutatorPanelSetup`]
- [x] [Review][Patch] Wrap PITCH APVTS writes in `recipeHydrating_` and clamp octave window on write [`PatchMutatorPanel.cpp:198`]
- [x] [Review][Patch] Clamp MODE index on write like hydrate does [`PatchMutatorPanel.cpp:186`]
- [x] [Review][Patch] Remove dead `findMutatorIntDescriptor` after Amount/Random slider removal [`PatchMutatorPanelInternal.h:85`]
- [x] [Review][Patch] `static_assert` octave encode assumes max octaves &lt; 10 [`PatchMutatorPanelInternal.h:101`]

- [x] [Review][Defer] Pitch id helpers / recipe-property list / Compare-lock UI tests — deferred, cover in tests chunk (pure helpers OK)
- [x] [Review][Defer] Component ids on hand-built MODE/PITCH combos — deferred, harness tooling follow-on
- [x] [Review][Defer] Compare `setInactiveAppearance` on HierarchicalComboBox — deferred, History also uses setEnabled-only

### Review Findings — Chunk 4 (tests)

- [x] [Review][Decision] Add a Must coverage pack now, or leave remaining gaps for a follow-on after Standalone smoke? — Chose Must pack now (pipeline + RETRY + harness).

- [x] [Review][Patch] Assert Matrix Mod motion through full `mutated()` / `apply`, not only `ensureMatrixModMotion` direct [`MutationCalibrationTests.cpp`]
- [x] [Review][Patch] Assert family cutoff→resonance through full `apply`, not only `applyFamilyGestures` direct [`MutationCalibrationTests.cpp`]
- [x] [Review][Patch] Assert RETRY `kTooSimilar` failure + footer copy; do not append History [`PatchMutatorEngineRetryTests.cpp`]
- [x] [Review][Patch] Harden `writeMatrixModBus` against unknown choice names (`indexOf` &lt; 0) [`MutationCalibrationTestSupport.h`]
- [x] [Review][Patch] Guard optional entry dereference after failed expect in RETRY diversity test [`PatchMutatorEngineRetryTests.cpp:204`]

- [x] [Review][Defer] Consonant/Dissonant set membership + octave-window effect — deferred, follow-on (interval-only coverage today)
- [x] [Review][Defer] Choice-step limits / WaveSelect CHOIX mid-jitter / active-DCO cardinality — deferred, follow-on
- [x] [Review][Defer] Escalated ENV floors + FM nudge; filter/Mix through full apply — deferred, follow-on
- [x] [Review][Defer] Engine `buildRecipeFromApvts` MODE A/R percentages + pitch octaves via harness — deferred, follow-on
- [x] [Review][Defer] Pitch UI id helpers / compact closed labels / recipe-property list — deferred, follow-on (manual UI OK for v1)
- [x] [Review][Defer] Soft probabilistic “ever moved” gates and Preserve empty-timbre invent — deferred, follow-on
