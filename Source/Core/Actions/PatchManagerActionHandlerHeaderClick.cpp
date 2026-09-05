#include "Core/Actions/PatchManagerActionHandler.h"

#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

    bool PatchManagerActionHandler::tryHandleModuleHeaderClicks(const juce::String& propertyId,
                                                                const DeviceMemoryLimits& limits)
    {
        namespace InternalWidgets = PluginIDs::PatchManagerSection::InternalPatchesModule::StandaloneWidgets;
        namespace ComputerWidgets = PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets;

        if (propertyId == InternalWidgets::kHeaderClick)
        {
            setNavigationFocus(PluginIDs::PatchManagerSection::NavigationFocus::kInternal);

            if (! arePatchCoordinatesEstablished())
                return true;

            if (! confirmPatchContextChange())
                return true;

            abandonPendingComputerSelectSettle();
            abandonPendingInternalNavSettle();
            patchNavDebouncer_.cancel();
            computerSelectDebouncer_.cancel();
            beginPendingDeviceLoad(captureInternalCoordinates(limits));
            loadCurrentPatchFromDevice(limits);
            return true;
        }

        if (propertyId == ComputerWidgets::kHeaderClick)
        {
            setNavigationFocus(PluginIDs::PatchManagerSection::NavigationFocus::kComputer);

            if (readComputerPatchesSelectedId() < 1)
                return true;

            loadSelectedPatchFileImmediately(limits);
            return true;
        }

        return false;
    }

} // namespace Core
