---
title: 'Footer synth version dot format'
type: 'bugfix'
created: '2026-08-02'
status: 'done'
route: 'one-shot'
---

# Footer synth version dot format

## Intent

**Problem:** The footer showed the raw Device Inquiry revision (e.g. `V1234` or space-padded forms) instead of a two-pack dotted display users expect.

**Approach:** Keep APVTS `deviceVersion` unchanged; format only for footer display by stripping spaces/tabs and inserting `.` before the last two characters when the cleaned string is undotted and at least three characters long.

## Suggested Review Order

- Display-only formatter: strip whitespace, split undotted packs, preserve existing dots
  [`DeviceVersionDisplayFormat.h:9`](../../Source/GUI/Helpers/DeviceVersionDisplayFormat.h#L9)

- Wire formatted version into the right-zone device detail string
  [`FooterPanel.cpp:289`](../../Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp#L289)

- Unit coverage for intent examples and short/whitespace edge cases
  [`DeviceVersionDisplayFormatTests.cpp:21`](../../Tests/Unit/DeviceVersionDisplayFormatTests.cpp#L21)
