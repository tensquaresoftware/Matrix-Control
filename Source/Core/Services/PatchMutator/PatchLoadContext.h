#pragma once

#include <juce_core/juce_core.h>

namespace Core
{

    // Explicit single source of truth for where the editor patch currently came from.
    // Updated at real load sites (Computer .syx, device dump after nav, INIT, PASTE) and
    // frozen by PatchMutatorEngine on the first MUTATE to build stable Export folder names.
    // Deliberately NOT an Observer framework (see spec: use an explicit context, not events).
    class PatchLoadContext
    {
    public:
        enum class Origin
        {
            kDeviceMemory,
            kComputerFile
        };

        Origin origin { Origin::kDeviceMemory };
        int bank { 0 };
        int patch { 0 };
        juce::String fileStem; // Computer origin only — raw stem, sanitized on demand.
        // Computer origin only — absolute .syx path for leave-modal Save overwrite (chantier 2).
        juce::String knownSyxFullPath;

        static PatchLoadContext deviceMemory(int bank, int patch);
        static PatchLoadContext computerFile(const juce::String& fileStem,
                                             const juce::String& knownSyxFullPath = {});

        // Export session folder basename:
        //   Device / INIT / PASTE : `{Name} @ B{b}-P{pp:02d}` or `B{b}-P{pp:02d}` when name empty
        //   Computer file         : `{Name} @ SyxFile` when the sanitized current patch name
        //                           is non-empty, else `{Stem} @ SyxFile` as a fallback.
        juce::String computeExportBasename(const juce::String& patchName) const;

    private:
        juce::String computeDeviceBasename(const juce::String& patchName) const;
        juce::String computeComputerBasename(const juce::String& patchName) const;
    };

} // namespace Core
