---
title: 'No-synth footer copy and ERROR severity'
type: 'bugfix'
created: '2026-07-31'
status: 'done'
route: 'one-shot'
---

# No-synth footer copy and ERROR severity

## Intent

**Problem:** When no Matrix synth is detected, the footer guidance still said “power-cycle the Matrix” and named plain MIDI From / MIDI To, and it was shown as INFO instead of a clear ERROR.

**Approach:** Update `kDeviceLockGuidance` wording and publish that specific message with `uiMessageSeverity` `"error"` from both MIDI re-assert and CompareLockBinder paths (unsupported-device lock copy stays INFO).

## Suggested Review Order

**Copy**

- Canonical English string for the locked, undetected-synth footer.
  [`PluginDisplayNames.h:45`](../../Source/Shared/Definitions/PluginDisplayNames.h#L45)

**Severity wiring**

- Port clear/re-assert: error only when guidance is the no-synth string.
  [`MidiManager.cpp:30`](../../Source/Core/MIDI/MidiManager.cpp#L30)

- Device-lock binder: error helper for no-synth; info for unsupported Matrix.
  [`CompareLockBinder.cpp:82`](../../Source/GUI/Helpers/CompareLockBinder.cpp#L82)

**Tests**

- Expect error severity when lock guidance is re-asserted after empty port clear.
  [`MidiPortOpenFailurePropagationTests.cpp:168`](../../Tests/Unit/MidiPortOpenFailurePropagationTests.cpp#L168)
