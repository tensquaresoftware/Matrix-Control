#include "SysExDecoder.h"
#include "SysExConstants.h"

SysExDecoder::SysExDecoder(SysExParser& parserRef)
    : parser(parserRef)
{
}

bool SysExDecoder::decodePatchSysEx(const juce::MemoryBlock& sysEx, uint8_t* output) const
{
    if (output == nullptr)
    {
        return false;
    }

    // Validate message first
    auto validation = parser.validateSysEx(sysEx);
    if (!validation.isValid || validation.messageType != SysExParser::MessageType::kPatch)
    {
        return false;
    }

    // Extract packed data: F0 10 06 01 <patch_number> <nibbles...> <checksum> F7
    // Data starts at index 5 (after F0 10 06 01 <patch_number>)
    return extractPackedData(sysEx, 5, SysExConstants::kPatchPackedDataSize, output);
}

bool SysExDecoder::decodeMasterSysEx(const juce::MemoryBlock& sysEx, uint8_t* output) const
{
    if (output == nullptr)
    {
        return false;
    }

    // Validate message first
    auto validation = parser.validateSysEx(sysEx);
    if (!validation.isValid || validation.messageType != SysExParser::MessageType::kMaster)
    {
        return false;
    }

    // Extract packed data: F0 10 06 03 <version> <nibbles...> <checksum> F7
    // Data starts at index 5 (after F0 10 06 03 <version>)
    return extractPackedData(sysEx, 5, SysExConstants::kMasterPackedDataSize, output);
}

DeviceIdInfo SysExDecoder::decodeDeviceId(const juce::MemoryBlock& sysEx) const
{
    DeviceIdInfo info = {};
    info.isValid = false;

    if (sysEx.getSize() < 15)
    {
        return info;
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());

    // Validate structure: F0 7E <chan> 06 02 10 06 00 02 00 <rev-0> <rev-1> <rev-2> <rev-3> F7
    if (data[0] != SysExConstants::kSysExStart ||
        data[1] != SysExConstants::DeviceInquiry::kUniversalNonRealtimeId ||
        data[3] != SysExConstants::DeviceInquiry::kSubIdGeneralInfo ||
        data[4] != SysExConstants::DeviceInquiry::kSubIdDeviceIdReply)
    {
        return info;
    }

    // Extract device information
    info.manufacturerId = data[5];
    info.familyLow = data[6];
    info.familyHigh = data[7];
    info.memberLow = data[8];
    info.memberHigh = data[9];

    // Extract version (4 bytes, typically ASCII)
    if (sysEx.getSize() >= 15)
    {
        char versionStr[5] = {0};
        versionStr[0] = static_cast<char>(data[10]);
        versionStr[1] = static_cast<char>(data[11]);
        versionStr[2] = static_cast<char>(data[12]);
        versionStr[3] = static_cast<char>(data[13]);
        info.version = juce::String(versionStr).trim();
    }

    // Validate Matrix-1000
    info.isValid = (info.manufacturerId == SysExConstants::DeviceInquiry::kExpectedManufacturer &&
                    info.familyLow == SysExConstants::DeviceInquiry::kExpectedFamily &&
                    info.memberLow == SysExConstants::DeviceInquiry::kExpectedMemberLow &&
                    info.memberHigh == SysExConstants::DeviceInquiry::kExpectedMemberHigh);

    return info;
}

size_t SysExDecoder::packNibbles(const uint8_t* nibbles, size_t numNibbles, uint8_t* output)
{
    if (nibbles == nullptr || output == nullptr || numNibbles % 2 != 0)
    {
        return 0;
    }

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

bool SysExDecoder::extractPackedData(const juce::MemoryBlock& sysEx,
                                    size_t dataStartIndex,
                                    size_t expectedPackedSize,
                                    uint8_t* output) const
{
    if (output == nullptr || sysEx.getSize() < dataStartIndex + expectedPackedSize * 2 + 2)
    {
        return false;
    }

    const auto* data = static_cast<const uint8_t*>(sysEx.getData());
    size_t totalSize = sysEx.getSize();

    // Checksum is second-to-last byte (before F7)
    size_t checksumIndex = totalSize - 2;
    size_t numNibbles = checksumIndex - dataStartIndex;

    if (numNibbles != expectedPackedSize * 2)
    {
        return false;
    }

    // Pack nibbles into bytes
    size_t packedBytes = packNibbles(&data[dataStartIndex], numNibbles, output);
    return packedBytes == expectedPackedSize;
}

