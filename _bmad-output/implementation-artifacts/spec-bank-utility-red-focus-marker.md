---
title: 'Bank Utility red focus marker'
type: 'feature'
created: '2026-09-05'
status: 'done'
route: 'one-shot'
baseline_commit: '548280d0a09fd7bb446989f6fe0d01a1abeb11b2'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/spec-patch-nav-focus-and-bank-marker.md'
---

# Bank Utility red focus marker

## Intent

**Problem:** The DEVICE-badge fill on the current Bank Utility bank button reads as disabled/grayed, so users cannot reliably spot the bank of work.

**Approach:** Renegotiate the prior badge rule: mark the current bank with red button text (same focus red as Internal NumberBoxes / PATCH NAME), only while Internal Patches owns navigation focus and coordinates are established. When Computer (or none) owns focus, bank buttons look normal; CurrentBankNumber still shows the destination bank for STORE.

## Suggested Review Order

- Gate marker on Internal focus + established coordinates; clear looks first.
  [`BankUtilityPanel.cpp:256`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L256)

- Red text on all interactive button states; keep natural backgrounds.
  [`BankUtilityPanel.cpp:285`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L285)

- Refresh when navigation focus changes without touching paste enablement.
  [`BankUtilityPanel.cpp:140`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.cpp#L140)

- Comment documents focus + coordinates gate.
  [`BankUtilityPanel.h:75`](../../Source/GUI/Panels/MainComponent/BodyPanel/SharedPanel/PatchManagerPanel/Modules/BankUtilityPanel.h#L75)
