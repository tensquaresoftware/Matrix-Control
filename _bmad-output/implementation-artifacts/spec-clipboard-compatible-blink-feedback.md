---
title: 'Clipboard compatible blink feedback'
type: 'feature'
created: '2026-07-25'
status: 'done'
baseline_commit: 'e7464cbbb8b6b74a0d6ef4216949bbce68c7ed67'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-compare-copy-button-red-active-text.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** After a type-aware Copy, users cannot see which Paste targets belong with the source; Compare already teaches a red blink language, but C/P do not reuse it.

**Approach:** While a clipboard feedback session is active, blink the source Copy and its relevant Paste targets in red at Compare’s 2 Hz rate, alternating like turn signals (Copy lit ↔ Paste lit). Escape clears the clipboard and stops feedback; successful Paste or a new Copy stops feedback without emptying the clipboard (new Copy restarts feedback).

## Boundaries & Constraints

**Always:**
- Blink cadence and look match Compare: `startTimerHz(2)`, alpha `1.0f` ↔ `0.35f`, lit phase uses Button On / red text (`ColourChart::kRed`).
- Alternation: source Copy on one half-period; relevant Paste target(s) on the opposite half-period (all Paste targets that blink share the same phase).
- Module mode (DCO / ENV / LFO): blink source **C** + **P** on every *other* compatible module (`areModulesCompatible`); do **not** blink the source module’s own **P**.
- MatrixModulation / FullPatch (Internal Patches): on the patch where Copy happened, only **C** / **COPY** blinks. After a different patch is loaded into the editor, **C**/**COPY** and **P**/**PASTE** alternate — PASTE only when that button is actually enabled (Internal: clipboard FullPatch **and** RAM bank).
- Feedback session is separate from `*PasteEnabled` (paste keeps clipboard content; enablement can stay true while blink stops).
- Escape: clear clipboard + disarm feedback + refresh paste mirrors. Successful paste: disarm feedback only. New Copy: disarm previous session, arm new one.

**Ask First:**
- Changing Compare’s own blink rate or alphas.
- Blinking MASTER or Init-only modules (they have no C/P today).

**Never:**
- Invent C/P on VCF/VCA, FM/TRACK, RAMP/PORTAMENTO, or MASTER (Init-only).
- Drive blink solely from `*PasteEnabled` (lifetime differs after paste).
- French UI strings; change compatibility matrix rules; change ROM paste gating logic beyond consuming existing enablement.

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| DCO1 Copy | Module clipboard = DCO1 | DCO1 **C** blinks; DCO2 **P** blinks opposite phase; DCO1 **P** does not blink | N/A |
| ENV1 Copy | Module clipboard = ENV1 | ENV1 **C** blinks; ENV2+ENV3 **P** opposite phase together | N/A |
| LFO1 Copy | Module clipboard = LFO1 | LFO1 **C** blinks; LFO2 **P** opposite phase | N/A |
| Matrix Copy same patch | MatrixModulation clipboard, no later patch load | Matrix **C** blinks alone | N/A |
| Matrix after other patch load | Same clipboard, editor loaded a different patch | Matrix **C** and **P** alternate | N/A |
| Internal COPY same patch | FullPatch clipboard, no later patch load | Internal **COPY** blinks alone | N/A |
| Internal after other patch + RAM | FullPatch clipboard, other patch loaded, bank RAM | **COPY** and **PASTE** alternate | N/A |
| Internal after other patch + ROM | FullPatch clipboard, other patch loaded, bank ROM | **COPY** blinks; **PASTE** stays gray / no blink | Existing ROM gate |
| Escape | Feedback active | Blink stops; buttons opaque Off; clipboard empty; paste mirrors false | N/A |
| Successful Paste | Feedback active | Blink stops; clipboard content kept; paste mirrors unchanged | N/A |
| New Copy | Feedback active | Previous blink stops; new source/targets arm | N/A |
| Empty clipboard | No content | No feedback blink | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/Services/ClipboardService.{h,cpp}` -- add `clear()`; expose mode/source for feedback; optional copy-generation / origin token for cross-patch Matrix/FullPatch phase
- `Source/Core/PluginProcessor.cpp` -- publish ephemeral APVTS feedback mirrors; call `clear` + refresh on Escape path; arm/disarm around copy/paste
- `Source/Core/Actions/ModuleActionHandler.cpp` -- arm feedback after module/MM/full-patch copy; disarm after successful module/MM paste
- `Source/Core/Actions/PatchManagerActionHandler.cpp` -- disarm after successful Internal paste; signal patch-load for Matrix/FullPatch “other patch” phase
- `Source/GUI/PluginEditor.cpp` -- `keyPressed` Escape → clear clipboard feedback (when editor focused; ignore when modal dialogs own Escape)
- `Source/GUI/Widgets/ModuleHeader.{h,cpp}` -- apply feedback blink to C/P (alpha + toggle/red) from APVTS mirrors
- `Source/GUI/Panels/.../MatrixModulationPanel.{h,cpp}` -- same for Matrix C/P
- `Source/GUI/Panels/.../InternalPatchesPanel.{h,cpp}` -- same for COPY/PASTE (respect existing enablement)
- `Source/GUI/Panels/.../PatchMutatorPanel.cpp` -- reference only: `startTimerHz(2)`, alpha 1.0/0.35, `setToggleState` for red On text
- `Source/GUI/Helpers/` -- prefer a small shared blink helper/coordinator reused by ModuleHeader / Matrix / Internal rather than three divergent timers
- `Tests/Unit/ClipboardServiceTests.cpp` -- `clear()` + mode Empty; feedback arm/disarm / origin-token behaviour if Core-owned

## Tasks & Acceptance

**Execution:**
- [x] `Source/Core/Services/ClipboardService.{h,cpp}` -- add `clear()` returning to Empty; track whatever Core needs for feedback origin / cross-patch eligibility -- clipboard SSOT
- [x] `Source/Core/PluginProcessor.cpp` (+ IDs if needed) -- ephemeral feedback properties (active, phase owner ids or bitflags, cross-patch-ready); refresh with paste mirrors; Escape entry -- Core→GUI contract
- [x] `Source/Core/Actions/ModuleActionHandler.cpp` / `PatchManagerActionHandler.cpp` -- arm on copy; disarm on successful paste; mark cross-patch-ready on patch load while Matrix/FullPatch clipboard held -- session lifecycle
- [x] `Source/GUI/Helpers/` + `ModuleHeader` / `MatrixModulationPanel` / `InternalPatchesPanel` -- 2 Hz alternating blink on source Copy vs relevant Paste buttons; stop restores opaque Off -- visual UX
- [x] `Source/GUI/PluginEditor.cpp` -- Escape clears clipboard + feedback when no higher-priority Escape consumer -- cancel path
- [x] `Tests/Unit/` -- cover `clear()`, feedback disarm rules that live in Core, and cross-patch-ready transition -- I/O matrix edges

**Acceptance Criteria:**
- Given a DCO1 Copy, when feedback is active, then DCO1 C and DCO2 P blink at 2 Hz in opposite phases with red lit text, and DCO1 P does not blink.
- Given an ENV1 Copy, when feedback is active, then ENV1 C blinks opposite ENV2 and ENV3 P (those two P in sync).
- Given Matrix Mod Copy on the source patch, when no other patch has been loaded yet, then only Matrix C blinks.
- Given Matrix Mod clipboard, when a different patch is loaded, then Matrix C and P alternate.
- Given Internal COPY then another patch on a ROM bank, when feedback is active, then COPY may blink but PASTE does not (stays disabled).
- Given active feedback, when Escape is pressed in the editor, then blink stops and the clipboard is empty.
- Given active feedback, when Paste succeeds, then blink stops and clipboard content remains pasteable.
- Given active feedback, when the user Copies again, then feedback restarts for the new source/targets.

## Spec Change Log

## Design Notes

Do **not** key blink lifetime off `*PasteEnabled` alone: paste keeps content, so mirrors stay true while UX must stop pulsing. Prefer a dedicated feedback session flag (+ source identity + “cross-patch ready” for Matrix/FullPatch).

Alternation = one shared 2 Hz timer with inverted alpha for Copy vs Paste roles (Compare uses a single button; here two roles share the period out of phase).

Module self-paste remains allowed by Core; blink simply omits the source module’s own P so the eye reads “here → there”.

Patch-load detection for Matrix/FullPatch should hook the existing load paths (Internal / computer / synth dump / etc.) that already clear or replace patch content — set cross-patch-ready when the loaded patch is not the copy-origin session.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-ControlTests` -- unit tests for clear + Core feedback rules pass
- Run `Matrix-ControlTests` filtering `Clipboard` -- expected green

**Manual checks:**
- Black + cream: DCO1→DCO2, ENV1→ENV2/3, LFO1→LFO2 alternating red blink
- Matrix C alone, then load another patch → C↔P; Internal COPY alone, then other patch RAM → COPY↔PASTE; ROM → PASTE gray
- Escape clears; Paste stops blink but second Paste still works; new Copy retargets blink

## Suggested Review Order

**Feedback roles (who blinks)**

- Pure mapping from clipboard mode + session flags to C/P roles
  [`ClipboardFeedbackResolver.cpp:47`](../../Source/Core/Services/ClipboardFeedbackResolver.cpp#L47)

**Session lifecycle**

- Arm on copy; remember patch-load origin for Matrix/FullPatch
  [`PluginProcessor.cpp:2107`](../../Source/Core/PluginProcessor.cpp#L2107)

- Unlock Paste blink only when loaded context differs from copy origin
  [`PluginProcessor.cpp:2124`](../../Source/Core/PluginProcessor.cpp#L2124)

- Escape clears clipboard only when there is something to cancel
  [`PluginProcessor.cpp:2151`](../../Source/Core/PluginProcessor.cpp#L2151)

- Empty clipboard SSOT used by Escape
  [`ClipboardService.cpp:219`](../../Source/Core/Services/ClipboardService.cpp#L219)

**Visual blink**

- 2 Hz phase driver shared via APVTS `kCopyLit`
  [`PluginEditor.cpp:72`](../../Source/GUI/PluginEditor.cpp#L72)

- Compare-style red On + alternating alpha on one button
  [`ClipboardFeedbackButtonBinding.cpp:48`](../../Source/GUI/Helpers/ClipboardFeedbackButtonBinding.cpp#L48)

- Module C/P bindings
  [`ModuleHeader.cpp:300`](../../Source/GUI/Widgets/ModuleHeader.cpp#L300)

- Matrix and Internal bindings
  [`MatrixModulationPanel.cpp:348`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/MatrixModulationPanel/MatrixModulationPanel.cpp#L348)

**Cancel path**

- Editor Escape → clear when active/content present
  [`PluginEditor.cpp:698`](../../Source/GUI/PluginEditor.cpp#L698)

**Tests**

- Resolver truth table including LFO and cross-patch flags
  [`ClipboardFeedbackResolverTests.cpp:1`](../../Tests/Unit/ClipboardFeedbackResolverTests.cpp#L1)
