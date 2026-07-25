---
organization: Ten Square Software
project: Matrix-Control
title: Story V1.3 — Release Hygiene and Hardening
author: BMad Agent
status: done
baseline_commit: 266d84988a8f7279a78312b5e522e977f1f471cb
sources:
  - implementation-artifacts/deferred-work.md
  - implementation-artifacts/u-10-release-gate-prod-audit-sign-off-and-d-062-d-063.md
  - implementation-artifacts/8-2-device-inquiry-and-footer-identity.md
  - implementation-artifacts/spec-8-4-virtual-instrument-registration-and-bus-layout.md
  - implementation-artifacts/11-2-cd-release-pipeline.md
  - implementation-artifacts/v1-2-device-connection-and-ports.md
  - project-context.md
  - CONVENTIONS.md
created: 2026-07-25
updated: 2026-07-25
---

# Story V1.3: Release Hygiene and Hardening

Status: done

<!-- Ultimate context engine analysis completed - comprehensive developer guide created -->

## Story

As a maintainer preparing Matrix-Control V1,
I want Release artefacts guarded against TestComponent re-entry, conventions aligned with Debug-only sandbox policy, and MidiManager protected against latent Device Inquiry lifetime bugs and MASTER outbound bypass,
so that production builds and disconnect/teardown paths stay safe without a large refactor (U-10 / 8-2 / 8-4 deferred hardening).

## Brownfield Reality (Read First)

**Epic U (U-10) and Epic 8 are done.** TestComponent is already excluded from Release via CMake `$<CONFIG:Debug>` + `#if JUCE_DEBUG` wiring. Device Inquiry and MASTER Matrix-1000 allowlist ship. This story is **V1 deferred-work hygiene + light hardening**, not a rewrite of CI, inquiry, or MASTER edit UX.

| Area | Status today |
|------|----------------|
| CMake `TEST_SANDBOX_SOURCES` only for `CONFIG:Debug` (D-063) | **Done** (U-10) — preserve |
| `#if JUCE_DEBUG` editor/logo/header TestComponent wiring | **Done** (U-10) — preserve |
| Local `nm`/`strings` Release proof that `TestComponent` is absent | **Done once** (U-10 AC2) — **no CI regression check** |
| `project-context.md` Dev harness = Debug-only | **Done** (U-10 AC3) |
| `CONVENTIONS.md` E2E/GUI row cites `TestComponent` | **Missing Debug-only qualifier** |
| Device Inquiry / async patch `Timer`/`callAsync` capture raw `this` | **Latent UAF** if `MidiManager` destroyed mid-flight (8-2 defer) |
| `asyncRequestToken_` stale-request cancel | **Done** — keeps living-object races safe; **does not** protect destroyed `this` |
| `MidiManager::sendMaster` gate | **Editor outbound only** — not `isMasterEditAllowed` (8-4 defer; upstream call sites already gated) |
| Flush MASTER APVTS → hardware when edit becomes allowed after detection | **Absent** — next parameter edit only |

**Validation constraint (travel / V1 exit):** no Matrix hardware required. Matrix-Simulator optional. Prove CI hygiene with synthetic binaries / Release artefact string scan; prove MidiManager guards with unit tests (injectable APVTS / direct `sendMaster` / liveness-sentinel behaviour). No audible synth UAT.

## Acceptance Criteria

1. **Given** the GitHub Release workflow builds platform Release artefacts (macOS / Windows / Linux matrix in `.github/workflows/release.yml`)  
   **When** a Release binary would contain sandbox evidence (`TestComponent` string and/or equivalent `Source/GUI/Tests` re-entry markers as defined by the check)  
   **Then** the workflow **fails before** codesign/notarize/pack/publish  
   **And** the check is a maintainable script (prefer extending `Scripts/release/prepare_release.py` or a sibling under `Scripts/release/`) with pytest coverage, not a one-off YAML `grep`  
   **And** the check is **static** on built artefacts (byte/`strings`-class scan) — no MIDI device, no Matrix-Simulator, no plugin launch  
   **And** Debug builds may still contain `TestComponent` (check must not run against Debug artefacts as a failure condition).

2. **Given** `CONVENTIONS.md` testing table row for E2E / GUI  
   **When** a reader consults conventions after this story  
   **Then** the row states that `TestComponent` / GUI harness is **Debug-only** and excluded from Release (align with `_bmad-output/project-context.md` § Dev test harness / D-063)  
   **And** no other conventions rewrite is required.

3. **Given** Device Inquiry (and the sibling async single-patch chain that shares the same `Timer`/`callAsync` + `asyncRequestToken_` pattern) has in-flight delayed callbacks  
   **When** `MidiManager` is destroyed before those callbacks run  
   **Then** queued `Timer::callAfterDelay` / `MessageManager::callAsync` bodies must **not** dereference a dangling `MidiManager*` (no use-after-free)  
   **And** the existing `asyncRequestToken_` stale-request semantics remain for living-object cancel/supersede races  
   **And** preferred mechanism: JUCE-idiomatic liveness for non-`Component` types (`juce::WeakReference` + `JUCE_DECLARE_WEAK_REFERENCEABLE(MidiManager)`, or an equivalent shared alive/sentinel flag captured by value) — **not** `Component::SafePointer` (MidiManager is a `juce::Thread`)  
   **And** apply the guard to **both** Device Inquiry and `requestSinglePatchAsync` closures that capture raw `this` (same defect class; do not leave the patch path as the remaining UAF twin).

4. **Given** any future call site invokes `MidiManager::sendMaster`  
   **When** `isMasterEditAllowed` is false (undetected, Unknown, or Matrix-6/6R)  
   **Then** `sendMaster` **fail-closes** (no enqueue) even if `isEditorOutboundAllowed()` is true  
   **And** existing upstream gates in `PluginProcessor` / `ModuleActionHandler` remain (defense-in-depth, not replacement)  
   **And** a unit test proves direct `sendMaster` is a no-op for non–Matrix-1000 / undetected states.

5. **Given** MASTER edit becomes allowed after a successful Matrix-1000 detection  
   **When** the user has not yet edited a MASTER parameter  
   **Then** this story **does not** auto-flush stored MASTER APVTS to hardware  
   **And** Dev Notes / Completion Notes record the product rule explicitly: **next edit only** (no unlock flush) — closes the 8-4 Edge Case defer by documentation, not new SysEx traffic.

6. **And** the following remain **out of scope**: dry-run GitHub Release with live secrets (maintainer checklist on return), Universal macOS binary, Windows Authenticode, compile-time sync of `JUCE_DEBUG` ↔ `$<CONFIG:Debug>` (keep as known residual unless a one-line guard falls out naturally).

7. **And** `Matrix-Control_Tests` pass; release-script pytest suite covers the new hygiene helper; English-only source; no French in code or user-visible strings; Core ↛ GUI preserved.

## Locked Product Decisions (do not re-litigate)

| Topic | Decision | Why |
|-------|----------|-----|
| Release hygiene check location | **`release.yml` build job**, after Release build, **before** codesign/notarize/pack | Fail early; all three OS matrix legs; matches U-10 proof intent |
| Forbidden evidence | At minimum ASCII **`TestComponent`** in Release plugin binaries (VST3/AU/Standalone as discovered by existing release helpers); optional extra `Test*` class names if cheap | Same signal U-10 used with `strings`/`nm` |
| Cross-platform scan | Prefer **pure-Python byte scan** over `nm`/`strings` OS tools | Windows PE has no `nm`; one implementation for macOS/Linux/Windows |
| CONVENTIONS vs project-context | Align CONVENTIONS row to project-context Debug-only wording; project-context already correct | U-10 deferred docs pass |
| Inquiry + async patch lifetime | Guard **both** chains in this story | Identical capture pattern; half-fix leaves the same crash class |
| Liveness primitive | `WeakReference` **or** shared alive sentinel — pick one, document in Dev Agent Record | MidiManager is not a Component |
| `sendMaster` fail-closed | **In scope** (small) | Defense-in-depth for FR-46; deferred from 8-4 |
| MASTER unlock flush | **Out of scope — document “next edit only”** | Product OK without auto-push; avoids surprise SysEx on connect |
| Hardware / Simulator | Not required for AC pass | Travel / V1 exit constraint |
| GitHub Release secrets dry-run | Out of scope | Maintainer checklist; Epic 11.2 defer |
| Universal / Authenticode | Out of scope | Explicit V1 policy |

## Tasks / Subtasks

- [x] **T1 — Release artefact hygiene check** (AC: #1, #6, #7)
  - [x] Add `Scripts/release/` helper (extend `prepare_release.py` or sibling) that discovers Release binaries and fails if forbidden strings present
  - [x] Wire step into `.github/workflows/release.yml` `build` job after Release build / before codesign
  - [x] Add pytest fixtures for positive/negative binary content in `Scripts/release/tests/`
  - [x] Do **not** change CMake Debug gating unless a proven regression requires it — gate already correct

- [x] **T2 — CONVENTIONS.md Debug-only qualifier** (AC: #2)
  - [x] Update E2E/GUI testing table row (~line 876) to state Debug-only / excluded from Release
  - [x] Keep change short; cite D-063 / project-context alignment in Completion Notes

- [x] **T3 — MidiManager async lifetime guard** (AC: #3, #7)
  - [x] Introduce liveness primitive on `MidiManager` (`WeakReference` or shared sentinel)
  - [x] Guard all Device Inquiry `Timer`/`callAsync` closures that capture `this`
  - [x] Guard matching `requestSinglePatchAsync` closures (same pattern)
  - [x] Keep `asyncRequestToken_` checks; liveness is additional, not a replacement
  - [x] Prefer a small unit/harness proof that a destroyed owner’s deferred callback is a no-op (if practical); otherwise document smoke + code-review checklist for closure capture

- [x] **T4 — `sendMaster` fail-closed + document no unlock flush** (AC: #4, #5, #7)
  - [x] Include `MasterEditGate.h`; add private helper mirroring `isEditorOutboundAllowed` style
  - [x] Early-return in `sendMaster` when `!isMasterEditAllowed(...)`
  - [x] Unit test: Matrix-6 / undetected / Unknown → no enqueue when `sendMaster` called directly
  - [x] Document in story Completion Notes / Dev Agent Record: MASTER unlock = **next edit only** (no auto-flush)

- [x] **T5 — Deferred-work bookkeeping** (AC: #5, #6)
  - [x] Mark resolved U-10 CI/docs and 8-2 SafePointer / 8-4 `sendMaster` deferrals in `deferred-work.md` (strike-through + resolution note)
  - [x] Leave unlock-flush as documented product rule (resolved-by-doc) rather than open feature debt unless Guillaume reopens it

## Dev Notes

### Scope cut (intentional)

| In | Out |
|----|-----|
| CI Release hygiene for TestComponent | Dry-run tag + secrets Release |
| CONVENTIONS one-row docs sync | Broad docs TOC / CONTRIBUTING rewrite |
| Inquiry + async-patch lifetime guard | Full MidiManager threading redesign |
| `sendMaster` FR-46 fail-closed | Auto MASTER flush on unlock |
| | Universal macOS / Authenticode |
| | Compiling `JUCE_DEBUG`↔CMake config sync (unless free) |

### Architecture compliance

- **Core ↛ GUI:** MidiManager changes stay in `Source/Core/MIDI/`. No AlertWindow; no GUI includes.
- **FR-46:** MASTER SysEx only for detected Matrix-1000 — centralize fail-closed at `sendMaster` without removing upstream gates.
- **D-063:** Release must not ship TestComponent; automate the proof U-10 did once by hand.
- **JUCE 8.0.12:** Prefer `juce::WeakReference` for non-Component lifetime; do not invent a custom timer cancel framework.

### Current code — what exists / what changes

**Release gate (UPDATE files):**

- `CMakeLists.txt` — `TEST_SANDBOX_SOURCES` + `$<$<CONFIG:Debug>:…>` on `Matrix-Control` (preserve).
- `.github/workflows/release.yml` — Release matrix build + pack; **add hygiene step** after build.
- `.github/workflows/build-and-test.yml` — Debug-only CI; does **not** replace Release hygiene (Debug may contain TestComponent).
- `Scripts/release/prepare_release.py` — existing pack/finalize/discover helpers; extend or sibling.

**MidiManager lifetime (UPDATE):**

- `Source/Core/MIDI/MidiManager.cpp` — Device Inquiry: `armAsyncDeviceInquiryCapture`, `sendArmedDeviceInquiry`, `pollOutboundIdleThenDeviceInquiry` capture raw `this` in `callAsync` / `callAfterDelay`.
- Same file — async patch twin: `armAsyncSinglePatchCapture`, `sendArmedSinglePatchRequest`, `pollOutboundIdleThenRequest`.
- `cancelPendingSysExRequest()` bumps `asyncRequestToken_` and cancels receiver one-shot — **insufficient alone** once the object is destroyed (token load itself needs a live `this`).
- `~MidiManager()` calls cancel + `stopThread` but cannot dequeue already-posted JUCE timer/async lambdas.

**sendMaster (UPDATE):**

```cpp
// Today: editor outbound only
void MidiManager::sendMaster(...) {
    if (! isEditorOutboundAllowed()) return;
    // missing isMasterEditAllowed
}
```

Upstream already gated: `PluginProcessor::isMasterEditOutboundAllowed`, `ModuleActionHandler` MASTER INIT. Add MidiManager defense-in-depth only.

**Preserve:** inquiry success/failure behaviour, footer identity, Unknown lock from v1-2, Debug sandbox UX (Shift+Ctrl+logo).

### File structure requirements

| Path | Action |
|------|--------|
| `Scripts/release/prepare_release.py` and/or `Scripts/release/check_release_hygiene.py` | NEW or UPDATE — hygiene assert |
| `Scripts/release/tests/…` | UPDATE — pytest for forbidden-string helper |
| `.github/workflows/release.yml` | UPDATE — invoke check on Release artefacts |
| `CONVENTIONS.md` | UPDATE — one E2E/GUI row qualifier |
| `Source/Core/MIDI/MidiManager.h` | UPDATE — liveness + optional private master-gate helper |
| `Source/Core/MIDI/MidiManager.cpp` | UPDATE — guarded closures + `sendMaster` fail-closed |
| `Tests/Unit/MidiManagerTests.cpp` | UPDATE — sendMaster gate (+ lifetime if feasible) |
| `_bmad-output/implementation-artifacts/deferred-work.md` | UPDATE — resolve listed deferrals |

### Previous story intelligence (V1.2)

- Travel validation: unit + optional Matrix-Simulator; no hardware audible UAT — **reuse**.
- v1-2 touched inquiry success path for Unknown; do **not** regress Unknown product lock or footer constants.
- Shared async token single-flight (inquiry vs patch) stays; lifetime guard must compose with it.
- Prefer small, surgical Core changes with explicit ACs over broad MidiManager cleanup.

### Git intelligence (recent)

- `266d849` Polish V1 controls/footer (unrelated GUI polish — do not bundle).
- `a79ba58` / `97d2098` V1.2 device connection — preserve Unknown / port coherence.
- `9ff5531` U-10 release gate — this story’s CI/docs follow-ups.

### Testing requirements

| Layer | Expectation |
|-------|-------------|
| Release script pytest | Forbidden string present → fail; absent → pass; synthetic tiny binaries |
| `MidiManagerTests` | `sendMaster` no-op when not Matrix-1000 / not detected |
| Lifetime | Best-effort unit proof of liveness no-op; else explicit review checklist that every deferred lambda captures WeakReference/sentinel **before** `this` use |
| Full plugin GUI | Not required |
| Hardware / Simulator | Optional smoke only |

### Project context reference

- `_bmad-output/project-context.md` — Dev test harness Debug-only; Core ↛ GUI; English source.
- `CONVENTIONS.md` — testing table; Clean Code; no French in source.
- Deferred sources: U-10 CI/docs; 8-2 inquiry SafePointer; 8-4 `sendMaster` / unlock flush.

### References

- [Source: `_bmad-output/implementation-artifacts/deferred-work.md` — U-10 CI/docs; 8-2 SafePointer; 8-4 sendMaster / flush]
- [Source: `_bmad-output/implementation-artifacts/u-10-release-gate-prod-audit-sign-off-and-d-062-d-063.md` — AC2 local nm/strings]
- [Source: `_bmad-output/implementation-artifacts/8-2-device-inquiry-and-footer-identity.md` — Review defer Timer/callAsync raw `this`]
- [Source: `_bmad-output/implementation-artifacts/spec-8-4-virtual-instrument-registration-and-bus-layout.md` — sendMaster / unlock flush]
- [Source: `CMakeLists.txt` — `TEST_SANDBOX_SOURCES` + `$<CONFIG:Debug>`]
- [Source: `.github/workflows/release.yml` — Release build matrix]
- [Source: `Source/Core/MIDI/MidiManager.cpp` — inquiry + async patch + `sendMaster`]
- [Source: `Source/Core/MIDI/MasterEditGate.h` — `isMasterEditAllowed`]
- [Source: `_bmad-output/project-context.md` § Dev test harness]

## Dev Agent Record

### Agent Model Used

Composer (Cursor agent router)

### Debug Log References

- pytest `Scripts/release/tests/`: 28 passed
- `Matrix-Control_Tests` (macos-debug-arm64): exit 0 (includes FR-46 sendMaster + WeakReference tests)

### Completion Notes List

- **T1:** Added sibling `Scripts/release/check_release_hygiene.py` — pure-Python byte scan for ASCII `TestComponent` over discovered VST3/AU/Standalone Release artefacts; wired in `release.yml` after unit tests and before codesign/pack; pytest covers positive/negative binaries + workflow ordering.
- **T2:** `CONVENTIONS.md` E2E/GUI row now states Debug-only harness excluded from Release (D-063 / project-context alignment).
- **T3:** Liveness primitive = `juce::WeakReference` + `JUCE_DECLARE_WEAK_REFERENCEABLE(MidiManager)`. Guarded Device Inquiry and `requestSinglePatchAsync` `Timer`/`callAsync` closures; `asyncRequestToken_` checks retained. Unit proof: WeakReference clears after destroy.
- **T4:** `sendMaster` fail-closes via private `isMasterEditOutboundAllowed()` (`MasterEditGate`); upstream gates unchanged. Unit test: undetected / Matrix-6 / Unknown no-op; Matrix-1000 still enqueues.
- **Product rule (AC5):** MASTER unlock = **next edit only** — no auto-flush of stored MASTER APVTS to hardware when Matrix-1000 becomes allowed after detection. Documented here and in deferred-work as resolved-by-doc.
- **T5:** Struck through / resolved U-10 CI+CONVENTIONS, 8-2 WeakReference, 8-4 sendMaster, and 8-4 unlock-flush (by-doc) entries in `deferred-work.md`. Compile-time `JUCE_DEBUG`↔CMake sync left as known residual (AC6 out of scope).

### File List

- `Scripts/release/check_release_hygiene.py` (new)
- `Scripts/release/tests/test_check_release_hygiene.py` (new)
- `.github/workflows/release.yml`
- `CONVENTIONS.md`
- `Source/Core/MIDI/MidiManager.h`
- `Source/Core/MIDI/MidiManager.cpp`
- `Tests/Unit/MidiManagerTests.cpp`
- `_bmad-output/implementation-artifacts/deferred-work.md`
- `_bmad-output/implementation-artifacts/sprint-status.yaml`
- `_bmad-output/implementation-artifacts/v1-3-release-hygiene-and-hardening.md`

### Change Log

- 2026-07-25: Implemented V1.3 release hygiene CI check, CONVENTIONS Debug-only qualifier, MidiManager WeakReference async guards, sendMaster FR-46 fail-closed, deferred-work resolutions; status → review.
- 2026-07-25: Code review — clear outbound wake callback on destroy; hygiene fails on zero scanned files; Matrix-6R sendMaster coverage; status → done.

### Review Findings

- [x] [Review][Patch] Clear outbound-queue wake callback in ~MidiManager [Source/Core/MIDI/MidiManager.cpp:109] — fixed 2026-07-25: `setWakeConsumerCallback(nullptr)` at start of destructor
- [x] [Review][Patch] Fail hygiene when zero artefact files are scanned [Scripts/release/check_release_hygiene.py:78] — fixed 2026-07-25: SystemExit when `iter_artefact_files` is empty + pytest
- [x] [Review][Patch] Extend FR-46 sendMaster unit test to Matrix-6R [Tests/Unit/MidiManagerTests.cpp:225] — fixed 2026-07-25
- [x] [Review][Defer] Hygiene pytest only synthesizes Linux artefact layouts [Scripts/release/tests/test_check_release_hygiene.py:31] — deferred, pre-existing coverage gap; scan logic exercised; platform discovery shared with prepare_release
- [x] [Review][Defer] processOutboundQueue has no FR-46/MASTER re-check (TOCTOU after sendMaster gate) [Source/Core/MIDI/MidiManager.cpp:1112] — deferred, pre-existing; AC4 scoped fail-closed at sendMaster only

## Story Completion Status

- Status: **done**
- Note: Code review complete — patches applied; two items deferred
- Sprint key: `v1-3-release-hygiene-and-hardening`
