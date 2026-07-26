---
title: 'Clipboard envelope shape copy and footer INFO'
type: 'feature'
created: '2026-07-26'
status: 'done'
baseline_commit: 'd5d785300f7ffb3b79629c350aec1a545a7997dd'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-clipboard-compatible-blink-feedback.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Envelope C/P always copies the full ENV module (including amplitude, trigger, and mode), so users cannot transfer only the DADSR shape. Clipboard operations are also silent in the footer, and a unused SUCCESS severity still exists beside INFO.

**Approach:** ALT+C on ENV 1/2/3 copies only Delay/Attack/Decay/Sustain/Release; paste applies that shape-only payload. Every clipboard copy and paste publishes an English footer message (INFO on success, WARNING/ERROR when applicable). Retire SUCCESS severity globally — successful ops use INFO only.

## Boundaries & Constraints

**Always:**
- Shape-only applies only to ENV modules; shape params are exactly Delay, Attack, Decay, Sustain, Release (leave Amplitude, Amp&lt;Velocity, Trigger Mode, Envelope Mode, LFO 1 Trigger untouched on paste).
- ALT is read at C click time and stored with the clipboard payload; P pastes whatever mode was copied (full vs shape) — no ALT required on paste.
- ALT+C on non-ENV surfaces (DCO, LFO, Matrix Modulation, Internal Patches) ignores ALT and performs a normal full copy.
- Footer messages on every successful C and every successful P; English UI strings in `PluginDisplayNames` (no French in source).
- Severity vocabulary is INFO / WARNING / ERROR only; remove SUCCESS from enums, footer parse/prefix, ExceptionPropagator, and skin colour slots.
- Existing blink feedback and paste-enable rules stay; shape-only ENV clipboard remains compatible with other ENV paste targets the same way as full ENV.

**Ask First:**
- Changing Compare blink rate/look, or inventing C/P on modules that lack them.
- Renaming existing `format*Success` helper function names (severity retirement does not require renaming those string builders).

**Never:**
- Shape-only copy for DCO/LFO/Matrix/Internal.
- Publishing `"success"` severity after this change.
- French footer strings; changing envelope↔envelope compatibility matrix beyond the shape filter.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| ENV full copy | C on ENV 1, ALT up | Clipboard = full ENV 1; INFO names ENV 1 copied | N/A |
| ENV shape copy | ALT+C on ENV 2 | Clipboard = shape-only ENV 2; INFO lists Delay…Release | N/A |
| ENV full paste | Full ENV 1 clipboard → P on ENV 3 | All ENV 3 params match ENV 1 mapping; INFO ENV 1→ENV 3 | N/A |
| ENV shape paste | Shape ENV 2 clipboard → P on ENV 1 | Only D/A/D/S/R on ENV 1 change; other ENV 1 params unchanged; INFO shape→ENV 1 | N/A |
| Non-ENV ALT+C | ALT+C on DCO 1 | Full DCO 1 copy (ALT ignored); INFO DCO 1 copied | N/A |
| Matrix / Internal C+P | Copy then paste when enabled | INFO on C naming source; INFO on P confirming paste | ROM/disabled paste stays gated (no false success) |
| Paste blocked | P clicked while paste disabled / empty | No model change; WARNING (or no-op if click never fires) | WARNING text if path can fire |
| Paste failure | Paste path returns failure after attempt | Model unchanged; ERROR | ERROR footer |
| Escape clear | Escape while clipboard held | Clipboard emptied; blink disarmed (existing); optional INFO cleared/updated — do not leave a stale “copied” claim | N/A |
| SUCCESS retired | Any former Success path | No SUCCESS badge/prefix; Info colour/path only | Stale `"success"` APVTS → treat as Info or None consistently |

</frozen-after-approval>

## Code Map

- `Source/Core/Services/ClipboardService.{h,cpp}` -- shape-only flag/mode on module clipboard; filter copy + paste for D/A/D/S/R
- `Source/Core/Actions/ModuleActionHandler.cpp` -- accept shape-only on ENV copy; publish footer on module/Matrix C+P
- `Source/Core/Actions/PatchManagerActionHandler.cpp` -- footer on Internal paste (and copy if routed here)
- `Source/GUI/Widgets/ModuleHeader.cpp` -- detect ALT at C click; signal shape-only intent to Core
- `Source/GUI/Panels/.../MatrixModulationPanel.cpp` / `InternalPatchesPanel.cpp` -- ensure C path can ignore ALT; no shape mode
- `Source/Shared/Definitions/PluginDisplayNames.h` -- clipboard footer message catalog (English)
- `Source/GUI/Panels/.../FooterPanel.{h,cpp}` -- remove Success severity path
- `Source/Shared/Exceptions/WidgetFactoryExceptions.h` / `Source/Core/Exceptions/ExceptionPropagator.h` -- drop Success
- `Source/GUI/Skins/SkinValues.h` / `Skin.cpp` / `SkinColoursWidgetsLayout.h` -- drop `kFooterMessageSuccess` / `kMessageSuccess`
- `Source/GUI/Helpers/GrayedControlHelper.{h,cpp}` -- reuse Info/Warning/Error publishers from Core or GUI as needed
- `Tests/Unit/ClipboardServiceTests.cpp` -- shape-only copy/paste isolation + full ENV regression
- `Tests/Unit/` (footer/severity or display-name tests if present) -- SUCCESS removal / clipboard message builders

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Services/ClipboardService.{h,cpp}` -- add shape-only module payload; copy/paste only D/A/D/S/R when set; expose query for message builders -- clipboard SSOT
- [x] `Source/GUI/Widgets/ModuleHeader.cpp` (+ Matrix/Internal C if needed) -- read ALT at C; ENV only forwards shape-only; others ignore ALT -- gesture
- [x] `Source/Core/Actions/ModuleActionHandler.cpp` / `PatchManagerActionHandler.cpp` -- wire shape flag into copy; publish INFO/WARNING/ERROR on C and P for all clipboard surfaces -- actions + footer
- [x] `Source/Shared/Definitions/PluginDisplayNames.h` -- English clipboard footer strings per Design Notes catalog -- UI copy SSOT
- [x] `FooterPanel` + `WidgetFactoryExceptions` + `ExceptionPropagator` + skin Success slots -- remove SUCCESS severity end-to-end -- severity cleanup
- [x] `Tests/Unit/ClipboardServiceTests.cpp` (+ message/severity tests as needed) -- cover I/O matrix shape vs full; SUCCESS gone -- verification

**Acceptance Criteria:**
- Given ENV 2 with distinct non-shape params, when ALT+C then P on ENV 1, then only Delay/Attack/Decay/Sustain/Release match ENV 2 and other ENV 1 params are unchanged.
- Given ENV full C without ALT then P on another ENV, when paste completes, then behavior matches today’s full interchange and footer INFO names source→target.
- Given ALT+C on DCO/LFO/Matrix/Internal, when copy runs, then payload is full (not shape) and footer INFO reports the normal copy.
- Given any clipboard C or successful P, when the action completes, then the footer shows an English INFO (or WARNING/ERROR on failure/blocked) and never SUCCESS.
- Given the plugin builds, when Success severity symbols are searched, then no Success enum/prefix/skin slot remains for footer messaging.

## Spec Change Log

## Design Notes

**Alt → Core:** Capture ALT in the C button click path (`ModifierKeys` or Logo-style `mouseDown`), then pass a shape-only intent into the copy action (ephemeral property or dedicated widget id). Do not rely on async polling after the click.

**Shape filter:** Prefer filtering by the five envelope stage display names / descriptors already used in cross-ENV paste mapping — do not hardcode only ENV1 parameter IDs.

**Footer catalog (English — implement as named constants/formatters):**

| Event | Severity | Message pattern |
|-------|----------|-----------------|
| Module full copy | INFO | `{Module} copied to clipboard.` |
| ENV shape copy | INFO | `Envelope shape copied to clipboard (Delay, Attack, Decay, Sustain, Release).` |
| Module full paste | INFO | `{Source} pasted successfully to {Target}.` |
| ENV shape paste | INFO | `Envelope shape pasted to {Target} (Delay, Attack, Decay, Sustain, Release).` |
| Matrix copy | INFO | `Matrix Modulation copied to clipboard.` |
| Matrix paste | INFO | `Matrix Modulation pasted successfully.` |
| Internal copy | INFO | `Patch copied to clipboard.` |
| Internal paste | INFO | `Patch pasted successfully.` |
| Paste blocked / empty | WARNING | `Nothing to paste.` / `Paste is not available.` (pick one consistent pair) |
| Paste failed | ERROR | `Paste failed.` (or surface-specific if a real failure path exists) |

Module labels: `DCO 1`, `DCO 2`, `ENV 1`, `ENV 2`, `ENV 3`, `LFO 1`, `LFO 2` (product names).

**SUCCESS retirement:** Dead path today (publishers already use `"info"`; Success colour equals Info grey). Mechanical delete — no visual redesign.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-ControlTests` then run ClipboardService (+ related) unit tests -- expected: pass, including shape-only cases
- `rg -n 'Severity::Success|MessageSeverity::Success|kSeveritySuccessPrefix|kFooterMessageSuccess|"success"' Source/` -- expected: no live Success severity publishers/enums (allow unrelated word hits in comments/patch names only if unavoidable)

**Manual checks:**
- ALT+C ENV 1 → footer shape INFO; P on ENV 3 → shape applied + paste INFO; non-shape ENV 3 knobs unchanged
- Plain C ENV 1 → P ENV 2 → full paste + full INFO; blink feedback still works
- ALT+C DCO 1 → full copy INFO (not shape wording)
- Confirm footer badge never shows SUCCESS after a save/load/clipboard action

## Suggested Review Order

**Shape-only clipboard**

- Filter D/A/D/S/R at copy; flag only valid for ENV modules
  [`ClipboardService.cpp:192`](../../Source/Core/Services/ClipboardService.cpp#L192)

- ALT read at C click; ENV-only shape intent into APVTS
  [`ModuleHeader.cpp:288`](../../Source/GUI/Widgets/ModuleHeader.cpp#L288)

- Consume shape flag, clear always, publish copy/paste footers
  [`ModuleActionHandler.cpp:160`](../../Source/Core/Actions/ModuleActionHandler.cpp#L160)

**Footer catalog and Internal paste**

- English INFO/WARNING/ERROR clipboard strings
  [`PluginDisplayNames.h:186`](../../Source/Shared/Definitions/PluginDisplayNames.h#L186)

- Check pasteFullPatch result before success INFO
  [`PatchManagerActionHandler.cpp:345`](../../Source/Core/Actions/PatchManagerActionHandler.cpp#L345)

**SUCCESS retirement**

- Map stale `"success"` to Info; no SUCCESS badge path
  [`FooterPanel.cpp:181`](../../Source/GUI/Panels/MainComponent/FooterPanel/FooterPanel.cpp#L181)

- Strip ephemeral shape flag from host save state
  [`PluginProcessor.cpp:644`](../../Source/Core/PluginProcessor.cpp#L644)

**Tests**

- Shape paste leaves all non-shape ENV params untouched
  [`ClipboardServiceTests.cpp:183`](../../Tests/Unit/ClipboardServiceTests.cpp#L183)
