---
organization: Ten Square Software
project: Matrix-Control
title: Story V1.2 — Device Connection and Ports
author: BMad Agent
status: done
baseline_commit: 76e4a4d
sources:
  - implementation-artifacts/deferred-work.md
  - implementation-artifacts/spec-8-4-virtual-instrument-registration-and-bus-layout.md
  - implementation-artifacts/8-3-ui-lock-without-synth.md
  - implementation-artifacts/8-2-device-inquiry-and-footer-identity.md
  - implementation-artifacts/2-9b-header-routing-controls-uat-slice.md
  - implementation-artifacts/v1-1-unsaved-navigation-consistency.md
  - planning-artifacts/epics.md
  - planning-artifacts/prds/prd-matrix-control-2026-05-25/prd.md
  - project-context.md
created: 2026-07-25
updated: 2026-07-25
---

# Story V1.2: Device Connection and Ports

Status: done

<!-- Ultimate context engine analysis completed - comprehensive developer guide created -->

## Story

As a sound designer connecting Matrix hardware (or Matrix-Simulator on the road),
I want clear feedback when the device type is unsupported, when ports fail to open, when MIDI From and Keyboard From collide, and when slow MIDI dump timing fails,
so that the plugin never looks connected while silently misbehaving before V1 ships (FR-1 / FR-2 / FR-39 hardening).

## Brownfield Reality (Read First)

**Epic 8 and Epic 2 port wiring are done.** Device Inquiry, FR-2 lock on `deviceDetected=false`, R-3 port-open footer feedback, header MIDI From / MIDI To / Keyboard From, and async dump exist. This story is **V1 deferred-work hardening**, not a rewrite of inquiry or the dual-role MIDI stack.

| Area | Status today |
|------|----------------|
| Device Inquiry + footer identity (FR-1 / FR-53) | **Done** — preserve |
| FR-2 UI lock when `deviceDetected=false` | **Done** via `CompareLockBinder` — preserve / extend |
| MASTER allowlist (Matrix-1000 only) + M-6 MASTER gray (FR-46) | **Done** — preserve |
| Detected + `deviceType=Unknown` → whole-GUI gray + explicit footer | **Missing** (confirmed 8-4 review; deferred) |
| Live inquiry path for “Matrix family, unknown member” | **Blocked today** — `SysExDecoder::validateMatrixFamilyDevice` requires known member bytes; unknown member → inquiry **failure** (`deviceDetected=false`) + generic lock copy, not the confirmed Unknown product rule |
| Dump settle / queue-idle | **Rigid** — `50` ms settle / `500` ms idle in `PatchManagerActionHandler` + `MidiManager` defaults / inquiry literals |
| Dump fail after Continue (coords / dirty / footer) | **Done in v1-1** — preserve; this story hardens **timing**, not navigation rollback |
| R-3 port open failure → footer | **Done** — preserve |
| Combo revert on interactive open fail | **Partial** — `PluginEditor` onChange reverts; **restore/sync** can leave combo/APVTS showing a port that is not open |
| MIDI FROM + KEYBOARD FROM same device id | **Unguarded** — two `juce::MidiInput` opens; OS-dependent (often OK on macOS, often fails on WinMM) |
| Audible Mutate / INIT on real hardware | **Out of scope** — checklist when hardware returns |

**Validation constraint (travel):** no Matrix hardware UAT and no audible synth requirement. Prove with **unit tests + Matrix-Simulator** (Device Inquiry / types Matrix-1000 and Matrix-6 provisional / UI lock). Inject APVTS or inquiry fixtures for Unknown-member cases if the simulator has no Unknown profile yet. Full patch-dump on simulator is **not** required — use injectable MidiManager settle/idle seams for timeout behaviour.

## Acceptance Criteria

1. **Given** Device Inquiry yields an Oberheim Matrix-family Device ID whose member bytes are neither Matrix-1000 nor Matrix-6/6R  
   **When** device status is applied  
   **Then** the product treats this as **connected but unsupported**: `deviceType=Unknown` is visible to UI/gates, and the **entire editable GUI** is grayed / non-interactive in the same lock language as FR-2 (`CompareLockBinder` / section lock — PATCH EDIT, MASTER EDIT, MATRIX MODULATION, PATCH MANAGER, module I/C/P)  
   **And** Header routing (MIDI From / MIDI To / Keyboard From) and shell prefs remain usable so the user can change ports  
   **And** footer **left zone** shows an **explicit English** unsupported-device message (new `PluginDisplayNames` constant) that is **distinct** from:
   - `kDeviceLockGuidance` (no synth / check cables)
   - Matrix-6 MASTER-only copy
   - `kUnsupportedDeviceFooterMessage` (MASTER-scoped wording)  
   **And** editor SysEx and Program Change stay blocked while this Unknown lock is active (Device Inquiry still allowed)  
   **And** when a supported type is later detected, Unknown lock and its footer clear and normal FR-2 / FR-46 rules apply  
   **And** no hardware audible UAT — unit tests with injected status and/or Matrix-Simulator smoke for supported types suffice.

2. **Given** async single-patch dump via `MidiManager::requestSinglePatchAsync` (Internal / bank / NumberBox / Mutator-related navigation)  
   **When** outbound queue-idle wait and post-bank/PC settle run on a slow MIDI interface  
   **Then** settle and queue-idle are **not** left as unexplained magic `50` / `500` only  
   **And** they are either:
   - **(preferred)** derived from / floored against the active `SysExDelayProfile` (or a documented device-aware delay profile hook), **or**
   - raised to safer documented defaults for slow interfaces  
   **And** inquiry’s matching settle/idle literals in `MidiManager` stay consistent with the dump path (single named SSOT — no silent drift)  
   **And** a slow / timed-out dump must **not** leave a silent stale buffer while UI coordinates advanced — align with v1-1 dump-failure rollback + clear footer (empty callback / idle timeout already fails dump; ensure user-visible failure remains)  
   **And** unit tests cover injectable settle/idle (or documented seams) without live hardware.

3. **Given** Standalone mode with MIDI From and Keyboard From both selectable  
   **When** the user would assign the **same** MIDI input device identifier to both  
   **Then** the product prevents an ambiguous double-`MidiInput` open (UI guard and/or Core rejection before open)  
   **And** a clear English footer explains that MIDI From and Keyboard From must use **distinct** devices  
   **And** plugin mode is unchanged (Keyboard From remains grayed **HOST**).

4. **Given** the user selects MIDI From, MIDI To, or (standalone) Keyboard From in the header combo **or** ports are restored/synced from persisted state  
   **When** port open fails (`kNotFound` / `kOpenRejected`)  
   **Then** the ComboBox must **not** display a port as “chosen” if that port is not actually open  
   **And** combo selection, APVTS port id, and open-port reality stay coherent (revert to last successful open, or none sentinel)  
   **And** existing R-3 footer / MidiLogger feedback for MIDI From / MIDI To open failure is preserved  
   **And** Keyboard From open failure gets at least clear combo resync (footer parity with R-3 is allowed if cheap).

5. **And** Core ↛ GUI remains: no new `AlertWindow` in Core for these cases; footer + grayed controls only (D-038).

6. **And** `Matrix-Control_Tests` pass; macOS Debug build clean; English-only source; no French in code or user-visible strings.

## Locked Product Decisions (do not re-litigate)

| Topic | Decision | Why |
|-------|----------|-----|
| Unknown Matrix-family member | **Connected-but-unsupported**: surface `deviceType=Unknown` with **whole-GUI lock** + **distinct footer**; do **not** pretend the device is a supported Matrix | Confirmed 8-4 review (2026-07-19); MASTER allowlist alone is insufficient |
| Lock predicate | Extend FR-2 lock language so Unknown is locked even if inquiry marks the device “present” — e.g. lock when `!deviceDetected` **or** `deviceType==Unknown` (or shared `isSupportedMatrixDevice` helper used by binder + outbound gate) | Today FR-2 unlocks solely on `deviceDetected`; unlocking Unknown would send SysEx to an unsupported unit |
| Inquiry decode path | Adjust decode / inquiry success so Matrix-family + unknown member can reach the Unknown product state (today `validateMatrixFamilyDevice` forces failure → `deviceDetected=false` + cable guidance) | Without this, AC1 cannot be reached from live inquiry |
| Non-Matrix manufacturer / family | Keep **rejected** / undetected behaviour (not the Unknown-supported-family case) | Different product meaning: “nothing we recognize” vs “Matrix family we don’t support yet” |
| MASTER gate | Keep Matrix-1000 allowlist | Shipped in 8-4 |
| Dump timing | Prefer **profile-aware floors** + named SSOT shared by dump + inquiry; clear failure over silent stale buffer | Slow interfaces are the deferred risk |
| Same-device MIDI+Keyboard | **Refuse** the conflicting selection with footer (do not silently keep both opens) | Predictable > OS-dependent |
| Combo vs open | Combo always mirrors **successful** open (interactive + restore/sync) | User must not believe a dead port is active |
| Hardware audible UAT | **Not required** | Travel / Matrix-Simulator + unit fakes |
| Mutate/INIT audible on hardware | **Out of scope** — checklist when hardware returns | Explicit user constraint |
| `updateDeviceStatus` property order race | **Out of scope** unless a one-line fix falls out of Unknown work | Separate 8-4 defer |
| `setStateInformation` full host reload resync | **Out of scope** beyond port open/combo coherence in this story’s AC4 | 2-9b sibling defer |
| Mid-window dump memcmp false-abort | **Out of scope** | Accepted v1-1 residual |

## Tasks / Subtasks

- [x] **T1 — Unknown / unsupported device lock** (AC: #1, #5, #6)
  - [x] Introduce a single supported-device predicate (e.g. `MatrixDeviceTypes` / `DeviceTypeRegistry` / `EditorOutboundGate` helper) used by UI lock **and** editor outbound
  - [x] Change inquiry / decode path so Matrix-family + unknown member reaches `deviceType=Unknown` with the product lock (not only cable-guidance undetected)
  - [x] Extend `CompareLockBinder` (or composed lock) to gray the same FR-2 targets when Unknown
  - [x] Add distinct English footer constant under `PluginDisplayNames::FooterPanel`; wire set/clear with exact-string clear pattern (do not fight Compare / device-lock footers)
  - [x] Keep Header ports usable; preserve MASTER allowlist and M-6 MASTER gray for supported M-6
  - [x] Unit tests: injected Unknown → locked UI predicate / outbound blocked / footer text; supported type unlocks; non-Matrix rejection still undetected
  - [x] Optional Matrix-Simulator smoke: Matrix-1000 + Matrix-6 profiles still unlock; Unknown via fixture if simulator has no Unknown profile

- [x] **T2 — Dump / inquiry settle & idle robustness** (AC: #2, #6)
  - [x] Replace magic `50` / `500` with named SSOT (shared by `PatchManagerActionHandler` dump path and `MidiManager` inquiry settle/idle)
  - [x] Prefer floors from `SysExDelayProfile` (or documented multiplier) with safer minimums; document chosen policy in Completion Notes
  - [x] Confirm idle timeout / empty dump still triggers v1-1 failure path (rollback + footer) — no silent stale success
  - [x] Unit tests: injectable settle/idle timeout forces empty/fail path; existing dump-failure regressions stay green

- [x] **T3 — Same-device MIDI From + Keyboard From** (AC: #3, #5, #6)
  - [x] Guard in `PluginProcessor` / Header wiring (standalone only): reject selecting the same identifier for both roles
  - [x] Clear English footer in `PluginDisplayNames`
  - [x] Unit test: conflict rejected; APVTS/open state unchanged for the role that would conflict; plugin HOST path untouched

- [x] **T4 — Combo / open coherence** (AC: #4, #5, #6)
  - [x] Audit interactive onChange (already reverts) **and** restore / `syncMidiPortsFromStateImpl` / deferred host sync paths
  - [x] On open failure after restore/sync: clear or revert APVTS id **and** resync Header combo to match open reality (none sentinel if nothing open)
  - [x] Preserve R-3 `MidiPortOpenFeedback` for From/To
  - [x] Tests: extend `MidiPortOpenFailurePropagationTests` / routing tests for sync-path failure + combo/APVTS coherence helper if extractable

- [x] **T5 — Verification** (AC: #6)
  - [x] `Matrix-Control_Tests` green
  - [x] Optional Standalone smoke with Matrix-Simulator (inquiry + lock for M-1000 / M-6); no audible Mutate/INIT hardware UAT
  - [x] Mark closed deferred-work bullets when story completes (dev-story / review)

## Dev Notes

### Epic / V1 context

Implementation sprint (Epics 0–11, U, T) is complete. Epic V1 hardens deferred-work before release. Upstream: Epic 8 (inquiry, FR-2 lock, FR-46), Epic 2 / 2.9b (header ports), R-3 (open failure footer), Mutator async dump, v1-1 dump-failure navigation.

[Source: `deferred-work.md` — 8-4 Unknown GUI; mutator dump timeouts; 2-9b combo mismatch + same-device]

### Current code to extend (UPDATE — do not fork FR-2 / MidiManager)

**Device type / inquiry**

| Piece | Role today |
|-------|------------|
| `DeviceTypeRegistry::fromMemberBytes` | Known members → M-1000 / M-6; else `kUnknown` |
| `SysExDecoder::validateMatrixFamilyDevice` | Requires known member → `isValid`; **blocks** live Unknown success |
| `MidiManager::finishAsyncDeviceInquirySuccess` | `updateDeviceStatus(true, …, deviceType)` |
| `MidiManager::finishAsyncDeviceInquiryFailure` | `updateDeviceStatus(false)` + `updateErrorState` (often **not** `uiMessageText`) |
| `EditorOutboundGate` / `CompareLockBinder` | Lock & outbound on `deviceDetected` only |
| `MasterEditGate` | Detected **and** Matrix-1000 |
| `DeviceMemoryLimits::resolve(kUnknown)` | Falls back to Matrix-1000 limits — **do not** rely on this while Unknown UI is unlocked |

**Dump timing**

| Constant | Value | Location |
|----------|-------|----------|
| `kDeviceSettleMs` | 50 | `PatchManagerActionHandler.cpp` anonymous namespace |
| `kOutboundIdleTimeoutMs` | 500 | same |
| `requestSinglePatchAsync` defaults | 50 / 500 | `MidiManager.h` |
| Inquiry settle/idle | literal 50 / 500 | `MidiManager.cpp` `performDeviceInquiry` |
| Reply timeout | `SysExConstants::kDefaultTimeoutMs` (2000) | keep unless profiling says otherwise |

**Ports / combos**

| Path | Behaviour |
|------|-----------|
| `PluginEditor` MIDI From/To/Keyboard onChange | Revert combo on failed `set*Port` |
| `PluginProcessor::setMidi*Port` | Success-only APVTS write + inquiry refresh |
| `syncMidiPortsFromStateImpl` | Can keep APVTS id when device listed but open rejected |
| `MidiPortOpenFeedback` | R-3 footer for From/To |
| `KeyboardFromMidiInput` | Separate `juce::MidiInput`; no same-id guard |

**Matrix-Simulator** (sibling repo `/Volumes/Guillaume/Dev/Projects/JUCE/Matrix-Simulator`): profiles Matrix-1000 + Matrix-6 provisional; inquiry/lock smoke only. Do **not** reintroduce into Matrix-Control `Tools/`. Unknown-member may need unit fixture if simulator is not extended (optional; not required to edit simulator for this story).

### What must be preserved

1. FR-2 lock language (`setInterceptsMouseClicks(false,false)` + alpha) — see `CompareLockBinder` hit-test comment.
2. Header ports usable while body locked.
3. Device Inquiry still sendable while locked.
4. Instrument path notes/CC/PB not gated by FR-2.
5. R-3 open-failure footer for MIDI From/To.
6. v1-1 dump failure rollback / no silent overwrite / no `onPatchLoaded` on fail.
7. Plugin Keyboard From = HOST, disabled.
8. Core ↛ GUI; English-only strings.
9. MASTER Matrix-1000 allowlist; M-6 BANK UTILITY / memory limits behaviour.

### Anti-patterns (will fail review)

1. Setting `deviceDetected=true` for Unknown **without** extending UI lock + outbound gate (would unlock editing).
2. Reusing Matrix-6 MASTER footer or cable-guidance copy for whole-GUI Unknown.
3. Leaving dump settle `50`/`500` duplicated as magic literals after “fixing” only one call site.
4. Treating idle timeout as success or applying a partial/stale dump.
5. Silently allowing same-id MIDI From + Keyboard From.
6. Showing a combo selection for a port that failed to open (especially restore/sync).
7. Requiring audible hardware Mutate/INIT as an AC.
8. Rewriting Device Inquiry encode/registry from scratch or moving Matrix-Simulator into this repo.
9. French in source or user-visible strings.
10. New Core `AlertWindow` for port/device guidance.

### Previous story intelligence

**v1-1:** Dump failure after Continue rolls back coords; generation tokens; footer on fail. Keep that behaviour; this story only tunes settle/idle and ensures timeout still fails visibly.

**8.3:** Prefer extending `CompareLockBinder` composition over a second binder that fights `setInterceptsMouseClicks` / footer ownership. Exact-string footer clear pattern is mandatory.

**8.4 review:** Unknown whole-GUI rule confirmed; MASTER allowlist kept; MASTER Unknown footer already patched (scoped — not AC1).

**2.9b / R-3:** Interactive revert exists; sync-path coherence and same-device guard do not. R-3 footer is shipped — do not remove while fixing combo desync.

### Git intelligence

- `76e4a4d` — Harden unsaved navigation consistency for V1.1 dump and Cancel paths (baseline)
- `9ff5531` — Mark Epic U done after U-10 release gate
- Recent focus: V1 deferred-work after full implementation sprint

### Latest tech notes (JUCE 8.0.12)

- `juce::MidiInput::openDevice` may return null when exclusive (typical WinMM) even if the device appears in the list — treat as open failure + combo resync.
- Multiple clients on one input can work on CoreMIDI; product still forbids MIDI From + Keyboard From on the same id for predictable UX.
- Prefer named constants / profile floors over ad-hoc `Timer::callAfterDelay(50, …)` literals.
- Footer messaging via existing APVTS `uiMessageText` / severity — no third footer API.

### Project structure notes

| Action | Path |
|--------|------|
| UPDATE | `Source/Core/MIDI/SysEx/SysExDecoder.{h,cpp}` (family vs member validity for Unknown path) |
| UPDATE | `Source/Core/MIDI/MidiManager.{h,cpp}` (inquiry success/Unknown; settle/idle SSOT) |
| UPDATE | `Source/Core/MIDI/EditorOutboundGate.h` (+ tests) |
| UPDATE | `Source/GUI/Helpers/CompareLockBinder.{h,cpp}` |
| UPDATE | `Source/Shared/Definitions/PluginDisplayNames.h` (+ optionally `MatrixDeviceTypes.h` helper) |
| UPDATE | `Source/Core/Actions/PatchManagerActionHandler.cpp` (dump settle/idle) |
| UPDATE | `Source/Core/PluginProcessor.{h,cpp}` (same-device guard; sync-path open/APVTS coherence) |
| UPDATE | `Source/GUI/PluginEditor.cpp` / `HeaderPanel` if combo resync helpers needed |
| UPDATE | `Source/Core/MIDI/MidiPortOpenFeedback.*` only if Keyboard From footer parity |
| UPDATE | Tests: `DeviceTypeRegistryTests`, `EditorOutboundGateTests`, `MidiPortOpenFailurePropagationTests`, `MidiManagerTests`, `PatchManagerActionHandlerTests`, new conflict test if needed |
| AVOID | Rewriting FR-51 / DirtyPatchTracker; bus layout / VI registration; Matrix-Simulator repo unless Guillaume asks |

### Testing requirements

- Prefer Core/unit tests with APVTS injection and MidiManager fakes over GUI automation.
- Required coverage matrix:
  - Unknown lock predicate + outbound blocked + distinct footer
  - Supported M-1000 / M-6 unlock regression
  - Non-Matrix inquiry still undetected
  - Dump idle/settle injectable timeout → fail path (no silent success)
  - Same-device Keyboard From conflict rejected (standalone)
  - Sync/restore open failure → APVTS + logical selection coherent (combo helper if testable)
  - Existing R-3 / FR-2 / v1-1 dump-fail regressions green
- Matrix-Simulator: optional inquiry/lock smoke for M-1000 and M-6; **not** audible Mutate/INIT proof.

### References

- [Source: `deferred-work.md` — code review of spec-8-4 Unknown whole-GUI]
- [Source: `deferred-work.md` — mutator dump settle/idle timeouts]
- [Source: `deferred-work.md` — 2-9b combo/backend mismatch + same-device MIDI+Keyboard]
- [Source: `spec-8-4-virtual-instrument-registration-and-bus-layout.md` — Decision 1 / Review Defer]
- [Source: `8-3-ui-lock-without-synth.md` — FR-2 binder + footer patterns]
- [Source: `2-9b-header-routing-controls-uat-slice.md` — port wiring + deferred guards]
- [Source: `v1-1-unsaved-navigation-consistency.md` — dump failure UX]
- [Source: `epics.md` — Epic 8 FR-1 / FR-2 / FR-53]
- [Source: `prd.md` — FR-1 / FR-2]
- [Source: `project-context.md` — dual-role MIDI, Core↛GUI]

## Dev Agent Record

### Agent Model Used

Composer (Cursor agent)

### Debug Log References

- macOS Debug ARM64: `cmake --build --preset macos-debug-arm64` clean; `Matrix-Control_Tests` exit 0.

### Completion Notes List

- **Unknown lock (AC1):** `MatrixDeviceTypes::isSupportedMatrixDevice` + `EditorOutboundGate` now require detected **and** supported type. `SysExDecoder::validateMatrixFamilyDevice` accepts Oberheim Matrix family without known member bytes so inquiry can set `deviceDetected=true` + `deviceType=Unknown`. `CompareLockBinder` locks FR-2 sections and shows `kUnsupportedMatrixDeviceFooter` (distinct from cable guidance / MASTER-scoped copy). Header ports remain usable; Device Inquiry still bypasses outbound gate.
- **Dump/inquiry timing (AC2):** New `Core::MidiRequestTiming` SSOT — settle = `max(50, profileDelay×5)`, idle = `max(500, profileDelay×50)`. Shared by `PatchManagerActionHandler` dump path and `MidiManager::performDeviceInquiry`. Stock M-1000 (10 ms) keeps historical 50/500 floors; stock M-6 (20 ms) raises to 100/1000. Idle timeout / empty callback paths unchanged (still fail visibly via v1-1).
- **Combo/open coherence (AC4):** Option 2 — standalone / reporting sync align APVTS to open reality via `maybeAlignApvtsPortIdAfterOpenAttempt`; soft intermediate plugin retries keep the desired id. Editor restore selects combos after restore; ValueTree listener resyncs port combos. R-3 open-failure footer preserved.
- **Same-device guard (AC3):** Interactive setters **and** `syncMidiPortsFromStateImpl` refuse identical MIDI From / Keyboard From ids; conflict footer clears on successful distinct set/clear.
- **Review patches (2026-07-25):** Queued editor SysEx re-gated on drain when Unknown lands; Keyboard From open failure leaves APVTS unchanged (no empty-id race); tests cover soft vs reporting sync, conflict footer clear, injectable dump idle timeout.

### File List

- `Source/Shared/Definitions/MatrixDeviceTypes.h`
- `Source/Shared/Definitions/PluginDisplayNames.h`
- `Source/Core/MIDI/EditorOutboundGate.h`
- `Source/Core/MIDI/SysEx/SysExDecoder.cpp`
- `Source/Core/MIDI/Queue/MidiRequestTiming.h` (new)
- `Source/Core/MIDI/MidiPortStateCoherence.h` (new)
- `Source/Core/MIDI/MidiManager.h`
- `Source/Core/MIDI/MidiManager.cpp`
- `Source/Core/Actions/PatchManagerActionHandler.cpp`
- `Source/Core/PluginProcessor.cpp`
- `Source/GUI/Helpers/CompareLockBinder.h`
- `Source/GUI/Helpers/CompareLockBinder.cpp`
- `Source/GUI/Panels/MainComponent/BodyPanel/MasterEditPanel/MasterEditPanel.cpp`
- `Source/GUI/PluginEditor.cpp`
- `Tests/Unit/EditorOutboundGateTests.cpp`
- `Tests/Unit/DeviceTypeRegistryTests.cpp`
- `Tests/Unit/MidiPortOpenFailurePropagationTests.cpp`
- `Tests/Unit/MidiPortStateCoherenceTests.cpp` (new)
- `Tests/Unit/MidiManagerTests.cpp`
- `Tests/Unit/PatchManagerActionHandlerTests.cpp`
- `Tests/Unit/PatchMutatorEngineTests.cpp`
- `CMakeLists.txt`
- `_bmad-output/implementation-artifacts/deferred-work.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/v1-2-device-connection-and-ports.md`

### Review Findings

- [x] [Review][Decision] Port combo vs open reality during restore retries — **Resolved: option 2** (truth at end of attempts only). Intermediate plugin-host soft sync may keep desired id; standalone / final attempt / non-retry paths must align APVTS+combo to open reality.
- [x] [Review][Patch] Apply option 2 port coherence [`PluginProcessor.cpp`] — `setStateInformation` / standalone / last deferred attempt must run `coherentPortIdAfterOpenAttempt`; keep soft intermediate plugin retries (`reportOpenFailures=false`) so desired id survives for reopen.
- [x] [Review][Patch] Same-device MIDI From + Keyboard From guard bypassed on sync [`PluginProcessor.cpp:1171`] — `syncMidiPortsFromStateImpl` opens via `midiManager->setMidiInputPort` and skips `isMidiFromKeyboardFromConflict` (AC3).
- [x] [Review][Patch] Keyboard From open failure clears APVTS before editor restore [`PluginProcessor.cpp:757`] — unlike MIDI From, failure writes empty `keyboardFromPortId`, racing `valueTreePropertyChanged` combo resync vs onChange restore (AC4).
- [x] [Review][Patch] Conflict footer not cleared after conflict resolved [`PluginProcessor.cpp:688-708`] — successful set/clear leaves `kMidiFromKeyboardFromConflictFooter` until something else overwrites it.
- [x] [Review][Patch] Queued editor SysEx not re-checked when Unknown lands [`MidiManager.cpp:1045`] — enqueue gates, but `processOutboundQueue` / `sendQueuedSysEx` still drain already-queued editor SysEx after inquiry sets Unsupported (AC1).
- [x] [Review][Patch] AC2/AC3/AC4 test gaps — timing arithmetic only (no injectable idle → dump fail); conflict helper only (no Processor reject + footer); sync coherence test does not call `syncMidiPortsFromStateImpl` / `reportOpenFailures` gate (AC6 testing matrix).
- [x] [Review][Patch] Stale FR-2 comments in `MidiManager.h` still say outbound allowed on `deviceDetected` alone while code also requires supported type.
- [x] [Review][Patch] `deferred-work.md` marks combo/sync and same-device items fully Resolved (v1-2) despite sync bypass / soft-failure gaps — temper wording after AC4 decision + patches.
- [x] [Review][Defer] `DeviceMemoryLimits::resolve(kUnknown)` still falls back to Matrix-1000 [`DeviceMemoryLimits.cpp:51`] — deferred, pre-existing; story Project Map already warns; lock via CompareLockBinder is the V1.2 product path.
- [x] [Review][Defer] Brief Header combo stale possible under rapid APVTS port-id churn + `callAsync` [`PluginEditor.cpp:969`] — deferred, pre-existing race class; cosmetic.
- [x] [Review][Defer] Interactive MIDI From/To open failure does not immediately refresh Device Inquiry [`PluginProcessor.cpp:700`] — deferred, pre-existing close-before-fail window until onChange restores prior port.
- [x] [Review][Defer] `EditorOutboundGate::maySendEditorSysEx` unused by production send path — deferred, pre-existing; inquiry uses ungated `sendSysExWithDelay`; gate still used for UI/outbound allow predicates.
- [x] [Review][Defer] No MidiManager inquiry-success → Unknown lock/footer fixture — deferred; decoder/registry + live Simulator Unknown Device path cover reachability.

## Change Log

- 2026-07-25: Story created from deferred-work V1 priority lot (device connection & MIDI ports); status ready-for-dev.
- 2026-07-25: Implemented Unknown lock, MidiRequestTiming SSOT, same-device guard, combo/open coherence; tests + macOS Debug build green; status → review.
- 2026-07-25: Code review — 1 decision (option 2), 8 patches applied, 5 deferred; status → done.
