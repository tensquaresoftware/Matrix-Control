#include "Core/Actions/PatchManagerActionHandler.h"

#include "Core/Services/PatchFileService.h"
#include "Shared/Definitions/PluginIDs.h"

namespace Core
{

    void PatchManagerActionHandler::handleOpenPatchFolder(const DeviceMemoryLimits& limits)
    {
        if (patchFileService_ == nullptr || ! pickFolder_)
            return;

        const juce::File folder = pickFolder_();

        if (! folder.isDirectory())
            return;

        seedCommittedComputerPatchesSelectionIfNeeded();

        const juce::String previousFolderPath = apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();
        const int previousSelectedId = lastCommittedComputerPatchesSelectedId_;

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            folder.getFullPathName(),
            nullptr);
        scanAndPublishFolder(folder);

        const auto& scan = patchFileService_->getLastScanResult();
        if (! scan.folderUsable || scan.validCount < 1)
        {
            apvts_.state.setProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
                0,
                nullptr);
            rememberComputerPatchesSelection(0);
            return;
        }

        pendingBrowserRestoreOnCancel_ = ComputerPatchesBrowserSnapshot {
            previousFolderPath,
            previousSelectedId
        };

        constexpr int kFirstPatchFileId = 1;
        const int beforeId = readComputerPatchesSelectedId();
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            kFirstPatchFileId,
            nullptr);

        if (readComputerPatchesSelectedId() == beforeId && beforeId == kFirstPatchFileId)
            handleLoadSelectedPatchFile(limits);
    }

    void PatchManagerActionHandler::resetComputerPatchesBrowserAfterSessionLoad()
    {
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            0,
            nullptr);
        rememberComputerPatchesSelection(0);
        lastStableComputerPatchesSelectedId_ = 0;
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0,
            nullptr);

        if (patchFileService_ != nullptr && patchFileService_->hasCachedScanResult())
            clearPublishedScanCache();
        else
            bumpScanRevision();

        pendingBrowserRestoreOnCancel_.reset();
        reloadPatchNameOverlayFromApvts();
    }

    void PatchManagerActionHandler::discardComputerPatchesScanCacheQuietly()
    {
        if (patchFileService_ != nullptr && patchFileService_->hasCachedScanResult())
            patchFileService_->clearLastScan();
    }

    void PatchManagerActionHandler::rescanPersistedComputerPatchesFolder()
    {
        const auto path = apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            juce::String()).toString();

        if (path.isEmpty())
        {
            clearPublishedScanCache();
            return;
        }

        scanAndPublishFolder(juce::File(path));
    }

    std::optional<int> PatchManagerActionHandler::advanceComputerPatchesSelection(bool isNext)
    {
        const int currentId = readComputerPatchesSelectedId();
        if (currentId < 1 || ! isComputerPatchesScanCurrent())
            return std::nullopt;

        const int count = patchFileService_->getLastScanResult().sortedValidFileNames.size();
        if (count < 1 || currentId > count)
            return std::nullopt;

        seedCommittedComputerPatchesSelectionIfNeeded();

        const int nextId = isNext
            ? (currentId >= count ? 1 : currentId + 1)
            : (currentId <= 1 ? count : currentId - 1);

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            nextId,
            nullptr);
        return nextId;
    }

    int PatchManagerActionHandler::readComputerPatchesSelectedId() const
    {
        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            0));
    }

    bool PatchManagerActionHandler::isComputerPatchesScanCurrent() const
    {
        if (patchFileService_ == nullptr)
            return false;

        const auto& scan = patchFileService_->getLastScanResult();
        const auto expectedFolder = resolveRescanFolder();

        return scan.folderUsable
            && scan.folder.isDirectory()
            && expectedFolder.isDirectory()
            && scan.folder.getFullPathName() == expectedFolder.getFullPathName();
    }

    juce::File PatchManagerActionHandler::fileAtComputerPatchesIndex(int index) const
    {
        const auto& scan = patchFileService_->getLastScanResult();

        if (index < 0 || index >= scan.sortedValidFileNames.size())
            return {};

        return scan.folder.getChildFile(scan.sortedValidFileNames[index]);
    }

    void PatchManagerActionHandler::noteStableComputerPatchesSelection(int selectedId)
    {
        if (selectedId >= 1)
            lastStableComputerPatchesSelectedId_ = selectedId;
    }

    int PatchManagerActionHandler::resolveComputerPatchesCancelRevertId() const
    {
        if (lastCommittedComputerPatchesSelectedId_ >= 1)
            return lastCommittedComputerPatchesSelectedId_;

        if (lastStableComputerPatchesSelectedId_ >= 1)
            return lastStableComputerPatchesSelectedId_;

        return static_cast<int>(apvts_.state.getProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0));
    }

    void PatchManagerActionHandler::revertComputerPatchesSelectionIfNeeded(int previousSelectedId)
    {
        if (readComputerPatchesSelectedId() == previousSelectedId)
            return;

        suppressComputerPatchesSelectLoad_ = true;
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            previousSelectedId,
            nullptr);
        suppressComputerPatchesSelectLoad_ = false;
        noteStableComputerPatchesSelection(previousSelectedId);
    }

    void PatchManagerActionHandler::rememberComputerPatchesSelection(int selectedId)
    {
        lastCommittedComputerPatchesSelectedId_ = selectedId;
        noteStableComputerPatchesSelection(selectedId);
    }

    void PatchManagerActionHandler::seedCommittedComputerPatchesSelectionIfNeeded()
    {
        if (lastCommittedComputerPatchesSelectedId_ >= 1)
            return;

        const int currentId = readComputerPatchesSelectedId();
        if (currentId >= 1)
        {
            lastCommittedComputerPatchesSelectedId_ = currentId;
            noteStableComputerPatchesSelection(currentId);
        }
    }

    void PatchManagerActionHandler::restoreComputerPatchesBrowser(const juce::String& folderPath,
                                                                  int selectedId)
    {
        suppressComputerPatchesSelectLoad_ = true;

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kFolderPath,
            folderPath,
            nullptr);

        if (folderPath.isEmpty())
            clearPublishedScanCache();
        else
            scanAndPublishFolder(juce::File(folderPath));

        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StandaloneWidgets::kSelectPatchFile,
            selectedId,
            nullptr);

        suppressComputerPatchesSelectLoad_ = false;
        noteStableComputerPatchesSelection(selectedId);
    }

    void PatchManagerActionHandler::abortComputerPatchesNavigation()
    {
        if (pendingBrowserRestoreOnCancel_.has_value())
        {
            const auto snapshot = *pendingBrowserRestoreOnCancel_;
            pendingBrowserRestoreOnCancel_.reset();
            restoreComputerPatchesBrowser(snapshot.folderPath, snapshot.selectedId);
            noteStableComputerPatchesSelection(snapshot.selectedId);
            apvts_.state.setProperty(
                PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
                0,
                nullptr);
            return;
        }

        revertComputerPatchesSelectionIfNeeded(resolveComputerPatchesCancelRevertId());
        apvts_.state.setProperty(
            PluginIDs::PatchManagerSection::ComputerPatchesModule::StateProperties::kSelectPatchCancelBaseline,
            0,
            nullptr);
    }

} // namespace Core
