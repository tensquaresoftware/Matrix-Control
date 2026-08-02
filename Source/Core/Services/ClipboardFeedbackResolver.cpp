#include "ClipboardFeedbackResolver.h"

namespace Core
{

namespace
{
    void setModuleCopy(ClipboardFeedbackBlinkState& state, PatchModuleKind kind, bool value) noexcept
    {
        switch (kind)
        {
            case PatchModuleKind::Dco1: state.dco1Copy = value; break;
            case PatchModuleKind::Dco2: state.dco2Copy = value; break;
            case PatchModuleKind::Env1: state.env1Copy = value; break;
            case PatchModuleKind::Env2: state.env2Copy = value; break;
            case PatchModuleKind::Env3: state.env3Copy = value; break;
            case PatchModuleKind::Lfo1: state.lfo1Copy = value; break;
            case PatchModuleKind::Lfo2: state.lfo2Copy = value; break;
        }
    }

    void setModulePaste(ClipboardFeedbackBlinkState& state, PatchModuleKind kind, bool value) noexcept
    {
        switch (kind)
        {
            case PatchModuleKind::Dco1: state.dco1Paste = value; break;
            case PatchModuleKind::Dco2: state.dco2Paste = value; break;
            case PatchModuleKind::Env1: state.env1Paste = value; break;
            case PatchModuleKind::Env2: state.env2Paste = value; break;
            case PatchModuleKind::Env3: state.env3Paste = value; break;
            case PatchModuleKind::Lfo1: state.lfo1Paste = value; break;
            case PatchModuleKind::Lfo2: state.lfo2Paste = value; break;
        }
    }

    constexpr PatchModuleKind kAllModules[] = {
        PatchModuleKind::Dco1,
        PatchModuleKind::Dco2,
        PatchModuleKind::Env1,
        PatchModuleKind::Env2,
        PatchModuleKind::Env3,
        PatchModuleKind::Lfo1,
        PatchModuleKind::Lfo2,
    };

    void applyModuleModeFeedback(ClipboardFeedbackBlinkState& state,
                                 const ClipboardService& clipboard) noexcept
    {
        const auto source = clipboard.getSourceModuleKind();
        if (! source.has_value())
        {
            state.active = false;
            return;
        }

        setModuleCopy(state, *source, true);

        for (const auto target : kAllModules)
        {
            if (target == *source)
                continue;

            if (clipboard.canPasteModule(target))
                setModulePaste(state, target, true);
        }
    }

    void applyMatrixModeFeedback(ClipboardFeedbackBlinkState& state,
                                 const ClipboardService& clipboard,
                                 bool crossPatchReady) noexcept
    {
        state.matrixModulationCopy = true;
        state.matrixModulationPaste = crossPatchReady && clipboard.canPasteMatrixModulation();
    }

    void applyFullPatchModeFeedback(ClipboardFeedbackBlinkState& state,
                                    const ClipboardService& clipboard,
                                    bool crossPatchReady) noexcept
    {
        state.internalPatchesCopy = true;
        state.internalPatchesPaste = crossPatchReady && clipboard.canPasteFullPatch();
    }
}

ClipboardFeedbackBlinkState resolveClipboardFeedback(const ClipboardService& clipboard,
                                                     bool sessionActive,
                                                     bool crossPatchReady) noexcept
{
    ClipboardFeedbackBlinkState state;

    if (! sessionActive || ! clipboard.hasContent())
        return state;

    state.active = true;

    switch (clipboard.getMode())
    {
        case ClipboardMode::Empty:
            state.active = false;
            break;

        case ClipboardMode::Module:
            applyModuleModeFeedback(state, clipboard);
            break;

        case ClipboardMode::MatrixModulation:
            applyMatrixModeFeedback(state, clipboard, crossPatchReady);
            break;

        case ClipboardMode::FullPatch:
            applyFullPatchModeFeedback(state, clipboard, crossPatchReady);
            break;
    }

    return state;
}

} // namespace Core
