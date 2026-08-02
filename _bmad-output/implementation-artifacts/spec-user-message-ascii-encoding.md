---
organization: Ten Square Software
project: Matrix-Control
title: User Message ASCII Encoding
author: BMad Agent
type: bugfix
created: '2026-08-02'
status: done
baseline_commit: '4e8f5403fa43aaaa9c90f8c2637df5f4bcc4afc1'
review_loop_iteration: 0
context: []
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Footer status text for Compare mode shows mojibake (`a€`) instead of a dash because the copy uses a UTF-8 em dash (U+2014). The same risk exists in other status/dialog strings that still use non-ASCII punctuation.

**Approach:** Replace the Compare footer em dash with ASCII ` - ` immediately. Inventory all status/dialog user messages; present any other non-ASCII findings for approval before changing them. Keep button labels, menus, tooltips, and History UI sentinels out of scope.

## Boundaries & Constraints

**Always:**
- Status/dialog copy shown in footer, confirmation bodies/titles, and progress dialogs must use ASCII-safe punctuation (hyphen `-`, ellipsis `...`, colon) consistent with existing footer guidance strings.
- Compare locked footer must read: `Compare mode - editing and patch/bank changes are locked. Click COMPARE again to exit.`
- After Compare fix, list remaining non-ASCII status/dialog hits and wait for explicit approval before editing them.

**Ask First:**
- Changing any status/dialog string other than `kCompareLockedFooter` (known candidate: `FooterPanel::kMidiQueuePressureMessage`).
- Expanding scope to button labels, tooltips, menus, or History sentinels (`kHistoryRootSentinel`).

**Never:**
- Rewrite product wording beyond punctuation/encoding safety.
- Change History combobox sentinel glyphs or Mutator button faces.
- Introduce new localization or message systems.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Enter Compare | User toggles COMPARE on | Footer shows ASCII Compare lock message (no mojibake) | N/A |
| Device lock clears while Compare on | Compare active; device lock footer cleared | Footer restored to same ASCII Compare message | N/A |
| MIDI queue pressure (pre-approval) | Outbound queue backed up | Still uses current string until approved | Do not change without Ask First |

</frozen-after-approval>

## Code Map

- `Source/Shared/Definitions/PluginDisplayNames.h` -- `PatchMutatorModule::Messages::kCompareLockedFooter` (fix); `FooterPanel::kMidiQueuePressureMessage` (Ask First); other status/dialog constants already ASCII
- `Source/Core/Services/PatchMutator/PatchMutatorEngineActions.cpp` -- sets Compare footer via `kCompareLockedFooter`
- `Source/GUI/Helpers/CompareLockBinder.cpp` -- re-applies Compare footer after device-lock clear
- `Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp` -- paints `uiMessageText`; already truncates with ASCII `...`
- `Source/GUI/Helpers/TextFitHelpers.h` -- prior ASCII ellipsis pattern (`fitWithAsciiEllipsis`)
- `Source/Core/Services/PatchMutator/PatchMutatorEngineInternal.h` -- additional mutator footer messages (ASCII today)

## Tasks & Acceptance

**Execution:**
- [x] `Source/Shared/Definitions/PluginDisplayNames.h` -- replace `\xe2\x80\x94` in `kCompareLockedFooter` with ASCII ` - ` -- stop Compare mojibake
- [x] Chat / review handoff -- present exhaustive status/dialog inventory and flag `kMidiQueuePressureMessage` (literal U+2014) for approval -- honor Ask First before further edits
- [x] Grep verification -- confirm no remaining `\xe2\x80\x94` / U+2014 inside approved status/dialog constants after any approved follow-up fixes

**Acceptance Criteria:**
- Given Compare mode is entered, when the footer status message is shown, then the text uses ASCII hyphen punctuation and does not display `a€` or other em-dash mojibake.
- Given Compare is still active after a device-lock footer is cleared, when the Compare footer is restored, then it shows the same ASCII Compare message.
- Given the audit of status/dialog copy, when non-Compare non-ASCII strings are found, then they are reported for approval and not changed until Guillaume confirms.

## Spec Change Log

## Design Notes

Prior pass already forced ASCII ellipsis in footer truncation (`TextFitHelpers`, `FooterPanel`). Most guidance strings already use ` - `. The Compare string uniquely used C-escaped UTF-8 em dash; MIDI queue pressure uses a raw UTF-8 em dash in source.

**Approved Compare target:**
```text
Compare mode - editing and patch/bank changes are locked. Click COMPARE again to exit.
```

**Known Ask-First candidate (not in auto-fix set):**
```text
Send queue backed up — click PANIC to clear stuck notes.
→ proposed: Send queue backed up - click PANIC to clear stuck notes.
```

## Verification

**Commands:**
- `rg -n '\\\\xe2\\\\x80\\\\x94|—' Source/Shared/Definitions/PluginDisplayNames.h Source/Core/Services/PatchMutator` -- expected: no hits in Compare message after fix; MIDI pressure only until approved
- `python3 Scripts/quality/lint_touched.py` -- expected: clean on touched C++ headers

**Manual checks (if no CLI):**
- Toggle Patch Mutator COMPARE and confirm footer text shows a normal hyphen, not `a€`.

## Suggested Review Order

- ASCII hyphen replaces UTF-8 em dash in Compare footer copy
  [`PluginDisplayNames.h:1121`](../../Source/Shared/Definitions/PluginDisplayNames.h#L1121)
