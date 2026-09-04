#include "WidgetDimensionRegistry.h"

#include "GUI/Layout/Design/Design.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{
    namespace BW = TSS::Design::Atoms::Widths::Button;
    using namespace PluginIDs;

    struct ExactButtonWidthEntry
    {
        const char* widgetId;
        int width;
    };

    constexpr ExactButtonWidthEntry kExactStandaloneButtonWidths[] = {
        { PatchManagerSection::BankUtilityModule::StandaloneWidgets::kCopyBank,
          BW::kPatchManagerCopyBank },
        { PatchManagerSection::BankUtilityModule::StandaloneWidgets::kPasteBank,
          BW::kPatchManagerPasteBank },
        { PatchManagerSection::BankUtilityModule::StandaloneWidgets::kImportBank,
          BW::kPatchManagerImportBank },
        { PatchManagerSection::BankUtilityModule::StandaloneWidgets::kExportBank,
          BW::kPatchManagerExportBank },

        { PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kInitPatch,
          BW::kInternalPatchesInit },
        { PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kCopyPatch,
          BW::kInternalPatchesCopy },
        { PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kPastePatch,
          BW::kInternalPatchesPaste },
        { PatchManagerSection::InternalPatchesModule::StandaloneWidgets::kStorePatch,
          BW::kInternalPatchesStore },

        { PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kOpenPatchFolder,
          BW::kComputerPatchesLoad },
        { PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSavePatchAs,
          BW::kComputerPatchesSaveAs },
        { PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSavePatchFile,
          BW::kComputerPatchesSave },

        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kMutate,
          BW::kPatchMutatorMutate },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kRetry,
          BW::kPatchMutatorRetry },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kCompare,
          BW::kPatchMutatorCompare },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kDelete,
          BW::kPatchMutatorDelete },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kClear,
          BW::kPatchMutatorClear },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kExport,
          BW::kPatchMutatorExport },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kHistoryPrevious,
          BW::kPatchMutatorHistoryNav },
        { PatchManagerSection::PatchMutatorModule::StandaloneWidgets::kHistoryNext,
          BW::kPatchMutatorHistoryNav },
    };

    std::optional<int> findExactStandaloneButtonWidth(const juce::String& widgetId)
    {
        for (const auto& entry : kExactStandaloneButtonWidths)
        {
            if (widgetId == entry.widgetId)
                return entry.width;
        }

        return std::nullopt;
    }

    std::optional<int> resolvePrefixStandaloneButtonWidth(const juce::String& widgetId)
    {
        if (widgetId.startsWith("bankUtilitySelectBank"))
            return BW::kPatchManagerBankSelect;

        if (widgetId.startsWith("patchMutatorEnable"))
            return BW::kInit;

        return std::nullopt;
    }

    std::optional<int> resolveSuffixStandaloneButtonWidth(const juce::String& widgetId)
    {
        if (widgetId.endsWith("LoadPrevious") || widgetId.endsWith("LoadNext"))
            return BW::kInit;

        if (widgetId.endsWith("Init"))
            return BW::kInit;

        if (widgetId.endsWith("Copy"))
            return BW::kCopy;

        if (widgetId.endsWith("Paste"))
            return BW::kPaste;

        return std::nullopt;
    }
}

std::optional<int> WidgetDimensionRegistry::resolveStandaloneButtonWidth(const juce::String& widgetId)
{
    if (const auto exact = findExactStandaloneButtonWidth(widgetId))
        return exact;

    if (const auto prefix = resolvePrefixStandaloneButtonWidth(widgetId))
        return prefix;

    return resolveSuffixStandaloneButtonWidth(widgetId);
}

bool WidgetDimensionRegistry::isStandaloneButtonWidthResolvable(const juce::String& widgetId)
{
    return resolveStandaloneButtonWidth(widgetId).has_value();
}
