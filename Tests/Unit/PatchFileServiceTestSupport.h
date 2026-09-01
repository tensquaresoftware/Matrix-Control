#pragma once

#include <cstring>

#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchMutator/MutationNaming.h"

namespace PatchFileServiceTestSupport
{
    inline void expectEditBufferPatchHeader(juce::UnitTest& test, const juce::MemoryBlock& sysEx)
    {
        test.expectEquals(static_cast<int>(sysEx.getSize()),
                          static_cast<int>(SysExConstants::kPatchToEditBufferMessageLength));
        const auto* data = static_cast<const juce::uint8*>(sysEx.getData());
        test.expectEquals(static_cast<int>(data[0]), static_cast<int>(SysExConstants::kSysExStart));
        test.expectEquals(static_cast<int>(data[1]), static_cast<int>(SysExConstants::kManufacturerIdOberheim));
        test.expectEquals(static_cast<int>(data[2]), static_cast<int>(SysExConstants::kDeviceIdMatrix1000));
        test.expectEquals(static_cast<int>(data[3]),
                          static_cast<int>(SysExConstants::Opcode::kSinglePatchToEditBuffer));
        test.expectEquals(static_cast<int>(data[4]), 0);
    }

    inline juce::File createTempDir(juce::UnitTest& test, const char* prefix)
    {
        auto dir = juce::File::getSpecialLocation(juce::File::tempDirectory)
                       .getNonexistentChildFile(prefix, "", false);
        test.expect(dir.createDirectory(), "Temp dir should be created");
        return dir;
    }

    inline Core::PatchModel makeDistinctBuffer(int seed)
    {
        Core::PatchModel model;
        const auto marker = static_cast<juce::uint8>(seed & 0xFF);
        std::memset(model.data(), marker, Core::PatchModel::kBufferSize);
        model.data()[8] = marker;
        return model;
    }

    inline Core::PatchModel makeParentBuffer(int seed)
    {
        Core::PatchModel model;
        const auto marker = static_cast<juce::uint8>((seed + 100) & 0xFF);
        std::memset(model.data(), marker, Core::PatchModel::kBufferSize);
        model.data()[9] = marker;
        return model;
    }

    inline Core::PatchModel namedResult(int rootIndex, int retryIndex, int seed)
    {
        auto model = makeDistinctBuffer(seed);
        Core::MutationNaming::applyPatchName(model, rootIndex, retryIndex);
        return model;
    }
}
