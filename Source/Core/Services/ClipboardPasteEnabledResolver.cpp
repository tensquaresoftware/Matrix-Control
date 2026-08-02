#include "ClipboardPasteEnabledResolver.h"

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

namespace
{
    namespace PatchEdit = PluginIDs::PatchEditSection;

    struct WidgetIdKindEntry
    {
        const char* widgetId;
        PatchModuleKind kind;
    };

    constexpr WidgetIdKindEntry kCopyOrPasteWidgetIds[] = {
        { PatchEdit::Dco1Module::StandaloneWidgets::kCopy, PatchModuleKind::Dco1 },
        { PatchEdit::Dco1Module::StandaloneWidgets::kPaste, PatchModuleKind::Dco1 },
        { PatchEdit::Dco2Module::StandaloneWidgets::kCopy, PatchModuleKind::Dco2 },
        { PatchEdit::Dco2Module::StandaloneWidgets::kPaste, PatchModuleKind::Dco2 },
        { PatchEdit::Envelope1Module::StandaloneWidgets::kCopy, PatchModuleKind::Env1 },
        { PatchEdit::Envelope1Module::StandaloneWidgets::kPaste, PatchModuleKind::Env1 },
        { PatchEdit::Envelope2Module::StandaloneWidgets::kCopy, PatchModuleKind::Env2 },
        { PatchEdit::Envelope2Module::StandaloneWidgets::kPaste, PatchModuleKind::Env2 },
        { PatchEdit::Envelope3Module::StandaloneWidgets::kCopy, PatchModuleKind::Env3 },
        { PatchEdit::Envelope3Module::StandaloneWidgets::kPaste, PatchModuleKind::Env3 },
        { PatchEdit::Lfo1Module::StandaloneWidgets::kCopy, PatchModuleKind::Lfo1 },
        { PatchEdit::Lfo1Module::StandaloneWidgets::kPaste, PatchModuleKind::Lfo1 },
        { PatchEdit::Lfo2Module::StandaloneWidgets::kCopy, PatchModuleKind::Lfo2 },
        { PatchEdit::Lfo2Module::StandaloneWidgets::kPaste, PatchModuleKind::Lfo2 },
    };

    std::optional<PatchModuleKind> kindFromCopyOrPasteId(const juce::String& widgetId)
    {
        for (const auto& entry : kCopyOrPasteWidgetIds)
            if (widgetId == entry.widgetId)
                return entry.kind;

        return std::nullopt;
    }
}

ClipboardPasteEnabledState resolvePasteEnabled(const ClipboardService& clipboard)
{
    ClipboardPasteEnabledState state;
    state.dco1 = clipboard.canPasteModule(PatchModuleKind::Dco1);
    state.dco2 = clipboard.canPasteModule(PatchModuleKind::Dco2);
    state.env1 = clipboard.canPasteModule(PatchModuleKind::Env1);
    state.env2 = clipboard.canPasteModule(PatchModuleKind::Env2);
    state.env3 = clipboard.canPasteModule(PatchModuleKind::Env3);
    state.lfo1 = clipboard.canPasteModule(PatchModuleKind::Lfo1);
    state.lfo2 = clipboard.canPasteModule(PatchModuleKind::Lfo2);
    state.internalPatches = clipboard.canPasteFullPatch();
    state.matrixModulation = clipboard.canPasteMatrixModulation();
    return state;
}

std::optional<PatchModuleKind> patchModuleKindFromWidgetId(const juce::String& widgetId)
{
    return kindFromCopyOrPasteId(widgetId);
}

} // namespace Core
