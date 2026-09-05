---
title: 'NumberBox edit border and font'
type: 'bugfix'
created: '2026-09-05'
status: 'done'
route: 'one-shot'
baseline_commit: '242b28934e8f1ad538786d47a340c17d58125850'
---

# NumberBox edit border and font

## Intent

**Problem:** Double-clicking an editable NumberBox hid or thinned the border at some UI scales, enlarged the font, and showed a white selection block — edit mode did not match the idle control.

**Approach:** Keep the parent-drawn border (ceil inset so fractional strokes stay uncovered). On edit: fill with navigation focus red, clear the field for full retype, disable text selection, white bold digits and white caret; Escape / focus-lost restores the previous value.

## Suggested Review Order

**Edit appearance**

- Shared scaled font for paint and live editor refresh.
  [`NumberBox.cpp:167`](../../Source/GUI/Widgets/NumberBox.cpp#L167)

- Inset editor so parent border stays visible; sync colours on look/scale change.
  [`NumberBox.cpp:194`](../../Source/GUI/Widgets/NumberBox.cpp#L194)

- Fill under border with editor background while editing to avoid a colour ring.
  [`NumberBox.cpp:95`](../../Source/GUI/Widgets/NumberBox.cpp#L95)

**Helpers**

- Stroke thickness and inset helpers; layout on resize.
  [`NumberBox.h:83`](../../Source/GUI/Widgets/NumberBox.h#L83)
