---
organization: Ten Square Software
project: Matrix-Control
title: Spec — Residual MIDI Note Off starvation + PANIC alert
author: BMad Agent
status: done
type: bugfix
baseline_commit: af37794
created: 2026-08-02
updated: 2026-08-02
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/implementation-artifacts/passation-bug-notes-midi-stuck-first-slider-lag.md'
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** After the June 2026 queued-SysEx non-blocking fix, held notes can still stick on the Matrix-1000 during live Matrix Mod / SysEx edit bursts. Remaining software gaps: (1) when a SysEx is parked as pending, already-queued realtime can wait out the full inter-SysEx sleep; (2) Matrix Mod coalesce (10 ms) equals the stock gate, so continuous edits keep ~100% SysEx duty; (3) Debug MidiLogger double-logs + flushes on the MIDI consumer. Users also need a always-available PANIC safety net and a visible queue-pressure alert.

**Approach:** Fix residual consumer drain + ease live SysEx pressure; add header PANIC (CC 123 then CC 121 on active channel via realtime enqueue); drive red PANIC look + centre-footer MIDI alert from realtime queue depth with hysteresis. No GUI includes from Core.

## Boundaries & Constraints

**Always:**
- Keep realtime-before-SysEx dequeue priority.
- After parking pending SysEx, drain remaining queued realtime before sleeping on the gate.
- PANIC: always enabled; no tooltip; far-right header with scaled 12 px right padding; `TSS::Button`; CC 123/0 then CC 121/0 on APVTS `midiChannel` via normal realtime path (thin documented priority only if needed for promptness).
- Alert engage: red button fill, black text, white hover text; centre footer badge `MIDI` (red fill, black text) + locked message; leave left footer for general app messages; right footer stays device status.
- Alert clears on hysteresis (depth down / settle) with or without PANIC click.
- Unit-test residual drain, Panic CC payload, hysteresis engage/clear without hardware.
- English UI strings only; quality gate on touched C++.

**Ask First:**
- Changing SysExDelayProfile stock gate ms itself (prefer coalesce/drain/logger first).
- Routing init / paste / reorder `dispatchBus` floods through the coalescer (those bursts are intentional today).
- Migrating RPC `sendSysExWithDelay` (inquiry / patch dump) off blocking sleep in this change.

**Never:**
- Tooltips; Panic near MIDI port combos or in the footer; treating empty queue alert as proof hardware still has stuck notes.
- GUI dependencies in `Source/Core/`.
- Expanding into BUG-MIDI-02 (first-slider lag) beyond incidental shared micro-fixes.
- Broad MIDI architecture rewrite.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Residual drain | Queue has SysEx (gate closed) then realtime already queued | Consumer parks SysEx, sends all queued realtime, then waits for gate | N/A |
| Live Matrix Mod | Continuous bus edit ~30 s with 3+ held notes | Coalesce > gate; no software realtime starvation; UAT: no ghost notes | Hardware UAT by Guillaume |
| Panic click | Active channel N | Enqueue CC123/0 then CC121/0 realtime on N; always clickable | If no MIDI out, existing connection error path |
| Alert engage | Realtime depth ≥ high threshold (sustained) | PANIC alert look + centre footer MIDI message | Do not touch left footer band |
| Alert clear | Depth ≤ low threshold for settle window, or after drain | Normal PANIC look; centre alert cleared | Panic click alone does not require alert to have been on |
| Logger hot path | Queued SysEx send with file logging on | No double logSysExSent; no per-message flush blocking consumer | Console/file logging may be slightly less synchronous |

</frozen-after-approval>

## Code Map

- `Source/Core/MIDI/MidiManagerOutbound.cpp` — `processOutboundQueue` / `handleOutboundMessage` / `run`; residual realtime drain-before-sleep
- `Source/Core/MIDI/Queue/MidiOutboundQueue.{h,cpp}` — add safe realtime (and optionally SysEx) depth getters
- `Source/Core/MIDI/Queue/SysExInterMessageDelay.*` — gate math; leave stock delay; avoid `waitUntilReady` on queued path
- `Source/Core/PluginProcessor.h` — `MatrixModSysExCoalesceTimer::kCoalesceDelayMs` raise above gate
- `Source/Core/Loggers/MidiLogger.*` / `MidiSender.cpp` — remove duplicate queued SysEx log and/or defer flush off consumer hot path
- `Source/Core/MIDI/MidiManager.*` / `EditorPath.*` — Panic API: enqueue CC123+CC121 on active channel
- `Source/GUI/Panels/MainComponent/HeaderPanel/*` — PANIC button, far-right layout, alert look toggle
- `Source/GUI/Layout/PanelDimensions.h` / `DesignPanels.h` / `DimensionFactory.cpp` — `rightPadding` 12 + panic width
- `Source/GUI/Panels/MainComponent/FooterPanel/*` — paint centre band alert (badge+detail); dedicated state, not left `uiMessage*`
- `Source/GUI/PluginEditorTimers.cpp` — poll depth @ HeaderRefreshTimer; hysteresis → header look + footer/APVTS signal
- `Source/GUI/Looks/LookBuilders.*` / skin colour IDs — alert `ButtonLook` (red bg / black text / white hover)
- `Source/Shared/Definitions/PluginDisplayNames.h` — `PANIC`, badge `MIDI`, locked footer sentence
- `Tests/Unit/MidiManagerTests.cpp` / `MidiOutboundQueueTests.cpp` (+ new Panic / hysteresis tests as needed)

## Tasks & Acceptance

**Execution:**
- [x] `MidiManagerOutbound.cpp` -- After pending SysEx park, drain remaining realtime before gate sleep -- closes residual Note Off delay
- [x] `PluginProcessor.h` -- Raise Matrix Mod coalesce above stock gate (default **40 ms**) -- cuts live SysEx duty
- [x] `MidiSender.cpp` + `MidiLogger.*` -- Stop double `logSysExSent` on queued path; avoid flush-per-message on consumer -- removes Debug I/O starvation
- [x] `MidiOutboundQueue.*` -- Add `realtimeDepth()` (mutex-safe) -- feeds alert + tests
- [x] `MidiManager.*` / `EditorPath.*` -- `sendPanic()` enqueues CC123 then CC121 on `midiChannel` via realtime -- safety net
- [x] `HeaderPanel.*` + dimensions -- Far-right PANIC (`TSS::Button`), scaled 12 px right pad, always enabled, no tooltip, alert look via `setLook`
- [x] `FooterPanel.*` + display names -- Centre-band MIDI badge + locked message; left band untouched for this feature
- [x] `PluginEditorTimers.cpp` (+ thin helper) -- Hysteresis on realtime depth (engage ≥ **32**, clear ≤ **4** after **250 ms** settle); drive PANIC look + centre footer
- [x] `Tests/Unit/*` -- Cover drain-before-sleep, Panic CC order/channel, depth/hysteresis; extend existing starvation test if needed
- [x] Quality -- `Scripts/quality/lint_touched.py` on touched C++; build + unit tests

**Acceptance Criteria:**
- Given SysEx parked pending with realtime already queued, when consumer yields, then all queued realtime is sent before gate sleep.
- Given continuous Matrix Mod edit ~30 s with 3+ held notes (VST3/AU/standalone), when keys release, then no ghost notes on hardware UAT.
- Given PANIC click, when MIDI out is available, then CC123/0 and CC121/0 leave on the active channel via realtime queue promptly.
- Given realtime depth crosses high threshold, when hysteresis engages, then PANIC shows alert look and centre footer shows `MIDI` + locked sentence; left footer unchanged.
- Given depth stays ≤ low threshold for settle window, when hysteresis clears, then normal PANIC look and centre alert disappear (with or without Panic click).
- Given patch 0x06 / master / Matrix Mod reorder paths, when exercised, then no intentional behavioural regression beyond coalesce timing on live valueTree Matrix Mod.

## Spec Change Log

## Design Notes

**Residual root cause (post-dd45bd0):** Classic H1 sleep-in-`sendSysExWithDelay` for *queued* SysEx is fixed. Remaining: (A) `processOutboundQueue` breaks on pending SysEx while realtime may still sit behind it in the queue, then `run()` sleeps up to gate ms; (B) coalesce 10 ms == stock gate → saturated SysEx wire during slider drag; (C) `MidiSender::sendSysEx` + `sendQueuedSysEx` both call `logSysExSent`, each flushing the file under Debug.

**Defaults (override only via Ask First / Edit):** coalesce **40 ms**; engage depth **32**; clear depth **4**; settle **250 ms**. Panic stays usable anytime; alert is a pressure proxy, not a hardware stuck-note oracle.

**Footer centre:** New dedicated alert state (APVTS props or FooterPanel setters from editor timer) — do not overload left `uiMessageText` / `uiMessageSeverity`.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- plugin + tests build
- `ctest --preset macos-debug-arm64 -R 'MidiManager|MidiOutbound|SysExInter|Panic|QueuePressure' --output-on-failure` -- unit suite for this change (adjust filter to actual test names)
- `python3 Scripts/quality/lint_touched.py` -- clean on touched C++

**Manual checks (if no CLI):**
- Hardware UAT: 3+ held notes + continuous Matrix Mod AMOUNT ~30 s → no ghosts; click PANIC clears stuck voice; force queue pressure (if possible) → centre alert + red PANIC, clears when quiet.

## Suggested Review Order

**Residual consumer drain**

- Park pending SysEx, then drain realtime without touching SysEx FIFO
  [`MidiManagerOutbound.cpp:146`](../../Source/Core/MIDI/MidiManagerOutbound.cpp#L146)

- Call drain before gate sleep when handle returns false
  [`MidiManagerOutbound.cpp:171`](../../Source/Core/MIDI/MidiManagerOutbound.cpp#L171)

- Realtime-only pop API (no SysEx reorder)
  [`MidiOutboundQueue.cpp:61`](../../Source/Core/MIDI/Queue/MidiOutboundQueue.cpp#L61)

**Panic safety net**

- CC 123 then 121 via realtime front; Omni/Mono → all channels
  [`MidiManager.cpp:193`](../../Source/Core/MIDI/MidiManager.cpp#L193)

- Thin front-insert for prompt Panic under backlog
  [`MidiOutboundQueue.cpp:33`](../../Source/Core/MIDI/Queue/MidiOutboundQueue.cpp#L33)

- Header click → MidiManager::sendPanic
  [`PluginEditorHeader.cpp:52`](../../Source/GUI/PluginEditorHeader.cpp#L52)

**Queue-pressure alert UI**

- Hysteresis engage/clear (2 ticks / 250 ms settle)
  [`RealtimeQueuePressureMonitor.h:12`](../../Source/Core/MIDI/Queue/RealtimeQueuePressureMonitor.h#L12)

- Poll depth @ 30 Hz → PANIC look + centre footer
  [`PluginEditorTimers.cpp:54`](../../Source/GUI/PluginEditorTimers.cpp#L54)

- Centre-band MIDI badge + locked message
  [`FooterPanel.cpp:144`](../../Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp#L144)

- Alert ButtonLook (red / black / white hover)
  [`LookBuilders.cpp:40`](../../Source/GUI/Looks/LookBuilders.cpp#L40)

**SysEx pressure relief**

- Matrix Mod coalesce raised above stock gate
  [`PluginProcessor.h:254`](../../Source/Core/PluginProcessor.h#L254)

**Tests**

- Panic all-channel payload + drain FIFO guard
  [`MidiManagerRealtimeTests.cpp`](../../Tests/Unit/MidiManagerRealtimeTests.cpp)

- Hysteresis engage/clear unit coverage
  [`RealtimeQueuePressureMonitorTests.cpp`](../../Tests/Unit/RealtimeQueuePressureMonitorTests.cpp)

