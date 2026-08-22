#pragma once

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{
    // FR-51 leave-context modal choice (Persist action is chosen separately).
    enum class UnsavedEditConfirmChoice
    {
        kCancel,
        kDiscard,
        kPersist
    };

    // Leave-modal Persist action: Store (RAM), Save (known .syx), or Save As (picker).
    enum class UnsavedEditPersistKind
    {
        kStore,
        kSave,
        kSaveAs
    };

    namespace UnsavedEditWarning
    {
        inline bool isAtRisk(bool isDirty, bool notStoredInRam) noexcept
        {
            return isDirty || notStoredInRam;
        }

        // Settings policy: warn when dirty and/or not yet STORED in RAM, unless never-warn.
        inline bool shouldPrompt(int policyId, bool isDirty, bool notStoredInRam = false) noexcept
        {
            using namespace PluginIDs::Settings::UnsavedEditWarningPolicy;

            if (policyId == kNeverWarn)
                return false;

            return isAtRisk(isDirty, notStoredInRam);
        }
    }
} // namespace Core
