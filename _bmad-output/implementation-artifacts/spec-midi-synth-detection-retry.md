---
title: 'MIDI synth presence retry without port retouch'
type: 'feature'
created: '2026-08-02'
status: 'done'
baseline_commit: '2cc058555fcdaee2c723d11287bd381e66f312aa'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/epic-8-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Device Inquiry runs mainly when MIDI From/To ports are chosen or changed. If the synth is absent at startup (or drops later via cable/power/bad contact) while the same ports stay selected, detection does not retry — the user must juggle header port combos to unlock again.

**Approach:** Keep the existing Device Inquiry → APVTS (`deviceDetected` / `deviceType` / `deviceVersion`) → footer / Epic 8 UI-lock pipeline. Add (1) periodic re-inquiry on already-open ports while undetected and while detected (heartbeat), and (2) OS MIDI device-list change handling that refreshes port lists and reuses the existing port-sync → inquiry path. Expose both intervals as named constants for easy UAT tuning.

## Boundaries & Constraints

**Always:**
- Reuse `MidiManager::performDeviceInquiry` / `updateDeviceStatus` / failure clear paths — no parallel detection state machine.
- Message-thread orchestration only; never block the audio thread or `MidiManager::run` on hardware wait.
- While From+To are open and synth is unreachable: keep UI locked and connection feedback as today.
- On successful inquiry: set detection props, update footer identity / device type, unlock editable sections via existing gates.
- On lost synth (heartbeat timeout/failure): clear to undetected / locked with coherent footer feedback.
- Named constants (single SSOT, easy to tweak): undetected retry **1000 ms**, detected heartbeat **5000 ms**.
- Do not start a new inquiry tick while an inquiry (or other async SysEx capture sharing the same token) is already in flight — skip that tick.
- Periodic retry/heartbeat must bypass the same-port debounce (`shouldStartDeviceInquiry`) so the same open pair can be re-inquired.

**Ask First:**
- Changing the product intervals away from 1 s / 5 s (constants may still be edited for UAT without renegotiation).
- Skipping OS MIDI device-list refresh entirely if implementation hits a host-specific blocker.

**Never:**
- Header MIDI combo UI redesign.
- Hosted vs standalone model changes beyond what list-change / retry strictly needs.
- Windows multi-client / loopMIDI docs (already covered elsewhere).
- Inventing a second `deviceDetected` writer or UI-lock path.
- Spamming inquiry when either From or To is unset / not open.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Late plug / power-on | From+To open, `deviceDetected=false`, synth becomes reachable | Within ~1 s + inquiry timeout, inquiry succeeds; detected=true; UI unlocks; footer identity updates | On timeout keep undetected; next 1 s tick retries |
| Bad contact recovers | Same ports, undetected after prior failure | Same as late plug — no port combo change required | Same |
| Synth drops while detected | Detected=true, then no Device ID reply on heartbeat | Heartbeat (~5 s) inquiry fails → `updateDeviceStatus(false)`; UI locks; guidance footer | Clear last-pair debounce as failure path already does so recovery can retry at 1 s |
| Ports incomplete | From or To missing/closed | No periodic inquiry; clear detection if ports were lost (existing `clearDeviceDetectionAfterPortLoss`) | N/A |
| OS MIDI list change | USB interface appear/disappear | Refresh header MIDI lists; `syncMidiPortsFromState` → existing `refreshDeviceInquiryAfterPortSync` | If prior open IDs gone, clear detection via existing path |
| Inquiry in flight | Timer fires during pending inquiry/dump capture | Skip tick; do not cancel user-facing dumps solely for heartbeat | Next eligible tick after idle |
| Port combo change | User changes From/To | Existing immediate inquiry path unchanged | Debounce still applies to sync-triggered starts |

</frozen-after-approval>

## Code Map

- `Source/Core/MIDI/MidiManagerDeviceInquiry.cpp` (+ `MidiManager.h`) -- Own presence timer; schedule/stop with port availability; call force re-inquiry; keep finish success/failure SSOT
- `Source/Core/MIDI/DeviceInquiryTrigger.h` -- Add force/retry predicate (or companion helper) distinct from same-pair debounce; keep `shouldStartDeviceInquiry` for port-sync
- `Tests/Unit/DeviceInquiryTriggerTests.cpp` -- Cover force/retry vs debounce predicates
- `Source/Core/MIDI/Queue/MidiRequestTiming.h` -- Add `kDevicePresenceRetryUndetectedMs = 1000` and `kDevicePresenceHeartbeatDetectedMs = 5000` (or adjacent Device Inquiry timing SSOT if cleaner)
- `Source/Core/PluginProcessorMidiPorts.cpp` / `PluginProcessor.h` -- Wire `juce::MidiDeviceListConnection` → header list refresh + `syncMidiPortsFromState` (no second inquiry sender)
- `Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.h` -- Existing `refreshPortLists()` / `populateMidiPortLists()` reused on list change
- `Source/Core/MIDI/EditorOutboundGate.h` / CompareLockBinder path -- No product-rule change; consume APVTS as today

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/MIDI/Queue/MidiRequestTiming.h` -- Add named presence-retry / heartbeat interval constants (1000 / 5000) -- UAT-tunable SSOT
- [x] `Source/Core/MIDI/DeviceInquiryTrigger.h` + `Tests/Unit/DeviceInquiryTriggerTests.cpp` -- Add explicit force-retry predicate for open ports (bypass last-pair debounce); unit-test matrix rows for debounce vs force
- [x] `Source/Core/MIDI/MidiManager.h` + `MidiManagerDeviceInquiry.cpp` -- Presence timer: 1 s while ports open && !detected; 5 s while detected; skip if async capture busy; force `performDeviceInquiry`; start/stop with port sync / clear paths
- [x] `Source/Core/PluginProcessorMidiPorts.cpp` (+ header as needed) -- Subscribe to MIDI device list changes; refresh header port lists; call existing sync → inquiry refresh
- [x] Manual / build verification -- Build preset + unit tests; smoke late-connect and unplug without touching port combos

**Acceptance Criteria:**
- Given From+To correctly configured and synth absent, when the synth later becomes reachable without changing ports, then Device Inquiry succeeds, `deviceDetected` becomes true, footer identity updates, and editable sections unlock.
- Given synth was detected, when it becomes unreachable on the same ports, then within one heartbeat interval plus inquiry timeout, detection clears and the UI locks with coherent feedback.
- Given either MIDI port is unset/closed, when the presence timer would fire, then no Device Inquiry is sent.
- Given an OS MIDI device list change, when ports still resolve, then lists refresh and the existing sync/inquiry path runs; when open devices disappear, detection clears via the existing port-loss path.
- Given constants `kDevicePresenceRetryUndetectedMs` and `kDevicePresenceHeartbeatDetectedMs`, when UAT needs a different cadence, then only those named values need editing.

## Spec Change Log

- 2026-08-02 (post-UAT harden): Outbound-idle timeout during Device Inquiry start soft-aborts without clearing detection (`softAbortDeviceInquiryOutboundBusy`). Avoids UI re-lock under dense realtime/SysEx traffic; KEEP presence timer + force inquiry + 1s/5s constants + OS list-change soft sync.
## Design Notes

Port-sync debounce (`shouldStartDeviceInquiry`) must stay for repeated `syncMidiPortsFromState` without spam. Presence retry needs a separate force path so success does not permanently block re-inquiry on the same pair.

Prefer one `juce::Timer` on `MidiManager` (message thread) that picks the next interval from current `deviceDetected` after each completed inquiry attempt, rather than two overlapping timers.

Heartbeat shares `asyncRequestToken_` with async dumps — skipping ticks while busy avoids cancelling bank/patch loads for presence checks.

Outbound-idle timeout while starting Device Inquiry (queue never quiet enough under dense play/edit) uses `softAbortDeviceInquiryOutboundBusy`: release the capture token without clearing `deviceDetected` / footer lock. Real Device ID timeout or unsupported reply still clears detection as before.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds
- `ctest --preset macos-debug-arm64 -R DeviceInquiryTrigger` (or project-equivalent unit-test target filter) -- expected: predicate tests pass
- `python3 Scripts/quality/lint_touched.py` -- expected: clean on touched C++ files

**Manual checks (if no CLI):**
- Ports set, synth off → locked; power on synth without changing combos → unlocks within ~1 s + timeout
- Unplug / power off while unlocked → relocks within ~5 s + timeout without touching combos
- USB MIDI interface hot-plug → header lists update; inquiry follows existing sync behaviour

## Suggested Review Order

**Presence retry / heartbeat**

- Entry: timer tick skips busy capture then force-inquires open From/To
  [`MidiManagerDeviceInquiry.cpp:60`](../../Source/Core/MIDI/MidiManagerDeviceInquiry.cpp#L60)

- Interval SSOT switches 1 s undetected ↔ 5 s detected
  [`MidiManagerDeviceInquiry.cpp:35`](../../Source/Core/MIDI/MidiManagerDeviceInquiry.cpp#L35)

- UAT-tunable interval constants
  [`MidiRequestTiming.h:18`](../../Source/Core/MIDI/Queue/MidiRequestTiming.h#L18)

- Force predicate bypasses same-pair debounce
  [`DeviceInquiryTrigger.h:28`](../../Source/Core/MIDI/DeviceInquiryTrigger.h#L28)

**Async capture safety**

- Busy flag set around Device Inquiry start/finish
  [`MidiManagerDeviceInquiry.cpp:323`](../../Source/Core/MIDI/MidiManagerDeviceInquiry.cpp#L323)

- Same flag on async patch cancel/finish paths
  [`MidiManagerAsyncPatch.cpp:17`](../../Source/Core/MIDI/MidiManagerAsyncPatch.cpp#L17)

**OS MIDI list change**

- Soft port sync + Keyboard From reopen, then header refresh
  [`PluginProcessorMidiPorts.cpp:295`](../../Source/Core/PluginProcessorMidiPorts.cpp#L295)

- MidiDeviceListConnection install with WeakReference
  [`PluginProcessorMidiPorts.cpp:280`](../../Source/Core/PluginProcessorMidiPorts.cpp#L280)

- Header combos re-populated and re-selected from APVTS
  [`PluginEditorHeader.cpp:71`](../../Source/GUI/PluginEditorHeader.cpp#L71)

**Tests**

- Force vs debounce predicate coverage
  [`DeviceInquiryTriggerTests.cpp:63`](../../Tests/Unit/DeviceInquiryTriggerTests.cpp#L63)
