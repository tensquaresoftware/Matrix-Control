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

        // Returns the name segment of a Bank Utility export stem ("P10 - NYLON 12" -> "NYLON 12"),
        // or empty when the stem is slot-only ("P10").
        static juce::String nameFromBankExportStem(juce::String stem);

        // Oberheim ROM placeholder names embedded in factory dumps (e.g. "BNK4: 05").
        static bool isOberheimBankPlaceholderName(const juce::String& nameFromBytes) noexcept;

        // Location label shared by Mutator export folders and empty-name patch fallback: "B1-P23".
        static juce::String formatBankPatchLabel(int bank, int patchNumber);

        // True when bytes 0-7 yielded a non-blank display name (BNK placeholders count as usable).
        static bool isUsablePatchName(const juce::String& nameFromBytes) noexcept;

        // Returns nameFromBytes when usable; otherwise formatBankPatchLabel(bank, patchNumber).
        static juce::String resolvePatchNameOrBankPatchFallback(const juce::String& nameFromBytes,
                                                                int bank,
                                                                int patchNumber);

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
