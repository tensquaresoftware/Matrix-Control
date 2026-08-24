#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
    /** Classification of editorial Undo/Redo keyboard shortcuts (Cmd/Ctrl+Z / Shift). */
    enum class EditorialUndoRedoShortcut
    {
        kNone,
        kUndo,
        kRedo
    };

    /**
        Matches platform undo/redo KeyPresses using keyCode + modifiers.
        Tolerates textCharacter == 0 (macOS clears it when Command is down).
    */
    inline EditorialUndoRedoShortcut classifyEditorialUndoRedoShortcut(const juce::KeyPress& key)
    {
        using juce::ModifierKeys;

        const auto undo = juce::KeyPress('z', ModifierKeys::commandModifier, 0);
        const auto redo = juce::KeyPress('z',
                                         ModifierKeys::commandModifier | ModifierKeys::shiftModifier,
                                         0);

        if (key == redo)
            return EditorialUndoRedoShortcut::kRedo;

        if (key == undo)
            return EditorialUndoRedoShortcut::kUndo;

        return EditorialUndoRedoShortcut::kNone;
    }

    inline bool isEditorialUndoRedoShortcut(const juce::KeyPress& key)
    {
        return classifyEditorialUndoRedoShortcut(key) != EditorialUndoRedoShortcut::kNone;
    }
}
