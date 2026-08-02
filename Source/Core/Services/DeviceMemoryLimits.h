#pragma once

#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace Core
{
    struct PatchCoordinates
    {
        int bank = 0;
        int patch = 0;
    };

    class DeviceMemoryLimits
    {
    public:
        static DeviceMemoryLimits resolve(MatrixDeviceTypes::Type deviceType) noexcept;

        bool hasBankConcept() const noexcept { return hasBankConcept_; }
        int minBankNumber() const noexcept { return minBankNumber_; }
        int maxBankNumber() const noexcept { return maxBankNumber_; }
        int minPatchNumber() const noexcept { return minPatchNumber_; }
        int maxPatchNumber() const noexcept { return maxPatchNumber_; }
        bool hasRomBanks() const noexcept { return hasRomBanks_; }
        int internalPatchSlotCount() const noexcept { return internalPatchSlotCount_; }

        bool isRomBank(int bankNumber) const noexcept;
        bool isPasteStoreAllowed(int bankNumber) const noexcept;

        PatchCoordinates advancePatch(PatchCoordinates current,
                                    int direction) const noexcept;

        int wrapPatchWithinDevice(PatchCoordinates current, int direction) const noexcept;

    private:
        struct InitArgs
        {
            bool hasBankConcept = true;
            int minBankNumber = 0;
            int maxBankNumber = 0;
            int minPatchNumber = 0;
            int maxPatchNumber = 0;
            bool hasRomBanks = false;
            int internalPatchSlotCount = 0;
        };

        explicit DeviceMemoryLimits(const InitArgs& args) noexcept;

        bool hasBankConcept_ = true;
        int minBankNumber_ = 0;
        int maxBankNumber_ = 0;
        int minPatchNumber_ = 0;
        int maxPatchNumber_ = 0;
        bool hasRomBanks_ = false;
        int internalPatchSlotCount_ = 0;
    };
}
