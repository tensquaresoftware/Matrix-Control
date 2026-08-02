---
organization: Ten Square Software
project: Matrix-Control
title: Spec — Device unresponsive presence + SysEx brake
author: BMad Agent
status: done
type: bugfix
baseline_commit: 5daf797
created: 2026-08-02
updated: 2026-08-02
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/implementation-artifacts/spec-bug-midi-01-residual-panic-alert.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Under SysEx edit pressure the Matrix-1000 can stop answering Device Inquiry while still sounding stuck notes. Matrix-Control then clears `deviceDetected` and shows a misleading “No synth detected / check cables” message. PANIC may still leave the host, but a wedged MIDI In cannot be fixed in software.

**Approach:** On inquiry timeout while already detected, keep detection; mark the device MIDI-unresponsive; show an honest overload / power-cycle footer; freeze editor SysEx (and PC) until inquiry succeeds again; strengthen Panic with CC 120 All Sound Off. True disconnect remains port-loss / never-detected failure paths.

## Boundaries & Constraints

**Always:**
- Presence/heartbeat inquiry timeout while `deviceDetected==true` must not clear detection or reset to Unknown.
- Show locked English footer about synth not responding / stop editing / power-cycle; Panic remains usable when MIDI To is set.
- While unresponsive: block editor SysEx and Program Change producers/consumer (Device Inquiry still allowed).
- Clear unresponsive on successful Device ID; clear on port-loss hard clear.
- Initial / never-detected inquiry failure still clears detection as today.
- No GUI includes from Core.

**Ask First:**
- Requiring N consecutive timeouts before unresponsive (default: first timeout while detected).
- Full 0–127 Note Off spray on Panic (default: CC 120 + 123 + 121 only).

**Never:**
- Claiming software can recover a MIDI-deaf Matrix without power-cycle.
- Reusing the cable-check disconnect copy for this case.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Presence timeout, already detected | Inquiry times out | Stay detected; unresponsive=true; overload footer; SysEx/PC frozen | Inquiry retries via presence timer |
| Presence timeout, never detected | Inquiry times out | Clear detection + existing disconnect guidance | As today |
| Inquiry success after unresponsive | Valid Device ID | unresponsive=false; clear overload footer if owned | N/A |
| Panic while unresponsive | Click PANIC, MIDI To set | Enqueue CC120, CC123, CC121 (realtime front) | Hardware may still ignore |

</frozen-after-approval>

## Code Map

- `Source/Core/MIDI/MidiManagerDeviceInquiry.cpp` — soft unresponsive on timeout
- `Source/Core/MIDI/EditorOutboundGate.h` — gate SysEx/PC when unresponsive
- `Source/Core/MIDI/MidiManagerOutbound.cpp` / `MidiManager.cpp` — read unresponsive flag
- `Source/GUI/Helpers/CompareLockBinder.cpp` — lock + footer ownership
- `Source/Shared/Definitions/PluginDisplayNames.h` — overload copy
- `Source/Core/MIDI/MidiManager.cpp` — Panic CC 120
- `Tests/Unit/EditorOutboundGateTests.cpp` (+ inquiry soft-path unit if feasible)

## Tasks & Acceptance

**Execution:**
- [x] Soft presence timeout path
- [x] Unresponsive APVTS flag + SysEx/PC gate
- [x] Footer + section lock messaging
- [x] Panic adds CC 120
- [x] Tests + lint

**Acceptance Criteria:**
- Given detected + inquiry timeout, when presence heartbeat fails, then detection stays and overload guidance is shown (not cable-check copy).
- Given unresponsive, when user edits patch params, then no editor SysEx/PC is enqueued/sent until inquiry succeeds.
- Given Panic click, when MIDI To is open, then CC 120 then 123 then 121 leave via realtime front (channel rules unchanged).

## Spec Change Log

## Design Notes

Property: `deviceMidiUnresponsive` (bool on APVTS state). Footer severity: `warning` or `error` — use `error` for visibility matching user pain. Panic order dequeue: 120 → 123 → 121. Ephemeral: strip on save; force false on session load / MidiManager ctor.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64`
- unit tests for EditorOutboundGate / MidiManager Panic
- `python3 Scripts/quality/lint_touched.py`

## Suggested Review Order

**Presence timeout → honest unresponsive**

- Soft-abort when already detected keeps detection and sets the freeze flag.
  [`MidiManagerDeviceInquiry.cpp:259`](../../Source/Core/MIDI/MidiManagerDeviceInquiry.cpp#L259)

- Timeout branch chooses soft vs hard failure.
  [`MidiManagerDeviceInquiry.cpp:290`](../../Source/Core/MIDI/MidiManagerDeviceInquiry.cpp#L290)

**Editor freeze gate**

- Single gate property + SysEx/PC/section lock predicates.
  [`EditorOutboundGate.h:11`](../../Source/Core/MIDI/EditorOutboundGate.h#L11)

- Consumer drops Program Change if freeze flipped after enqueue.
  [`MidiManagerOutbound.cpp:64`](../../Source/Core/MIDI/MidiManagerOutbound.cpp#L64)

**UI lock + copy**

- Overload footer English copy.
  [`PluginDisplayNames.h:54`](../../Source/Shared/Definitions/PluginDisplayNames.h#L54)

- Section lock + footer ownership while unresponsive.
  [`CompareLockBinder.cpp:79`](../../Source/GUI/Helpers/CompareLockBinder.cpp#L79)

**Panic**

- CC 120 before 123/121 via realtime front.
  [`MidiManager.cpp:199`](../../Source/Core/MIDI/MidiManager.cpp#L199)

**Persistence + tests**

- Do not persist wedged lock across project save/load.
  [`PluginProcessorState.cpp:54`](../../Source/Core/PluginProcessorState.cpp#L54)

- Gate + Panic unit coverage.
  [`EditorOutboundGateTests.cpp:45`](../../Tests/Unit/EditorOutboundGateTests.cpp#L45)
