# Patch Mutator — Playable Calibration Synthesis

**Session:** `brainstorm-patch-mutator-playable-calibration-2026-08-26`  
**Mode:** Creative Partner  
**Status:** Converged (MoSCoW revised by Guillaume)  
**Languages:** Chat FR; this artifact EN  
**Out of scope:** BMad create-story in this session  

**Companion docs (same folder):**
- `inventaire-parametres-patch-edit-matrix-modulation.md`
- `parametres-extinctions-son.md`
- `interactions-contre-intuitives.md`
- `.memlog.md`

---

## 1. Product through-line

Make **MUTATE / RETRY** results **audible and interesting**, including when **Matrix Mod** is enabled — without structural silence or systematic smothering.

**Playable ≠ timid.**  
- **MODE** (`Kindred → Drift → Warp → Wild`) = artistic distance from the seed.  
- **Anti-silence guards** = invariant under all MODEs.  
- Visible **Amount / Random** are replaced by **MODE** + **Pitch**; Amount×Random remain **internal** profiles (named constants per MODE for tuning).

---

## 2. Failure-mode map

| Class | Ear | Typical cause | Session status |
|-------|-----|---------------|----------------|
| **A** | Total silence on note | VCA1 / VCA2←ENV2 / ENV2 Amp path | Heard; primary |
| **B** | Smothered / near-inaudible | VCF Frequency low + Resonance low | Heard |
| **C** | No source | Dual WaveSelect OFF; Mix→OFF DCO | Theoretical; rules decided |
| **D** | “Dead to play” | ENV2 Delay/Attack extremes | Secondary |
| **E** | Same as A/B/C via bus | MM Amount toward risk destinations | Plausible; product = living MM |
| **F** | Interesting-but-dead MM | Modulation on, amp/filter collapsed | Success = audible **and** motion |

**Demoted:** Pulse Width extremes (pinched but audible).  
**SysEx lesson:** signed Remote Edit / MM Amounts must be `& 0x7F` on the wire (fixed + CI compliance tests) — do not re-derive “no effect” from pre-fix hardware.

---

## 3. Architecture decisions (First Principles)

| Decision | Choice |
|----------|--------|
| Extensible policy layer vs hardcode-only | **Policy table from v1** (even with few rows) |
| When rules run | **Hybrid:** simple LOCK / BORNE / CHOIX **during** jitter; **coupled** rules in **post-apply** |
| Behavior surface | Mutator panel: **MODE** + **Pitch** (replace Amount/Random UI lines) |
| Amount×Random | Stay in algorithm; **named constants per MODE**; tune in tests then freeze v1 |
| Module toggles | **No cheat:** never write params of a disabled Mutator module; may **read** full patch (e.g. MM relay detect) |
| Pitch Preserve | **Global:** blocks DCO FREQUENCY **and** MM destinations that change DCO pitch |

### Three tiers (always)

1. **Algo-implicit / sacred** — e.g. VCA1 Volume LOCK Kindred–Warp; Wild = ultra-light neighborhood + floor.  
2. **Silent playable guards** — floors, coupled post-apply, MM Amount caps (all MODEs).  
3. **Creative permission** — MODE + Pitch (not a grid of padlocks on Patch Edit).

### Default continuous-param rule

**MODE-scaled neighborhood around seed** (Kindred tight → Wild wide), **except:** implicit LOCKs, anti-silence floors, discrete CHOIX, MM Source/Dest reroutes (gated by MODE ladder).

---

## 4. MODE & Pitch (UX)

### MODE (ex-Behavior)

| MODE | Artistic intent | Hidden A×R | Matrix Mod (sketch) |
|------|-----------------|------------|---------------------|
| **Kindred** | Essence preserved | Low / low | **Amounts only** on already-active buses |
| **Drift** | Recognizable drift | Mid / mid-low | Amounts + optional **one** safe timbre bus |
| **Warp** | Bold reshape | High / mid-high | More Source/Dest reroutes (bias living) |
| **Wild** | Everything except silence | Very high / high | Freer reroutes; anti-silence Amount caps still on |

Anti-silence **never** disabled by MODE.

### Pitch combo (single control)

`Preserve` → `Consonant 1 Oct` … `Consonant 5 Oct` → `Free`

- **Preserve:** leave both DCOs’ pitch intact (Patch Edit + MM).  
- **Consonant:** **discrete semitone jumps** (5th / 4th / maj/min 3rd) within N octaves — **not** continuous jitter. DCO FREQUENCY is already semitone-calibrated (+7 = fifth, etc.).  
- **Free:** MODE-scaled neighborhood.  
Manual documents consonant interval set.

**Rejected label:** `Fixed` (sounds like keyboard stuck on one note).  
**Rejected:** per-param padlocks on Patch Edit (clutter; fights Mutator philosophy).

---

## 5. MoSCoW (revised)

### Must (v1)

- Policy layer + post-apply pass  
- Anti-silence: VCA chain, Frequency+Resonance couple, dual WaveSelect OFF forbid, MM Amount caps on risk dests  
- Mutator UI: **MODE** + **Pitch**; Amount/Random hidden; constants per MODE  
- Toggle respect (write enabled only; read OK)  
- Pitch **Preserve** global  
- WaveSelect **cardinality** (preserve 1 vs 2 active DCOs) + Noise policy  
- **VCA2←ENV2** high / LOCK Kindred–Warp unless MM→VCA2 Level relay; Wild ultra-light + floor; no inventing MM relay from scratch in v1  
- **Effective-role escalation** for ENV Amp when MM Source drives risk dest (ENV1/ENV3; LFO only if **sole** VCA opener)  
- **FM Amount nudge** in Warp/Wild when FM mods need audibility  
- **Kindred MM = Amounts only** on live buses  

### Should (v1 aspirational / early follow-on)

- Footer / discreet indicator when a guard corrected the patch  
- Golden tests “dead → playable”  
- Keyboard Mode / Unison LOCK through Warp, free in Wild  
- Measured Noise inject when seed had no Noise  

### Won’t (this effort)

- Per-param padlocks in Patch Edit  
- Duplicate Behavior/MODE in Settings as primary surface  
- Changing base Amount×Random **curve math** (only profiles / caps)  
- create-story inside this brainstorm session  

---

## 6. Parameter → policy (summary)

Legend: **LOCK** / **BORNE** (MODE neighborhood + floors) / **CHOIX** / **POST** (post-apply) / **LIBRE\*** (MODE-BORNE default).

### DCO 1 / DCO 2

| Param | Policy |
|-------|--------|
| WaveSelect | **CHOIX + POST:** never dual OFF; preserve active-DCO cardinality; Noise keep if seed had it; optional moderate Noise add if slot free |
| FREQUENCY (pitch) | Gated by **Pitch** combo (Preserve / Consonant jumps / Free) |
| Sync | MODE-scaled discrete CHOIX (more freedom toward Wild) |
| Wave Shape, Pulse Width, Detune (DCO2) | MODE-BORNE near seed |
| Freq←LFO, PW←LFO, Levers, Keybd/Porta, KeyClick | LIBRE\* |

### VCF / VCA

| Param | Policy |
|-------|--------|
| VCA1 Volume | LOCK Kindred–Warp; Wild ultra-light BORNE + floor |
| VCA2←ENV2 | LOCK / very high floor Kindred–Warp unless MM→VCA2; Wild ultra-light + floor |
| ENV2 Amplitude | Hard floor all MODEs; MODE-BORNE above floor; couple Amp←Vel |
| VCA1←Vel, ENV2 Amp←Vel | MODE-BORNE; coupled with bases; careful with strong negatives |
| Frequency + Resonance | MODE-BORNE; **POST** if both too low bump one/both; low+high res OK |
| Freq←ENV1 / Pressure | Constrain strong negatives when cutoff already low |
| Mix | MODE-BORNE; **POST** if extreme toward OFF DCO |
| Levers, Keybd/Porta | LIBRE\* |

### FM / TRACK

| Param | Policy |
|-------|--------|
| All | MODE-BORNE; no silence floor; **Warp/Wild:** nudge FM Amount when mods present so FM is audible |

### Ramp / Portamento

| Param | Policy |
|-------|--------|
| Rates / porta mod | MODE-BORNE |
| Discrete modes | Kindred near seed |
| Keyboard Mode / Unison | Sacred: LOCK through Warp; free Wild (**Should** if deferred) |

### ENV 1 / 2 / 3

| Envelope | Default role | Amp policy |
|----------|--------------|------------|
| ENV1 | Filter | Soft/conditional floor; **escalate** if MM Source=ENV1 (or Track) → risk dest |
| ENV2 | Amplitude (hardwired→VCA2) | Hard floor + MODE-BORNE; Kindred soft limits on Delay/Attack & sustain0+short release |
| ENV3 | FM | No default silence floor; Warp/Wild may raise Amp for audible FM; MM escalation if drives risk dest |

Same ADSR **structure**; different Mutator treatment by **default role** + **effective role**.

### LFO 1 / 2

| Policy | Detail |
|--------|--------|
| Default | MODE-BORNE; LFO→VCA = tremolo, not permanent silence |
| Hard Amp min | **Only** if LFO is **sole** VCA opener (MM relay + VCA2←ENV2 dead) |
| Kindred soft | Avoid very slow + deep toward volume (feel of silence) |

### Matrix Mod

| MODE | Mutation |
|------|----------|
| Kindred | Amounts on active buses almost only |
| Drift | + optional one safe timbre bus |
| Warp / Wild | Increasing Source/Dest freedom; bias ENV/LFO/Track/Ramp → timbre/motion |
| All | Cap unfavorable Amounts to VCA1/VCA2/ENV2 Amp/Mix/VCF Freq; Pitch dests follow Pitch combo |

---

## 7. Constraint map (three axes)

| Policy | Hardware | Hidden A×R / algo | UX |
|--------|----------|-------------------|-----|
| VCA cascade floors | VCA1→VCA2 series; VCA2 base 0 | Floors above MODE profiles | F/A toggle must be on to write |
| VCA2←ENV2 exception | MM→VCA2 Level can replace ENV2 drive | Read MM even if MM toggle off | Write VCA2←ENV2 only if F/A on |
| Filter couple | Res can rescue low cutoff | POST after jitter | F/A on to write |
| Dual OFF / Mix | Source path | POST / CHOIX | D1/D2 on as needed |
| MODE profiles | — | Constants `Amount/Random` per MODE | MODE combo on Mutator |
| Pitch Preserve | Semitone FREQUENCY | Skip pitch mutations | Global over D1/D2/MM |
| No cheat toggles | — | Write mask by toggles | Honest UI |

**Known limit:** if silence already lives in a **disabled** module’s values, Mutator cannot repair without cheating — seed should be playable, or user enables that toggle.

---

## 8. Rejected / deferred

| Item | Disposition |
|------|-------------|
| PW as priority killer | Rejected (audible pinch) |
| Equal danger VCA1 / ENV2 Amp / Vel | Rejected — Vel is soft-kill |
| Padlocks on every Patch Edit control | Rejected |
| Settings as primary MODE home | Deferred — Mutator panel primary |
| Similar (Ableton) as MODE name | Rejected vs Kindred |
| Fixed as Pitch off | Rejected |
| LFO Amp hard floor whenever MM→VCA | Rejected — too blunt; sole-opener only |
| create-story this session | Won’t |
| Change core Amount×Random distribution math | Won’t (profiles only) |

---

## 9. Recommended next step

1. **Spec addendum** (or spike) to `mutation-algorithm-spec.md`: policy table shape, post-apply hooks, MODE→(Amount,Random) constant table (placeholders), Pitch discrete jump table, MM MODE ladder.  
2. Then **create-story** / build for implementation + golden “dead → playable” vectors.  
3. Optional: Figma MODE + Pitch Mutator layout (Guillaume).  
4. Optional hardware confirmations still open in `interactions-contre-intuitives.md` §6 (ENV1/LFO/FM/FREQ headroom).

**Do not** start create-story until Guillaume asks.

---

## 10. Techniques run

1. Failure Mode Analysis  
2. First Principles  
3. Morphological Analysis (D1→D2→F/A→F/T→R/P→E1→E2→E3→L1→L2→MM)  
4. Constraint Mapping  
5. Converge: MoSCoW (revised Should→Must, Could→Should)
