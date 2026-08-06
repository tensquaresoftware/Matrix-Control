# Prompt Quick Dev — Dirty / unsaved patch UX — chantier 2

Copier-coller le bloc ci-dessous dans une **conversation Cursor fraîche**, puis lancer `/bmad-quick-dev` (ou coller avec le skill attaché).

---

```
/bmad-quick-dev

## Intent

Implement **chantier 2** of the Dirty vs unsaved patch UX for Matrix-Control.

Product SSOT (frozen decisions):
`Documentation/Development/Plans/2026/08/2026-08-03-Dirty-Vs-Unsaved-Patch-UX.md`
— especially §0 decisions 5–6, §4.5, §5.2–5.3, §6 (Computer Patches load), §8 items 6–7.

Baseline already on `main` (chantier 1 shipped):
`_bmad-output/implementation-artifacts/spec-dirty-unsaved-patch-ux-chantier-1.md`
Commit: `75f397d` — leave-context modal is Cancel / Discard / Store (RAM) or Save As (ROM); risk = dirty ∨ not-STORED-in-RAM; INIT marks not-STORED; PASTE stays silent on entry.

## Problem

Two gaps remain after chantier 1:

1. **SAVE AS from a device-origin patch** still refreshes the clean snapshot and typically clears the “not STORED in RAM” signal, so leaving the slot after Save As can stay silent even though the RAM slot was never STOREd. Product decision: keep the same at-risk rule as post-INIT (lecture B) until STORE.

2. **Dirty patch that originated from a `.syx` file** should offer Cancel / Discard / **Save** (overwrite the known file) or **Save As** when there is no clear target — not only Store / Save As as for device/ROM. File intent takes priority for chantier 2; do not add a fourth “also Store RAM” button in the same modal unless the plan explicitly requires it later.

## In scope

- After successful SAVE / SAVE AS from a **device-origin** (or INIT / not-yet-RAM-persisted) context: keep `patchNotStoredInRam_` (or equivalent) set so subsequent leave still prompts with the chantier 1 at-risk family (RAM: Cancel / Discard / Store).
- Track **computer-file origin** (known `.syx` path vs none) so leave-context Persist for file-origin dirty can run Save vs Save As appropriately.
- Wire modal button sets / copy for file-origin dirty: Cancel / Discard / Save (or Save As).
- Preserve Settings “never warn” suppression for the whole risk family.
- Keep Mutator history modal **after** the risk modal; do not merge with name-recon / IMPORT / Defrag / Master INIT dialogs.
- Unit tests for: Save As from device keeps not-STORED; file-origin dirty Persist Save path; never-warn still skips; Cancel abort unchanged.
- English-only UI strings. Core must not include AlertWindow / dialog types.
- Follow CONVENTIONS.md / project-context; run lint_touched on touched C++ and related unit tests.

## Out of scope

- Chantier 3 (plugin / DAW / session close warning).
- Changing PASTE entry silence or INIT not-STORED behaviour from chantier 1 (reuse; do not regress).
- Merging unrelated modals; French UI strings; inventing a 4-button Store+Save modal.
- Broad refactors unrelated to origin tracking / Persist paths.

## Suggested approach

1. Read the plan §0 / §4.5 / §5.2 / §8 and the chantier 1 spec + Code Map.
2. Trace current SAVE / SAVE AS / `captureCleanSnapshot` / `patchNotStoredInRam_` clear sites.
3. Spec then implement: origin + Persist button semantics; keep risk after device-origin Save As; file-origin Save/Save As.
4. Extend gate / policy / handler tests; verify build + targeted tests.
5. Quick-dev review; append only true deferrals to `deferred-work.md`.

## Done when

- Device-origin Save As then leave without STORE → at-risk modal (Store when RAM allowed).
- Dirty `.syx`-origin leave → Cancel / Discard / Save (known path) or Save As (no clear path).
- Never-warn still silent; Mutator order unchanged; chantier 1 INIT/PASTE behaviour preserved.
- Spec artifact under `_bmad-output/implementation-artifacts/` with status ready for review; tests green.
```

---

*Fichier d’aide — coller le bloc fenced ci-dessus, pas tout le fichier.*
