#pragma once

#include "Shared/Definitions/MatrixDeviceTypes.h"

namespace Core
{
    /** MASTER EDIT outbound / INIT allowed only for Matrix-1000 while detected (FR-46). */
    inline bool isMasterEditAllowed(bool deviceDetected,
                                    MatrixDeviceTypes::Type deviceType) noexcept
    {
        return deviceDetected && deviceType == MatrixDeviceTypes::Type::kMatrix1000;
    }

    /** Hide MASTER EDIT column content on Matrix-6/6R when the section is unlocked (FR-46 UI). */
    inline bool shouldHideMasterEditContent(bool rootLocked,
                                            bool deviceDetected,
                                            MatrixDeviceTypes::Type deviceType) noexcept
    {
        const bool isMatrix6Family = deviceType == MatrixDeviceTypes::Type::kMatrix6
                                  || deviceType == MatrixDeviceTypes::Type::kMatrix6R;
        return ! rootLocked && deviceDetected && isMatrix6Family;
    }
}
