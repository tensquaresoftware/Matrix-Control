#pragma once

#include <juce_core/juce_core.h>

namespace Core
{

    struct PatchFileNameSanitizer
    {
        static juce::String sanitizeFileStem(juce::String input);
        static juce::String sanitizeToMatrixName(juce::String stem);
        // Same filtering as sanitizeToMatrixName but returns an empty string instead of the
        // fallback when nothing survives — used where a blank Matrix name must stay blank.
        static juce::String sanitizeToMatrixNameOrEmpty(juce::String stem);

        // OS path segment for export folders / display stems: strip OS-forbidden characters,
        // trim, uppercase — keep spaces and Matrix-exotic punctuation that filesystems allow.
        // Empty when nothing usable remains (no PATCH fallback).
        static juce::String sanitizeOsPathSegmentOrEmpty(juce::String input);

        // Filename stem helper: strip directories + .syx, then OS-sanitize (spaces kept).
        // Falls back to kEmptyNameFallback when empty — suitable for .syx filenames.
        static juce::String sanitizeOsFileStem(juce::String input);

        static juce::String ensureSyxExtension(const juce::String& stem);

        // Bank Utility EXPORT filename stem: "Pxx - {Name}" (slot 0-99, name OS-sanitized from
        // the payload's name bytes), or "Pxx" when the sanitized name is empty.
        static juce::String bankExportFileStem(int slot0to99, juce::String patchNameFromBytes);

        // Charset SSOT for the Matrix name (A-Z, 0-9, space, -, _) — reused by the Patch Name
        // inline editor to accept/reject typed characters without duplicating the rule.
        static bool isAllowedMatrixChar(juce::juce_wchar character) noexcept;

        static constexpr const char* kEmptyNameFallback = "PATCH";
        static constexpr int kMaxNameLength = 8;

    private:
        static juce::String stripOsForbiddenChars(juce::String text);
        static juce::String stripPathAndSyxExtension(juce::String input);
    };

} // namespace Core
