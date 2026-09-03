#include "Core/Services/PatchFileNameSanitizer.h"

namespace Core
{

    juce::String PatchFileNameSanitizer::stripPathAndSyxExtension(juce::String input)
    {
        if (input.containsChar('/') || input.containsChar('\\'))
        {
            const int lastSep = juce::jmax(input.lastIndexOfChar('/'), input.lastIndexOfChar('\\'));
            input = input.substring(lastSep + 1);
        }

        if (input.endsWithIgnoreCase(".syx"))
            input = input.upToLastOccurrenceOf(".", false, false);

        return input.trim();
    }

    juce::String PatchFileNameSanitizer::sanitizeFileStem(juce::String input)
    {
        return sanitizeToMatrixName(stripOsForbiddenChars(stripPathAndSyxExtension(std::move(input))));
    }

    juce::String PatchFileNameSanitizer::sanitizeOsFileStem(juce::String input)
    {
        const auto filtered = sanitizeOsPathSegmentOrEmpty(stripPathAndSyxExtension(std::move(input)));

        if (filtered.isEmpty())
            return kEmptyNameFallback;

        return filtered;
    }

    juce::String PatchFileNameSanitizer::normalizeMatrixSaveStemOrEmpty(juce::String input)
    {
        const auto stem = stripPathAndSyxExtension(std::move(input)).trim().toUpperCase();

        if (stem.isEmpty() || stem.length() > kMaxNameLength)
            return {};

        for (const auto character : stem)
        {
            if (! isAllowedMatrixChar(character))
                return {};
        }

        return stem;
    }

    bool PatchFileNameSanitizer::isExactMatrixFileStem(const juce::String& input)
    {
        return normalizeMatrixSaveStemOrEmpty(input).isNotEmpty();
    }

    juce::String PatchFileNameSanitizer::sanitizeOsPathSegmentOrEmpty(juce::String input)
    {
        input = stripOsForbiddenChars(input.trim()).toUpperCase().trim();

        while (input.endsWithChar('.') || input.endsWithChar(' '))
            input = input.dropLastCharacters(1).trimEnd();

        constexpr int kMaxOsPathSegmentLength = 64;
        return input.substring(0, kMaxOsPathSegmentLength).trimEnd();
    }

    juce::String PatchFileNameSanitizer::sanitizeToMatrixName(juce::String stem)
    {
        const auto filtered = sanitizeToMatrixNameOrEmpty(std::move(stem));

        if (filtered.isEmpty())
            return kEmptyNameFallback;

        return filtered;
    }

    juce::String PatchFileNameSanitizer::sanitizeToMatrixNameOrEmpty(juce::String stem)
    {
        stem = stem.toUpperCase();
        juce::String filtered;

        for (auto character : stem)
        {
            if (isAllowedMatrixChar(character))
                filtered += character;
        }

        return filtered.substring(0, kMaxNameLength);
    }

    juce::String PatchFileNameSanitizer::ensureSyxExtension(const juce::String& stem)
    {
        if (stem.endsWithIgnoreCase(".syx"))
            return stem;

        return stem + ".syx";
    }

    juce::String PatchFileNameSanitizer::bankExportFileStem(int slot0to99, juce::String patchNameFromBytes)
    {
        const auto slotLabel = "P" + juce::String(juce::jlimit(0, 99, slot0to99)).paddedLeft('0', 2);
        const auto sanitizedName = sanitizeOsPathSegmentOrEmpty(std::move(patchNameFromBytes));

        if (sanitizedName.isEmpty())
            return slotLabel;

        return slotLabel + " - " + sanitizedName;
    }

    juce::String PatchFileNameSanitizer::formatBankPatchLabel(int bank, int patchNumber)
    {
        const int clampedBank = juce::jlimit(0, 9, bank);
        const int clampedPatch = juce::jlimit(0, 99, patchNumber);
        return "B" + juce::String(clampedBank) + "-P" + juce::String(clampedPatch).paddedLeft('0', 2);
    }

    juce::String PatchFileNameSanitizer::nameFromBankExportStem(juce::String stem)
    {
        stem = stripPathAndSyxExtension(std::move(stem)).trim();

        // "Pxx - Name" or "Pxx-Name" (tolerant), else slot-only "Pxx".
        if (stem.length() >= 3
            && stem[0] == 'P'
            && juce::CharacterFunctions::isDigit(stem[1])
            && juce::CharacterFunctions::isDigit(stem[2]))
        {
            auto remainder = stem.substring(3).trim();
            if (remainder.startsWithChar('-'))
                remainder = remainder.substring(1).trim();

            return sanitizeOsPathSegmentOrEmpty(remainder);
        }

        return {};
    }

    bool PatchFileNameSanitizer::isOberheimBankPlaceholderName(const juce::String& nameFromBytes) noexcept
    {
        return nameFromBytes.trim().toUpperCase().startsWith("BNK");
    }

    juce::String PatchFileNameSanitizer::formatOberheimBankPlaceholderName(int bank, int patchNumber)
    {
        const int clampedBank = juce::jlimit(0, 9, bank);
        const int clampedPatch = juce::jlimit(0, 99, patchNumber);
        return "BNK" + juce::String(clampedBank) + ": "
            + juce::String(clampedPatch).paddedLeft('0', 2);
    }

    bool PatchFileNameSanitizer::isUsablePatchName(const juce::String& nameFromBytes) noexcept
    {
        return nameFromBytes.trim().isNotEmpty();
    }

    juce::String PatchFileNameSanitizer::resolvePatchNameOrBankPatchFallback(const juce::String& nameFromBytes,
                                                                            int bank,
                                                                            int patchNumber)
    {
        if (isUsablePatchName(nameFromBytes))
            return nameFromBytes.trimEnd();

        return formatBankPatchLabel(bank, patchNumber);
    }

    juce::String PatchFileNameSanitizer::stripOsForbiddenChars(juce::String text)
    {
        for (int i = text.length(); --i >= 0;)
        {
            const auto character = text[i];

            if (character < 32
                || character == '/'
                || character == '\\'
                || character == ':'
                || character == '*'
                || character == '?'
                || character == '"'
                || character == '<'
                || character == '>'
                || character == '|')
            {
                text = text.substring(0, i) + text.substring(i + 1);
            }
        }

        return text;
    }

    bool PatchFileNameSanitizer::isAllowedMatrixChar(juce::juce_wchar character) noexcept
    {
        return (character >= 'A' && character <= 'Z')
            || (character >= '0' && character <= '9')
            || character == ' '
            || character == '-'
            || character == '_';
    }

} // namespace Core
