#pragma once

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{
    namespace MutatorDeleteWarning
    {
        // Settings policy: prompt before Patch Mutator Delete unless the user chose never-warn.
        inline bool shouldPrompt(int policyId) noexcept
        {
            using namespace PluginIDs::Settings::MutatorDeleteWarningPolicy;

            return policyId != kNeverWarn;
        }
    }
} // namespace Core
