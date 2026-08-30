#pragma once

#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"

// Test-only structural playability checklist (silence taxonomy classes A/B/C/E,
// priorities 1–2). Thresholds come from MutationCalibration — not a second policy SSOT.
namespace MutationPlayabilityOracle
{
    struct Result
    {
        bool passed = true;
        const char* ruleId = "";
    };

    // Returns the first failing rule, or passed=true when every priority-1/2 check holds.
    Result check(const Core::PatchModel& patch);
} // namespace MutationPlayabilityOracle
