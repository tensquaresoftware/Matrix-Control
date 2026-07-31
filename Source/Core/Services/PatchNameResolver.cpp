#include "PatchNameResolver.h"

#include "Core/Services/PatchFileNameSanitizer.h"

namespace Core
{

    juce::String PatchNameResolver::resolve(const juce::String& deviceName,
                                            int bank,
                                            int patchNumber,
                                            bool isRomBank,
                                            const juce::String& factoryName,
                                            const juce::String& overlayName,
                                            Mode mode)
    {
        if (mode == Mode::kHardware)
        {
            if (! PatchFileNameSanitizer::isUsablePatchName(deviceName))
                return PatchFileNameSanitizer::formatBankPatchLabel(bank, patchNumber);

            return deviceName;
        }

        if (isRomBank && factoryName.isNotEmpty())
            return factoryName;

        if (PatchFileNameSanitizer::isOberheimBankPlaceholderName(deviceName)
            || ! PatchFileNameSanitizer::isUsablePatchName(deviceName))
        {
            if (overlayName.isNotEmpty())
                return overlayName;
        }

        if (! PatchFileNameSanitizer::isUsablePatchName(deviceName))
            return PatchFileNameSanitizer::formatBankPatchLabel(bank, patchNumber);

        return deviceName;
    }

} // namespace Core
