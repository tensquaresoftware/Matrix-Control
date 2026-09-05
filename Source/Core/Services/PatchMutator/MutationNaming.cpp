#include "Core/Services/PatchMutator/MutationNaming.h"

namespace Core
{

    juce::String MutationNaming::formatRootLabel(int rootIndex)
    {
        if (! isValidRootIndex(rootIndex))
            return {};

        return "M" + formatTwoDigit(rootIndex);
    }

    juce::String MutationNaming::formatRetryLabel(int retryIndex)
    {
        if (! isValidRetryIndex(retryIndex))
            return {};

        return "R" + formatTwoDigit(retryIndex);
    }

    juce::String MutationNaming::formatPatchName(int rootIndex, int retryIndex)
    {
        if (! isValidRootIndex(rootIndex))
            return {};

        const auto rootLabel = formatRootLabel(rootIndex);
        if (rootLabel.isEmpty())
            return {};

        if (retryIndex == MutationHistoryStore::kRootOnly)
            return rootLabel;

        if (! isValidRetryIndex(retryIndex))
            return {};

        return rootLabel + "-R" + formatTwoDigit(retryIndex);
    }

    juce::String MutationNaming::formatExportStem(int rootIndex, int retryIndex)
    {
        return formatPatchName(rootIndex, retryIndex);
    }

    MutatorExportFileKey MutationNaming::parseExportFileName(const juce::String& fileName)
    {
        MutatorExportFileKey key;
        auto stem = fileName;

        if (stem.endsWithIgnoreCase(".syx"))
            stem = stem.dropLastCharacters(4);

        if (stem.equalsIgnoreCase(kInitialExportStem))
        {
            key.kind = MutatorExportFileKey::Kind::kInitial;
            return key;
        }

        if (const auto root = tryParseRootExportStem(stem))
        {
            key.kind = MutatorExportFileKey::Kind::kRoot;
            key.rootIndex = *root;
            return key;
        }

        if (const auto retry = tryParseRetryExportStem(stem))
        {
            key.kind = MutatorExportFileKey::Kind::kRetry;
            key.rootIndex = retry->rootIndex;
            key.retryIndex = retry->retryIndex;
            return key;
        }

        return key;
    }

    int MutationNaming::compareMutatorOpenKeys(const MutatorExportFileKey& leftKey,
                                               const MutatorExportFileKey& rightKey,
                                               const juce::String& left,
                                               const juce::String& right)
    {
        if (leftKey.rootIndex != rightKey.rootIndex)
            return leftKey.rootIndex < rightKey.rootIndex ? -1 : 1;

        if (leftKey.kind != rightKey.kind)
            return leftKey.kind == MutatorExportFileKey::Kind::kRoot ? -1 : 1;

        if (leftKey.kind == MutatorExportFileKey::Kind::kRetry
            && leftKey.retryIndex != rightKey.retryIndex)
            return leftKey.retryIndex < rightKey.retryIndex ? -1 : 1;

        return left.compareIgnoreCase(right);
    }

    int MutationNaming::compareOpenListFileNames(const juce::String& left, const juce::String& right)
    {
        const auto leftKey = parseExportFileName(left);
        const auto rightKey = parseExportFileName(right);

        if (leftKey.kind == MutatorExportFileKey::Kind::kInitial
            && rightKey.kind == MutatorExportFileKey::Kind::kInitial)
            return left.compareIgnoreCase(right);

        if (leftKey.kind == MutatorExportFileKey::Kind::kInitial)
            return -1;

        if (rightKey.kind == MutatorExportFileKey::Kind::kInitial)
            return 1;

        const bool leftMutator = leftKey.kind == MutatorExportFileKey::Kind::kRoot
                              || leftKey.kind == MutatorExportFileKey::Kind::kRetry;
        const bool rightMutator = rightKey.kind == MutatorExportFileKey::Kind::kRoot
                               || rightKey.kind == MutatorExportFileKey::Kind::kRetry;

        if (leftMutator && rightMutator)
            return compareMutatorOpenKeys(leftKey, rightKey, left, right);

        return left.compareIgnoreCase(right);
    }

    std::optional<int> MutationNaming::tryParseRootExportStem(const juce::String& stem)
    {
        if (stem.length() != 3 || stem[0] != 'M')
            return std::nullopt;

        const int rootIndex = stem.substring(1, 3).getIntValue();

        if (! isValidRootIndex(rootIndex) || formatRootLabel(rootIndex) != stem)
            return std::nullopt;

        return rootIndex;
    }

    std::optional<MutationNaming::RetryExportIndices> MutationNaming::tryParseRetryExportStem(
        const juce::String& stem)
    {
        if (stem.length() != 7 || stem[0] != 'M' || stem[3] != '-' || stem[4] != 'R')
            return std::nullopt;

        RetryExportIndices indices;
        indices.rootIndex = stem.substring(1, 3).getIntValue();
        indices.retryIndex = stem.substring(5, 7).getIntValue();

        if (! isValidRootIndex(indices.rootIndex) || ! isValidRetryIndex(indices.retryIndex)
            || formatPatchName(indices.rootIndex, indices.retryIndex) != stem)
            return std::nullopt;

        return indices;
    }

    int MutationNaming::parseEngineRetryToken(const juce::String& token)
    {
        if (! token.startsWithChar('R') || token.length() < 3)
            return -1;

        const int retryIndex = token.substring(1, 3).getIntValue();
        if (! isValidRetryIndex(retryIndex))
            return -1;

        // Reject non-canonical forms (e.g. "R0x", "R00extra") that still parse digits.
        if (formatRetryLabel(retryIndex) != token)
            return -1;

        return retryIndex;
    }

    HistorySubmenuDisplay MutationNaming::buildHistorySubmenuDisplay(
        int rootIndex,
        const juce::StringArray& engineRetryLabels)
    {
        HistorySubmenuDisplay result;
        if (! isValidRootIndex(rootIndex) || engineRetryLabels.size() <= 1)
            return result;

        const auto rootOnlyLabel = formatPatchName(rootIndex, MutationHistoryStore::kRootOnly);
        if (rootOnlyLabel.isEmpty())
            return result;

        result.labels.add(rootOnlyLabel);
        result.retryIndices.add(MutationHistoryStore::kRootOnly);

        for (int i = 1; i < engineRetryLabels.size(); ++i)
        {
            const int retryIndex = parseEngineRetryToken(engineRetryLabels[i]);
            if (retryIndex < 0)
                continue;

            const auto label = formatPatchName(rootIndex, retryIndex);
            if (label.isEmpty())
                continue;

            result.labels.add(label);
            result.retryIndices.add(retryIndex);
        }

        if (result.labels.size() <= 1)
            return {};

        return result;
    }

    juce::StringArray MutationNaming::buildHistorySubmenuDisplayLabels(
        int rootIndex,
        const juce::StringArray& engineRetryLabels)
    {
        return buildHistorySubmenuDisplay(rootIndex, engineRetryLabels).labels;
    }

    bool MutationNaming::applyPatchName(PatchModel& model, int rootIndex, int retryIndex)
    {
        const auto name = formatPatchName(rootIndex, retryIndex);
        if (name.isEmpty())
            return false;

        model.setName(name);
        return true;
    }

    bool MutationNaming::isValidRootIndex(int rootIndex) noexcept
    {
        return rootIndex >= 0 && rootIndex < MutationHistoryStore::kMaxRoots;
    }

    bool MutationNaming::isValidRetryIndex(int retryIndex) noexcept
    {
        return retryIndex >= 0 && retryIndex < MutationHistoryStore::kMaxRetriesPerRoot;
    }

    juce::String MutationNaming::formatTwoDigit(int index)
    {
        return juce::String::formatted("%02d", index);
    }

} // namespace Core
