---
title: 'Header port names uppercase'
type: 'chore'
created: '2026-08-02'
status: 'done'
route: 'one-shot'
---

# Header port names uppercase

## Intent

**Problem:** Available audio and MIDI port names in the header comboboxes kept the OS mixed case, while the rest of the GUI (labels, sentinels) is uppercase.

**Approach:** Uppercase only the display strings when adding device/channel items; keep port identifiers and selection logic unchanged.

## Suggested Review Order

- MIDI input port labels uppercased at populate (MIDI From / Keyboard From)
  [`HeaderPanel.cpp:212`](../../Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp#L212)

- MIDI output port labels uppercased at populate (MIDI To)
  [`HeaderPanel.cpp:234`](../../Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp#L234)

- Audio input channel labels uppercased at populate (Audio From)
  [`HeaderPanel.cpp:324`](../../Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp#L324)
