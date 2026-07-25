---
title: 'Standalone audio input passthrough'
type: 'bugfix'
created: '2026-07-25'
status: 'done'
baseline_commit: 'ac0e8ac596bea5f0dbab5672e83145f0cb4b1fa6'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/deferred-work.md'
  - '{project-root}/_bmad-output/implementation-artifacts/r-1-remove-plugin-audio-input-bus.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** In the standalone app, after enabling physical input channels in Audio Settings and choosing a source in Audio From, the synth signal can reach the interface hardware while Matrix-Control stays silent: peak meter dead, no passthrough. Demo repro: Focusrite Scarlett 6i6, inputs 1/2 enabled, Audio From mono 1, Matrix-1000.

**Approach:** Make Audio From source ids address the **compacted active-input bus** that JUCE feeds the processor (not raw hardware bit positions), guarantee JUCE standalone input mute is cleared whenever a real Audio From source is applied, and keep the Audio From combo visually in sync with the applied source when the catalog appears after channels are enabled.

## Boundaries & Constraints

**Always:**
- Standalone-only fix (AD-11 / Story R-1): hosted VST3/AU keep no input bus; do not reintroduce a plugin input bus.
- Selecting a non-empty Audio From source must unmute JUCE `StandalonePluginHolder` input monitoring (`muteInput` / Settings “Mute audio input”).
- Catalog display names may still show hardware channel numbers (1-based); stored `audioFromSourceId` values must index the compacted active channel list used by `AudioPassthroughProcessor`.
- Peak meter and stereo output passthrough must reflect post-gain input when a valid in-range source is selected.

**Ask First:**
- Changing persisted `audioFromSourceId` string format (e.g. migrating old `mono:N` hardware indices) if a migration strategy is required for existing user sessions.
- Broadening scope to non-audio deferred items from Story 2.7 (weak symbols / MSVC, audio-thread APVTS reads).

**Never:**
- Change hosted-plugin bus layout or show Audio From / Input Gain / peak in plugin mode.
- Rely on the user manually unchecking Settings “Mute audio input” as the only fix.
- Rewrite AudioDeviceManager / replace JUCE standalone holder plumbing.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Happy path (demo) | Active inputs 1/2; Audio From mono channel 1; signal present | Peak moves; dry synth heard on standalone outputs | N/A |
| Non-zero pair | Only hardware 3/4 active; pick mono of first active | Uses compacted index 0 into 2-ch bus; peak/audio live | N/A |
| Out-of-range index | Mono index ≥ active input count | Clear outputs; peak 0 (safe silence) | No crash |
| Catalog empty → populated | Enable inputs after launch; prior empty/sentinel | Combo shows applied source; mute cleared; first valid source or restored id applied | Stale id kept in APVTS without forcing wrong kind |
| Session restore | Persisted valid source + active channels | prepare/sync applies routing **and** unmute | Missing channels: keep preference, no wrong mono/stereo flip |
| Hosted plugin | Any | No input bus; silent output; audio header controls hidden | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/Audio/AudioInputSourceCatalog.cpp` -- builds `mono:`/`stereo:` ids from active bitmask; today uses raw hardware indices
- `Source/Core/Audio/AudioPassthroughProcessor.cpp` -- maps mono index into processor input bus; OOR → silence + peak 0
- `Source/Core/Audio/StandaloneAudioInputRouterStandalone.cpp` -- active-device catalog; `enableInputMonitoring()` clears mute
- `Source/Core/PluginProcessor.cpp` -- `setAudioFromSourceId` / `syncAudioPassthroughFromSourceId` / `processBlock` bus wiring; unmute only on set path today
- `Source/GUI/PluginEditor.cpp` -- `refreshAudioFromCombo`, device-change listener, unmute on change
- `Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp` -- `populateAudioFromCombo` / sentinel / selection sync
- `Tests/Unit/AudioPassthroughProcessorTests.cpp` -- extend OOR + mono index coverage
- New or extended catalog unit test -- active bitmask → compacted source ids

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Audio/AudioInputSourceCatalog.cpp` (+ header if needed) -- emit/resolve compacted active-channel indices for mono/stereo source ids; fix stereo active-channel pair stride (step by 2, matching device scan) -- raw hardware bit position must not be used as bus index
- [x] `Source/Core/PluginProcessor.cpp` -- ensure every path that applies a non-empty Audio From source (including `syncAudioPassthroughFromSourceId` / runtime restore) unmutes standalone input monitoring when wrapper is standalone
- [x] `Source/GUI/PluginEditor.cpp` + `HeaderPanel.cpp` -- when catalog transitions to non-empty, combo selection matches the source id actually applied (no blank/sentinel with live APVTS id; no silent auto-apply without visible selection)
- [x] `Tests/Unit/AudioPassthroughProcessorTests.cpp` -- add OOR mono index → silence/peak 0; keep existing in-range mono/stereo cases green
- [x] Catalog unit test (new or existing Tests/Unit) -- bitmask with non-zero-based active pair yields `mono:0`/`mono:1`/`stereo:0` compacted ids and correct display labels
- [ ] Manual smoke (standalone + multi-in interface if available) -- Settings enable 1/2, Audio From mono 1, confirm peak + passthrough; optionally try 3/4 pair
  - Pending Guillaume hardware UAT (Scarlett / Matrix-1000); build + unit tests green on macos-debug-arm64.

**Acceptance Criteria:**
- Given standalone with at least one active input channel and a selected Audio From source that maps into that set, when audio arrives on that channel, then the peak indicator moves and passthrough is audible on the standalone outputs.
- Given only a non-zero-based active pair (e.g. 3/4), when the user selects the corresponding mono/stereo Audio From entry, then routing uses compacted bus indices and audio/peak work.
- Given a non-empty Audio From source is applied (UI change, device-change refresh, or session restore), when the standalone holder is present, then JUCE input mute is false so the processor receives real input samples.
- Given the hosted plugin build, when loaded in a DAW, then behavior remains AD-11 (no input bus; audio header controls hidden).

## Design Notes

JUCE `StandalonePluginHolder` defaults `muteInput` to true (feedback protection) and zeroes input callbacks until unmuted. Matrix-Control already calls `enableInputMonitoring()` from `setAudioFromSourceId` and on some device-change paths; sync/restore must not leave mute stuck on.

Compacted indexing example: hardware bits 2+3 active only → bus channels are [0]=HW2, [1]=HW3; catalog must expose `mono:0` / `mono:1` / `stereo:0` (display may still say “3” / “4”), never `mono:2` as a bus index.

Deferred Story 2.7 notes in `deferred-work.md` (unstable numeric ids; combo not refreshed on device change) are in-scope hardening for this bugfix only.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- build succeeds
- `ctest --preset macos-debug-arm64 -R 'AudioPassthrough|AudioInput'` (or project’s usual unit-test target filter) -- new/updated audio unit tests pass

**Manual checks (if no CLI):**
- Standalone: Audio Settings → enable inputs → Audio From mono 1 with live synth → peak alive + hear passthrough; confirm Settings mute ends up off after selecting a real source.
- Optional: enable inputs 3/4 only → matching Audio From entries still pass audio.
- Hosted plugin smoke: no Audio From packet; no input bus regression.

## Suggested Review Order

**Unmute / remute (root cause for silent demo)**

- Clear input mute when a real Audio From source is applied; remute on empty.
  [`PluginProcessor.cpp:896`](../../Source/Core/PluginProcessor.cpp#L896)

- Message-thread safe toggle of JUCE standalone mute flag.
  [`StandaloneAudioInputRouterStandalone.cpp:70`](../../Source/Core/Audio/StandaloneAudioInputRouterStandalone.cpp#L70)

**Compacted channel catalog**

- Map active hardware channels to bus indices while keeping hardware labels.
  [`AudioInputSourceCatalog.cpp:67`](../../Source/Core/Audio/AudioInputSourceCatalog.cpp#L67)

- Emit stereo only for adjacent hardware pairs (stride 2).
  [`AudioInputSourceCatalog.cpp:85`](../../Source/Core/Audio/AudioInputSourceCatalog.cpp#L85)

**Audio From combo sync**

- Sentinel id 1 vs channel items from id 2 (no collision with first source).
  [`HeaderPanel.cpp:489`](../../Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp#L489)

- When catalog fills, select the same id that is applied to the processor.
  [`PluginEditor.cpp:996`](../../Source/GUI/PluginEditor.cpp#L996)

**Tests**

- Out-of-range mono index stays silent.
  [`AudioPassthroughProcessorTests.cpp:294`](../../Tests/Unit/AudioPassthroughProcessorTests.cpp#L294)

- Compacted ids for pair 3/4 and non-adjacent active channels.
  [`AudioInputSourceCatalogTests.cpp:18`](../../Tests/Unit/AudioInputSourceCatalogTests.cpp#L18)
