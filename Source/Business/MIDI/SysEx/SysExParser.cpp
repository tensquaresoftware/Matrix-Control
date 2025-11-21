#include "SysExParser.h"
#include "SysExConstants.h"
#include <vector>

SysExParser::ValidationResult SysExParser::validateSysEx(const juce::MemoryBlock& sysEx) const
{
    // Step 1: Validate structure
    if (!validateStructure(sysEx))
    {
        return ValidationResult::failure("Invalid SysEx structure: missing F0 or F7");
    }

    // Step 2: Validate Manufacturer and Device ID
    if (!validateManufacturerAndDevice(sysEx))
    {
        return ValidationResult::failure("Invalid Manufacturer ID or Device ID");
    }

    // Step 3: Validate message type and length
    auto typeResult = validateMessageType(sysEx);
    if (!typeResult.isValid)
    {
        return typeResult;
    }

    // Step 4: Validate checksum
    if (!validateChecksum(sysEx))
    {
        return ValidationResult::failure("Invalid checksum");
    }

    return ValidationResult::success(typeResult.messageType);
}

bool SysExParser::validateStructure(const juce::MemoryBlock& sysEx) const
{
    if (sysEx.getSize() < SysExConstants::kMinSysExLength)
    {
        return false;
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());
    
    // Check start byte
    if (data[0] != SysExConstants::kSysExStart)
    {
        return false;
    }

    // Check end byte
    if (data[sysEx.getSize() - 1] != SysExConstants::kSysExEnd)
    {
        return false;
    }

    return true;
}

bool SysExParser::validateChecksum(const juce::MemoryBlock& sysEx) const
{
    if (sysEx.getSize() < SysExConstants::kMinSysExLength)
    {
        return false;
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());
    size_t totalSize = sysEx.getSize();

    // Device ID messages don't have Oberheim checksum
    if (data[0] == SysExConstants::kSysExStart &&
        data[1] == SysExConstants::DeviceInquiry::kUniversalNonRealtimeId)
    {
        return true;  // Device ID messages don't use Oberheim checksum
    }

    // Standard Oberheim messages: F0 10 06 <opcode> [optional data]
    // For patch/master messages: F0 10 06 01/03 <patch_number or version> <nibbles...> <checksum> F7
    // Checksum is calculated on packed data (before unpacking to nibbles)
    
    // Find where data starts (after header)
    size_t headerSize = 4;  // F0 10 06 <opcode>
    if (totalSize <= headerSize + 2)  // Need at least checksum + F7
    {
        return false;
    }

    // For patch messages: header is F0 10 06 01 <patch_number>
    // For master messages: header is F0 10 06 03 <version>
    // So data starts at index 5
    size_t dataStartIndex = 5;
    
    // Checksum is second-to-last byte (before F7)
    size_t checksumIndex = totalSize - 2;
    uint8_t receivedChecksum = data[checksumIndex];

    // Extract nibbles (between header and checksum)
    size_t numNibbles = checksumIndex - dataStartIndex;
    if (numNibbles == 0 || (numNibbles % 2 != 0))
    {
        return false;  // Must have even number of nibbles
    }

    // Pack nibbles back to bytes for checksum calculation
    size_t numPackedBytes = numNibbles / 2;
    std::vector<uint8_t> packedData(numPackedBytes);
    packNibbles(&data[dataStartIndex], numNibbles, packedData.data());

    // Calculate checksum
    uint8_t calculatedChecksum = calculateChecksum(packedData.data(), numPackedBytes);

    return (calculatedChecksum & 0x7F) == receivedChecksum;
}

SysExParser::ValidationResult SysExParser::validateMessageType(const juce::MemoryBlock& sysEx) const
{
    if (sysEx.getSize() < 4)
    {
        return ValidationResult::failure("Message too short");
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());

    // Check for Device ID response (Universal SysEx: F0 7E <chan> 06 02)
    if (data[0] == SysExConstants::kSysExStart &&
        data[1] == SysExConstants::DeviceInquiry::kUniversalNonRealtimeId &&
        data[3] == SysExConstants::DeviceInquiry::kSubIdGeneralInfo &&
        data[4] == SysExConstants::DeviceInquiry::kSubIdDeviceIdReply)
    {
        // Device ID response: variable length, minimum check
        if (sysEx.getSize() < 15)  // Minimum expected length
        {
            return ValidationResult::failure("Device ID message too short");
        }
        return ValidationResult::success(MessageType::kDeviceId);
    }

    // Standard Oberheim messages: F0 10 06 <opcode>
    if (data[1] != SysExConstants::kManufacturerIdOberheim ||
        data[2] != SysExConstants::kDeviceIdMatrix1000)
    {
        return ValidationResult::failure("Not an Oberheim Matrix-1000 message");
    }

    uint8_t opcode = data[3];
    MessageType messageType = getMessageTypeFromOpcode(opcode);

    // Validate length according to message type
    size_t expectedLength = 0;
    switch (messageType)
    {
        case MessageType::kPatch:
            expectedLength = SysExConstants::kPatchMessageLength;
            break;
        case MessageType::kMaster:
            expectedLength = SysExConstants::kMasterMessageLength;
            break;
        case MessageType::kSplitPatch:
            expectedLength = SysExConstants::kSplitPatchMessageLength;
            break;
        case MessageType::kUnknown:
            return ValidationResult::failure("Unknown message type");
        case MessageType::kDeviceId:
            // Already handled above
            break;
    }

    if (sysEx.getSize() != expectedLength)
    {
        return ValidationResult::failure(
            juce::String("Invalid message length: expected ") + juce::String(expectedLength) +
            ", got " + juce::String(sysEx.getSize()));
    }

    return ValidationResult::success(messageType);
}

uint8_t SysExParser::calculateChecksum(const uint8_t* data, size_t length)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i)
    {
        checksum += data[i];
    }
    return checksum & 0x7F;
}

size_t SysExParser::packNibbles(const uint8_t* nibbles, size_t numNibbles, uint8_t* output)
{
    size_t numBytes = numNibbles / 2;
    for (size_t i = 0; i < numBytes; ++i)
    {
        // Low nibble first, then high nibble (Oberheim format)
        uint8_t lowNibble = nibbles[i * 2] & 0x0F;
        uint8_t highNibble = nibbles[i * 2 + 1] & 0x0F;
        output[i] = static_cast<uint8_t>(lowNibble | (highNibble << 4));
    }
    return numBytes;
}

bool SysExParser::validateManufacturerAndDevice(const juce::MemoryBlock& sysEx) const
{
    if (sysEx.getSize() < 4)
    {
        return false;
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());

    // Device ID messages (Universal SysEx) don't have Oberheim manufacturer ID
    if (data[0] == SysExConstants::kSysExStart &&
        data[1] == SysExConstants::DeviceInquiry::kUniversalNonRealtimeId)
    {
        // For Device ID, validate later in validateMessageType
        return true;
    }

    // Check Manufacturer ID (byte 1)
    if (data[1] != SysExConstants::kManufacturerIdOberheim)
    {
        return false;
    }

    // Check Device ID (byte 2)
    if (data[2] != SysExConstants::kDeviceIdMatrix1000)
    {
        return false;
    }

    return true;
}

SysExParser::MessageType SysExParser::getMessageTypeFromOpcode(uint8_t opcode)
{
    switch (opcode)
    {
        case SysExConstants::Opcode::kSinglePatchData:
            return MessageType::kPatch;
        case SysExConstants::Opcode::kMasterParameterData:
            return MessageType::kMaster;
        case SysExConstants::Opcode::kSplitPatchData:
            return MessageType::kSplitPatch;
        default:
            return MessageType::kUnknown;
    }
}

