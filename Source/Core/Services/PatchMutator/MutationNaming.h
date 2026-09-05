#pragma once

#include <juce_core/juce_core.h>

#include <optional>

#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchMutator/MutationHistoryStore.h"

namespace Core
{

    /** Parallel History N2 presentation rows from engine pipe tokens ("—|R00|…"). */
    struct HistorySubmenuDisplay
    {
        juce::StringArray labels;
        juce::Array<int> retryIndices; // parallel to labels; [0] is kRootOnly when non-empty
    };

    /** Parsed Computer Patches / export filename for Open list ordering. */
    struct MutatorExportFileKey
    {
        enum class Kind
        {
            kNone,
            kInitial,
            kRoot,
            kRetry
        };

        Kind kind = Kind::kNone;
        int rootIndex = -1;
        int retryIndex = -1;
    };

    struct MutationNaming
    {
        static constexpr const char* kInitialExportStem = "INITIAL";

        static juce::String formatRootLabel(int rootIndex);
        static juce::String formatRetryLabel(int retryIndex);
        static juce::String formatPatchName(int rootIndex,
                                            int retryIndex = MutationHistoryStore::kRootOnly);
        static juce::String formatExportStem(int rootIndex,
                                             int retryIndex = MutationHistoryStore::kRootOnly);

        /** Recognise INITIAL / Mxx / Mxx-Ryy export file names (with or without .syx). */
        static MutatorExportFileKey parseExportFileName(const juce::String& fileName);

        /** Open list order: INITIAL, then Mxx before its Mxx-Ryy, else case-insensitive alpha. */
        static int compareOpenListFileNames(const juce::String& left, const juce::String& right);

        /** Presentation labels + retry indices for History N2.
            Returns empty when there are no real retries (empty or sentinel-only).
            Otherwise: Mxx (@ kRootOnly), then Mxx-Ryy for each valid retry token. */
        static HistorySubmenuDisplay buildHistorySubmenuDisplay(
            int rootIndex,
            const juce::StringArray& engineRetryLabels);

        /** Labels only — same contract as buildHistorySubmenuDisplay().labels. */
        static juce::StringArray buildHistorySubmenuDisplayLabels(
            int rootIndex,
            const juce::StringArray& engineRetryLabels);

        static bool applyPatchName(PatchModel& model,
                                   int rootIndex,
                                   int retryIndex = MutationHistoryStore::kRootOnly);

    private:
        struct RetryExportIndices
        {
            int rootIndex = -1;
            int retryIndex = -1;
        };

        static bool isValidRootIndex(int rootIndex) noexcept;
        static bool isValidRetryIndex(int retryIndex) noexcept;
        static juce::String formatTwoDigit(int index);
        static int parseEngineRetryToken(const juce::String& token);
        static std::optional<int> tryParseRootExportStem(const juce::String& stem);
        static std::optional<RetryExportIndices> tryParseRetryExportStem(const juce::String& stem);
        static int compareMutatorOpenKeys(const MutatorExportFileKey& leftKey,
                                          const MutatorExportFileKey& rightKey,
                                          const juce::String& left,
                                          const juce::String& right);
    };

} // namespace Core
