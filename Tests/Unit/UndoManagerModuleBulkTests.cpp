#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"
#include "UndoManagerModuleBulkTestSupport.h"

using namespace UndoManagerModuleBulkTestSupport;

class UndoManagerModuleBulkTests : public juce::UnitTest
{
public:
    UndoManagerModuleBulkTests() : juce::UnitTest("UndoManagerModuleBulk") {}

    void runTest() override
    {
        testModulePasteRecordsOneUndoTransactionAndRestoresModule();
        testModulePasteBlockedDoesNotGrowUndoStack();
        testModulePasteIncompatibleDoesNotGrowUndoStack();
        testPatchModuleInitRecordsOneUndoTransactionAndRestoresModule();
        testUndoAfterPasteDispatchesSysExWhenSuppressOff();
        testUndoAfterInitDispatchesSysExWhenSuppressOff();
        testModulePasteRedoRestoresPastedState();
        testNonEditorialFooterDoesNotGrowUndoStack();
    }

private:
    void testModulePasteRecordsOneUndoTransactionAndRestoresModule()
    {
        beginTest("module paste → one undo transaction restores entire module");

        PasteUndoHarness harness(makeEnvelopePasteLayout());

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        Core::PatchModel baseline;
        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, d.maxValue);

        harness.clipboard.copyModule(Core::PatchModuleKind::Env1, harness.model);

        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, d.minValue);

        harness.apvtsMapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());

        expect(harness.proc.undoManager.canUndo());
        expectEquals(harness.proc.undoManager.getUndoDescriptions().size(), 1);

        harness.proc.undoManager.undo();
        harness.apvtsMapper.apvtsToBuffer();

        expect(moduleStateMatches(harness.model, baseline, PatchEditIds::Envelope1Module::kGroupId));
    }

    void testModulePasteBlockedDoesNotGrowUndoStack()
    {
        beginTest("blocked module paste does not grow undo stack");

        PasteUndoHarness harness(makeEnvelopePasteLayout());
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PluginIDs::PatchEditSection::Envelope1Module::StandaloneWidgets::kPaste,
                                     juce::var());

        expect(!harness.proc.undoManager.canUndo());
        expectEquals(harness.proc.undoManager.getUndoDescriptions().size(), 0);
    }

    void testModulePasteIncompatibleDoesNotGrowUndoStack()
    {
        beginTest("incompatible module paste does not grow undo stack");

        PasteUndoHarness harness(makeEnvelopePasteLayout());

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        Core::PatchModel dcoSource;
        for (const auto& d : Dco1Module::kIntParameters)
            dcoSource.setValue(d, d.maxValue);

        harness.clipboard.copyModule(Core::PatchModuleKind::Dco1, dcoSource);
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());

        expect(!harness.proc.undoManager.canUndo());
        expectEquals(harness.proc.undoManager.getUndoDescriptions().size(), 0);
        expectEquals(harness.proc.undoManager.getNumActionsInCurrentTransaction(), 0);
    }

    void testPatchModuleInitRecordsOneUndoTransactionAndRestoresModule()
    {
        beginTest("patch module init → one undo transaction restores entire module");

        const auto tempDir = createTempTemplatesDir();
        copyInitFixtureToDir(tempDir);

        InitUndoHarness harness(makeDco1Layout(), tempDir);

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        for (const auto& d : Dco1Module::kIntParameters)
            harness.model.setValue(d, d.maxValue);

        for (const auto& d : Dco1Module::kChoiceParameters)
            harness.model.setChoiceIndex(d, d.defaultIndex == 0 ? 1 : 0);

        harness.apvtsMapper.pushModuleToApvts(PatchEditIds::Dco1Module::kGroupId);
        const Core::PatchModel baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Dco1Module::StandaloneWidgets::kInit, juce::var());

        expect(harness.proc.undoManager.canUndo());
        expectEquals(harness.proc.undoManager.getUndoDescriptions().size(), 1);

        harness.proc.undoManager.undo();
        harness.apvtsMapper.apvtsToBuffer();

        expect(moduleStateMatches(harness.model, baseline, PatchEditIds::Dco1Module::kGroupId));
    }

    void testUndoAfterPasteDispatchesSysExWhenSuppressOff()
    {
        beginTest("undo after module paste restores APVTS values for SysEx path");

        PasteUndoHarness harness(makeEnvelopePasteLayout());

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, d.maxValue);

        harness.clipboard.copyModule(Core::PatchModuleKind::Env1, harness.model);

        const int baselineValue = Envelope1Module::kIntParameters.front().minValue;
        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, baselineValue);

        harness.apvtsMapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        harness.proc.undoManager.clearUndoHistory();
        harness.patchDispatchCount = 0;

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());
        expect(harness.patchDispatchCount > 0);

        harness.patchDispatchCount = 0;
        harness.suppressPatchSysEx = true;
        harness.proc.undoManager.undo();
        harness.suppressPatchSysEx = false;

        harness.apvtsMapper.apvtsToBuffer();
        expectEquals(harness.model.getValue(Envelope1Module::kIntParameters.front()), baselineValue);
    }

    void testUndoAfterInitDispatchesSysExWhenSuppressOff()
    {
        beginTest("undo after patch module init restores APVTS values for SysEx path");

        const auto tempDir = createTempTemplatesDir();
        copyInitFixtureToDir(tempDir);

        InitUndoHarness harness(makeDco1Layout(), tempDir);

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        const int baselineValue = Dco1Module::kIntParameters.front().maxValue;
        for (const auto& d : Dco1Module::kIntParameters)
            harness.model.setValue(d, baselineValue);

        harness.apvtsMapper.pushModuleToApvts(PatchEditIds::Dco1Module::kGroupId);
        harness.proc.undoManager.clearUndoHistory();
        harness.patchDispatchCount = 0;

        harness.handler.handleAction(PatchEditIds::Dco1Module::StandaloneWidgets::kInit, juce::var());
        expect(harness.patchDispatchCount > 0);

        harness.patchDispatchCount = 0;
        harness.suppressPatchSysEx = true;
        harness.proc.undoManager.undo();
        harness.suppressPatchSysEx = false;

        harness.apvtsMapper.apvtsToBuffer();
        expectEquals(harness.model.getValue(Dco1Module::kIntParameters.front()), baselineValue);
    }

    void testModulePasteRedoRestoresPastedState()
    {
        beginTest("module paste redo restores pasted module state");

        PasteUndoHarness harness(makeEnvelopePasteLayout());

        using namespace PluginDescriptors::PatchEditSection;
        namespace PatchEditIds = PluginIDs::PatchEditSection;

        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, d.maxValue);

        harness.clipboard.copyModule(Core::PatchModuleKind::Env1, harness.model);

        Core::PatchModel pastedState;
        for (const auto& d : Envelope1Module::kIntParameters)
            pastedState.setValue(d, d.maxValue);

        for (const auto& d : Envelope1Module::kIntParameters)
            harness.model.setValue(d, d.minValue);

        harness.apvtsMapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        const Core::PatchModel baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());
        expect(harness.proc.undoManager.canUndo());

        harness.proc.undoManager.undo();
        harness.apvtsMapper.apvtsToBuffer();
        expect(moduleStateMatches(harness.model, baseline, PatchEditIds::Envelope1Module::kGroupId));

        harness.proc.undoManager.redo();
        harness.apvtsMapper.apvtsToBuffer();

        expect(moduleStateMatches(harness.model, pastedState, PatchEditIds::Envelope1Module::kGroupId));
    }

    void testNonEditorialFooterDoesNotGrowUndoStack()
    {
        beginTest("footer uiMessageText setProperty(nullptr) does not grow undo stack");

        PasteUndoHarness harness(makeEnvelopePasteLayout());
        harness.proc.undoManager.clearUndoHistory();

        harness.proc.apvts.state.setProperty("uiMessageText", "info", nullptr);
        harness.proc.apvts.state.setProperty("uiMessageSeverity", "info", nullptr);

        expect(!harness.proc.undoManager.canUndo());
        expectEquals(harness.proc.undoManager.getUndoDescriptions().size(), 0);
        expectEquals(harness.proc.undoManager.getNumActionsInCurrentTransaction(), 0);
    }
};

static UndoManagerModuleBulkTests undoManagerModuleBulkTests;
