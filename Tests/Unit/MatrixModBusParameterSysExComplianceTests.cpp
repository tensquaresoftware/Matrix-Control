#include <vector>

#include <juce_core/juce_core.h>

#include "Core/MIDI/EditorPath.h"
#include "Core/MIDI/MatrixModBusParameterSysExDispatcher.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Core/Models/PackedFieldCodec.h"
#include "Core/Models/PatchModel.h"
#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "SysExWireCompliance.h"

// Descriptor-driven wire compliance for Matrix Mod bus edit (0x0B).
// Coverage: 30 dispatch points (10 buses x source/amount/destination).
class MatrixModBusParameterSysExComplianceTests : public juce::UnitTest
{
public:
    MatrixModBusParameterSysExComplianceTests()
        : juce::UnitTest("MatrixModBusParameterSysExCompliance")
    {
    }

    void runTest() override
    {
        testAllBusesWireCompliance();
        testSignedAmountWireRoundTrip();
        testRegressionNeg63Amount();
        testRegressionNeg9Amount();
    }

private:
    struct BusConfiguration
    {
        int busIndex;
        int sourceIndex;
        int amount;
        int destinationIndex;
    };

    static void configureBus(Core::PatchModel& model, const BusConfiguration& configuration)
    {
        using namespace PluginDescriptors::MatrixModulationSection;

        const auto& intParams = kModulationBusIntParameters[static_cast<size_t>(configuration.busIndex)];
        const auto& choiceParams = kModulationBusChoiceParameters[static_cast<size_t>(configuration.busIndex)];

        model.setChoiceIndex(choiceParams[0], configuration.sourceIndex);
        model.setValue(intParams[0], configuration.amount);
        model.setChoiceIndex(choiceParams[1], configuration.destinationIndex);
    }

    static juce::uint8 wireByteFromModel(const Core::PatchModel& model, int sysExOffset) noexcept
    {
        const size_t offset = Core::PackedFieldCodec::safeOffset(sysExOffset, Core::PatchModel::kBufferSize);
        return static_cast<juce::uint8>(model.data()[offset] & 0x7F);
    }

    static juce::uint8 wireAmountFromModel(const Core::PatchModel& model, int busIndex) noexcept
    {
        using namespace PluginDescriptors::MatrixModulationSection;
        return wireByteFromModel(model, kModulationBusIntParameters[static_cast<size_t>(busIndex)][0].sysExOffset);
    }

    struct MatrixModDispatchContext
    {
        Core::PatchModel& model;
        Core::MidiOutboundQueue& queue;
        juce::String parameterId;
        SysExWireCompliance::MatrixModBusWireExpectation expected;
    };

    void dispatchBusParameterAndExpect(const MatrixModDispatchContext& context)
    {
        Core::MidiActivityTracker tracker;
        SysExEncoder encoder;

        Core::MatrixModBusParameterSysExDispatcher dispatcher(
            context.model,
            [&](juce::uint8 bus, juce::uint8 source, juce::uint8 amount, juce::uint8 destination)
            {
                Core::EditorPath editorPath(context.queue, tracker);
                editorPath.enqueueSysEx(encoder.encodeMatrixModBusEdit(bus, source, amount, destination));
            });

        dispatcher.dispatch(context.parameterId);

        auto msg = context.queue.dequeue();
        expect(msg.has_value(), context.parameterId);
        if (! msg.has_value())
            return;

        expect(SysExWireCompliance::assertValidSysExDataBytes(msg->sysExData), context.parameterId);
        expect(SysExWireCompliance::assertMatrixModBusEditMatches(msg->sysExData, context.expected),
               context.parameterId);
        expect(context.queue.isEmpty(), context.parameterId);
    }

    void testAllBusesWireCompliance()
    {
        beginTest("Matrix Mod 0x0B — all buses and dispatch fields (30 dispatch points)");

        using namespace PluginDescriptors::MatrixModulationSection;

        int dispatchPointCount = 0;

        for (int bus = 0; bus < Matrix1000Limits::kModulationBusCount; ++bus)
        {
            const auto& intParams = kModulationBusIntParameters[static_cast<size_t>(bus)];
            const auto& choiceParams = kModulationBusChoiceParameters[static_cast<size_t>(bus)];

            const BusConfiguration configuration {
                bus,
                bus % choiceParams[0].choices.size(),
                (bus % 2 == 0) ? -63 : 63,
                (bus * 3) % choiceParams[1].choices.size(),
            };

            Core::PatchModel model;
            configureBus(model, configuration);

            const SysExWireCompliance::MatrixModBusWireExpectation expected {
                static_cast<juce::uint8>(bus),
                wireByteFromModel(model, choiceParams[0].sysExOffset),
                wireAmountFromModel(model, bus),
                wireByteFromModel(model, choiceParams[1].sysExOffset),
            };

            const std::vector<juce::String> parameterIds {
                intParams[0].parameterId,
                choiceParams[0].parameterId,
                choiceParams[1].parameterId,
            };

            for (const auto& parameterId : parameterIds)
            {
                Core::MidiOutboundQueue queue;
                dispatchBusParameterAndExpect({ model, queue, parameterId, expected });
                ++dispatchPointCount;
            }
        }

        expectEquals(dispatchPointCount, 30, "Matrix Mod dispatch points");
    }

    void testSignedAmountWireRoundTrip()
    {
        beginTest("Matrix Mod amount — signed wire round-trip [-63, 63]");

        using namespace PluginDescriptors::MatrixModulationSection;
        const auto& amountDescriptor = kModulationBusIntParameters[0][0];

        for (int value = amountDescriptor.minValue; value <= amountDescriptor.maxValue; ++value)
        {
            const juce::uint8 packed = Core::PackedFieldCodec::encodeField(
                value, amountDescriptor.minValue, amountDescriptor.maxValue);
            const juce::uint8 wire = static_cast<juce::uint8>(packed & 0x7F);

            expect(wire <= 0x7F);
            expectEquals(Core::PackedFieldCodec::decodeField(
                             wire, amountDescriptor.minValue, amountDescriptor.maxValue),
                         value);
        }
    }

    void testRegressionNeg63Amount()
    {
        beginTest("Regression — bus 0 amount -63 wire byte 0x41 (not 0xC1)");

        Core::PatchModel model;
        Core::MidiOutboundQueue queue;
        configureBus(model, { 0, 1, -63, 2 });

        dispatchBusParameterAndExpect({
            model,
            queue,
            PluginDescriptors::MatrixModulationSection::kModulationBusIntParameters[0][0].parameterId,
            { 0, 1, 0x41, 2 },
        });
    }

    void testRegressionNeg9Amount()
    {
        beginTest("Regression — bus 0 amount -9 wire byte 0x77 (not 0xF7 EOX)");

        Core::PatchModel model;
        Core::MidiOutboundQueue queue;
        configureBus(model, { 0, 1, -9, 2 });

        dispatchBusParameterAndExpect({
            model,
            queue,
            PluginDescriptors::MatrixModulationSection::kModulationBusIntParameters[0][0].parameterId,
            { 0, 1, 0x77, 2 },
        });
    }
};

static MatrixModBusParameterSysExComplianceTests matrixModBusParameterSysExComplianceTests;
