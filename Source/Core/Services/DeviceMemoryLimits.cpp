#include "DeviceMemoryLimits.h"

#include "Shared/Definitions/Matrix1000Limits.h"
#include "Shared/Definitions/Matrix6Or6RLimits.h"

namespace Core
{
    DeviceMemoryLimits::DeviceMemoryLimits(const InitArgs& args) noexcept
        : hasBankConcept_(args.hasBankConcept)
        , minBankNumber_(args.minBankNumber)
        , maxBankNumber_(args.maxBankNumber)
        , minPatchNumber_(args.minPatchNumber)
        , maxPatchNumber_(args.maxPatchNumber)
        , hasRomBanks_(args.hasRomBanks)
        , internalPatchSlotCount_(args.internalPatchSlotCount)
    {
    }

    DeviceMemoryLimits DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type deviceType) noexcept
    {
        switch (deviceType)
        {
            case MatrixDeviceTypes::Type::kMatrix6:
            case MatrixDeviceTypes::Type::kMatrix6R:
                return DeviceMemoryLimits(InitArgs{
                    .hasBankConcept = false,
                    .minBankNumber = 0,
                    .maxBankNumber = 0,
                    .minPatchNumber = Matrix6Or6RLimits::kMinPatchNumber,
                    .maxPatchNumber = Matrix6Or6RLimits::kMaxPatchNumber,
                    .hasRomBanks = false,
                    .internalPatchSlotCount = Matrix6Or6RLimits::kInternalPatchSlotCount,
                });

            case MatrixDeviceTypes::Type::kMatrix1000:
                return DeviceMemoryLimits(InitArgs{
                    .hasBankConcept = true,
                    .minBankNumber = Matrix1000Limits::kMinBankNumber,
                    .maxBankNumber = Matrix1000Limits::kMaxBankNumber,
                    .minPatchNumber = Matrix1000Limits::kMinPatchNumber,
                    .maxPatchNumber = Matrix1000Limits::kMaxPatchNumber,
                    .hasRomBanks = true,
                    .internalPatchSlotCount =
                        (Matrix1000Limits::kMaxBankNumber - Matrix1000Limits::kMinBankNumber + 1)
                        * (Matrix1000Limits::kMaxPatchNumber - Matrix1000Limits::kMinPatchNumber + 1),
                });

            case MatrixDeviceTypes::Type::kUnknown:
            default:
                return resolve(MatrixDeviceTypes::Type::kMatrix1000);
        }
    }

    bool DeviceMemoryLimits::isRomBank(int bankNumber) const noexcept
    {
        if (!hasRomBanks_)
            return false;

        return bankNumber >= Matrix1000Limits::kMinBankNumber + 2
            && bankNumber <= Matrix1000Limits::kMaxBankNumber;
    }

    bool DeviceMemoryLimits::isPasteStoreAllowed(int bankNumber) const noexcept
    {
        if (!hasBankConcept_)
            return true;

        return !isRomBank(bankNumber);
    }

    int DeviceMemoryLimits::wrapPatchWithinDevice(PatchCoordinates current, int direction) const noexcept
    {
        const int span = maxPatchNumber_ - minPatchNumber_ + 1;
        int offset = (current.patch - minPatchNumber_) + direction;
        offset %= span;
        if (offset < 0)
            offset += span;

        return minPatchNumber_ + offset;
    }

    PatchCoordinates DeviceMemoryLimits::advancePatch(PatchCoordinates current,
                                                      int direction) const noexcept
    {
        const int step = direction < 0 ? -1 : 1;
        auto result = current;

        if (!hasBankConcept_)
        {
            result.patch = wrapPatchWithinDevice(current, step);
            result.bank = 0;
            return result;
        }

        const int nextPatch = current.patch + step;

        if (nextPatch >= minPatchNumber_ && nextPatch <= maxPatchNumber_)
        {
            result.patch = nextPatch;
            return result;
        }

        result.patch = wrapPatchWithinDevice(current, step);
        result.bank = current.bank;
        return result;
    }
}
