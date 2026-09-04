---
title: 'Version 0.2.0-alpha in About and restore CI patch fixtures'
type: 'chore'
created: '2026-09-04'
status: 'done'
baseline_commit: 'a79016129f5fa234b86bb263e96406a0421d4972'
review_loop_iteration: 0
context:
  - '{project-root}/CONVENTIONS.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** About still shows `0.1.2-alpha`, and GitHub Actions is red because unit tests still load `Patch 5/66/71/808.syx` from `Tests/Fixtures/Patches/User/` after those reference fixtures were deleted.

**Approach:** Bump the CMake version SSOT to `0.2.0` (keep prerelease suffix `alpha`) so About shows `0.2.0-alpha`, and restore the four deleted User reference fixtures from git history so the existing tests pass again without rewriting expectations.

## Boundaries & Constraints

**Always:**
- Version SSOT remains `project(Matrix-Control VERSION …)` in `CMakeLists.txt` plus `MATRIX_CONTROL_PRERELEASE_SUFFIX`; About must keep using `PluginVersion::getVersionString()` (no hardcoded About string).
- Display string format stays as today: `0.2.0-alpha` with **no** leading `v` (the `v` prefix is for git tags only).
- Restore exactly the four fixtures the suite names: `Patch 5.syx`, `Patch 66.syx`, `Patch 71.syx`, `Patch 808.syx` under `Tests/Fixtures/Patches/User/`, from the parent of commit `8253c7b6`.
- Align `.luthier.json` `projectVersion` to `0.2.0` for sidecar consistency.

**Ask First:**
- Creating or pushing git tag `v0.2.0-alpha` / opening a GitHub Release.
- Changing About to show a leading `v`.
- Retargeting or rewriting tests instead of restoring the four fixtures.
- Restoring additional deleted files (`WARMPAD.syx`, `flute.syx`) beyond the four Patch fixtures.

**Never:**
- Hardcoding the version string in `AboutPanel` or elsewhere outside the CMake → `PluginVersion` path.
- Rewriting dozens of Patch Manager / PatchModel tests to point at Factory/Web fixtures as a shortcut.
- Changing release workflow / `validate-tag` logic in this change.
- Re-adding `BADBOY.syx` (unrelated to the failing suite).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| About version | Plugin built with VERSION `0.2.0` and suffix `alpha` | About Version row shows `0.2.0-alpha` | N/A |
| PluginVersion unit test | Rebuild after CMake bump | `PluginVersionTests` still passes (asserts compile-time macros) | N/A |
| PatchModel round-trip | `resolvePatchFixtureFile("Patch 5.syx")` | Fixture loads; decoded name `BNK4: 05` | Missing file must not happen after restore |
| CI Build and Test | Push/commit with restored fixtures + version bump | macOS/Windows/Linux jobs green for unit suite (no segfault from missing refs) | If another failure appears, stop and report — do not broaden scope |

</frozen-after-approval>

## Code Map

- `CMakeLists.txt:48` -- change `VERSION 0.1.2` → `0.2.0`; keep `MATRIX_CONTROL_PRERELEASE_SUFFIX "alpha"` at lines 51–52
- `CMakeLists.txt:642,792` -- READ-ONLY compile defines wiring `MATRIX_CONTROL_PROJECT_VERSION` / suffix
- `Source/Shared/Definitions/PluginVersion.cpp:25-33` -- READ-ONLY `getVersionString()` = version + optional `-{suffix}`
- `Source/GUI/About/AboutPanel.cpp:99` -- READ-ONLY About value from `PluginVersion::getVersionString()`
- `Tests/Unit/PluginVersionTests.cpp:27-37` -- READ-ONLY; follows CMake macros after reconfigure
- `.luthier.json:4` -- update `projectVersion` to `"0.2.0"`
- `Tests/Unit/PatchModelTests.cpp:41-57` -- READ-ONLY expectations (`BNK4: 05`, etc.); needs fixtures present
- `Tests/Unit/PatchFixturePaths.h:13-25` -- READ-ONLY resolver `Patches/User/` then legacy root
- `Tests/Fixtures/Patches/User/Patch {5,66,71,808}.syx` -- restore from `8253c7b6^` (deleted in `8253c7b6`)
- Related consumers (READ-ONLY unless restore incomplete): `PatchFileServiceTests`, `SysExParserTests`, `PatchNameSyncerTests`, `ApvtsPatchMapperTests`, PatchManager action-handler suites that copy User `Patch *.syx`

## Tasks & Acceptance

**Execution:**
- [x] `CMakeLists.txt` -- set `project(Matrix-Control VERSION 0.2.0)`; leave suffix `alpha` -- About SSOT bump
- [x] `.luthier.json` -- set `projectVersion` to `0.2.0` -- keep Luthier metadata aligned
- [x] `Tests/Fixtures/Patches/User/Patch 5.syx` (+ `66`, `71`, `808`) -- restore blobs from `8253c7b6^` via git checkout of those paths only -- unblock CI without rewriting tests
- [x] Verify locally -- run `PluginVersionTests` and `PatchModel` fixture-dependent unit tests (or full `Matrix-Control_Tests` if practical) after reconfigure/build

**Acceptance Criteria:**
- Given a Debug rebuild after the CMake bump, when About opens, then the Version row shows `0.2.0-alpha` (no leading `v`).
- Given the four User Patch fixtures are restored, when `PatchModel` round-trip tests run, then fixtures load from disk and expected names match (`BNK4: 05`, and the other three reference names).
- Given the same commit is what CI would build, when unit tests run, then they no longer fail with missing `Patch 5.syx` / segfault from empty reference buffers.
- Given `.luthier.json` is opened, when reading `projectVersion`, then it equals `0.2.0`.

## Spec Change Log

## Verification

**Commands:**
- `cmake --preset macos-debug-arm64 && cmake --build --preset macos-debug-arm64 --target Matrix-Control_Tests` -- expected: build succeeds
- Run unit test binary (path from preset artefacts) filtering or full suite covering PluginVersion + PatchModel -- expected: pass; no missing-fixture failures
- `git status` -- expected: CMakeLists, `.luthier.json`, and four restored `.syx` files only (plus this spec if tracked)

**Manual checks (if no CLI):**
- Open About in Standalone after rebuild — Version reads `0.2.0-alpha`

## Suggested Review Order

**Version SSOT**

- Bump project version so About shows `0.2.0-alpha` via PluginVersion
  [`CMakeLists.txt:48`](../../CMakeLists.txt#L48)

- Keep Luthier sidecar aligned with CMake VERSION
  [`.luthier.json:4`](../../.luthier.json#L4)

**CI fixtures**

- Restore the four User Patch fixtures tests still resolve by name
  [`Patch 5.syx`](../../Tests/Fixtures/Patches/User/Patch%205.syx)

**Verification pin**

- Assert exact display string so a missed CMake bump fails the suite
  [`PluginVersionTests.cpp:40`](../../Tests/Unit/PluginVersionTests.cpp#L40)
