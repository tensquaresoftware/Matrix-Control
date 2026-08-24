#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Helpers/EditorialUndoRedoShortcuts.h"

class EditorialUndoRedoShortcutTests : public juce::UnitTest
{
public:
    EditorialUndoRedoShortcutTests() : juce::UnitTest("EditorialUndoRedoShortcut") {}

    void runTest() override
    {
        matchesCommandZAsUndoWithZeroTextCharacter();
        matchesCommandShiftZAsRedoWithZeroTextCharacter();
        matchesCtrlVariants();
        rejectsPlainZAndUnrelatedShortcuts();
    }

private:
    void matchesCommandZAsUndoWithZeroTextCharacter()
    {
        beginTest("matchesCommandZAsUndoWithZeroTextCharacter");

        using juce::ModifierKeys;
        const juce::KeyPress key('z', ModifierKeys::commandModifier, 0);

        expect(TSS::isEditorialUndoRedoShortcut(key));
        expectEquals(static_cast<int>(TSS::classifyEditorialUndoRedoShortcut(key)),
                     static_cast<int>(TSS::EditorialUndoRedoShortcut::kUndo));
    }

    void matchesCommandShiftZAsRedoWithZeroTextCharacter()
    {
        beginTest("matchesCommandShiftZAsRedoWithZeroTextCharacter");

        using juce::ModifierKeys;
        const juce::KeyPress key('z',
                                 ModifierKeys::commandModifier | ModifierKeys::shiftModifier,
                                 0);

        expect(TSS::isEditorialUndoRedoShortcut(key));
        expectEquals(static_cast<int>(TSS::classifyEditorialUndoRedoShortcut(key)),
                     static_cast<int>(TSS::EditorialUndoRedoShortcut::kRedo));
    }

    void matchesCtrlVariants()
    {
        beginTest("matchesCtrlVariants");

        using juce::ModifierKeys;

        // commandModifier is Ctrl on Windows/Linux and Command on macOS.
        const juce::KeyPress undo('z', ModifierKeys::commandModifier, 'z');
        const juce::KeyPress redo('z',
                                  ModifierKeys::commandModifier | ModifierKeys::shiftModifier,
                                  'Z');

        expectEquals(static_cast<int>(TSS::classifyEditorialUndoRedoShortcut(undo)),
                     static_cast<int>(TSS::EditorialUndoRedoShortcut::kUndo));
        expectEquals(static_cast<int>(TSS::classifyEditorialUndoRedoShortcut(redo)),
                     static_cast<int>(TSS::EditorialUndoRedoShortcut::kRedo));
    }

    void rejectsPlainZAndUnrelatedShortcuts()
    {
        beginTest("rejectsPlainZAndUnrelatedShortcuts");

        using juce::ModifierKeys;

        expect(! TSS::isEditorialUndoRedoShortcut(juce::KeyPress('z', 0, 'z')));
        expect(! TSS::isEditorialUndoRedoShortcut(
            juce::KeyPress('z', ModifierKeys::altModifier, 0)));
        expect(! TSS::isEditorialUndoRedoShortcut(
            juce::KeyPress('y', ModifierKeys::commandModifier, 0)));
        expectEquals(static_cast<int>(TSS::classifyEditorialUndoRedoShortcut(
                         juce::KeyPress(juce::KeyPress::escapeKey))),
                     static_cast<int>(TSS::EditorialUndoRedoShortcut::kNone));
    }
};

static EditorialUndoRedoShortcutTests editorialUndoRedoShortcutTests;
