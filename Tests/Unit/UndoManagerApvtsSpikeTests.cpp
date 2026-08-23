#include <optional>
#include <unordered_set>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/MIDI/EditorPath.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PackedFieldCodec.h"
#include "Core/Models/PatchModel.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Shared/Definitions/ApvtsTypes.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{
using IntDesc = PluginDescriptors::IntParameterDescriptor;

std::optional<IntDesc> findDco1FrequencyDescriptor()
{
    for (const auto& d : Core::ApvtsPatchMapper::buildIntDescriptors())
    {
        if (d.parameterId == PluginIDs::PatchEditSection::Dco1Module::ParameterWidgets::kFrequency)
            return d;
    }

    return std::nullopt;
}

bool sysExMatchesRemoteEdit(const juce::MemoryBlock& block,
                            juce::uint8 expectedParam,
                            juce::uint8 expectedValue)
{
    if (block.getSize() != 7)
        return false;

    const auto* data = static_cast<const juce::uint8*>(block.getData());
    return data[0] == SysExConstants::kSysExStart
        && data[1] == SysExConstants::kManufacturerIdOberheim
        && data[2] == SysExConstants::kDeviceIdMatrix1000
        && data[3] == SysExConstants::Opcode::kRemoteParameterEdit
        && data[4] == expectedParam
        && data[5] == expectedValue
        && data[6] == SysExConstants::kSysExEnd;
}

juce::AudioProcessorValueTreeState::ParameterLayout makeDco1FrequencyLayout()
{
    const auto frequencyDesc = findDco1FrequencyDescriptor();
    jassert(frequencyDesc.has_value());

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID(frequencyDesc->parameterId, 1),
        frequencyDesc->displayName,
        frequencyDesc->minValue,
        frequencyDesc->maxValue,
        frequencyDesc->defaultValue));

    return layout;
}

int readIntParameterValue(const juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterId)
{
    return juce::roundToInt(apvts.getParameterAsValue(parameterId).getValue());
}

void writeIntParameterValue(juce::AudioProcessorValueTreeState& apvts,
                            const juce::String& parameterId,
                            int value)
{
    apvts.getParameterAsValue(parameterId).setValue(static_cast<float>(value));
}

void simulateSliderDrag(juce::AudioProcessorValueTreeState& apvts,
                        juce::UndoManager& undoManager,
                        const juce::String& parameterId,
                        int newValue)
{
    undoManager.beginNewTransaction();

    if (auto* param = apvts.getParameter(parameterId))
        param->beginChangeGesture();

    writeIntParameterValue(apvts, parameterId, newValue);

    if (auto* param = apvts.getParameter(parameterId))
        param->endChangeGesture();
}

class UndoTestAudioProcessor : public juce::AudioProcessor
{
public:
    explicit UndoTestAudioProcessor(juce::AudioProcessorValueTreeState::ParameterLayout layout)
        : juce::AudioProcessor(BusesProperties())
        , apvts(*this, &undoManager, "P", std::move(layout))
    {
    }

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;

    const juce::String getName() const override { return "UndoSpikeHarness"; }
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

struct ProcessorPathHarness : UndoTestAudioProcessor, juce::ValueTree::Listener
{
    Core::PatchModel model;
    Core::ApvtsPatchMapper mapper;
    Core::MidiOutboundQueue queue;
    Core::MidiActivityTracker tracker;
    SysExEncoder encoder;
    Core::PatchParameterSysExDispatcher dispatcher;
    std::unordered_set<juce::String> patchParameterIds_;
    bool suppressPatchParameterSysEx_ { false };
    bool sysExDispatchEnabled_ { true };

    explicit ProcessorPathHarness(juce::AudioProcessorValueTreeState::ParameterLayout layout)
        : UndoTestAudioProcessor(std::move(layout))
        , mapper(apvts, model)
        , dispatcher(model,
                     [this](int parameterNumber, juce::uint8 packedValue)
                     {
                         if (!sysExDispatchEnabled_)
                             return;

                         Core::EditorPath editorPath(queue, tracker);
                         editorPath.enqueueSysEx(encoder.encodeRemoteParameterEdit(
                             static_cast<juce::uint8>(parameterNumber),
                             packedValue));
                     })
    {
        const auto frequencyDesc = findDco1FrequencyDescriptor();
        jassert(frequencyDesc.has_value());
        patchParameterIds_.insert(frequencyDesc->parameterId);
        apvts.state.addListener(this);
    }

    ~ProcessorPathHarness() override
    {
        apvts.state.removeListener(this);
    }

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                  const juce::Identifier& property) override
    {
        const juce::String parameterId = resolveParameterIdFromTree(treeWhosePropertyHasChanged, property);

        if (patchParameterIds_.count(parameterId) == 0)
            return;

        if (!suppressPatchParameterSysEx_)
            mapper.apvtsToBuffer();

        if (!suppressPatchParameterSysEx_)
            dispatcher.dispatch(parameterId);
    }

    juce::uint8 packedFrequencyByte() const
    {
        const auto frequencyDesc = findDco1FrequencyDescriptor();
        jassert(frequencyDesc.has_value());
        const size_t offset = Core::PackedFieldCodec::safeOffset(frequencyDesc->sysExOffset, Core::PatchModel::kBufferSize);
        return model.data()[offset];
    }

private:
    static juce::String resolveParameterIdFromTree(juce::ValueTree& tree, const juce::Identifier& property)
    {
        const juce::String propertyId = property.toString();
        if (propertyId != ApvtsTypes::kValue)
            return propertyId;

        if (tree.getType().toString() == ApvtsTypes::kParam)
        {
            const juce::var idProperty = tree.getProperty("id");
            if (idProperty.isString() && idProperty.toString().isNotEmpty())
                return idProperty.toString();
        }

        return tree.getType().toString();
    }
};
} // namespace

class UndoManagerApvtsSpikeTests : public juce::UnitTest
{
public:
    UndoManagerApvtsSpikeTests() : juce::UnitTest("UndoManagerApvtsSpike") {}

    void runTest() override
    {
        testUndoRedoRestoresValueModelAndSysEx();
        testNonEditorialPropertyDoesNotRecordUndo();
        testUndoRestoresApvtsWhenSysExDispatchDisabled();
        testOneTransactionMemoryFootprint();
        testMultiWriteDragGroupsOneTransaction();
    }

private:
    void testUndoRedoRestoresValueModelAndSysEx()
    {
        beginTest("Slider drag undo/redo restores APVTS, model, and 0x06 SysEx");

        const auto frequencyDesc = findDco1FrequencyDescriptor();
        expect(frequencyDesc.has_value());

        ProcessorPathHarness harness(makeDco1FrequencyLayout());
        const auto& parameterId = frequencyDesc->parameterId;

        writeIntParameterValue(harness.apvts, parameterId, 20);
        harness.mapper.apvtsToBuffer();
        harness.undoManager.clearUndoHistory();
        while (harness.queue.dequeue().has_value()) {}

        simulateSliderDrag(harness.apvts, harness.undoManager, parameterId, 45);
        expectEquals(readIntParameterValue(harness.apvts, parameterId), 45);
        expectEquals(static_cast<int>(harness.packedFrequencyByte()), 45);
        while (harness.queue.dequeue().has_value()) {}

        expect(harness.undoManager.canUndo());
        harness.undoManager.undo();

        expectEquals(readIntParameterValue(harness.apvts, parameterId), 20);
        expectEquals(static_cast<int>(harness.packedFrequencyByte()), 20);

        auto undoMsg = harness.queue.dequeue();
        expect(undoMsg.has_value());
        expect(sysExMatchesRemoteEdit(undoMsg->sysExData, static_cast<juce::uint8>(frequencyDesc->sysExId), 20));
        expect(harness.queue.isEmpty());

        harness.undoManager.redo();
        expectEquals(readIntParameterValue(harness.apvts, parameterId), 45);
        expectEquals(static_cast<int>(harness.packedFrequencyByte()), 45);

        auto redoMsg = harness.queue.dequeue();
        expect(redoMsg.has_value());
        expect(sysExMatchesRemoteEdit(redoMsg->sysExData, static_cast<juce::uint8>(frequencyDesc->sysExId), 45));
        expect(harness.queue.isEmpty());
    }

    void testNonEditorialPropertyDoesNotRecordUndo()
    {
        beginTest("Non-editorial setProperty(nullptr) does not grow undo stack");

        ProcessorPathHarness harness(makeDco1FrequencyLayout());
        harness.undoManager.clearUndoHistory();

        expect(!harness.undoManager.canUndo());
        harness.apvts.state.setProperty("navigationProbe", 1, nullptr);
        expect(!harness.undoManager.canUndo());
    }

    void testUndoRestoresApvtsWhenSysExDispatchDisabled()
    {
        beginTest("Undo restores APVTS/model when SysEx dispatch is disabled");

        const auto frequencyDesc = findDco1FrequencyDescriptor();
        expect(frequencyDesc.has_value());

        ProcessorPathHarness harness(makeDco1FrequencyLayout());
        harness.sysExDispatchEnabled_ = false;
        const auto& parameterId = frequencyDesc->parameterId;

        writeIntParameterValue(harness.apvts, parameterId, 15);
        harness.mapper.apvtsToBuffer();
        harness.undoManager.clearUndoHistory();

        simulateSliderDrag(harness.apvts, harness.undoManager, parameterId, 40);
        expect(harness.queue.isEmpty());

        harness.undoManager.undo();
        expectEquals(readIntParameterValue(harness.apvts, parameterId), 15);
        expectEquals(static_cast<int>(harness.packedFrequencyByte()), 15);
        expect(harness.queue.isEmpty());
    }

    void testOneTransactionMemoryFootprint()
    {
        beginTest("One slider drag records one undo transaction (memory sample)");

        const auto frequencyDesc = findDco1FrequencyDescriptor();
        expect(frequencyDesc.has_value());

        ProcessorPathHarness harness(makeDco1FrequencyLayout());
        const auto& parameterId = frequencyDesc->parameterId;

        writeIntParameterValue(harness.apvts, parameterId, 10);
        harness.undoManager.clearUndoHistory();

        simulateSliderDrag(harness.apvts, harness.undoManager, parameterId, 25);

        expectEquals(harness.undoManager.getUndoDescriptions().size(), 1);
        expect(harness.undoManager.canUndo());

        // Spike sample: one dco1Frequency drag → one undo transaction; description may be empty in JUCE default.
        const int undoUnits = harness.undoManager.getNumberOfUnitsTakenUpByStoredCommands();
        expect(undoUnits > 0);
        expect(undoUnits < 1024);
    }

    void testMultiWriteDragGroupsOneTransaction()
    {
        beginTest("Multiple writes inside one gesture form one undo transaction");

        const auto frequencyDesc = findDco1FrequencyDescriptor();
        expect(frequencyDesc.has_value());

        ProcessorPathHarness harness(makeDco1FrequencyLayout());
        const auto& parameterId = frequencyDesc->parameterId;

        writeIntParameterValue(harness.apvts, parameterId, 5);
        harness.mapper.apvtsToBuffer();
        harness.undoManager.clearUndoHistory();

        harness.undoManager.beginNewTransaction();
        if (auto* param = harness.apvts.getParameter(parameterId))
            param->beginChangeGesture();

        for (int value = 10; value <= 30; value += 5)
            writeIntParameterValue(harness.apvts, parameterId, value);

        if (auto* param = harness.apvts.getParameter(parameterId))
            param->endChangeGesture();

        expectEquals(readIntParameterValue(harness.apvts, parameterId), 30);
        expectEquals(harness.undoManager.getUndoDescriptions().size(), 1);

        harness.undoManager.undo();
        expectEquals(readIntParameterValue(harness.apvts, parameterId), 5);
    }
};

static UndoManagerApvtsSpikeTests undoManagerApvtsSpikeTests;
