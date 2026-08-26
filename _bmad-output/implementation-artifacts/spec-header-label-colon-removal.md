---
title: 'Header routing label colon removal'
type: 'chore'
created: '2026-08-26'
status: 'done'
route: 'one-shot'
---

# Header routing label colon removal

## Intent

**Problem:** Header routing labels displayed a trailing colon and space (`MIDI FROM :`, etc.), adding visual noise in an already crowded bar.

**Approach:** Remove the trailing ` :` from the five `PluginDisplayNames::HeaderPanel` label constants; Settings labels and footer copy are unchanged in this pass.

## Suggested Review Order

- Single source of truth for header label strings
  [`PluginDisplayNames.h:16`](../../Source/Shared/Definitions/PluginDisplayNames.h#L16)

- Header panel binds labels from display-name constants
  [`HeaderPanel.cpp:64`](../../Source/GUI/Panels/MainComponent/HeaderPanel/HeaderPanel.cpp#L64)
