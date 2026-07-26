---
title: 'Patch Mutator Delete confirmation'
type: 'feature'
created: '2026-07-26'
status: 'done'
baseline_commit: '0c2e54ada9e81332e9769f666884495093d1f212'
review_loop_iteration: 0
context:
  - '{project-root}/_bmad-output/project-context.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-patch-mutator-flush-confirmation.md'
  - '{project-root}/_bmad-output/implementation-artifacts/spec-confirmation-modal-button-order.md'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** Clicking Delete (D) in the Patch Mutator removes the selected mutation or retry immediately, so a mis-click can destroy history the user still wanted. Power users who delete often also need a way to stop being asked every time, and to turn the guard back on later.

**Approach:** Before running Delete, show a confirmation with Cancel / Delete and a "Don't ask again" checkbox. Persist that choice as a Settings policy (same durability as unsaved-edit warning). Settings POLICIES exposes WARN ALWAYS / NEVER WARN so the user can re-enable confirmation. Skip the dialog when the policy is NEVER WARN.

## Boundaries & Constraints

**Always:**
- Gate Delete with an optional Core `std::function<bool()>` (true = proceed), same shape as Flush confirm; call it only when Delete would actually remove something (selection valid / button enabled path).
- Modal is synchronous on the message thread (fail closed / abort off-thread), so Core stays GUI-free.
- Visual LTR: Cancel left, Delete right and default (Enter = Delete; Escape / dismiss = Cancel). Match confirmation-modal button-order rules.
- English copy in `PluginDisplayNames` only (dialog title/body/buttons/checkbox + Settings label/items).
- Persist policy on APVTS Settings state (like `settingsUnsavedEditWarningPolicy`), default WARN ALWAYS; checking "Don't ask again" and confirming writes NEVER WARN.
- Settings POLICIES row mirrors unsaved-edit: label + combo WARN ALWAYS / NEVER WARN; changing to WARN ALWAYS restores prompts.
- Missing gate (headless / tests): proceed without modal (same as Flush).

**Ask First:**
- Confirming Flush/Mutate/other Mutator actions in this change.
- Replacing Flush’s native alert with the new custom dialog.
- Storing this preference outside APVTS (e.g. standalone-only PropertiesFile).
- Async-only overlay that breaks the sync bool gate contract.

**Never:**
- Put AlertWindow / dialog UI includes in Core.
- Change `deleteSelected()` cascade / selection-fallback / audition semantics — only gate before the existing delete.
- French UI strings.
- Rely on `showOrderedConfirmAlert` alone (no checkbox support).

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| Delete + Confirm | Policy WARN ALWAYS; valid selection; Confirm | `deleteSelected()` runs as today | N/A |
| Delete + Cancel | Policy WARN ALWAYS; Cancel / Escape / dismiss | No delete; history unchanged | N/A |
| Don't ask again + Confirm | Checkbox checked; Confirm | Delete runs; policy → NEVER WARN | N/A |
| Policy NEVER WARN | Settings NEVER WARN; Delete | No modal; delete runs immediately | N/A |
| Settings re-enable | User sets WARN ALWAYS after NEVER WARN | Next Delete shows modal again | N/A |
| Don't ask again + Cancel | Checkbox checked; Cancel | No delete; policy unchanged | N/A |
| Headless / no gate | Gate unset | Delete runs without modal | N/A |
| Off message thread | Gate called off message thread | Abort (no delete) | `jassert` / return false |
| Empty / no selection | Delete blocked / disabled | No modal | N/A |

</frozen-after-approval>

## Code Map

- `Source/Core/Actions/MutatorActionHandler.*` — `handleDelete`; inject `DeleteConfirmModalGate` before `deleteSelected()`
- `Source/Core/PluginProcessor.*` — `setMutatorDeleteConfirmModalGate`; init Settings policy property; clear gate on editor teardown
- `Source/GUI/PluginEditor.cpp` — wire sync modal with checkbox; skip when policy NEVER WARN; clear gate on destroy
- `Source/GUI/Dialogs/` — new delete-confirm dialog (MasterInit-style or modal AlertWindow + checkbox); not `showOrderedConfirmAlert`
- `Source/GUI/Settings/SettingsPanel.*` — POLICIES row for mutator-delete warning policy
- `Source/Shared/Definitions/PluginIDs.h` — `Settings::kMutatorDeleteWarningPolicy` + ids
- `Source/Shared/Definitions/PluginDisplayNames.h` — `Dialogs::MutatorDeleteConfirm` + Settings labels
- `Source/Core/Services/` — tiny policy helper (mirror `UnsavedEditWarningPolicy.h`) optional
- Existing Flush confirm / unsaved-edit Settings wiring — patterns to mirror (read-only)

## Tasks & Acceptance

**Execution:**
- [x] `Source/Shared/Definitions/PluginIDs.h` + `PluginDisplayNames.h` — add Settings policy ids/strings and `Dialogs::MutatorDeleteConfirm` (title, body, Cancel, Delete, Don't ask again) — English SSOT
- [x] `Source/Core/Services/` (or inline) + `PluginProcessor.*` — default WARN ALWAYS property; optional `shouldPrompt` helper — durable preference
- [x] `Source/Core/Actions/MutatorActionHandler.*` — optional `DeleteConfirmModalGate`; call in `handleDelete` before engine delete — Core stays GUI-free
- [x] `Source/GUI/Dialogs/*` + `PluginEditor.cpp` — sync Cancel|Delete modal with checkbox; on Confirm+checked write NEVER WARN; LTR + Enter/Escape rules — checkbox needs custom modal
- [x] `Source/GUI/Settings/SettingsPanel.*` + editor Settings open wiring — POLICIES combo WARN ALWAYS / NEVER WARN — reversible preference
- [x] Unit / handler tests where a MutatorActionHandler harness exists — Cancel skips delete; NEVER WARN skips gate; else manual — cover I/O matrix where cheap

**Acceptance Criteria:**
- Given history has a selectable entry and policy is WARN ALWAYS, when the user presses Delete (D), then a confirmation appears before any history removal, with Cancel left, Delete right (default), and a "Don't ask again" checkbox.
- Given that dialog, when the user chooses Delete, then delete runs with today’s cascade / fallback / audition behavior; when Cancel or Escape, then history is unchanged.
- Given the user checks "Don't ask again" and confirms Delete, when later Deletes occur in the same restored plugin state, then no confirmation appears until Settings sets WARN ALWAYS again.
- Given Settings POLICIES mutator-delete control is NEVER WARN, when Delete is pressed, then delete runs with no modal; given WARN ALWAYS, when Delete is pressed, then the modal returns.
- Given headless or unset gate, when Delete is dispatched, then behavior matches today’s direct delete (no crash).

## Design Notes

Native `showOrderedConfirmAlert` cannot host a checkbox. Keep the **sync bool gate** (Flush contract) and implement a **custom modal** that can: host the checkbox, map Confirm+checked → write NEVER WARN, and honor LTR Cancel → Delete.

Settings persistence follows unsaved-edit: APVTS `state` property (host session / plugin state), not a separate PropertiesFile. That is the project’s durable user-policy store.

Suggested copy (English, tweak only if SSOT review needs it):
- Title: `Delete mutation?`
- Body: short warning that the selected mutation or retry will be removed (root delete cascades retries).
- Checkbox: `Don't ask again`
- Settings label: `MUTATOR DEL :` with `WARN ALWAYS` / `NEVER WARN` (same item strings as UNSAVED).

## Verification

**Commands:**
- `cmake --build --preset macos-debug-arm64` — build succeeds
- Unit test binary / MutatorActionHandler or related tests if extended — Cancel / NEVER WARN cases pass

**Manual checks (if no CLI):**
- Delete with WARN ALWAYS → modal; Cancel leaves history; Delete removes as today
- Check Don't ask again → Confirm → subsequent Deletes silent; Settings → WARN ALWAYS → modal returns
- Escape / dismiss does not delete and does not change policy

## Suggested Review Order

**Delete gate (Core)**

- Optional sync gate before engine delete, same shape as Flush.
  [`MutatorActionHandler.cpp:99`](../../Source/Core/Actions/MutatorActionHandler.cpp#L99)

- Processor injects delete gate into handler construction.
  [`PluginProcessor.cpp:359`](../../Source/Core/PluginProcessor.cpp#L359)

**Modal + Don't ask again**

- AlertWindow hosts checkbox; Enter stays on Delete.
  [`PluginEditor.cpp:147`](../../Source/GUI/PluginEditor.cpp#L147)

- Policy NEVER WARN skips modal; Confirm+checkbox writes NEVER WARN.
  [`PluginEditor.cpp:442`](../../Source/GUI/PluginEditor.cpp#L442)

**Settings persistence**

- English dialog and Settings copy SSOT.
  [`PluginDisplayNames.h:154`](../../Source/Shared/Definitions/PluginDisplayNames.h#L154)

- POLICIES row MUTATOR DEL : WARN ALWAYS / NEVER WARN.
  [`SettingsPanel.h:33`](../../Source/GUI/Settings/SettingsPanel.h#L33)

- Default WARN ALWAYS property on APVTS state.
  [`PluginProcessor.cpp:1260`](../../Source/Core/PluginProcessor.cpp#L1260)

**Tests**

- Cancel skips delete; disabled mirror skips modal.
  [`MutatorActionHandlerTests.cpp:279`](../../Tests/Unit/MutatorActionHandlerTests.cpp#L279)
