#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/Factories/ApvtsFactory.h"
#include "Core/Init/InitDefaults.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/DirtyPatchTracker.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{

using namespace PluginDescriptors::PatchEditSection;

const PluginDescriptors::IntParameterDescriptor& firstPatchIntDescriptor()
{
    jassert(! Dco1Module::kIntParameters.empty());
    return Dco1Module::kIntParameters.front();
}

class DirtyStateUndoAudioProcessor : public juce::AudioProcessor
{
public:
    DirtyStateUndoAudioProcessor()
        : juce::AudioProcessor(BusesProperties())
        , apvts(*this, &undoManager, "P", ApvtsFactory::createParameterLayout())
    {
    }

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;

    bool isEditorialUndoRedoEnabled() const
    {
        namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

        return ! static_cast<bool>(apvts.state.getProperty(MutatorState::kCompareActive, false));
    }

    bool performEditorialUndo()
    {
        if (! isEditorialUndoRedoEnabled() || ! undoManager.canUndo())
            return false;

        undoManager.undo();
        return true;
    }

    bool performEditorialRedo()
    {
        if (! isEditorialUndoRedoEnabled() || ! undoManager.canRedo())
            return false;

        undoManager.redo();
        return true;
    }

    const juce::String getName() const override { return "DirtyStateUndoHarness"; }
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

struct DirtyStateHarness
{
    DirtyStateUndoAudioProcessor proc;
    Core::PatchModel model;
    Core::ApvtsPatchMapper mapper;
    Core::PatchNameSyncer nameSyncer;
    Core::DirtyPatchTracker tracker;

    DirtyStateHarness()
        : mapper(proc.apvts, model)
        , nameSyncer(proc.apvts, model)
    {
        model.loadFrom(Core::InitDefaults::patchData());
        captureCheckpointBaseline();
    }

    void captureCheckpointBaseline()
    {
        proc.undoManager.clearUndoHistory();
        mapper.apvtsToBuffer();
        nameSyncer.apvtsToBuffer();
        model.normalizeNameEncoding();
        tracker.captureSnapshot(model);
    }

    void writeProbeValue(int value)
    {
        const auto& descriptor = firstPatchIntDescriptor();
        // getParameterAsValue stores the denormalised parameter domain (same as getRawParameterValue).
        proc.apvts.getParameterAsValue(descriptor.parameterId).setValue(static_cast<float>(value));
    }

    bool isDirtyViaSync()
    {
        return tracker.syncApvtsAndIsDirty(mapper, nameSyncer, model);
    }
};

class UndoManagerDirtyStateTests : public juce::UnitTest
{
public:
    UndoManagerDirtyStateTests() : juce::UnitTest("UndoManagerDirtyState") {}

    void runTest() override
    {
        undoToBaseline_reportsClean();
        undoThenRedo_reportsDirtyAgain();
        compareActive_blocksUndoWithoutChangingDirtyState();
    }

private:
    void undoToBaseline_reportsClean()
    {
        beginTest("Editorial undo to checkpoint baseline reports not dirty");

        DirtyStateHarness harness;
        harness.writeProbeValue(15);
        expect(harness.isDirtyViaSync());

        expect(harness.proc.performEditorialUndo());
        expect(! harness.isDirtyViaSync());
    }

    void undoThenRedo_reportsDirtyAgain()
    {
        beginTest("Editorial redo after undo reports dirty again");

        DirtyStateHarness harness;
        harness.writeProbeValue(18);
        expect(harness.proc.performEditorialUndo());
        expect(! harness.isDirtyViaSync());

        expect(harness.proc.performEditorialRedo());
        expect(harness.isDirtyViaSync());
    }

    void compareActive_blocksUndoWithoutChangingDirtyState()
    {
        beginTest("Compare active blocks editorial undo without changing dirty state");

        DirtyStateHarness harness;
        namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

        harness.writeProbeValue(20);
        expect(harness.isDirtyViaSync());

        harness.proc.apvts.state.setProperty(MutatorState::kCompareActive, true, nullptr);

        expect(! harness.proc.performEditorialUndo());
        expect(harness.isDirtyViaSync());
    }
};

static UndoManagerDirtyStateTests undoManagerDirtyStateTests;

} // namespace
