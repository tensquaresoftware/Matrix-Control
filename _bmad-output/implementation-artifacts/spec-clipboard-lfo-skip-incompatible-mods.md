---
organization: Ten Square Software
project: Matrix-Control
title: Clipboard LFO skip incompatible mod sources
author: BMad Agent
type: bugfix
created: '2026-07-25'
status: done
route: one-shot
baseline_commit: 76629dd1ca7be03e9b3e699619a9b56f2197b71a
updated: '2026-07-25'
---

# Clipboard LFO skip incompatible mod sources

## Intent

**Problem:** Cross-paste between LFO 1 and LFO 2 remapped SPEED<PRESSURE ↔ SPEED<KEYBD and AMP<RAMP1 ↔ AMP<RAMP2 even though those modulators are not the same source, so paste silently transferred incompatible values.

**Approach:** On LFO1↔LFO2 paste, skip those module-specific mod parameters (leave the target unchanged), matching the DCO Sync/Detune skip spirit; keep same-module paste unchanged; align D-060 / addendum / story 5.1 notes.

## Suggested Review Order

**Cross-paste skip rule**

- Explicit target-ID skip for LFO-specific mod sources (no remap).
  [`ClipboardService.cpp:140`](../../Source/Core/Services/ClipboardService.cpp#L140)

- Paste path applies skip before displayName matching.
  [`ClipboardService.cpp:426`](../../Source/Core/Services/ClipboardService.cpp#L426)

**Regression coverage**

- Cross-paste leaves Pressure/Keyboard and Ramp mods untouched; shared Speed/Amplitude copy.
  [`ClipboardServiceTests.cpp:242`](../../Tests/Unit/ClipboardServiceTests.cpp#L242)

- Same-module paste still copies the module-specific mods.
  [`ClipboardServiceTests.cpp:294`](../../Tests/Unit/ClipboardServiceTests.cpp#L294)

**Product SSOT**

- Addendum matrix row + D-060 amendment + story 5.1 LFO note updated for skip-not-map.
  [`addendum.md:144`](../planning-artifacts/prds/prd-matrix-control-2026-05-25/addendum.md#L144)
