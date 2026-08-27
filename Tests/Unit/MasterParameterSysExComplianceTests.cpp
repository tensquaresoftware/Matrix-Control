#include <vector>

#include <juce_core/juce_core.h>

#include "Core/MIDI/EditorPath.h"
#include "Core/MIDI/MasterParameterSysExDispatcher.h"
#include "Core/MIDI/MidiActivityTracker.h"
#include "Core/MIDI/Queue/MidiOutboundQueue.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Core/Models/ApvtsMasterMapper.h"
#include "Core/Models/MasterModel.h"
#include "Core/Models/PackedFieldCodec.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "SysExWireCompliance.h"

// Descriptor-driven wire compliance for Master parameter data (0x03).
// Coverage: 22 Master parameters (each dispatches the full master buffer).
class MasterParameterSysExComplianceTests : public juce::UnitTest
{
public:
    MasterParameterSysExComplianceTests() : juce::UnitTest("MasterParameterSysExCompliance") {}

    void runTest() override
    {
        testAllIntParametersWireCompliance();
        testAllChoiceParametersWireCompliance();
        testSignedWireRoundTripAllDescriptors();
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

    void dispatchAndExpectMaster(Core::MasterModel& model,
                                 Core::MidiOutboundQueue& queue,
                                 const juce::String& parameterId,
                                 juce::uint8 masterVersion)
    {
        Core::MidiActivityTracker tracker;
        SysExEncoder encoder;

        Core::MasterParameterSysExDispatcher dispatcher(
            model,
            [&](const juce::uint8* packedData)
            {
                Core::EditorPath editorPath(queue, tracker);
                editorPath.enqueueSysEx(encoder.encodeMasterSysEx(masterVersion, packedData));
            });

        dispatcher.dispatch(parameterId);

        auto msg = queue.dequeue();
        expect(msg.has_value(), parameterId);
        if (! msg.has_value())
            return;

        expect(SysExWireCompliance::assertValidSysExDataBytes(msg->sysExData), parameterId);
        expect(SysExWireCompliance::assertMasterMessageMatches(msg->sysExData, masterVersion), parameterId);

        const auto expected = encoder.encodeMasterSysEx(masterVersion, model.data());
        expect(msg->sysExData == expected, parameterId);
        expect(queue.isEmpty(), parameterId);
    }

    void testAllIntParametersWireCompliance()
    {
        beginTest("Master 0x03 — all int parameters (wire-safe full buffer)");

        constexpr juce::uint8 kMasterVersion = 0x03;

        const auto descriptors = Core::ApvtsMasterMapper::buildIntDescriptors();
        int parameterCount = 0;

        for (const auto& descriptor : descriptors)
        {
            ++parameterCount;

            for (const int testValue : intTestValues(descriptor))
            {
                Core::MasterModel model;
                Core::MidiOutboundQueue queue;
                model.setValue(descriptor, testValue);

                dispatchAndExpectMaster(model, queue, descriptor.parameterId, kMasterVersion);
            }
        }

        expectEquals(parameterCount, 11, "Master int parameters");
    }

    void testAllChoiceParametersWireCompliance()
    {
        beginTest("Master 0x03 — all choice parameters (wire-safe full buffer)");

        constexpr juce::uint8 kMasterVersion = 0x03;

        const auto descriptors = Core::ApvtsMasterMapper::buildChoiceDescriptors();
        int parameterCount = 0;

        for (const auto& descriptor : descriptors)
        {
            ++parameterCount;

            for (const int choiceIndex : choiceTestIndices(descriptor))
            {
                Core::MasterModel model;
                Core::MidiOutboundQueue queue;
                model.setChoiceIndex(descriptor, choiceIndex);

                dispatchAndExpectMaster(model, queue, descriptor.parameterId, kMasterVersion);
            }
        }

        expectEquals(parameterCount, 11, "Master choice parameters");
    }

    void testSignedWireRoundTripAllDescriptors()
    {
        beginTest("Master signed int — wire round-trip for every minValue < 0 field");

        const auto descriptors = Core::ApvtsMasterMapper::buildIntDescriptors();
        int signedCount = 0;

        for (const auto& descriptor : descriptors)
        {
            if (descriptor.minValue >= 0)
                continue;

            ++signedCount;

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

        expectEquals(signedCount, 4, "Master signed int parameters");
    }
};

static MasterParameterSysExComplianceTests masterParameterSysExComplianceTests;
