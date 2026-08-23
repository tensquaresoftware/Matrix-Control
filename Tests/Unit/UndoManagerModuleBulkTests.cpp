#include <cstring>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Actions/ModuleActionHandler.h"
#include "Core/Init/InitTemplateLoader.h"
#include "Core/Init/PatchModuleInitService.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/MIDI/SysEx/SysExDecoder.h"
#include "Core/MIDI/SysEx/SysExParser.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardService.h"
#include "Shared/Definitions/ApvtsTypes.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{

class UndoBulkTestAudioProcessor : public juce::AudioProcessor
{
public:
    explicit UndoBulkTestAudioProcessor(juce::AudioProcessorValueTreeState::ParameterLayout layout)
        : juce::AudioProcessor(BusesProperties())
        , apvts(*this, &undoManager, "P", std::move(layout))
    {
    }

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;

    const juce::String getName() const override { return "UndoBulkHarness"; }
    void prepareToPlay(double, int) override {}
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}
};

bool moduleStateMatches(const Core::PatchModel& lhs,
                        const Core::PatchModel& rhs,
                        const juce::String& moduleGroupId)
{
    for (const auto& d : Core::ApvtsPatchMapper::buildIntDescriptors())
    {
        if (d.parentGroupId == moduleGroupId && lhs.getValue(d) != rhs.getValue(d))
            return false;
    }

    for (const auto& d : Core::ApvtsPatchMapper::buildChoiceDescriptors())
    {
        if (d.parentGroupId == moduleGroupId && lhs.getChoiceIndex(d) != rhs.getChoiceIndex(d))
            return false;
    }

    return true;
}

juce::AudioProcessorValueTreeState::ParameterLayout makeEnvelopePasteLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    for (const auto& d : PluginDescriptors::PatchEditSection::Envelope1Module::kIntParameters)
    {
        layout.add(std::make_unique<juce::AudioParameterInt>(
            juce::ParameterID(d.parameterId, 1), d.displayName,
            d.minValue, d.maxValue, d.defaultValue));
    }

    for (const auto& d : PluginDescriptors::PatchEditSection::Envelope1Module::kChoiceParameters)
    {
        layout.add(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID(d.parameterId, 1), d.displayName,
            d.choices, d.defaultIndex));
    }

    return layout;
}

juce::AudioProcessorValueTreeState::ParameterLayout makeDco1Layout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    for (const auto& d : PluginDescriptors::PatchEditSection::Dco1Module::kIntParameters)
    {
        layout.add(std::make_unique<juce::AudioParameterInt>(
            juce::ParameterID(d.parameterId, 1), d.displayName,
            d.minValue, d.maxValue, d.defaultValue));
    }

    for (const auto& d : PluginDescriptors::PatchEditSection::Dco1Module::kChoiceParameters)
    {
        layout.add(std::make_unique<juce::AudioParameterChoice>(
            juce::ParameterID(d.parameterId, 1), d.displayName,
            d.choices, d.defaultIndex));
    }

    return layout;
}

juce::String resolveParameterIdFromTree(juce::ValueTree& tree, const juce::Identifier& property)
{
    const juce::String propertyId = property.toString();
    if (propertyId != ApvtsTypes::kValue)
        return {};

    if (tree.getType().toString() == ApvtsTypes::kParam)
    {
        const juce::var idProperty = tree.getProperty("id");
        if (idProperty.isString() && idProperty.toString().isNotEmpty())
            return idProperty.toString();
    }

    return {};
}

struct PasteUndoHarness : juce::ValueTree::Listener
{
    UndoBulkTestAudioProcessor proc;
    Core::PatchModel model;
    Core::ApvtsPatchMapper mapper;
    Core::ClipboardService clipboard;
    Core::PatchParameterSysExDispatcher patchDispatcher;
    Core::ModuleActionHandler handler;
    bool suppressPatchSysEx { false };
    int patchDispatchCount { 0 };

    explicit PasteUndoHarness(juce::AudioProcessorValueTreeState::ParameterLayout layout)
        : proc(std::move(layout))
        , mapper(proc.apvts, model)
        , patchDispatcher(model,
                          [this](int, juce::uint8) { ++patchDispatchCount; })
        , handler(Core::ModuleActionHandler::Dependencies {
                      proc.apvts,
                      &model,
                      &mapper,
                      &clipboard,
                      nullptr,
                      nullptr,
                      nullptr,
                      &patchDispatcher,
                      nullptr,
                      nullptr
                  },
                  Core::ActionExecutionHooks{
                      nullptr,
                      nullptr,
                      [this](bool suppress) { suppressPatchSysEx = suppress; },
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      [this](const juce::String& name) { proc.undoManager.beginNewTransaction(name); } })
    {
        proc.apvts.state.addListener(this);
    }

    ~PasteUndoHarness() override
    {
        proc.apvts.state.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                  const juce::Identifier& property) override
    {
        if (suppressPatchSysEx)
            return;

        const juce::String parameterId = resolveParameterIdFromTree(treeWhosePropertyHasChanged, property);
        if (parameterId.isEmpty())
            return;

        mapper.apvtsToBuffer();
        patchDispatcher.dispatch(parameterId);
    }
};

struct InitUndoHarness : juce::ValueTree::Listener
{
    UndoBulkTestAudioProcessor proc;
    Core::PatchModel model;
    Core::ApvtsPatchMapper mapper;
    SysExParser parser;
    SysExDecoder decoder;
    Core::InitTemplateLoader loader;
    Core::PatchParameterSysExDispatcher patchDispatcher;
    Core::PatchModuleInitService initService;
    Core::ModuleActionHandler handler;
    bool suppressPatchSysEx { false };
    int patchDispatchCount { 0 };
    juce::File templatesFolder;

    InitUndoHarness(juce::AudioProcessorValueTreeState::ParameterLayout layout, const juce::File& folder)
        : proc(std::move(layout))
        , mapper(proc.apvts, model)
        , decoder(parser)
        , loader(decoder)
        , patchDispatcher(model,
                          [this](int, juce::uint8) { ++patchDispatchCount; })
        , initService(model, mapper, loader, patchDispatcher, [this]() { return templatesFolder; })
        , handler(Core::ModuleActionHandler::Dependencies {
                      proc.apvts,
                      &model,
                      &mapper,
                      nullptr,
                      nullptr,
                      nullptr,
                      &initService,
                      &patchDispatcher,
                      nullptr,
                      nullptr
                  },
                  Core::ActionExecutionHooks{
                      nullptr,
                      nullptr,
                      [this](bool suppress) { suppressPatchSysEx = suppress; },
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      nullptr,
                      [this](const juce::String& name) { proc.undoManager.beginNewTransaction(name); } })
        , templatesFolder(folder)
    {
        proc.apvts.state.addListener(this);
    }

    ~InitUndoHarness() override
    {
        proc.apvts.state.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                  const juce::Identifier& property) override
    {
        if (suppressPatchSysEx)
            return;

        const juce::String parameterId = resolveParameterIdFromTree(treeWhosePropertyHasChanged, property);
        if (parameterId.isEmpty())
            return;

        mapper.apvtsToBuffer();
        patchDispatcher.dispatch(parameterId);
    }
};

juce::File createTempTemplatesDir()
{
    auto dir = juce::File::getSpecialLocation(juce::File::tempDirectory)
                   .getNonexistentChildFile("MatrixControlUndoModuleBulk", "", false);
    jassert(dir.createDirectory());
    return dir;
}

void copyInitFixtureToDir(const juce::File& dir)
{
    const auto source = juce::File(MATRIX_TEST_FIXTURES_DIR)
                            .getChildFile("Init")
                            .getChildFile(Core::InitTemplateLoader::kPatchInitFileName);
    source.copyFileTo(dir.getChildFile(Core::InitTemplateLoader::kPatchInitFileName));
}

} // namespace

class UndoManagerModuleBulkTests : public juce::UnitTest
{
public:
    UndoManagerModuleBulkTests() : juce::UnitTest("UndoManagerModuleBulk") {}

    void runTest() override
    {
        testModulePasteRecordsOneUndoTransactionAndRestoresModule();
        testModulePasteBlockedDoesNotGrowUndoStack();
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

        harness.mapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());

        expect(harness.proc.undoManager.canUndo());

        harness.proc.undoManager.undo();
        harness.mapper.apvtsToBuffer();

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

        harness.mapper.pushModuleToApvts(PatchEditIds::Dco1Module::kGroupId);
        const Core::PatchModel baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Dco1Module::StandaloneWidgets::kInit, juce::var());

        expect(harness.proc.undoManager.canUndo());

        harness.proc.undoManager.undo();
        harness.mapper.apvtsToBuffer();

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

        harness.mapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        harness.proc.undoManager.clearUndoHistory();
        harness.patchDispatchCount = 0;

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());
        expect(harness.patchDispatchCount > 0);

        harness.proc.undoManager.undo();
        harness.mapper.apvtsToBuffer();

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

        harness.mapper.pushModuleToApvts(PatchEditIds::Dco1Module::kGroupId);
        harness.proc.undoManager.clearUndoHistory();
        harness.patchDispatchCount = 0;

        harness.handler.handleAction(PatchEditIds::Dco1Module::StandaloneWidgets::kInit, juce::var());
        expect(harness.patchDispatchCount > 0);

        harness.proc.undoManager.undo();
        harness.mapper.apvtsToBuffer();

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

        harness.mapper.pushModuleToApvts(PatchEditIds::Envelope1Module::kGroupId);
        const Core::PatchModel baseline = harness.model;
        harness.proc.undoManager.clearUndoHistory();

        harness.handler.handleAction(PatchEditIds::Envelope1Module::StandaloneWidgets::kPaste, juce::var());
        expect(harness.proc.undoManager.canUndo());

        harness.proc.undoManager.undo();
        harness.mapper.apvtsToBuffer();
        expect(moduleStateMatches(harness.model, baseline, PatchEditIds::Envelope1Module::kGroupId));

        harness.proc.undoManager.redo();
        harness.mapper.apvtsToBuffer();

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
    }
};

static UndoManagerModuleBulkTests undoManagerModuleBulkTests;
