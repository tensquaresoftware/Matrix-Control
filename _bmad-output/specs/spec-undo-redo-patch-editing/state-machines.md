# State Machines — Editorial Undo vs Mutator vs Load

Diagrams for how editorial undo stack, Patch Mutator history, and patch-load checkpoints interact.

## Editorial undo stack lifecycle

```mermaid
stateDiagram-v2
    [*] --> Empty: plugin start / no patch
    Empty --> Active: first editorial edit\n(beginNewTransaction)
    Active --> Active: more edits\n(push transactions)
    Active --> Active: undo / redo\n(within stack)
    Active --> Empty: checkpoint\n(load / paste / mutate*\n*recommended)
    Active --> Active: eviction\n(>100 transactions)
    Empty --> Active: edit after checkpoint
```

## Three parallel histories (conceptual)

```mermaid
flowchart TB
    subgraph Editorial["Editorial Undo (APVTS UndoManager)"]
        E1[Transaction stack]
        E2[Undo / Redo]
    end

    subgraph Mutator["Patch Mutator (MutationHistoryStore)"]
        M1[MUTATE roots]
        M2[RETRY branches]
        M3[M/R navigation]
    end

    subgraph Baseline["Dirty baseline (DirtyPatchTracker)"]
        D1[Snapshot at load/sync]
        D2[isDirty compare]
    end

    UI[User edits] --> Editorial
    UI --> Mutator
    Load[Patch load / paste] --> Baseline
    Load --> Editorial
    Mutate[MUTATE / RETRY] --> Mutator
    Mutate --> Editorial
    Mutate --> Baseline

    Editorial --> MIDI[APVTS → Model → SysEx]
    Baseline --> Footer[Dirty footer UX]
```

## Compare mode vs undo

```mermaid
stateDiagram-v2
    [*] --> Editing: normal edit
    Editing --> CompareActive: Mutator C pressed\nkCompareActive true
    CompareActive --> Editing: C again / exit Compare
    CompareActive --> CompareActive: audition A/B\n(no editorial undo entries)
    Editing --> Editing: Cmd/Ctrl+Z undo OK
    CompareActive --> CompareActive: Cmd/Ctrl+Z NO-OP\n(undo disabled)
```

## Transaction grouping (slider example)

```mermaid
sequenceDiagram
    participant User
    participant Slider
    participant APVTS
    participant Undo as UndoManager
    participant MIDI

    User->>Slider: mouseDown / focus
    Slider->>APVTS: beginChangeGesture
  User->>Slider: drag / arrow keys
    Slider->>APVTS: setValueNotifyingHost
    APVTS->>MIDI: SysEx per change
    User->>Slider: mouseUp / focusLost
    Slider->>APVTS: endChangeGesture
    Note over Undo: one transaction boundary
    User->>Undo: Cmd/Ctrl+Z
    Undo->>APVTS: restore prior state
    APVTS->>MIDI: resync SysEx
```

## Checkpoint clears stack

```mermaid
flowchart LR
    A[User loads patch] --> B[suppress* hooks]
    B --> C[bufferToApvts / load path]
    C --> D[DirtyPatchTracker.captureSnapshot]
    D --> E[UndoManager clearUndoHistory]
    E --> F[Active editing resumes]
```
