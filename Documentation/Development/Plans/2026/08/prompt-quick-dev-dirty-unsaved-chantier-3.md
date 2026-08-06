# Prompt Quick Dev — Dirty / unsaved patch UX — chantier 3

Copier-coller le bloc ci-dessous dans une **conversation Cursor fraîche**, puis lancer `/bmad-quick-dev` (ou coller avec le skill attaché).

**Prérequis :** chantier 2 idéalement déjà sur `main` (Save As device conserve le risque RAM ; origine `.syx` → Save / Save As). Si chantier 2 n’est pas encore mergé, baser sur `main` + brancher après chantier 2, ou noter explicitement la dépendance.

---

```
/bmad-quick-dev

## Intent

Implement **chantier 3** of the Dirty vs unsaved patch UX for Matrix-Control: warn when closing the plugin / DAW session if the current patch is still at risk.

Product SSOT (frozen decisions):
`Documentation/Development/Plans/2026/08/2026-08-03-Dirty-Vs-Unsaved-Patch-UX.md`
— especially §0 decision 9, §5.4, §8 item 8.

Prior work:
- Chantier 1 on `main` (`spec-dirty-unsaved-patch-ux-chantier-1.md`, commit `75f397d`): risk = dirty ∨ not-STORED-in-RAM; leave modal Cancel / Discard / Persist (Store or Save As).
- Chantier 2 (must be done or included as dependency): device-origin Save As keeps RAM risk; `.syx`-origin dirty uses Save / Save As on leave.
- Related deferred note: `patchNotStoredInRam_` is in-memory only today — session close is exactly where persistence / last-chance warning matters (`deferred-work.md` entry from chantier 1 review).

## Problem

Today, closing the editor, unloading the plugin, or quitting the host can drop at-risk work (dirty edits and/or INIT / Save-As-without-STORE) with **no** FR-51-style prompt. Users expect a last chance consistent with leave-context rules, without inventing a new product language.

## In scope

- Detect at-risk state on plugin / editor / session teardown paths that JUCE actually exposes for this project (AudioProcessorEditor close, processor destructor constraints, standalone quit, host `getStateInformation` / close — choose the hooks that can still show a modal safely on the message thread).
- Reuse the same risk predicate and, as far as host APIs allow, the same Cancel / Discard / Persist family (RAM Store vs ROM/file Save–Save As per chantiers 1–2).
- Honour Settings “never warn” (suppress this family entirely).
- English-only strings; Core ↛ GUI (gate/callback from editor or processor message-thread UI binding).
- Document hard host limits honestly: some DAWs ignore or cannot block unload; prefer best-effort + standalone reliability over fake guarantees.
- Unit or harness coverage for the policy/predicate on close; manual checklist for standalone quit and at least one AU/VST3 close path if feasible.
- Follow CONVENTIONS.md / project-context; lint_touched on touched C++.

## Out of scope

- Re-opening chantiers 1–2 modal copy except where close must share strings.
- Persisting unsaved patch bytes into DAW project state as a substitute for Store/Save (unless a minimal flag is required to re-prompt after reload — ask first if you believe a state flag is mandatory).
- Merging Mutator history / IMPORT / name-recon into the close dialog.
- Guaranteeing every host will block quit (impossible); do not claim universal DAW coverage.

## Ask first if unclear

- Whether Cancel on close must **abort** host quit when the API allows it, vs only Discard/Persist with Cancel = stay in UI when possible.
- Whether a not-STORED flag must survive `getStateInformation` / restore for post-reload prompting (deferred from chantier 1) — only if close alone is insufficient.

## Suggested approach

1. Read plan §5.4 / §8 and chantier 1–2 specs + current gate binding in `PluginEditorPatchBindings` / `PluginProcessorGates`.
2. Map real close/unload entry points in this codebase (standalone vs plugin).
3. Spec: which hooks, modal behaviour, never-warn, host limitations.
4. Implement message-thread-safe prompting; Persist reuses existing Store / Save / Save As paths.
5. Tests + manual standalone checklist; review; defer true host impossibilities to `deferred-work.md`.

## Done when

- Standalone quit with at-risk patch shows the risk family (or documented equivalent) unless never-warn.
- Persist/Discard/Cancel behave as specified where the host allows blocking.
- No regression of leave-context chantiers 1–2.
- Spec under `_bmad-output/implementation-artifacts/` with honest host limitation notes; targeted tests green.
```

---

*Fichier d’aide — coller le bloc fenced ci-dessus, pas tout le fichier.*
