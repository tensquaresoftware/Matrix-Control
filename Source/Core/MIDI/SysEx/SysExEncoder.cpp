#include <cstring>

#include "SysExEncoder.h"

#include "../Utilities/MidiLogger.h"

juce::MemoryBlock SysExEncoder::encodePatchSysEx(juce::uint8 patchNumber, const juce::uint8* packedData) const
{
    if (packedData == nullptr)
    {
        MidiLogger::getInstance().logError("encodePatchSysEx: null packedData pointer");
        return {};
    }

    // Build header: F0 10 06 01 <patch_number>
    auto header = buildHeader(SysExConstants::Opcode::kSinglePatchData, patchNumber);

    // Unpack bytes to nibbles
    std::vector<juce::uint8> nibbles(SysExConstants::kPatchPackedDataSize * 2);
    size_t numNibbles = unpackBytes(packedData, SysExConstants::kPatchPackedDataSize, nibbles.data());

    // Calculate checksum on packed data
    juce::uint8 checksum = calculateChecksum(packedData, SysExConstants::kPatchPackedDataSize);

    // Build complete message
    juce::MemoryBlock message;
    message.append(header.data(), header.size());
    message.append(nibbles.data(), numNibbles);
    message.append(&checksum, 1);
    message.append(&SysExConstants::kSysExEnd, 1);

    return message;
}

juce::MemoryBlock SysExEncoder::encodeMasterSysEx(juce::uint8 version, const juce::uint8* packedData) const
{
    if (packedData == nullptr)
    {
        MidiLogger::getInstance().logError("encodeMasterSysEx: null packedData pointer");
        return {};
    }

    // Build header: F0 10 06 03 <version>
    auto header = buildHeader(SysExConstants::Opcode::kMasterParameterData, version);

    // Unpack bytes to nibbles
    std::vector<juce::uint8> nibbles(SysExConstants::kMasterPackedDataSize * 2);
    size_t numNibbles = unpackBytes(packedData, SysExConstants::kMasterPackedDataSize, nibbles.data());

    // Calculate checksum on packed data
    juce::uint8 checksum = calculateChecksum(packedData, SysExConstants::kMasterPackedDataSize);

    // Build complete message
    juce::MemoryBlock message;
    message.append(header.data(), header.size());
    message.append(nibbles.data(), numNibbles);
    message.append(&checksum, 1);
    message.append(&SysExConstants::kSysExEnd, 1);

    return message;
}

juce::MemoryBlock SysExEncoder::encodeDeviceInquiry()
{
    juce::MemoryBlock message;
    message.append(SysExConstants::DeviceInquiry::kRequestMessage,
                   SysExConstants::DeviceInquiry::kRequestMessageLength);
    return message;
}

juce::MemoryBlock SysExEncoder::encodeRequestMessage(juce::uint8 requestType, juce::uint8 patchNumber) const
{
    auto header = buildHeader(SysExConstants::Opcode::kRequestData, requestType);

    juce::MemoryBlock message;
    message.append(header.data(), header.size());
    message.append(&patchNumber, 1);
    message.append(&SysExConstants::kSysExEnd, 1);

    return message;
}

size_t SysExEncoder::unpackBytes(const juce::uint8* bytes, size_t numBytes, juce::uint8* output)
{
    for (size_t i = 0; i < numBytes; ++i)
    {
        // Low nibble first, then high nibble (Oberheim format)
        output[i * 2] = bytes[i] & 0x0F;
        output[i * 2 + 1] = (bytes[i] >> 4) & 0x0F;
    }
    return numBytes * 2;
}

juce::uint8 SysExEncoder::calculateChecksum(const juce::uint8* data, size_t length)
{
    juce::uint8 checksum = 0;
    for (size_t i = 0; i < length; ++i)
    {
        checksum += data[i];
    }
    return checksum & 0x7F;
}

std::vector<juce::uint8> SysExEncoder::buildHeader(juce::uint8 opcode, juce::uint8 headerData) const
{
    std::vector<juce::uint8> header;
    header.push_back(SysExConstants::kSysExStart);
    header.push_back(SysExConstants::kManufacturerIdOberheim);
    header.push_back(SysExConstants::kDeviceIdMatrix1000);
    header.push_back(opcode);
    if (headerData != 0 || opcode == SysExConstants::Opcode::kSinglePatchData ||
        opcode == SysExConstants::Opcode::kMasterParameterData ||
        opcode == SysExConstants::Opcode::kRequestData)
    {
        header.push_back(headerData);
    }
    return header;
}

