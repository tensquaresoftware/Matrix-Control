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
