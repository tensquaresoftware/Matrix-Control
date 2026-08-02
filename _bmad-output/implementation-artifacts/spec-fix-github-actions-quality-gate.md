---
title: 'Fix GitHub Actions quality-gate tracking'
type: 'bugfix'
created: '2026-08-02'
status: 'done'
route: 'one-shot'
baseline_commit: '1db101fe598572906fa797d033c177c33c4c2d36'
---

# Fix GitHub Actions quality-gate tracking

## Intent

**Problem:** Build and Test failed immediately on every main push because `Scripts/quality/` was ignored by a case-insensitive `Quality/` gitignore rule, so CI could not install or run the quality gate, and workflow structure tests still expected the pre-quality-gate job graph.

**Approach:** Root-anchor the legacy ignore rules, track the quality scripts, align workflow structure tests with the real job graph, and fail loud when the lint base ref is missing.

## Suggested Review Order

**Ignore collision**

- Root-anchor legacy folders so `Scripts/quality/` is tracked on case-insensitive clones.
  [`.gitignore:82`](../../../.gitignore#L82)

**Tracked gate scripts**

- Pin lizard for the CI quality-gate install step.
  [`requirements.txt:1`](../../../Scripts/quality/requirements.txt#L1)

- Fail when `--base` cannot be resolved instead of a vacuous green gate.
  [`lint_touched.py:450`](../../../Scripts/quality/lint_touched.py#L450)

**Workflow contract tests**

- Assert quality-gate exists on disk and is wired into needs/steps.
  [`test_build_and_test_workflow.py:33`](../../../Scripts/release/tests/test_build_and_test_workflow.py#L33)
