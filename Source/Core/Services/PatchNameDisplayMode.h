#pragma once

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{
    namespace PatchNameDisplay
    {
        // Settings: MATRIX-1000 PATCHES — DISPLAY MUSICAL NAMES (default) vs DISPLAY HARDWARE NAMES.
        inline bool isHardwareNames(int modeId) noexcept
        {
            using namespace PluginIDs::Settings::Matrix1000PatchesNamesMode;

            return modeId == kDisplayHardwareNames;
        }

        inline int normalize(int modeId) noexcept
        {
            using namespace PluginIDs::Settings::Matrix1000PatchesNamesMode;

            if (modeId == kDisplayMusicalNames || modeId == kDisplayHardwareNames)
                return modeId;

            return kDefault;
        }
    }
} // namespace Core
