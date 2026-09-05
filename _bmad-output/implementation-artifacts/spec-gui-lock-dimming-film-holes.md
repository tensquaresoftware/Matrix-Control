---
title: 'GUI lock dimming film with holes'
type: 'feature'
created: '2026-09-05'
status: 'done'
baseline_commit: '932f53fcafb5ec9b0f2a6066acf8358c95352b6b'
review_loop_iteration: 1
context:
  - '{project-root}/_bmad-output/implementation-artifacts/8-3-ui-lock-without-synth.md'
  - '{project-root}/_bmad-output/project-context.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiate">

## Intent

**Problem:** Section lock (no supported synth / device lock, and/or Compare) dims the UI unevenly: Patch Edit and Master Edit look darker than SharedPanel, and parent `setAlpha` cannot keep COMPARE at full contrast under a dimmed ancestor.

**Approach:** Replace section-level `setAlpha` dimming with a MainComponent-owned **dimming film** that paints a uniform overlay and punches **holes** (no paint + click-through) for Header, Footer, and — when Compare is active with a supported device — the COMPARE button only. One named constant sets film intensity.

## Boundaries & Constraints

**Always:**
- Film covers full MainComponent bounds when active; paints dimming (not parent `setAlpha` as the primary mechanism); blocks clicks outside holes; passes clicks inside holes.
- Hole sets: **device lock** (and existing equivalents: unsupported Matrix member, MIDI unresponsive) → Header + Footer only. **Compare + device OK** → Header + Footer + COMPARE button rect. Holes track layout (resize / UI scale).
- Preserve `Core::isSectionLocked` composition and footer device/Compare messages (exact-string clear rules from 8-3).
- One named film intensity constant (working default **0.75f**; historical binder alpha was 0.5f — different mechanism, calibrate visually via the constant only). Keep `GrayedControlHelper` local gray separate.
- PluginEditor modals / FileChoosers stay above the film (film inside MainComponent; editor overlays remain siblings of MainComponent).
- English-only source; no Core→GUI; commits only on request.

**Ask First:**
- Shipping a default intensity other than 0.75f without visual check.
- Dimming Header/Footer regions (future hole sets — out of this ship unless needed).

**Never:**
- Dimmed-but-clickable COMPARE.
- Section `setAlpha` / child-by-child opacity as the target dimming model.
- Double dimming (film + leftover section `setAlpha`).
- UI redesign beyond this lock film; docs outside Build artifacts.
- Reinstating obsolete binder-root SharedPanel opacity plan (`spec-uniform-gui-section-lock-opacity`).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Device lock | No synth / unsupported / MIDI unresponsive | Uniform body dim; Header+Footer holes; COMPARE covered | N/A |
| Compare + device OK | compareActive, supported device | Uniform body dim incl. rest of Mutator; holes Header+Footer+COMPARE (full contrast, clickable) | N/A |
| Device lock + Compare | Both true | Device hole set wins (no COMPARE hole); footer device copy | N/A |
| Resize / UI scale | Window or scale change while film on | Hole rects match Header/Footer/COMPARE | N/A |
| Modal over film | Settings/About/confirm/bank progress or FileChooser | Overlay usable above film; native chooser via existing raise-before-modal | N/A |
| Unlock | Device OK, Compare off | Film hidden; full interaction; no leftover alpha | N/A |

</frozen-after-approval>

## Code Map

- `Source/GUI/MainComponent.{h,cpp}` — owns `LockDimmingFilm` (idempotent `attachLockDimmingFilm` from `PluginEditor::createUiShell`); `resized`/`setUiScale` refresh hole geometry; APVTS listener drives mode.
- `Source/GUI/Helpers/LockDimmingFilm.{h,cpp}` — paint + `hitTest` via `lockDimmingFilmBlocksPoint`; intensity `kFilmIntensity = 0.75f`.
- `Source/GUI/Helpers/LockDimmingFilmPolicy.h` — pure mode/hole/hit helpers (`resolveLockDimmingFilmMode`, device lock wins over Compare).
- `Source/GUI/Helpers/CompareLockBinder.{h,cpp}` — footer sync + focus steal only (no section `setAlpha` / mouse intercepts).
- Binder sites (footer/focus only): `BodyPanel.cpp`, `SharedPanel.cpp`, `PatchManagerPanel.cpp` (Mutator `lockOnCompare=false`).
- `Source/Core/MIDI/EditorOutboundGate.h` — `isSectionLocked` (read-only).
- `PatchMutatorPanel::getCompareButtonBounds()` (+ Body/Shared/PatchManager accessors) — COMPARE hole geometry.
- `Source/GUI/Helpers/GrayedControlHelper.*` — local gray + footer setters (separate from film).
- `Tests/Unit/LockDimmingFilmPolicyTests.cpp` — I/O matrix policy + hit-test helper.
- `CMakeLists.txt` — `LockDimmingFilm.cpp` + policy tests registered.
- `8-3-ui-lock-without-synth.md` — composition, Mutator exception, footer exact-string clear.

**Intentional product delta vs today:** under Compare + device OK, the whole Patch Mutator was undimmed; this ship dims Mutator except the COMPARE hole (per approved intent).

## Tasks & Acceptance

**Execution:**
- [x] Add reusable GUI dimming-film component (paint + hole hit-test + intensity constant default 0.75f) under `Source/GUI/` — uniform overlay mechanism.
- [x] Wire film on `MainComponent` (APVTS-driven visibility / hole policy; refresh holes on resize/UI scale; Header/Footer/COMPARE sources) — shell ownership + layout tracking.
- [x] Expose COMPARE button bounds from Patch Mutator (minimal accessor) — hole geometry.
- [x] Migrate `CompareLockBinder`: remove section `setAlpha` / section mouse-intercept dimming; keep footer sync; avoid binder fights / double dimming; delete dead `kLockedAlpha` if unused — clean cutover.
- [x] Register new sources in `CMakeLists.txt`; build + `python3 Scripts/quality/lint_touched.py` on touched C++ — DoD.
- [x] Extract hole-policy helper + unit tests for I/O matrix rows — testable pure policy.

**Acceptance Criteria:**
- Given device lock, when UI is locked, then Patch Edit, SharedPanel (incl. background), and Master Edit share one uniform dim; Header and Footer stay clear and usable; COMPARE is not a hole.
- Given Compare + supported device, when Compare is active, then body dim is uniform, COMPARE is full-contrast and clickable, and Header/Footer stay clear.
- Given either lock state, when intensity constant changes, then only that constant recalibrates film strength.
- Given film active, when clicking outside holes, then clicks are blocked; inside holes, controls respond; no film+section double dim.
- Given resize or UI scale with film on, when layout updates, then holes stay aligned.
- Given lock transitions, when footer device/Compare messages apply, then 8-3 behavior is preserved.
- Given PluginEditor modal or FileChooser, when shown over a locked UI, then it remains usable above the film.

### Review Findings

- [x] [Review][Decision] Ship film intensity 0.75f vs frozen default 0.3f — resolved: keep 0.75f; patch updates frozen Always / Design Notes / Code Map / tasks to match (daylight recalibration still allowed via the constant).
- [x] [Review][Patch] Align spec intensity default to 0.75f [spec-gui-lock-dimming-film-holes.md] — renegotiated: frozen/working default, Ask First, Code Map, Design Notes, and execution task now say 0.75f.
- [x] [Review][Patch] Fix stale attachLockDimmingFilm constructor claim [Source/GUI/MainComponent.h:45] — comment now points at PluginEditor::createUiShell.
- [x] [Review][Patch] Avoid stealing keyboard focus from holes on every film refresh [Source/GUI/MainComponent.cpp:240] — `giveAwayKeyboardFocus` only when the film transitions from off to on.
- [x] [Review][Defer] Shell film attach / hole assembly / component hitTest lack automated coverage beyond policy helpers [Tests/Unit/LockDimmingFilmPolicyTests.cpp] — deferred, pre-existing verification gap (policy tests satisfy execution AC; GUI wiring stays manual-smoke)

## Design Notes

**Film hit model:** opaque to mouse outside holes (`hitTest` false inside hole rects so events reach components below). Prefer painting a semi-transparent fill with geometric holes over `setAlpha` on columns.

**Migration (simplest safe):** introduce film first; strip binder visual/hit dimming in the same change so both never stack; leave binders for APVTS listen + footer until a later cleanup if desired. Do not invent a second footer API.

**Extensibility:** hole list/geometry is data the shell refreshes — future Header/Footer partial dim = different hole set, same film.

**Default intensity:** use **0.75f** (smoke-validated; daylight recalibration still via the constant only). Former binder `0.5f` is not pixel-equivalent to an overlay.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` -- expected: success
- `python3 Scripts/quality/lint_touched.py` on touched `Source/**/*.cpp|h` -- expected: clean

**Manual checks:**
- No synth: uniform body dim; Header/Footer clear; no bright middle column.
- Compare + device: same uniformity; COMPARE full contrast + clickable; rest of Mutator dimmed.
- Device lock + Compare together: no COMPARE hole; footer device copy wins.
- Resize / UI scale: holes track.
- Open Settings/About while locked: modal usable.

## Suggested Review Order

**Film mechanism**

- Single intensity constant and overlay API.
  [`LockDimmingFilm.h:16`](../../Source/GUI/Helpers/LockDimmingFilm.h#L16)

- Geometric holes in paint + click-through hit-test.
  [`LockDimmingFilm.cpp:19`](../../Source/GUI/Helpers/LockDimmingFilm.cpp#L19)

**Hole policy**

- Device lock wins over Compare; pure mode resolution.
  [`LockDimmingFilmPolicy.h:24`](../../Source/GUI/Helpers/LockDimmingFilmPolicy.h#L24)

**Shell wiring**

- Attach film once from editor shell construction.
  [`PluginEditorUiConstruction.cpp:37`](../../Source/GUI/PluginEditorUiConstruction.cpp#L37)

- Refresh visibility, holes, focus steal, COMPARE retry.
  [`MainComponent.cpp:198`](../../Source/GUI/MainComponent.cpp#L198)

- Map Header/Footer/COMPARE rects into film space.
  [`MainComponent.cpp:180`](../../Source/GUI/MainComponent.cpp#L180)

**Migration**

- Binders keep footer + focus only (no section alpha).
  [`CompareLockBinder.cpp:20`](../../Source/GUI/Helpers/CompareLockBinder.cpp#L20)

- COMPARE bounds accessor for the hole.
  [`PatchMutatorPanel.cpp:138`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/PatchMutatorPanel.cpp#L138)

**Tests**

- I/O matrix policy rows + hit-test helper coverage.
  [`LockDimmingFilmPolicyTests.cpp:11`](../../Tests/Unit/LockDimmingFilmPolicyTests.cpp#L11)

