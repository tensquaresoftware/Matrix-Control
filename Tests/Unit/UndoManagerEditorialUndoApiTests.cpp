#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Shared/Definitions/PluginIDs.h"

namespace
{

constexpr auto kProbeParameterId = "editorialUndoProbe";

class EditorialUndoApiAudioProcessor : public juce::AudioProcessor
{
public:
    EditorialUndoApiAudioProcessor()
        : juce::AudioProcessor(BusesProperties())
        , apvts(*this, &undoManager, "P", makeLayout())
    {
    }

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;

    static juce::AudioProcessorValueTreeState::ParameterLayout makeLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;
        layout.add(std::make_unique<juce::AudioParameterInt>(
            juce::ParameterID(kProbeParameterId, 1),
            "Probe",
            0,
            100,
            0));
        return layout;
    }

    int readProbeValue() const
    {
        return juce::roundToInt(apvts.getParameterAsValue(kProbeParameterId).getValue());
    }

    void writeProbeValue(int value)
    {
        apvts.getParameterAsValue(kProbeParameterId).setValue(static_cast<float>(value));
    }

    bool isEditorialUndoRedoEnabled() const
    {
        namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

        return ! static_cast<bool>(apvts.state.getProperty(MutatorState::kCompareActive, false));
    }

    bool canPerformEditorialUndo() const
    {
        return isEditorialUndoRedoEnabled() && undoManager.canUndo();
    }

    bool canPerformEditorialRedo() const
    {
        return isEditorialUndoRedoEnabled() && undoManager.canRedo();
    }

    bool performEditorialUndo()
    {
        if (! canPerformEditorialUndo())
            return false;

        undoManager.undo();
        return true;
    }

    bool performEditorialRedo()
    {
        if (! canPerformEditorialRedo())
            return false;

        undoManager.redo();
        return true;
    }

    const juce::String getName() const override { return "EditorialUndoApiHarness"; }
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

class UndoManagerEditorialUndoApiTests : public juce::UnitTest
{
public:
    UndoManagerEditorialUndoApiTests() : juce::UnitTest("UndoManagerEditorialUndoApi") {}

    void runTest() override
    {
        testCompareActiveBlocksPerformEditorialUndo();
        testPerformEditorialUndoRestoresRecordedEdit();
        testPerformEditorialRedoRestoresAfterUndo();
    }

private:
    void testCompareActiveBlocksPerformEditorialUndo()
    {
        beginTest("Compare active blocks performEditorialUndo with unchanged stack");

        EditorialUndoApiAudioProcessor processor;
        namespace MutatorState = PluginIDs::PatchManagerSection::PatchMutatorModule::StateProperties;

        processor.writeProbeValue(10);
        const auto undoCountBefore = processor.undoManager.getUndoDescriptions().size();

        processor.apvts.state.setProperty(MutatorState::kCompareActive, true, nullptr);

        expect(! processor.performEditorialUndo());
        expectEquals(static_cast<int>(processor.undoManager.getUndoDescriptions().size()), undoCountBefore);
        expectEquals(processor.readProbeValue(), 10);
    }

    void testPerformEditorialUndoRestoresRecordedEdit()
    {
        beginTest("performEditorialUndo restores prior editorial parameter value");

        EditorialUndoApiAudioProcessor processor;

        processor.writeProbeValue(10);
        processor.undoManager.clearUndoHistory();
        processor.writeProbeValue(13);

        expect(processor.performEditorialUndo());
        expectEquals(processor.readProbeValue(), 10);
    }

    void testPerformEditorialRedoRestoresAfterUndo()
    {
        beginTest("performEditorialRedo restores value after editorial undo");

        EditorialUndoApiAudioProcessor processor;

        processor.writeProbeValue(10);
        processor.undoManager.clearUndoHistory();
        processor.writeProbeValue(13);

        processor.performEditorialUndo();
        expect(processor.performEditorialRedo());
        expectEquals(processor.readProbeValue(), 13);
    }
};

static UndoManagerEditorialUndoApiTests undoManagerEditorialUndoApiTests;

} // namespace
