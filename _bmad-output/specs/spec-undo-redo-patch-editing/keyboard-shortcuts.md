# Keyboard Shortcuts — Editorial Undo/Redo

## Undo / Redo

| Action | macOS | Windows / Linux |
|--------|-------|-----------------|
| Undo | ⌘Z | Ctrl+Z |
| Redo | ⌘⇧Z | Ctrl+Shift+Z |

Platform detection via JUCE `ModifierKeys::commandModifier` / standard key handling (same pattern as existing `PluginEditor` and widget key handlers).

## Focus and interception rules

### Handle at `PluginEditor::keyPressed`

- Intercept undo/redo when editorial undo is enabled and no blocking overlay applies.
- Return `true` when handled to stop propagation.

### Do not steal shortcuts

| Context | Behavior |
|---------|----------|
| Active text input (`PatchNameDisplay` if editable, Settings fields, modal text) | Native text undo/redo or no-op — **do not** route to patch undo |
| Settings window open | Settings `keyPressed` handles Escape; undo/redo only if focus not in text field |
| Bank transfer / progress dialogs | Dialog owns keyboard; no patch undo |
| About modal | Modal owns keyboard |
| Mutator Compare active | Patch undo/redo disabled entirely |

### Focus model

- `PluginEditor` grabs focus on background click (`grabKeyboardFocus` after `unfocusAllComponents`) — enables shortcuts when no child text field is focused.
- Sliders with focus: arrow keys adjust value (existing `Slider::keyPressed`); one arrow keypress is **not** one undo step. Arrow edits group per focus session; mouse slide groups per drag. Cmd/Ctrl+Z undoes the last editorial transaction (undo → pre-gesture value; redo → post-gesture value).

### Standalone vs hosted

Identical shortcut behavior in Standalone and plugin host formats. No host-specific undo delegation in v1.

`EDITOR_WANTS_KEYBOARD_FOCUS` must be `TRUE` in `juce_add_plugin` so hosts deliver Cmd/Ctrl+Z to the editor (`JucePlugin_EditorRequiresKeyboardFocus=1`). Runtime `setWantsKeyboardFocus` on the editor alone is not enough for many hosts (e.g. Ableton Live).

## Related shortcuts (unchanged)

| Key | Action | Notes |
|-----|--------|-------|
| Escape | Clear clipboard feedback | Existing `PluginEditor` behavior |
| Return on focused slider | Reset to default | Existing `Slider` behavior |
| C on module header | Copy module | Not Compare |
| C on Patch Mutator | Compare audition | Distinct from Cmd/Ctrl+Z |

## Implementation notes

- Match shortcuts with `KeyPress` equality on keyCode `'z'` + `commandModifier` (and `shiftModifier` for redo), same pattern as `TestComponent` zoom. Do **not** use `getTextCharacter() == 'z'`: on macOS, JUCE clears `textCharacter` when Command is down.
- GUI layer forwards undo/redo to Core (`PluginProcessor` or dedicated undo service) — Core performs `UndoManager::undo()` / `redo()`.
- Check `canUndo()` / `canRedo()` before acting; no-op silently if stack empty.
- Optional: suppress undo while MIDI bulk suppress hooks are active mid-operation (avoid partial undo during in-flight bulk).
