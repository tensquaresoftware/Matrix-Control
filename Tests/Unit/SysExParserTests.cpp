#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/MIDI/SysEx/SysExParser.h"

/**
 * Unit tests for SysExParser
 *
 * Run with: ./Matrix-Control_Standalone --unit-tests
 * Or integrate with JUCE UnitTest framework
 */
class SysExParserTests : public juce::UnitTest
{
public:
    SysExParserTests() : juce::UnitTest("SysExParser Tests") {}

    void runTest() override
    {
        testValidPatchStructure();
        testMissingEndByte();
        testMissingStartByte();
        testWrongManufacturer();
        testDeviceIdMessage();
    }

private:
    static void appendByte(juce::MemoryBlock& block, juce::uint8 value)
    {
        block.append(&value, 1);
    }

    void testValidPatchStructure()
    {
        beginTest("Valid patch SysEx structure");

        // Minimal patch header: F0 10 06 01 05 — incomplete payload must fail structure checks.
        juce::MemoryBlock validPatch;
        appendByte(validPatch, SysExConstants::kSysExStart);
        appendByte(validPatch, SysExConstants::kManufacturerIdOberheim);
        appendByte(validPatch, SysExConstants::kDeviceIdMatrix1000);
        appendByte(validPatch, SysExConstants::Opcode::kSinglePatchData);
        appendByte(validPatch, 5);

        SysExParser parser;
        expect(!parser.validateStructure(validPatch),
               "Incomplete message should fail structure validation");
    }

    void testMissingEndByte()
    {
        beginTest("Invalid SysEx structure (missing F7)");

        juce::MemoryBlock invalidSysEx;
        appendByte(invalidSysEx, SysExConstants::kSysExStart);
        appendByte(invalidSysEx, SysExConstants::kManufacturerIdOberheim);
        appendByte(invalidSysEx, SysExConstants::kDeviceIdMatrix1000);

        SysExParser parser;
        expect(!parser.validateStructure(invalidSysEx),
               "Message without F7 should fail structure validation");
    }

    void testMissingStartByte()
    {
        beginTest("Invalid SysEx structure (missing F0)");

        juce::MemoryBlock invalidSysEx;
        appendByte(invalidSysEx, SysExConstants::kManufacturerIdOberheim);
        appendByte(invalidSysEx, SysExConstants::kDeviceIdMatrix1000);
        appendByte(invalidSysEx, SysExConstants::kSysExEnd);

        SysExParser parser;
        expect(!parser.validateStructure(invalidSysEx),
               "Message without F0 should fail structure validation");
    }

    void testWrongManufacturer()
    {
        beginTest("Oberheim manufacturer ID validation");

        juce::MemoryBlock wrongManufacturer;
        appendByte(wrongManufacturer, SysExConstants::kSysExStart);
        appendByte(wrongManufacturer, 0x20);  // Wrong manufacturer
        appendByte(wrongManufacturer, SysExConstants::kDeviceIdMatrix1000);
        appendByte(wrongManufacturer, SysExConstants::Opcode::kSinglePatchData);
        appendByte(wrongManufacturer, 5);
        appendByte(wrongManufacturer, SysExConstants::kSysExEnd);

        SysExParser parser;
        const auto result = parser.validateSysEx(wrongManufacturer);
        expect(!result.isValid, "Wrong manufacturer should fail validation");
        expect(result.errorMessage.contains("Manufacturer")
                   || result.errorMessage.contains("Device"),
               "Error should mention manufacturer or device");
    }

    void testDeviceIdMessage()
    {
        beginTest("Device ID message validation");

        // Device ID response: F0 7E <chan> 06 02 10 06 00 02 00 <rev...> F7
        juce::MemoryBlock deviceId;
        appendByte(deviceId, SysExConstants::kSysExStart);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kUniversalNonRealtimeId);
        appendByte(deviceId, 0x7F);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kSubIdGeneralInfo);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kSubIdDeviceIdReply);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kExpectedManufacturer);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kExpectedFamily);
        appendByte(deviceId, 0x00);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kExpectedMemberLow);
        appendByte(deviceId, SysExConstants::DeviceInquiry::kExpectedMemberHigh);
        const juce::uint8 revision[4] = {0x20, 0x31, 0x31, 0x30};  // " 1.10"
        deviceId.append(revision, 4);
        appendByte(deviceId, SysExConstants::kSysExEnd);

        SysExParser parser;
        const auto result = parser.validateMessageType(deviceId);
        expect(result.isValid, "Valid Device ID message should pass validation");
        expect(result.messageType == SysExParser::MessageType::kDeviceId,
               "Message type should be DeviceId");
    }
};

static SysExParserTests sysExParserTests;
