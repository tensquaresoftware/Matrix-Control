---
title: 'Fix editorial undo stack collapse after first undo (Matrix Mod bus sources)'
type: 'bugfix'
created: '2026-08-26'
status: 'done'
route: 'one-shot'
---

# Fix editorial undo stack collapse after first undo (Matrix Mod bus sources)

## Intent

**Problem:** After three sequential Matrix Mod bus source edits in standalone, only one UNDO step was available; the stack collapsed because JUCE undo limits were misconfigured for full APVTS layouts.

**Approach:** Pass the editorial transaction depth (100) as `minimumTransactionsToKeep` in `UndoManager::setMaxNumberOfStoredUnits`, and add a full-layout unit test mirroring the smoke scenario.

## Suggested Review Order

1. [Source/Core/PluginProcessor.cpp](Source/Core/PluginProcessor.cpp) — production undo limit wiring (one-line fix).
2. [Tests/Unit/UndoManagerMatrixModSourceChoiceTests.cpp](Tests/Unit/UndoManagerMatrixModSourceChoiceTests.cpp) — full APVTS regression test and harness limit injection.
3. [Source/Core/PluginProcessorInternal.h](Source/Core/PluginProcessorInternal.h) — constant intent comment.
