#pragma once

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{
    namespace PatchNameDisplay
    {
        // Settings policy: Musical Names (default) vs Hardware Names for Patch Name UI.
        inline bool isHardwareNames(int modeId) noexcept
        {
            using namespace PluginIDs::Settings::PatchNameDisplayMode;

            return modeId == kHardwareNames;
        }

        inline int normalize(int modeId) noexcept
        {
            using namespace PluginIDs::Settings::PatchNameDisplayMode;

            if (modeId == kMusicalNames || modeId == kHardwareNames)
                return modeId;

            return kDefault;
        }
    }
} // namespace Core
