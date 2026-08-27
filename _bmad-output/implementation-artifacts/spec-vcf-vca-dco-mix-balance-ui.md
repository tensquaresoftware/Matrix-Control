---
title: 'VCF/VCA DCO mix Balance label and reverse UI'
type: 'feature'
created: '2026-08-27'
status: 'done'
route: 'one-shot'
---

# VCF/VCA DCO mix Balance label (polarity-correct)

## Intent

**Problem:** The VCF/VCA Balance control was labeled only `BALANCE`, so users could not tell which slider end was which oscillator. Hardware Mix polarity is 0 = DCO 2 only, 63 = DCO 1 only. An attempted UI-only slider reverse felt broken and conflicted with modulation-bus value direction.

**Approach:** Keep the normal slider gesture (0 left → 63 right). Use one shared label `DCO 2 | DCO 1 MIX` for both the Patch Edit parameter and the Matrix Mod destination, matching left/right ends to the heard oscillators. No SysEx / APVTS value remapping.

## Spec Change Log

- 2026-08-27 — UAT: reverse-horizontal UI reverted; label polarity corrected to `DCO 2 | DCO 1 MIX` (confirmed: 0=DCO2, 63=DCO1); Matrix Mod destination shares the same string.

## Suggested Review Order

**Shared label (SSOT)**

- Mix polarity comment + destination string used by Patch Edit Balance and Mod Matrix.
  [`PluginDisplayNames.h:523`](../../Source/Shared/Definitions/PluginDisplayNames.h#L523)

- Balance display name aliases the destination constant (no duplicate literal).
  [`PluginDisplayNames.h:696`](../../Source/Shared/Definitions/PluginDisplayNames.h#L696)

**Gesture unchanged**

- Balance descriptor: standard 0–63 range, no UI invert flag.
  [`PluginDescriptorsPatchEdit.cpp:318`](../../Source/Shared/Definitions/PluginDescriptorsPatchEdit.cpp#L318)
