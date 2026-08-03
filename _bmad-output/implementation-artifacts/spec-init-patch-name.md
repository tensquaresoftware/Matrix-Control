---
title: 'INIT Patch Name'
type: 'feature'
created: '2026-08-03'
status: 'done'
baseline_commit: '1d7d214240ce17c7ffba5351442f2a174435bff0'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-startup-patch-name-default.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** After Internal Patches INIT, Patch Name shows `--------` (empty-name UI fallback), same as cold start / session idle — so an initialized patch looks like “no identity.”

**Approach:** When full-patch INIT finishes and the loaded template name is blank (empty after trim), assign the patch name `INIT` into the model and APVTS. Keep startup / session-idle `--------`. Preserve any non-blank name already present in a custom `PatchInit.syx`.

## Boundaries & Constraints

**Always:**
- Apply only on full-patch INIT (`handleInternalPatchInit` path), after template load / hardcoded fallback into `PatchModel`.
- Treat blank as `PatchModel::getName().isEmpty()` (space-padded name bytes count as blank).
- Use a display-names SSOT constant for the assigned string `"INIT"` (alongside `kDefaultPatchName`, not the Mutator secondary label `INITIAL`).
- Keep existing push-to-APVTS / audition / dirty-snapshot flow after the name assignment.
- Bank export uniqueness stays slot-prefixed (`Pxx - INIT.syx`); duplicate `INIT` across slots is acceptable.

**Ask First:**
- Treating a literal template name `--------` as blank (default: do **not** — only empty/trim-empty).
- Baking `INIT` into `PatchInit.syx` / `InitDefaults` (default: **do not** — policy stays in the INIT handler so blank custom templates still get `INIT` without rewriting fixtures).

**Never:**
- Do not change session-startup / restore policy that forces `kDefaultPatchName` (`--------`).
- Do not rename Mutator HISTORY / Compare secondary label `INITIAL`.
- Do not invent per-slot unique names (`INIT 03`, etc.).
- Do not force `INIT` over a usable non-blank template name.
- Do not change module-level INIT (DCO / ENV / LFO slices) name behavior.
- Do not use characters outside the Matrix name charset (`<INIT>` forbidden).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Default INIT | Fixture / InitDefaults blank name | Model + `kPatchName` = `INIT`; UI shows `INIT` | N/A |
| Named template | Custom `PatchInit.syx` with e.g. `MYINIT` | Name stays `MYINIT` | N/A |
| Blank custom template | Custom file with spaces / empty name | Name becomes `INIT` | N/A |
| Cold start / session restore | No INIT this session | Still `--------` | N/A |
| Multi-slot STORE then bank EXPORT | Several RAM slots left as `INIT` | Files `Pxx - INIT.syx` (no stem collision) | N/A |
| Literal `--------` in template | Name bytes decode to `--------` | Keep `--------` (not treated as blank) | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` -- `handleInternalPatchInit`: after `initFullPatch()`, if name empty → `setName(INIT)` before APVTS push
- `Source/Shared/Definitions/PluginDisplayNames.h` -- SSOT constant for assigned INIT name near `kDefaultPatchName`
- `Source/Core/Models/PatchModel.{h,cpp}` -- `getName` / `setName` (read-only usage; charset 8 chars)
- `Source/Core/Models/PatchNameSyncer.cpp` -- existing `bufferToApvts` after assignment
- `Source/GUI/.../PatchNameDisplayPanel.cpp` -- empty → `--------` fallback unchanged; non-empty `INIT` displays as-is
- `Tests/Unit/PatchManagerActionHandlerBasicTests.cpp` -- assert name / `kPatchName` after INIT
- `Tests/Unit/PatchManagerActionHandlerGateTests.cpp` -- only if INIT assertions assume blank/`--------`

## Tasks & Acceptance

**Execution:**
- [x] `Source/Shared/Definitions/PluginDisplayNames.h` -- add SSOT `kInitPatchName` (or equivalent) = `"INIT"` next to Patch Name defaults -- single source for assigned name
- [x] `Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp` -- after successful full-patch init load, if `getName().isEmpty()` then `setName` from SSOT before `pushPatchModelToApvtsWithSuppress` -- product policy at INIT boundary
- [x] `Tests/Unit/PatchManagerActionHandlerBasicTests.cpp` (+ gate tests if needed) -- cover I/O matrix: blank → `INIT`; non-blank template preserved; no change to unrelated gates -- regression

**Acceptance Criteria:**
- Given a blank init template (default fixture or hardcoded fallback), when the user runs Internal Patches INIT, then Patch Name display and `kPatchName` are `INIT`.
- Given a custom init template with a non-blank name, when INIT runs, then that name is unchanged.
- Given session restore / cold start without INIT, when the UI shows Patch Name, then it remains `--------`.
- Given multiple RAM slots stored as `INIT` and bank EXPORT, when files are written, then stems remain unique via `Pxx` prefix.

## Spec Change Log

## Design Notes

Blank template name is intentional today (`GenerateInitFixtures` / eight `0x20` bytes). Prefer post-load assignment over rebaking fixtures so custom blank templates still get `INIT`, and startup idle stays a separate concept (`kDefaultPatchName`).

`INIT` (patch identity) ≠ button label reuse requirement, but the string may match `kInitPatch`; keep a dedicated Patch Name constant to avoid coupling UI chrome to payload policy. Mutator secondary `INITIAL` stays unrelated.

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` -- build succeeds
- Run `PatchManagerActionHandlerBasicTests` (and gate INIT cases if touched) -- new/updated assertions green

**Manual checks:**
- INIT on blank template → Patch Name `INIT`; restart plugin/session → `--------` again until next INIT
- Optional: STORE two slots as `INIT`, EXPORT bank → `Pxx - INIT.syx` files side by side

## Suggested Review Order

**Policy at INIT**

- Assign `INIT` only when the loaded name is blank, before APVTS push.
  [`PatchManagerActionHandlerInternalPatches.cpp:274`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L274)

- Helper isolates blank-check + SSOT assignment for lint complexity.
  [`PatchManagerActionHandlerInternalPatches.cpp:23`](../../Source/Core/Actions/PatchManagerActionHandlerInternalPatches.cpp#L23)

**Name SSOT**

- Dedicated `kInitPatchName`, kept separate from idle `--------` and Mutator `INITIAL`.
  [`PluginDisplayNames.h:879`](../../Source/Shared/Definitions/PluginDisplayNames.h#L879)

**Tests**

- Blank fallback, blank custom file, named preserve, literal `--------` keep.
  [`PatchManagerActionHandlerBasicTests.cpp:155`](../../Tests/Unit/PatchManagerActionHandlerBasicTests.cpp#L155)

- Duplicate `INIT` export stems stay unique via slot prefix.
  [`PatchFileNameSanitizerTests.cpp:104`](../../Tests/Unit/PatchFileNameSanitizerTests.cpp#L104)
