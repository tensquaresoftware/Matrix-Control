#include <vector>

#include <juce_core/juce_core.h>

#include "Core/MIDI/EditorPath.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PackedFieldCodec.h"
#include "Core/Models/PatchModel.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"
#include "SysExWireCompliance.h"

// Descriptor-driven wire compliance for Remote Edit (0x06) Patch Edit parameters.
// Coverage: 96 emitters (62 int + 34 choice, sysExId != kNoSysExId across Patch Edit modules).
// Matrix Mod bus fields use opcode 0x0B — see MatrixModBusParameterSysExComplianceTests.
// Hardware audition (VCA/ENV velocity at -63/-9) remains manual smoke; this suite is CI-only.
class PatchParameterSysExComplianceTests : public juce::UnitTest
{
public:
    PatchParameterSysExComplianceTests() : juce::UnitTest("PatchParameterSysExCompliance") {}

    void runTest() override
    {
        testAllIntParametersWireCompliance();
        testAllChoiceParametersWireCompliance();
        testSignedWireRoundTripAllDescriptors();
        testRegressionNeg63AndNeg9();
    }

private:
    static std::vector<int> intTestValues(const PluginDescriptors::IntParameterDescriptor& descriptor)
    {
        if (descriptor.minValue < 0)
            return { descriptor.minValue, -1, 0, 1, descriptor.maxValue };

        const int mid = (descriptor.minValue + descriptor.maxValue) / 2;
        return { descriptor.minValue, mid, descriptor.maxValue };
    }

    static std::vector<int> choiceTestIndices(const PluginDescriptors::ChoiceParameterDescriptor& descriptor)
    {
        const int last = static_cast<int>(descriptor.choices.size()) - 1;
        const int mid = last / 2;
        return { 0, mid, last };
    }

    static juce::uint8 wireValueFromModel(const Core::PatchModel& model, int sysExOffset) noexcept
    {
        const size_t offset = Core::PackedFieldCodec::safeOffset(sysExOffset, Core::PatchModel::kBufferSize);
        return static_cast<juce::uint8>(model.data()[offset] & 0x7F);
    }

    struct RemoteEditDispatchContext
    {
        Core::PatchModel& model;
        Core::MidiOutboundQueue& queue;
        juce::String parameterId;
        juce::uint8 expectedParam;
        juce::uint8 expectedWireValue;
    };

    void dispatchAndExpectRemoteEdit(const RemoteEditDispatchContext& context)
    {
        Core::MidiActivityTracker tracker;
        SysExEncoder encoder;

        Core::PatchParameterSysExDispatcher dispatcher(
            context.model,
            [&](int parameterNumber, juce::uint8 packedValue)
            {
                Core::EditorPath editorPath(context.queue, tracker);
                editorPath.enqueueSysEx(encoder.encodeRemoteParameterEdit(
                    static_cast<juce::uint8>(parameterNumber),
                    packedValue));
            });

        dispatcher.dispatch(context.parameterId);

        auto msg = context.queue.dequeue();
        expect(msg.has_value(), context.parameterId);
        if (! msg.has_value())
            return;

        expect(SysExWireCompliance::assertValidSysExDataBytes(msg->sysExData), context.parameterId);
        expect(SysExWireCompliance::assertRemoteEditMatches(
                   msg->sysExData,
                   context.expectedParam,
                   context.expectedWireValue),
               context.parameterId);
        expect(context.queue.isEmpty(), context.parameterId);
    }

    void testAllIntParametersWireCompliance()
    {
        beginTest("Remote Edit 0x06 — all Patch Edit int parameters (62 emitters)");

        const auto descriptors = Core::ApvtsPatchMapper::buildIntDescriptors();
        int emitterCount = 0;

        for (const auto& descriptor : descriptors)
        {
            if (descriptor.sysExId == PluginDescriptors::kNoSysExId)
                continue;

            ++emitterCount;

            for (const int testValue : intTestValues(descriptor))
            {
                Core::PatchModel model;
                Core::MidiOutboundQueue queue;

                model.setValue(descriptor, testValue);

                const juce::uint8 expectedWire = wireValueFromModel(model, descriptor.sysExOffset);
                expect(expectedWire <= 0x7F);

                dispatchAndExpectRemoteEdit({
                    model,
                    queue,
                    descriptor.parameterId,
                    static_cast<juce::uint8>(descriptor.sysExId),
                    expectedWire });
            }
        }

        expectEquals(emitterCount, 62, "Patch Edit int emitters");
    }

    void testAllChoiceParametersWireCompliance()
    {
        beginTest("Remote Edit 0x06 — all Patch Edit choice parameters");

        const auto descriptors = Core::ApvtsPatchMapper::buildChoiceDescriptors();
        int emitterCount = 0;

        for (const auto& descriptor : descriptors)
        {
            if (descriptor.sysExId == PluginDescriptors::kNoSysExId)
                continue;

            ++emitterCount;

            for (const int choiceIndex : choiceTestIndices(descriptor))
            {
                Core::PatchModel model;
                Core::MidiOutboundQueue queue;

                model.setChoiceIndex(descriptor, choiceIndex);

                const juce::uint8 expectedWire = wireValueFromModel(model, descriptor.sysExOffset);
                expect(expectedWire <= 0x7F);

                dispatchAndExpectRemoteEdit({
                    model,
                    queue,
                    descriptor.parameterId,
                    static_cast<juce::uint8>(descriptor.sysExId),
                    expectedWire });
            }
        }

        expectEquals(emitterCount, 34, "Patch Edit choice emitters");
    }

    void testSignedWireRoundTripAllDescriptors()
    {
        beginTest("Signed int — wire round-trip for every Patch Edit emitter with minValue < 0");

        const auto descriptors = Core::ApvtsPatchMapper::buildIntDescriptors();
        int signedEmitterCount = 0;

        for (const auto& descriptor : descriptors)
        {
            if (descriptor.sysExId == PluginDescriptors::kNoSysExId || descriptor.minValue >= 0)
                continue;

            ++signedEmitterCount;

            for (int value = descriptor.minValue; value <= descriptor.maxValue; ++value)
            {
                const juce::uint8 packed = Core::PackedFieldCodec::encodeField(
                    value, descriptor.minValue, descriptor.maxValue);
                const juce::uint8 wire = static_cast<juce::uint8>(packed & 0x7F);

                expect(wire <= 0x7F);
                expectEquals(Core::PackedFieldCodec::decodeField(wire, descriptor.minValue, descriptor.maxValue),
                             value);
            }
        }

        expectEquals(signedEmitterCount, 19, "Patch Edit signed int emitters");
    }

    void runRegressionCase(const char* parameterId, int sysExId, int testValue, juce::uint8 expectedWire)
    {
        const auto intDescs = Core::ApvtsPatchMapper::buildIntDescriptors();
        const PluginDescriptors::IntParameterDescriptor* descriptor = nullptr;
        for (const auto& candidate : intDescs)
        {
            if (candidate.parameterId == parameterId)
            {
                descriptor = &candidate;
                break;
            }
        }

        expect(descriptor != nullptr, parameterId);
        if (descriptor == nullptr)
            return;

        expectEquals(descriptor->sysExId, sysExId, parameterId);

        Core::PatchModel model;
        Core::MidiOutboundQueue queue;
        model.setValue(*descriptor, testValue);

        dispatchAndExpectRemoteEdit({
            model,
            queue,
            parameterId,
            static_cast<juce::uint8>(sysExId),
            expectedWire });
    }

    void testRegressionNeg63AndNeg9()
    {
        beginTest("Regression — VCA1/ENV2 velocity mod wire bytes for -63 and -9");

        runRegressionCase(PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1ModByVelocity,
                          28, -63, 0x41);
        runRegressionCase(PluginIDs::PatchEditSection::VcfVcaModule::ParameterWidgets::kVca1ModByVelocity,
                          28, -9, 0x77);
        runRegressionCase(PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets::kAmplitudeModByVelocity,
                          66, -63, 0x41);
        runRegressionCase(PluginIDs::PatchEditSection::Envelope2Module::ParameterWidgets::kAmplitudeModByVelocity,
                          66, -9, 0x77);
    }
};

static PatchParameterSysExComplianceTests patchParameterSysExComplianceTests;
