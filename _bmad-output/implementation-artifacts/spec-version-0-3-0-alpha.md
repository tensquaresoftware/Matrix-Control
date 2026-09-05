---
title: 'Bump product version to 0.3.0-alpha'
type: 'chore'
created: '2026-09-05'
status: 'done'
route: 'oneshot'
review_loop_iteration: 0
context:
  - '{project-root}/CONTRIBUTING.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Matrix-Control still reports `0.2.0-alpha` while the next milestone should ship as `0.3.0-alpha`.

**Approach:** Bump the CMake version SSOT to `0.3.0` (keep prerelease suffix `alpha`), align Luthier sidecar and the pinned PluginVersion unit expectation, then commit and push to `origin/main`.

</frozen-after-approval>

## Implementation Notes

- Bumped CMake `project(Matrix-Control VERSION …)` from `0.2.0` to `0.3.0`; left `MATRIX_CONTROL_PRERELEASE_SUFFIX` as `alpha`.
- Aligned `.luthier.json` `projectVersion` to `0.3.0`.
- Updated pinned expectation in `PluginVersionTests` to `0.3.0-alpha`.
- Did not create/push git tag `v0.3.0-alpha` (user asked commit + push of the version bump only).
- Left user manual `Version-Produit` alone (still stale at `0.1.2-alpha`; already tracked in deferred-work).
- Did not touch `.vscode/launch.json` `"version": "0.2.0"` (VS Code schema version, not product).
- Verified: reconfigure + build `Matrix-Control_Tests`; PluginVersion category `0 failure(s)`; `lint_touched.py --base HEAD` OK.
- Blind Hunter only (no other review layers configured for oneshot). Triage: process/lifecycle and oneshot-minimal-spec findings rejected as false; doc hygiene deferred.

## Review Triage Log

- Spec still `in-progress` mid-flight — **false**: Finalize sets `done` after review.
- Commit/push not yet done when reviewed — **false**: Intent steps completed after triage.
- Prefer `one-shot` spelling — **false**: workflow template uses `oneshot`.
- Missing Boundaries/AC/Code Map vs prior bump spec — **false**: oneshot route deletes those sections by design.
- User-manual Version-Produit still stale after 0.3.0 SSOT — **defer** (pre-existing docs drift; new deferred entry).
- CONTRIBUTING as sole context / unused tag guidance — **false**: harmless context pointer; tagging out of scope by user ask.
- CONTRIBUTING still exemplifies `v0.2.0-alpha` — **defer** (contributor doc example hygiene).
- Spec omits bare `0.3.0` vs display `0.3.0-alpha` Always — **false**: CMake suffix cache already encodes that contract.
- Opaque verification bullet — **low rejected**: Implementation Notes record commands; not worth expanding oneshot artifact.
- Empty CHANGELOG.md untouched — **defer** (changelog hygiene intentional gap).
