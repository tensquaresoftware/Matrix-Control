---
title: 'Confirmation modal button order (LTR Cancel → primary)'
type: 'feature'
created: '2026-07-26'
status: 'done'
baseline_commit: 'd5824b347b49494bcd0a6423093ac1b83b7839ad'
review_loop_iteration: 1
context:
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Confirmation modals do not share one left-to-right reading across macOS, Windows, and Linux — Cancel is not reliably leftmost, the primary action is not reliably rightmost/default, so Escape vs Enter and visual habit diverge by OS.

**Approach:** Enforce one visual LTR rule everywhere (Cancel → optional intermediate → primary; rightmost = default; Escape/dismiss = Cancel) for the six confirmation gates plus Master Init, using platform-aware presentation and role-based result mapping so Core contracts stay unchanged. When native chrome cannot guarantee LTR + Enter=primary, prefer a controlled path that does (Windows/Linux AlertWindow); keep macOS native NSAlert.

## Boundaries & Constraints

**Always:**
- Visual LTR on all three OSes: Cancel leftmost; primary rightmost and default (Enter, even after Tab/click focus on another button); Escape / dismiss / out-of-range = Cancel (no confirm side effect).
- Target orders: Unsaved Cancel→Continue; Flush Cancel→Continue; Defrag Cancel→Defrag; Export Cancel→Keep→Overwrite; History Cancel→Discard→Export; Reconcile Cancel→Internal→Filename; Master Init Cancel→Reset.
- Preserve Core gate contracts (bool / enums / sync callbacks); remap UI indices only in the editor helper layer.
- Keep English labels from `PluginDisplayNames`; no French in source.
- Prefer LTR + Enter=primary over OS-native chrome when they conflict: macOS keeps native NSAlert; Windows/Linux use controlled `AlertWindow` for these gates (documented exception — JUCE TaskDialog cannot set default to the rightmost button when Cancel is leftmost).

**Ask First:**
- Replacing all confirmations with custom-skinned dialogs (full Master Init–style chrome).
- Changing Core enum/bool signatures or async-migrating modal loops.
- Deviating from the listed LTR orders for any listed modal.
- Investing in a custom Windows TaskDialog wrapper solely to restore native chrome.

**Never:**
- Change business meaning of each choice (only display order + default).
- Touch About or FileChooser.
- Blindly reverse `withButton` while keeping today’s `((i+1)%N)` mapping (would invert Escape/confirm).
- Claim one identical `withButton` registration order works for visual LTR on both macOS NSAlert and Windows TaskDialog.
- Let Enter activate Cancel or middle when a non-primary control has focus.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 2-button primary | User clicks Continue/Defrag/Reset or presses Enter | Proceed / confirm path | N/A |
| 2-button cancel | User clicks Cancel, Escape, or dismiss | Abort / no confirm side effect | OOR native codes → Cancel |
| Enter with focus on Cancel | Tab or click Cancel, then Enter | Still runs primary (rightmost) | N/A |
| 3-button primary | Enter or rightmost (Overwrite / Export / Filename) | Matching enum primary | N/A |
| 3-button middle | Keep / Discard / Internal | Matching intermediate enum | N/A |
| 3-button cancel | Cancel / Escape / dismiss | Cancel / nullopt | OOR → Cancel |
| Headless / no gate | Tests without UI | Existing proceed/cancel defaults unchanged | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/PluginEditor.cpp` — replace `showMappedAlert` with `showOrderedConfirmAlert`; migrate six gates; Win/Linux AlertWindow focus/Enter policy
- `Source/GUI/Dialogs/MasterInitConfirmDialog.cpp` — Cancel left / Reset right; Enter → Reset even if Cancel was focused; Escape/outside → dismiss
- `Source/GUI/Dialogs/MasterInitConfirmDialog.h` — only if focus/default API needs a declaration
- `Source/Shared/Definitions/PluginDisplayNames.h` — label SSOT (read-only)
- `Source/Core/PluginProcessor.*` — gate contracts (read-only)
- JUCE `NativeMessageBox` (mac) + controlled `AlertWindow` (Win/Linux) — platform paths

## Tasks & Acceptance

**Execution:**
- [x] `Source/GUI/PluginEditor.cpp` -- add role-based `showOrderedConfirmAlert` (macOS native primary-first; Win/Linux AlertWindow Cancel→middle→primary with Escape=0 / Return=1 / middle=2); migrate six gates; ensure Enter always triggers primary (focus primary on show; Cancel/middle must not consume Return) -- LTR + Escape-safe + Enter=primary without breaking Core contracts
- [x] `Source/GUI/Dialogs/MasterInitConfirmDialog.cpp` (+ `.h` if needed) -- Cancel left / Reset right; Enter confirms Reset even if Cancel has focus; Escape/outside dismiss -- same LTR rule for custom dialog
- [ ] Manual smoke (macOS required; Windows/Linux when available) -- each listed modal: LTR order, Enter=primary (including after focusing Cancel), Escape=Cancel, correct business outcomes -- Guillaume UAT

**Acceptance Criteria:**
- Given each listed modal on macOS, Windows, and Linux, when it is shown, then buttons appear exactly in the target LTR order for that modal.
- Given any listed modal, when the user presses Enter (including after Tab/click on Cancel), then the rightmost (primary) action runs; when they press Escape or dismiss outside buttons, then Cancel runs with no confirm side effect.
- Given each gate’s historical meanings, when the user chooses Cancel / middle / primary, then processor-side behavior matches today’s semantics.
- Given Windows, when a confirmation gate runs, then it may use JUCE AlertWindow chrome (not TaskDialog) so LTR + Enter=primary hold.
- Given a future confirmation of the same type, when it is added, then it uses the same helper (or Master Init pattern), not ad-hoc opposite registration.

## Spec Change Log

- review_loop 1 (intent_gap): Frozen Always required both Windows native chrome and LTR+Enter=primary; JUCE TaskDialog cannot do both. Human chose prioritize button order — Always now allows Win/Linux AlertWindow; Ask First covers custom TaskDialog wrapper. Avoided shipping contradictory Always. KEEP: role-based helper, macOS native primary-first mapping, Master Init LTR, stable Cancel=0/primary=1/middle=2 codes.

## Design Notes

- **macOS native:** register `primary`, then middle, then `Cancel`; map `(raw+1)%N`; OOR → 0.
- **Windows / Linux:** controlled `AlertWindow`: add Cancel (0, Escape), optional middle (2), primary (1, Return). After building, give keyboard focus to primary and prevent Cancel/middle from treating Return as a click (e.g. `setWantsKeyboardFocus(false)` on non-primary, or equivalent).
- **Master Init:** layout Cancel left / Reset right; handle Return in dialog `keyPressed` → confirm; do not let focused Cancel swallow Enter.

```text
roles: cancel="Cancel", primary="Continue"
macOS: Continue, Cancel → visual Cancel | Continue
Win/Linux AlertWindow: Cancel, Continue + Return→Continue → visual Cancel | Continue
semantic: Continue→1, Cancel/Escape→0
```

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: build succeeds
- `./Builds/macOS/ARM/Debug/Matrix-Control_Tests_artefacts/Debug/Matrix-Control_Tests` -- expected: existing tests pass

**Manual checks (if no CLI):**
- On each OS: every listed modal — LTR order, Enter=rightmost (also after focusing Cancel), Escape=Cancel, correct outcomes.

## Suggested Review Order

**Ordered confirm helper**

- Platform split: macOS NSAlert vs Win/Linux AlertWindow
  [`PluginEditor.cpp:83`](../../Source/GUI/PluginEditor.cpp#L83)

- Cancel/middle unfocusable so Enter stays on primary
  [`PluginEditor.cpp:50`](../../Source/GUI/PluginEditor.cpp#L50)

**Gate call sites**

- Two-button Cancel → primary mapping unchanged for Core
  [`PluginEditor.cpp:299`](../../Source/GUI/PluginEditor.cpp#L299)

- Three-button Export Cancel → Keep → Overwrite
  [`PluginEditor.cpp:326`](../../Source/GUI/PluginEditor.cpp#L326)

- Reconcile primary is Filename (rightmost default)
  [`PluginEditor.cpp:436`](../../Source/GUI/PluginEditor.cpp#L436)

**Master Init**

- Cancel left, Reset right; Enter confirms
  [`MasterInitConfirmDialog.cpp:141`](../../Source/GUI/Dialogs/MasterInitConfirmDialog.cpp#L141)

- Cancel cannot steal Return
  [`MasterInitConfirmDialog.cpp:26`](../../Source/GUI/Dialogs/MasterInitConfirmDialog.cpp#L26)
