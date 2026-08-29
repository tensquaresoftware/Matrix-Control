---
organization: Ten Square Software
project: Matrix-Control
title: MutationAlgorithm Specification — Addendum Playable Calibration
author: BMad Agent
status: draft
version: "1.0-draft"
parent: mutation-algorithm-spec.md
sources:
  - mutation-algorithm-spec.md
  - ../../../../brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/intention-patch-mutator-jouable.md
  - ../../../../brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/synthesis-playable-calibration.md
  - ../../../../brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/parametres-extinctions-son.md
  - ../../../../brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/interactions-contre-intuitives.md
created: 2026-08-29
updated: 2026-08-29
---

# Addendum — Playable calibration

Normative extension to [`mutation-algorithm-spec.md`](mutation-algorithm-spec.md) (v1.1). Product decisions from brainstorm **playable calibration** (August 2026). Where this addendum conflicts with the parent on playability / MODE / Pitch / Matrix Modulation scaling, **this addendum wins**. The parent remains authoritative for buffer layout, early-exit, Amount×Random **curve math**, RNG, and golden-vector seed policy unless explicitly amended below.

**Product through-line:** MUTATE / RETRY results MUST be **audible and interesting**, including when Matrix Modulation is enabled — without structural silence or systematic smothering.

**Playable ≠ timid.**

| Layer | Role |
|---|---|
| **MODE** (`Kindred` → `Drift` → `Warp` → `Wild`) | Artistic distance from the seed |
| **Anti-silence guards** | Invariant under all MODEs |
| **PITCH** | Creative permission for DCO (and MM pitch destinations) |

**Out of scope for this addendum:** create-story / C++ implementation; footer contextual help (parked — see §11); changing the base Amount×Random distribution formula (§4 of the parent).

---

## A1. Amendments to parent Recipe / UI surface

### A1.1 MODE and PITCH replace visible AMOUNT / RANDOM

The Patch Mutator panel **MUST** expose:

| Control | Role |
|---|---|
| **MODE** | Kindred / Drift / Warp / Wild — sets internal Amount and Random profiles |
| **Pitch** | Preserve / Consonant ±N Oct / Dissonant ±N Oct / Free |

Visible Amount and Random sliders **MUST NOT** remain the primary Mutator UI. Amount (`A`) and Random (`R`) **remain inside the algorithm** as named constants per MODE (§A3). Parent §4 curve math is unchanged.

### A1.2 Recipe shape (normative direction)

`MutationRecipe` **MUST** eventually carry MODE and PITCH (exact C++ field names left to implementation). At apply time the engine **MUST**:

1. Resolve `(amountPercent, randomPercent)` from the MODE constant table (§A3).
2. Apply PITCH policy when mutating DCO FREQUENCY and Matrix Modulation pitch destinations (§A5).
3. Keep module toggles D1…L2 + MM as today (parent §2.2 / §2.2.1).

Legacy APVTS Amount/Random values **MAY** persist for migration; once MODE is the source of truth they **MUST NOT** override MODE profiles during MUTATE/RETRY.

### A1.3 Early exit

Parent §3 early-exit (`A == 0` OR `R == 0`) remains. MODE profiles **MUST** use `A > 0` and `R > 0` so production recipes never hit the no-op path via MODE alone.

---

## A2. Technical foundation — policy table + post-apply pass

### A2.1 Hybrid timing (MUST from v1)

| Phase | When | What runs |
|---|---|---|
| **During jitter** | Per eligible descriptor, before/while applying parent §4 | Simple policies: **LOCK**, **BORNE** (MODE neighborhood + floors), **CHOIX** (restricted discrete sets), Pitch gates |
| **Post-apply** | Once after all eligible descriptors have been processed | Coupled multi-parameter rules (§A6) |

Implementations **MUST** ship an extensible **policy table** (even with few rows initially) rather than scattering hard-coded floors only inside `MutationAlgorithm`. Numeric thresholds **SHOULD** live as named data/constants, not magic literals buried in conditionals.

### A2.2 Policy kinds

| Kind | Meaning |
|---|---|
| **LOCK** | Do not mutate; leave seed value |
| **BORNE** | MODE-scaled neighborhood around seed; optional absolute floor/ceiling |
| **CHOIX** | Discrete allowed set (WaveSelect cardinality, Sync aggressiveness, Pitch jumps, …) |
| **POST** | Correct after jitter using multiple parameters / MM bus state |
| **LIBRE\*** | Default MODE-scaled BORNE (continuous / non-killer params) |

### A2.3 Default continuous-param rule

Unless overridden by LOCK, anti-silence floor, discrete CHOIX, or MM Source/Dest ladder:

> **MODE-scaled neighborhood around seed** — Kindred tight → Wild wide.

### A2.4 Three tiers (always)

1. **Algo-implicit / sacred** — e.g. VCA1 Volume LOCK Kindred–Warp; Wild ultra-light neighborhood + floor.
2. **Silent playable guards** — floors, coupled post-apply, MM Amount caps on risk destinations — **all MODEs**.
3. **Creative permission** — MODE + Pitch (not a grid of per-parameter padlocks on Patch Edit).

### A2.5 Module toggles — read vs write (MUST)

| Action | Rule |
|---|---|
| **WRITE** | **MUST NOT** mutate parameters belonging to a disabled Mutator module toggle (parent §5). No cheat. |
| **READ** | **MAY** read the **full** patch (including Matrix Modulation buses when MM toggle is off) to decide intelligently (e.g. detect MM → VCA2 Level relay for VCA2←ENV2 exception). |

**Known limit:** if silence already lives in a **disabled** module’s values, the Mutator cannot repair without cheating. Seed **SHOULD** be playable, or the user enables that toggle.

Post-apply corrections **MUST** only write parameters whose owning Mutator toggle is on (or MM bytes when `enableMatrixMod`).

---

## A3. MODE → internal Amount / Random profiles

| MODE | Artistic intent | Hidden Amount | Hidden Random | Matrix Modulation ladder (summary) |
|---|---|---|---|---|
| **Kindred** | Cousin of the seed — essence preserved | Low | Low | **Amounts only** on already-active buses |
| **Drift** | Recognizable drift | Mid | Mid-low | Amounts + optional **one** safe timbre bus |
| **Warp** | Bold reshape | High | Mid-high | More Source/Dest reroutes; bias living pairs |
| **Wild** | Everything allowed **except silence** | Very high | High | Freer reroutes; anti-silence Amount caps still on |

Exact percent constants are **placeholders** for tuning during implementation tests, then freeze for v1:

| MODE | `kModeAmountPercent` (placeholder) | `kModeRandomPercent` (placeholder) |
|---|---|---|
| Kindred | 20 | 15 |
| Drift | 45 | 30 |
| Warp | 70 | 55 |
| Wild | 95 | 85 |

Implementations **MUST** expose these as named constants easy to retune without changing §4 math. Anti-silence guards **MUST NEVER** be disabled by MODE.

---

## A4. Sparse mutation and musical algo tracks (v1 MUST)

These tracks do **not** enlarge the Mutator GUI. They refine how MODE profiles and policies behave.

| # | Track | Normative intent |
|---|---|---|
| 1 | **Sparse Kindred** | Kindred mutates **fewer** parameters (or with lower effective hit rate) so gestures stay clear — not a full-buffer sprinkle. |
| 2 | **Family gestures** | Prefer coupled moves (e.g. envelope contour neighborhood; Frequency + Resonance treated as a family) over isolated extremes. |
| 3 | **MM expressive bias** | Prefer Source/Dest pairs that reveal Matrix Modulation: ENV / LFO / Track Generator / Ramps → timbre and motion destinations. |
| 4 | **LFO rates that breathe** | Kindred **SHOULD** avoid useless extremes (very slow + deep toward volume feels like silence on short notes). |
| 5 | **RETRY diversity** | RETRY **SHOULD** avoid a result too close to the previous mutation (distance heuristic — exact metric left to implementation). |
| 6 | **MM motion guarantee** | When `enableMatrixMod` is true, after mutation there **MUST** be at least one **audible motion** path (living Amount on a useful bus, or equivalent). Success = audible **and** movement. |
| 7 | **Pitch** | Preserve / Consonant / Dissonant / Free + DCO interval rules by MODE (§A5). |

---

## A5. Pitch policy

### A5.1 Control shape (two-level combo)

| Choice | Meaning |
|---|---|
| **Preserve** | Both DCOs’ pitch intact — Patch Edit **and** Matrix Modulation destinations that change DCO pitch |
| **Consonant** → **± 1 Oct** … **± 5 Oct** | Discrete consonant interval jumps within ±N octaves of current pitch |
| **Dissonant** → **± 1 Oct** … **± 5 Oct** | Same window; dissonant intervals (2nds, 7ths, tritone) for tension textures |
| **Free** | MODE-scaled continuous neighborhood (parent §4.1 on FREQUENCY), clamped to 0…63 |

Rejected label: `Fixed` (reads like keyboard stuck on one note).

### A5.2 Window around current value

For each oscillator, the legal window is approximately **N octaves below to N octaves above** its **current** frequency, clamped to hardware range **0…63**. Near the floor, most room is upward; near the ceiling, most room is downward. Do **not** promise “N octaves upward” as an absolute from zero.

DCO FREQUENCY is already **semitone-calibrated** (+7 ≈ fifth, +12 ≈ octave, etc.). Consonant / Dissonant **MUST** use **discrete allowed deltas**, not continuous Amount×Random jitter on FREQUENCY.

**Consonant interval set (normative sketch — document in user manual):** perfect 5th / 4th, major/minor 3rd, major/minor 6th, octave (and multiples within the ±N window). Skip unison-as-jump when already at target.

**Dissonant interval set (normative sketch):** minor/major 2nd, tritone, minor/major 7th (and multiples within window).

### A5.3 Dual-DCO interval preservation

When **both** DCO FREQUENCY parameters are mutable (D1 and D2 toggles on) and Pitch ≠ Preserve:

| MODE | Rule |
|---|---|
| **Kindred / Drift** | **MUST** preserve the **interval** between DCO1 and DCO2 (e.g. octave doubling stays an octave after jumps). |
| **Warp / Wild** | Each DCO **MAY** jump **independently**. |

### A5.4 Preserve is global

When Pitch = **Preserve**:

- **MUST NOT** mutate DCO1 / DCO2 FREQUENCY even if D1/D2 toggles are on.
- **MUST NOT** mutate Matrix Modulation **destinations** (or Amounts that only serve pitch dests — implementation detail) that change DCO pitch, even if MM is on.
- Other MM / Patch Edit mutation continues under MODE and anti-silence rules.

---

## A6. Anti-silence guards (all MODEs)

Guards apply after / during mutation per A2. Exact numeric floors are placeholders for hardware tuning.

### A6.1 Amplitude path (class A) — Priority 1

| Parameter / couple | Policy |
|---|---|
| **VCA 1 VOLUME** | **LOCK** Kindred / Drift / Warp. Wild: ultra-light BORNE near seed + hard floor (never near 0). |
| **VCA 2 ← ENV 2** | Kindred–Warp: **LOCK** or very high floor near seed/max, **unless** an existing MM bus already drives **VCA 2 Level** with usable Source + Amount (relay). Wild: ultra-light BORNE + floor + same exception. **MUST NOT** invent a new MM relay from scratch in v1. |
| **ENV 2 · AMPLITUDE** | Hard floor all MODEs; MODE-BORNE above floor; couple with **ENV 2 · AMP ← VELOCITY** for effective depth. |
| **VCA 1 ← VELOCITY** / **ENV 2 Amp ← VELOCITY** | MODE-BORNE; careful with strong negatives when base Volume/Amp already low. Velocity is a **soft-kill** (depends on playing); do not treat as equal to hard path-kills. Note: at Volume ≈ 63, **positive** velocity mod is nearly inaudible (ceiling); **negative** velocity still ducks — do not treat negatives as no-ops. |
| **ENV 2** role | Amplitude envelope (hardwired → VCA2) — stricter than ENV 1 (filter) or ENV 3 (FM). |

### A6.2 Filter smother (class B) — Priority 2

| Rule | Policy |
|---|---|
| **FREQUENCY + RESONANCE** | MODE-BORNE near seed. **POST:** if both too low, bump one or both. Low cutoff + **high** resonance **MAY** remain (resonance can rescue audibility). |
| **FREQ ← ENV 1 / PRESSURE** | Constrain strong **negatives** when cutoff already low. |

### A6.3 Sources / Mix (class C) — Priority 2

| Rule | Policy |
|---|---|
| **Dual WaveSelect OFF** | **FORBIDDEN** (CHOIX + POST). |
| **Active-DCO cardinality** | If seed has **two** active DCOs → keep two. If seed has **one** → keep exactly one (roles **MAY** swap). |
| **Noise** | If seed has Noise → keep it. If not → **MAY** add moderately in explorer MODEs when a slot is free (**Should** if deferred). |
| **Mix → OFF DCO** | MODE-BORNE; **POST** if Mix extreme toward an OFF oscillator — nudge Mix and/or WaveSelect (writes only if owning toggles on). |

Pulse Width extremes: **demoted** — pinched but audible; not a v1 hard-kill priority.

### A6.4 Matrix Modulation risk Amounts (class E)

When MM scope is active, for buses targeting risk destinations (**VCA 1 VOLUME**, **VCA 2 Level**, **ENV 2 AMPLITUDE**, **VCF FREQUENCY**, **DCO Mix**):

- **MUST** cap unfavorable Amounts (especially strong **negatives** toward amplitude destinations).
- Caps apply in **all** MODEs.
- Pitch destinations follow §A5.

### A6.5 Effective-role escalation

| Default role | Amp / depth policy |
|---|---|
| **ENV 1** (filter) | Soft / conditional floor; **escalate** to hard when any active MM bus has Source = ENV1 (or Track fed by ENV1) and Dest in risk set. |
| **ENV 2** (amplitude) | Hard floor always (above). |
| **ENV 3** (FM) | No default silence floor; Warp/Wild **MAY** raise Amp for audible FM; escalate if MM Source = ENV3 → risk dest. |
| **LFO 1 / 2** | MODE-BORNE; LFO → VCA is usually **tremolo**, not permanent silence. Hard Amp min **only** if LFO is the **sole** VCA opener (MM relay + VCA2←ENV2 effectively dead). Kindred soft-avoid very slow + deep toward volume. |

### A6.6 FM interest (not silence)

FM / TRACK: MODE-BORNE; no silence floor. **Warp / Wild:** nudge **FM Amount** upward when FM mods are present so FM remains audible.

### A6.7 Keyboard Mode / Unison (**Should**)

LOCK through Warp; free in Wild — if deferred from Must, treat as early follow-on.

---

## A7. Parameter → policy summary

Legend: **LOCK** / **BORNE** / **CHOIX** / **POST** / **LIBRE\***.

### A7.1 DCO 1 / DCO 2

| Param | Policy |
|---|---|
| WaveSelect | **CHOIX + POST:** never dual OFF; preserve active-DCO cardinality; Noise keep-if-seed; optional moderate Noise add |
| FREQUENCY | Gated by **Pitch** (§A5) |
| Sync | MODE-scaled discrete CHOIX (freer toward Wild) |
| Wave Shape, Pulse Width, Detune (DCO2) | MODE-BORNE near seed |
| Freq←LFO, PW←LFO, Levers, Keybd/Porta, KeyClick | LIBRE\* |

### A7.2 VCF / VCA

| Param | Policy |
|---|---|
| VCA1 Volume | LOCK Kindred–Warp; Wild ultra-light BORNE + floor |
| VCA2←ENV2 | LOCK / very high floor Kindred–Warp unless MM→VCA2; Wild ultra-light + floor |
| ENV2 Amplitude | Hard floor all MODEs; MODE-BORNE above; couple Amp←Vel |
| VCA1←Vel, ENV2 Amp←Vel | MODE-BORNE; coupled with bases |
| Frequency + Resonance | MODE-BORNE; **POST** if both too low |
| Freq←ENV1 / Pressure | Constrain strong negatives when cutoff low |
| Mix | MODE-BORNE; **POST** vs OFF DCO |
| Levers, Keybd/Porta | LIBRE\* |

### A7.3 FM / TRACK / Ramp / Portamento

| Area | Policy |
|---|---|
| FM / TRACK | MODE-BORNE; Warp/Wild FM Amount nudge when mods present |
| Ramp rates / porta mod | MODE-BORNE |
| Discrete ramp / porta modes | Kindred near seed |
| Keyboard Mode / Unison | Sacred LOCK through Warp; free Wild (**Should** if deferred) |

### A7.4 Envelopes / LFOs / Matrix Mod

See §A6.5 and §A3 / §A8. Shared ADSR **structure**; different Mutator treatment by **default role** + **effective role**.

---

## A8. Matrix Modulation MODE ladder (when `enableMatrixMod`)

Parent Option **A′** (recipe-gated) remains: mutate bytes 104–133 only when `enableMatrixMod == true`.

| MODE | Mutation |
|---|---|
| **Kindred** | Amounts on **already-active** buses almost only — the mode name must stay honest |
| **Drift** | + optional **one** safe timbre bus |
| **Warp / Wild** | Increasing Source/Dest freedom; bias ENV / LFO / Track / Ramp → timbre / motion |
| **All** | Cap unfavorable Amounts to risk destinations; pitch dests follow Pitch combo |

Kindred **MUST NOT** freely birth lethal Source→risk Dest buses.

---

## A9. Pipeline order (normative sketch)

```
1. Build recipe (MODE → A,R; Pitch; toggles)
2. Early-exit if A==0 or R==0 (parent §3)
3. For each eligible descriptor (parent §4.3 + §5 + §7):
     - Skip if Pitch Preserve blocks pitch field
     - Apply LOCK / BORNE / CHOIX / Pitch jump / MM ladder instead of or wrapping §4
4. Post-apply coupled pass (§A6) — writes only enabled modules
5. MM motion guarantee if enableMatrixMod (§A4 #6)
6. Return true/false per parent §3 (bytes 8–133 changed)
```

Bytes 0–7 remain excluded (parent §6).

---

## A10. MoSCoW

### Must (v1)

- Policy layer + post-apply pass  
- Anti-silence: VCA chain, Frequency+Resonance couple, dual WaveSelect OFF forbid, Mix vs OFF DCO, MM Amount caps on risk dests  
- Mutator UI: **MODE** + **Pitch**; Amount/Random hidden; named constants per MODE  
- Toggle respect (write enabled only; read full patch OK)  
- Pitch **Preserve** global (Patch Edit + MM pitch dests)  
- WaveSelect **cardinality** + Noise keep-if-seed  
- **VCA2←ENV2** high / LOCK Kindred–Warp unless MM→VCA2 relay; Wild ultra-light + floor; no inventing MM relay in v1  
- **Effective-role escalation** for ENV Amp when MM Source drives risk dest (ENV1/ENV3; LFO only if sole VCA opener)  
- **FM Amount nudge** in Warp/Wild when FM mods need audibility  
- **Kindred MM = Amounts only** on live buses  
- **Seven musical algo tracks** (§A4)

### Should (follow-on)

- Discreet indicator when a guard corrected the patch  
- Golden tests “dead → playable”  
- Keyboard Mode / Unison LOCK through Warp, free in Wild  
- Measured Noise inject when seed had no Noise  

### Won’t (this effort)

- Per-parameter padlocks on Patch Edit  
- Changing base Amount×Random **curve math** (profiles / caps only)  
- Footer contextual help as part of playable calibration (parked — §11)  
- create-story inside the brainstorm session  

---

## A11. Parked — footer contextual help

Ableton Live–style ephemeral help on the Mutator footer (hover + keyboard focus), starting with Compare / Delete / Flush / Export full names, is **parked** and out of scope for this addendum.

See: `_bmad-output/brainstorming/brainstorm-patch-mutator-playable-calibration-2026-08-26/parking-aide-contextuelle-footer.md`.

---

## A12. SysEx lesson (non-normative for mutation math)

Signed Remote Edit / Matrix Modulation Amounts must be masked `& 0x7F` on the wire (fixed + CI compliance tests). Do **not** re-derive “no effect” for negative mods from pre-fix hardware behavior when validating playable policies.

---

## A13. Traceability

| Decision / requirement | Section |
|---|---|
| Policy table + post-apply | §A2 |
| MODE → A×R constants | §A3 |
| Musical tracks ×7 | §A4 |
| Pitch Preserve / Consonant / Dissonant / Free | §A5 |
| Anti-silence guards | §A6 |
| MM MODE ladder | §A8 |
| MoSCoW | §A10 |
| Parent Amount×Random math | `mutation-algorithm-spec.md` §4 (unchanged) |
| Parent MM Option A′ | `mutation-algorithm-spec.md` §7.1 (unchanged) |
| Intention SSOT (product) | `intention-patch-mutator-jouable.md` |
| Structured synthesis | `synthesis-playable-calibration.md` |

---

## Revision history

| Date | Version | Change |
|---|---|---|
| 2026-08-29 | 1.0-draft | Initial addendum from playable-calibration brainstorm (Aug 2026) |
