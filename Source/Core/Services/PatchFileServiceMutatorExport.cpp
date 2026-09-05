#include "Core/Services/PatchFileService.h"

#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Core/Services/PatchMutator/MutationHistoryStore.h"
#include "Core/Services/PatchMutator/MutationNaming.h"

namespace Core
{

    PatchFileExportResult PatchFileService::validateMutatorExport(const juce::File& folder,
                                                                    const MutationHistoryStore& store)
    {
        PatchFileExportResult result;

        if (store.isEmpty())
        {
            result.errorMessage = "History empty";
            return result;
        }

        if (! folder.isDirectory() || ! folder.hasWriteAccess())
            result.errorMessage = "Folder not writable";
        else
            result.success = true;

        return result;
    }

    PatchFileExportResult PatchFileService::writeInitialSnapshot(const juce::File& folder,
                                                                 const MutatorExportWriteArgs& args)
    {
        const auto initialFile = folder.getChildFile("INITIAL.syx");
        return writeExportPatchFile(initialFile, args.store.getInitialSnapshot().data(), args);
    }

    PatchFileExportResult PatchFileService::writeExportPatchFile(const juce::File& file,
                                                                 const juce::uint8* packedData,
                                                                 const MutatorExportWriteArgs& args)
    {
        PatchModel stamped;
        stamped.loadFrom(packedData);

        // Keep the buffer's existing name when the live user name is blank — never wipe
        // bytes 0–7 to spaces just because the stamp argument was empty.
        if (args.userPatchName.trim().isNotEmpty())
            stamped.setName(args.userPatchName);

        PatchFileExportResult result;
        const auto save = savePatchSysExFile(file, stamped.data(), args.encoder);

        if (! save.success)
        {
            result.errorMessage = save.errorMessage;
            return result;
        }

        result.success = true;
        result.filesWritten = 1;
        return result;
    }

    PatchFileExportResult PatchFileService::writeRootEntry(const juce::File& folder,
                                                           int rootIndex,
                                                           const MutatorExportWriteArgs& args)
    {
        PatchFileExportResult result;
        result.success = true;

        if (const auto rootEntry = args.store.getEntry(rootIndex, MutationHistoryStore::kRootOnly))
        {
            const auto rootLabel = MutationNaming::formatRootLabel(rootIndex);
            const auto rootFile = folder.getChildFile(
                PatchFileNameSanitizer::ensureSyxExtension(rootLabel));
            return writeExportPatchFile(rootFile, rootEntry->result.data(), args);
        }

        return result;
    }

    PatchFileExportResult PatchFileService::writeRetryEntries(const juce::File& folder,
                                                              int rootIndex,
                                                              const MutatorExportWriteArgs& args)
    {
        PatchFileExportResult result;
        result.success = true;

        for (const auto retryIndex : args.store.getSortedRetryIndices(rootIndex))
        {
            if (const auto retryEntry = args.store.getEntry(rootIndex, retryIndex))
            {
                const auto stem = MutationNaming::formatExportStem(rootIndex, retryIndex);
                const auto retryFile = folder.getChildFile(
                    PatchFileNameSanitizer::ensureSyxExtension(stem));
                const auto write = writeExportPatchFile(retryFile, retryEntry->result.data(), args);

                if (! write.success)
                    return write;

                result.filesWritten += write.filesWritten;
            }
        }

        return result;
    }

    PatchFileExportResult PatchFileService::writeRootAndRetries(const juce::File& folder,
                                                                int rootIndex,
                                                                const MutatorExportWriteArgs& args)
    {
        PatchFileExportResult result;

        const auto rootWrite = writeRootEntry(folder, rootIndex, args);
        if (! rootWrite.success)
            return rootWrite;

        result.filesWritten += rootWrite.filesWritten;

        const auto retryWrite = writeRetryEntries(folder, rootIndex, args);
        if (! retryWrite.success)
            return retryWrite;

        result.filesWritten += retryWrite.filesWritten;
        result.success = true;
        return result;
    }

    PatchFileExportResult PatchFileService::writeAllRoots(const juce::File& folder,
                                                          const MutatorExportWriteArgs& args)
    {
        PatchFileExportResult result;

        for (const auto rootIndex : args.store.getSortedRootIndices())
        {
            const auto rootWrite = writeRootAndRetries(folder, rootIndex, args);

            if (! rootWrite.success)
                return rootWrite;

            result.filesWritten += rootWrite.filesWritten;
        }

        result.success = true;
        return result;
    }

    PatchFileExportResult PatchFileService::writeHistoryLayout(const juce::File& folder,
                                                               const MutatorExportWriteArgs& args)
    {
        PatchFileExportResult result;

        if (args.store.hasInitialSnapshot())
        {
            const auto initialWrite = writeInitialSnapshot(folder, args);
            if (! initialWrite.success)
                return initialWrite;

            result.filesWritten += initialWrite.filesWritten;
        }

        const auto rootsWrite = writeAllRoots(folder, args);
        if (! rootsWrite.success)
            return rootsWrite;

        result.filesWritten += rootsWrite.filesWritten;
        result.success = true;
        return result;
    }

    PatchFileExportResult PatchFileService::exportMutatorHistory(const juce::File& folder,
                                                                 const MutationHistoryStore& store,
                                                                 SysExEncoder& encoder,
                                                                 const juce::String& userPatchName)
    {
        const auto validation = validateMutatorExport(folder, store);
        if (! validation.success)
            return validation;

        const MutatorExportWriteArgs args { store, encoder, userPatchName };
        return writeHistoryLayout(folder, args);
    }

    juce::File PatchFileService::resolveKeepSessionFolder(const juce::File& parentFolder,
                                                          const juce::String& basename)
    {
        constexpr int kMaxKeepSuffix = 999;
        juce::File candidate = parentFolder.getChildFile(basename);

        for (int suffix = 2; candidate.exists() && suffix <= kMaxKeepSuffix; ++suffix)
            candidate = parentFolder.getChildFile(basename + "-" + juce::String(suffix));

        return candidate;
    }

    PatchFileExportResult PatchFileService::exportMutatorHistorySession(
        const MutatorHistorySessionExportArgs& args)
    {
        PatchFileExportResult result;

        if (args.store.isEmpty())
        {
            result.errorMessage = "History empty";
            return result;
        }

        if (args.clearExisting && args.sessionFolder.exists() && ! args.sessionFolder.deleteRecursively())
        {
            result.errorMessage = "Folder not writable";
            return result;
        }

        if (! args.sessionFolder.createDirectory())
        {
            result.errorMessage = "Folder not writable";
            return result;
        }

        const MutatorExportWriteArgs writeArgs { args.store, args.encoder, args.userPatchName };
        return writeHistoryLayout(args.sessionFolder, writeArgs);
    }

} // namespace Core
