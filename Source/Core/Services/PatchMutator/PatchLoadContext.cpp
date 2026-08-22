#include "Core/Services/PatchMutator/PatchLoadContext.h"

#include "Core/Services/PatchFileNameSanitizer.h"

namespace Core
{
    namespace
    {
        constexpr const char* kSyxFileLocationLabel = "SyxFile";
    } // namespace

    PatchLoadContext PatchLoadContext::deviceMemory(int bank, int patch)
    {
        PatchLoadContext context;
        context.origin = Origin::kDeviceMemory;
        context.bank = bank;
        context.patch = patch;
        return context;
    }

    PatchLoadContext PatchLoadContext::computerFile(const juce::String& fileStem,
                                                    const juce::String& knownSyxFullPath)
    {
        PatchLoadContext context;
        context.origin = Origin::kComputerFile;
        context.fileStem = fileStem;
        context.knownSyxFullPath = knownSyxFullPath;
        return context;
    }

    juce::String PatchLoadContext::computeExportBasename(const juce::String& patchName) const
    {
        if (origin == Origin::kComputerFile)
            return computeComputerBasename(patchName);

        return computeDeviceBasename(patchName);
    }

    juce::String PatchLoadContext::computeDeviceBasename(const juce::String& patchName) const
    {
        const auto location = PatchFileNameSanitizer::formatBankPatchLabel(bank, patch);
        const auto name = PatchFileNameSanitizer::sanitizeOsPathSegmentOrEmpty(patchName);

        if (name.isEmpty())
            return location;

        return name + " @ " + location;
    }

    juce::String PatchLoadContext::computeComputerBasename(const juce::String& patchName) const
    {
        const auto name = PatchFileNameSanitizer::sanitizeOsPathSegmentOrEmpty(patchName);

        if (name.isNotEmpty())
            return name + " @ " + juce::String(kSyxFileLocationLabel);

        const auto stem = PatchFileNameSanitizer::sanitizeOsFileStem(fileStem);
        return stem + " @ " + juce::String(kSyxFileLocationLabel);
    }

} // namespace Core
