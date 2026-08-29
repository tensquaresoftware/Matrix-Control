#pragma once

#include "Core/Services/PatchMutator/MutationPolicy.h"

namespace Core
{

    struct MutationRecipe
    {
        // MODE is the user-facing source of truth. Amount and Random stay as resolved
        // internals so the parent curve math and its golden vectors are untouched.
        int amountPercent = 0;
        int randomPercent = 0;
        MutationMode mode = MutationMode::kDrift;
        MutationPitchMode pitchMode = MutationPitchMode::kPreserve;
        int pitchOctaveWindow = MutationCalibration::kDefaultPitchOctaves;

        bool enableDco1 = false;
        bool enableDco2 = false;
        bool enableVcfVca = false;
        bool enableFmTrack = false;
        bool enableRampPortamento = false;
        bool enableEnvelope1 = false;
        bool enableEnvelope2 = false;
        bool enableEnvelope3 = false;
        bool enableLfo1 = false;
        bool enableLfo2 = false;
        bool enableMatrixMod = false;

        bool hasAnyVoiceModuleEnabled() const noexcept
        {
            return enableDco1 || enableDco2 || enableVcfVca || enableFmTrack || enableRampPortamento;
        }

        bool hasAnyModulatorModuleEnabled() const noexcept
        {
            return enableEnvelope1 || enableEnvelope2 || enableEnvelope3
                   || enableLfo1 || enableLfo2 || enableMatrixMod;
        }

        bool hasAnyModuleEnabled() const noexcept
        {
            return hasAnyVoiceModuleEnabled() || hasAnyModulatorModuleEnabled();
        }

        void applyModeCalibration() noexcept
        {
            amountPercent = modeAmountPercent(mode);
            randomPercent = modeRandomPercent(mode);
        }
    };

} // namespace Core
