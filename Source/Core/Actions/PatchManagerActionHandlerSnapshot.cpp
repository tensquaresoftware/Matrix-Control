#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchNameSyncer.h"
#include "Core/Services/DirtyPatchTracker.h"

namespace Core
{

    void PatchManagerActionHandler::syncDirtySnapshotFromApvts()
    {
        if (dirtyPatchTracker_ == nullptr || patchModel_ == nullptr || apvtsPatchMapper_ == nullptr)
            return;

        apvtsPatchMapper_->apvtsToBuffer();
        if (patchNameSyncer_ != nullptr)
            patchNameSyncer_->apvtsToBuffer();

        patchModel_->normalizeNameEncoding();
        dirtyPatchTracker_->captureSnapshot(*patchModel_);
        patchNotStoredInRam_ = false;
    }

    void PatchManagerActionHandler::captureCleanSnapshot()
    {
        syncDirtySnapshotFromApvts();
    }

    void PatchManagerActionHandler::establishEditorialCheckpoint()
    {
        if (hooks_.onEditorialCheckpoint)
        {
            hooks_.onEditorialCheckpoint();
            return;
        }

        syncDirtySnapshotFromApvts();
    }

} // namespace Core
