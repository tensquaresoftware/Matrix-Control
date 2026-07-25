---
title: 'System-style confirmation modals'
type: 'feature'
created: '2026-07-25'
status: 'done'
baseline_commit: 'a168e35a97c30fe34b3b0debb46caf4783fb4330'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Confirmation dialogs use JUCE-drawn `AlertWindow` chrome while folder/file pickers already use the OS look, so the standalone (and plugin) UI feels inconsistent after the native title bar work.

**Approach:** Keep the existing sync modal-gate contracts and button semantics, but show those confirmations through JUCE’s native alert path on macOS/Windows (Linux stays JUCE-drawn — known framework limit). Leave About, Master Init confirm, and FileChoosers unchanged.

## Boundaries & Constraints

**Always:**
- Preserve each gate’s return mapping (2-button: confirm→1 / cancel→0; 3-button: first→1 / second→2 / cancel→0).
- Keep processor-owned sync gate callbacks; do not redesign to async for this change.
- Enable native alerts via LookAndFeel and use AlertWindow static/`MessageBoxOptions` APIs that honor that flag — constructing `AlertWindow` + `runModalLoop` alone does **not** go native.
- Associate dialogs with the editor component when the API allows (host parenting).
- Stay on the message thread for modal display (existing `jassert`s; add where missing if touching that site).

**Ask First:**
- Changing Master Init confirm or About to native chrome.
- Converting gates from sync `runModalLoop` / `AlertWindow::show` to async-only callbacks.
- Adding a custom Linux GTK/Qt dialog path outside JUCE.

**Never:**
- Restyle FileChooser (already native where the OS allows).
- Change dialog copy, button labels, or gate business logic.
- Touch Core gate orchestration beyond editor UI lambdas / LookAndFeel enablement.
- Claim Linux will look OS-native with stock JUCE 8 (it reuses AlertWindow internally).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Unsaved / Defrag (2 buttons) | User picks Continue/Defrag or Cancel | Same bool / callback outcome as today | Cancel / dismiss → abort path |
| History / Export collision / Name reconcile (3 buttons) | User picks button 1, 2, or Cancel | Same enum / optional outcome as today | Cancel / dismiss → cancel path |
| macOS / Windows | Any of the five gates | OS-native alert chrome | N/A |
| Linux | Any of the five gates | Still JUCE AlertWindow look; codes unchanged | N/A |
| Headless / missing gate | Tests without UI gate | Proceed without modal (existing) | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/PluginEditor.cpp` -- five `AlertWindow` + `runModalLoop` gates; enable native alerts here and migrate to `MessageBoxOptions` / `AlertWindow::show` (or `showOkCancelBox` / `showYesNoCancelBox`)
- `Source/Core/PluginProcessor.*` -- gate slots and sync callers (read-only unless a compile break forces a tiny signature tweak — prefer none)
- `CMakeLists.txt` -- `JUCE_MODAL_LOOPS_PERMITTED=1` already required; keep
- JUCE `LookAndFeel::setUsingNativeAlertWindows` + `AlertWindow::show` -- native path only via these APIs, not raw `runModalLoop` on a constructed window

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/PluginEditor.cpp` -- call `juce::LookAndFeel::getDefaultLookAndFeel().setUsingNativeAlertWindows(true)` once during editor setup -- enable OS alerts for static AlertWindow helpers
- [x] `Source/GUI/PluginEditor.cpp` -- replace all five manual `AlertWindow`/`runModalLoop` blocks with `MessageBoxOptions` + `AlertWindow::show` (or equivalent ok/cancel / yes-no-cancel helpers), button order matching today’s return codes, `withAssociatedComponent(this)` where practical -- actually hit the native path
- [ ] Manual smoke (macOS at minimum) -- trigger unsaved-edit, mutator history gate, export collision, defrag, and name reconciliation; confirm native chrome and correct outcomes -- verify product intent (Guillaume UAT)

**Acceptance Criteria:**
- Given the editor is open on macOS or Windows, when any of the five confirmation gates runs, then the dialog uses OS-native chrome (not default JUCE AlertWindow paint).
- Given the same gates on Linux, when a confirmation runs, then button outcomes match today’s mapping even if chrome remains JUCE-drawn.
- Given each gate’s historical button meanings, when the user chooses each option, then processor-side behavior is unchanged.
- Given About, Master Init confirm, and FileChoosers, when this change ships, then those UIs are visually/behaviorally unchanged by this work.

## Spec Change Log

## Design Notes

Raw `AlertWindow alert(...); alert.runModalLoop()` ignores `setUsingNativeAlertWindows`. Prefer enabling the LookAndFeel flag, then showing via a helper that uses `NativeMessageBox::show` (plain indices) when native alerts are on, mapping to AlertWindow codes with out-of-range → cancel — so Windows Escape/`IDCANCEL` on 2-button dialogs does not confirm. When native is off (Linux), use `AlertWindow::show`.

Button order in `withButton` must match today’s semantic order (confirm/first action first; cancel last).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds

**Manual checks (if no CLI):**
- On macOS standalone: open patch folder picker (still native), then force an unsaved-edit Continue/Cancel and a 3-button mutator gate — confirmations look system-native; choices still do the right thing.
- Optionally Windows build smoke if available; Linux: expect JUCE chrome, correct codes.

## Suggested Review Order

**Native alert path**

- Enable OS alerts on the default LookAndFeel (macOS/Windows; Linux stays false internally).
  [`PluginEditor.cpp:162`](../../Source/GUI/PluginEditor.cpp#L162)

- Safe mapping: native plain indices + out-of-range → cancel (Windows Escape fix).
  [`PluginEditor.cpp:40`](../../Source/GUI/PluginEditor.cpp#L40)

**Gate migrations**

- Two-button Defrag confirm via `showMappedAlert`.
  [`PluginEditor.cpp:213`](../../Source/GUI/PluginEditor.cpp#L213)

- Three-button export collision (Overwrite / Keep / Cancel).
  [`PluginEditor.cpp:233`](../../Source/GUI/PluginEditor.cpp#L233)

- Three-button history gate (Export / Discard / Cancel).
  [`PluginEditor.cpp:264`](../../Source/GUI/PluginEditor.cpp#L264)

- Two-button unsaved-edit warning.
  [`PluginEditor.cpp:287`](../../Source/GUI/PluginEditor.cpp#L287)

- Three-button patch name reconciliation.
  [`PluginEditor.cpp:325`](../../Source/GUI/PluginEditor.cpp#L325)

**Lifecycle guards**

- Clear processor modal gates when the editor is destroyed.
  [`PluginEditor.cpp:845`](../../Source/GUI/PluginEditor.cpp#L845)
