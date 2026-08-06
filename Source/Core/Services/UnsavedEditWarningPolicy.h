#pragma once

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{
    // FR-51 leave-context modal choice (Store vs Save As is chosen by the GUI from RAM/ROM).
    enum class UnsavedEditConfirmChoice
    {
        kCancel,
        kDiscard,
        kPersist
    };

    namespace UnsavedEditWarning
    {
        // Settings policy: warn when dirty and/or not yet STORED in RAM, unless never-warn.
        inline bool shouldPrompt(int policyId, bool isDirty, bool notStoredInRam = false) noexcept
        {
            using namespace PluginIDs::Settings::UnsavedEditWarningPolicy;

            if (policyId == kNeverWarn)
                return false;

            return isDirty || notStoredInRam;
        }
    }
} // namespace Core
