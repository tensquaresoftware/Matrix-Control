#pragma once

#include "Core/Services/ClipboardService.h"

namespace Core
{

    struct ClipboardFeedbackBlinkState
    {
        bool active = false;

        bool dco1Copy = false;
        bool dco2Copy = false;
        bool env1Copy = false;
        bool env2Copy = false;
        bool env3Copy = false;
        bool lfo1Copy = false;
        bool lfo2Copy = false;
        bool matrixModulationCopy = false;
        bool internalPatchesCopy = false;

        bool dco1Paste = false;
        bool dco2Paste = false;
        bool env1Paste = false;
        bool env2Paste = false;
        bool env3Paste = false;
        bool lfo1Paste = false;
        bool lfo2Paste = false;
        bool matrixModulationPaste = false;
        bool internalPatchesPaste = false;
    };

    // Session flags are owned by PluginProcessor: active arms blink; crossPatchReady
    // unlocks Matrix/FullPatch paste-target blink after a later patch load.
    ClipboardFeedbackBlinkState resolveClipboardFeedback(const ClipboardService& clipboard,
                                                         bool sessionActive,
                                                         bool crossPatchReady) noexcept;

} // namespace Core
