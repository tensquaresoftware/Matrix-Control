#pragma once

#include <cstdint>

namespace SysExConstants
{
    // SysEx message delimiters
    constexpr uint8_t kSysExStart = 0xF0;
    constexpr uint8_t kSysExEnd = 0xF7;

    // Manufacturer and Device IDs
    constexpr uint8_t kManufacturerIdOberheim = 0x10;
    constexpr uint8_t kDeviceIdMatrix1000 = 0x06;

    // SysEx opcodes for Matrix-1000
    namespace Opcode
    {
        constexpr uint8_t kSinglePatchData = 0x01;
        constexpr uint8_t kSplitPatchData = 0x02;  // Matrix-6 compatibility
        constexpr uint8_t kMasterParameterData = 0x03;
        constexpr uint8_t kRequestData = 0x04;
        constexpr uint8_t kRemoteParameterEdit = 0x06;
        constexpr uint8_t kSetGroupMode = 0x07;
        constexpr uint8_t kSetBank = 0x0A;
        constexpr uint8_t kRemoteParameterEditMatrix = 0x0B;
    }

    // Request types (for opcode 0x04)
    namespace RequestType
    {
        constexpr uint8_t kRequestAllBank = 0x00;  // Not implemented in v1.0
        constexpr uint8_t kRequestSinglePatch = 0x01;
        constexpr uint8_t kRequestSplitPatch = 0x02;  // Matrix-6 compatibility
        constexpr uint8_t kRequestMasterParameters = 0x03;
        constexpr uint8_t kRequestEditBuffer = 0x04;
    }

    // SysEx message lengths (total transmitted bytes)
    constexpr size_t kPatchMessageLength = 275;  // 134 packed bytes = 268 nibbles + 5 header + 1 checksum + 1 EOX
    constexpr size_t kMasterMessageLength = 351;  // 172 packed bytes = 344 nibbles + 5 header + 1 checksum + 1 EOX
    constexpr size_t kSplitPatchMessageLength = 36 + 5 + 1 + 1;  // 36 bytes + header + checksum + EOX

    // Packed data sizes (before unpacking to nibbles)
    constexpr size_t kPatchPackedDataSize = 134;
    constexpr size_t kMasterPackedDataSize = 172;

    // Minimum SysEx message length (header + checksum + EOX)
    constexpr size_t kMinSysExLength = 7;  // F0 10 06 <opcode> <data...> <checksum> F7

    // Timeout values
    constexpr int kDefaultTimeoutMs = 2000;  // 2 seconds timeout for MIDI operations
    constexpr int kMinSysExDelayMs = 10;     // Minimum delay between SysEx messages

    // Device Inquiry (Universal SysEx)
    namespace DeviceInquiry
    {
        constexpr uint8_t kUniversalNonRealtimeId = 0x7E;
        constexpr uint8_t kDeviceIdAll = 0x7F;
        constexpr uint8_t kSubIdGeneralInfo = 0x06;
        constexpr uint8_t kSubIdDeviceIdRequest = 0x01;
        constexpr uint8_t kSubIdDeviceIdReply = 0x02;

        // Device Inquiry request message: F0 7E 7F 06 01 F7
        constexpr uint8_t kRequestMessage[] = { 0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 };
        constexpr size_t kRequestMessageLength = 6;

        // Expected response format: F0 7E <chan> 06 02 10 06 00 02 00 <rev-0> <rev-1> <rev-2> <rev-3> F7
        // Manufacturer: 0x10 (Oberheim)
        // Family: 0x06 (Matrix series)
        // Member: 0x00 0x02 (Matrix-1000)
        constexpr uint8_t kExpectedManufacturer = 0x10;
        constexpr uint8_t kExpectedFamily = 0x06;
        constexpr uint8_t kExpectedMemberLow = 0x00;
        constexpr uint8_t kExpectedMemberHigh = 0x02;
    }
}

