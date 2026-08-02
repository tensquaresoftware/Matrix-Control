#include "PatchNameResolver.h"

#include "Core/Services/PatchFileNameSanitizer.h"

namespace Core
{

    juce::String PatchNameResolver::resolve(const ResolveArgs& args)
    {
        if (args.mode == Mode::kHardware)
        {
            if (! PatchFileNameSanitizer::isUsablePatchName(args.deviceName))
                return PatchFileNameSanitizer::formatBankPatchLabel(args.bank, args.patchNumber);

            return args.deviceName;
        }

        if (args.isRomBank && args.factoryName.isNotEmpty())
            return args.factoryName;

        if (PatchFileNameSanitizer::isOberheimBankPlaceholderName(args.deviceName)
            || ! PatchFileNameSanitizer::isUsablePatchName(args.deviceName))
        {
            if (args.overlayName.isNotEmpty())
                return args.overlayName;
        }

        if (! PatchFileNameSanitizer::isUsablePatchName(args.deviceName))
            return PatchFileNameSanitizer::formatBankPatchLabel(args.bank, args.patchNumber);

        return args.deviceName;
    }

} // namespace Core
