#include "SysExEncoder.h"
#include <cstring>

juce::MemoryBlock SysExEncoder::encodePatchSysEx(uint8_t patchNumber, const uint8_t* packedData) const
{
    if (packedData == nullptr)
    {
        return {};
    }

    // Build header: F0 10 06 01 <patch_number>
    auto header = buildHeader(SysExConstants::Opcode::kSinglePatchData, patchNumber);

    // Unpack bytes to nibbles
    std::vector<uint8_t> nibbles(SysExConstants::kPatchPackedDataSize * 2);
    size_t numNibbles = unpackBytes(packedData, SysExConstants::kPatchPackedDataSize, nibbles.data());

    // Calculate checksum on packed data
    uint8_t checksum = calculateChecksum(packedData, SysExConstants::kPatchPackedDataSize);

    // Build complete message
    juce::MemoryBlock message;
    message.append(header.data(), header.size());
    message.append(nibbles.data(), numNibbles);
    message.append(&checksum, 1);
    message.append(&SysExConstants::kSysExEnd, 1);

    return message;
}

juce::MemoryBlock SysExEncoder::encodeMasterSysEx(uint8_t version, const uint8_t* packedData) const
{
    if (packedData == nullptr)
    {
        return {};
    }

    // Build header: F0 10 06 03 <version>
    auto header = buildHeader(SysExConstants::Opcode::kMasterParameterData, version);

    // Unpack bytes to nibbles
    std::vector<uint8_t> nibbles(SysExConstants::kMasterPackedDataSize * 2);
    size_t numNibbles = unpackBytes(packedData, SysExConstants::kMasterPackedDataSize, nibbles.data());

    // Calculate checksum on packed data
    uint8_t checksum = calculateChecksum(packedData, SysExConstants::kMasterPackedDataSize);

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

juce::MemoryBlock SysExEncoder::encodeRequestMessage(uint8_t requestType, uint8_t patchNumber) const
{
    // Build header: F0 10 06 04 <request_type> <patch_number> F7
    auto header = buildHeader(SysExConstants::Opcode::kRequestData, requestType);

    juce::MemoryBlock message;
    message.append(header.data(), header.size());
    message.append(&patchNumber, 1);
    message.append(&SysExConstants::kSysExEnd, 1);

    return message;
}

size_t SysExEncoder::unpackBytes(const uint8_t* bytes, size_t numBytes, uint8_t* output)
{
    for (size_t i = 0; i < numBytes; ++i)
    {
        // Low nibble first, then high nibble (Oberheim format)
        output[i * 2] = bytes[i] & 0x0F;
        output[i * 2 + 1] = (bytes[i] >> 4) & 0x0F;
    }
    return numBytes * 2;
}

uint8_t SysExEncoder::calculateChecksum(const uint8_t* data, size_t length)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i)
    {
        checksum += data[i];
    }
    return checksum & 0x7F;
}

std::vector<uint8_t> SysExEncoder::buildHeader(uint8_t opcode, uint8_t headerData) const
{
    std::vector<uint8_t> header;
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

