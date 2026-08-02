#pragma once

#include <juce_core/juce_core.h>

class SysExDecoder;
class SysExEncoder;

namespace Core
{

    class MutationHistoryStore;

    struct PatchFileSaveResult
    {
        bool success = false;
        juce::String errorMessage;
    };

    struct PatchFileLoadResult
    {
        bool success = false;
        juce::String errorMessage;
    };

    struct PatchFileExportResult
    {
        bool success = false;
        int filesWritten = 0;
        juce::String errorMessage;
    };

    struct PatchFolderScanResult
    {
        juce::File folder;
        bool folderUsable = false;
        int validCount = 0;
        int invalidCount = 0;
        juce::StringArray sortedValidFileNames;
        juce::String footerMessage;
        juce::String footerSeverity;
    };

    struct MutatorExportWriteArgs
    {
        const MutationHistoryStore& store;
        SysExEncoder& encoder;
        const juce::String& userPatchName;
    };

    struct MutatorHistorySessionExportArgs
    {
        juce::File sessionFolder;
        const MutationHistoryStore& store;
        SysExEncoder& encoder;
        bool clearExisting = false;
        juce::String userPatchName;
    };

    struct FolderScanCounts
    {
        int validCount = 0;
        int invalidCount = 0;
        int syxFileCount = 0;
    };

    class PatchFileService
    {
    public:
        static constexpr const char* kSyxExtension = ".syx";

        explicit PatchFileService(SysExDecoder& decoder) noexcept;

        PatchFolderScanResult scanFolder(const juce::File& folder);
        // patchNumber stamps the SysEx header slot byte (bank export needs the real slot 0-99;
        // Computer Patches Save/Save As always pass 0 — the encoded slot is irrelevant on load).
        PatchFileSaveResult savePatchSysExFile(const juce::File& targetFile,
                                               const juce::uint8* packedData,
                                               SysExEncoder& encoder,
                                               int patchNumber = 0);
        PatchFileLoadResult loadPatchSysExFile(const juce::File& file, juce::uint8* packedOut);
        // userPatchName is the live SSOT musical name (model + APVTS), stamped onto bytes
        // 0-7 of every exported .syx before encoding. Filenames/folder stems stay Mxx/Mxx-Ryy
        // via MutationNaming's format helpers, independent of the packed buffer's name bytes.
        PatchFileExportResult exportMutatorHistory(const juce::File& folder,
                                                   const MutationHistoryStore& store,
                                                   SysExEncoder& encoder,
                                                   const juce::String& userPatchName);
        // Hybrid session-folder export: writes the FR-33 layout inside sessionFolder.
        // When clearExisting is true the folder contents are removed first (Overwrite);
        // otherwise the folder must be created fresh.
        PatchFileExportResult exportMutatorHistorySession(const MutatorHistorySessionExportArgs& args);
        // First non-existing session folder among basename, basename-2, basename-3, … (Keep).
        static juce::File resolveKeepSessionFolder(const juce::File& parentFolder,
                                                   const juce::String& basename);
        const PatchFolderScanResult& getLastScanResult() const noexcept { return lastScan_; }
        bool hasCachedScanResult() const noexcept;
        void clearLastScan() noexcept;

    private:
        static bool hasSyxExtension(const juce::File& file) noexcept;
        static bool isFolderReadable(const juce::File& folder) noexcept;
        static juce::Array<juce::File> findSyxFiles(const juce::File& folder);
        static juce::File withSyxExtension(const juce::File& file);
        static PatchFileSaveResult makeSaveFailure(const char* message);
        bool validateTempSyxContents(const juce::File& tempFile) const;
        static bool replaceFileWithTemp(const juce::File& target, juce::File& tempFile);
        PatchFileSaveResult finalizeTempSyxWrite(const juce::File& target, juce::File tempFile);
        bool validateFileContents(const juce::File& file) const;
        void appendValidFileName(juce::StringArray& names, const juce::File& file) const;
        void collectSyxScanResults(const juce::Array<juce::File>& syxFiles,
                                   juce::StringArray& validNames,
                                   int& validCount,
                                   int& invalidCount) const;
        PatchFolderScanResult scanReadableFolder(const juce::File& folder) const;
        PatchFolderScanResult makeUnusableFolderResult(const juce::File& folder) const;
        PatchFolderScanResult makeScanResult(const juce::File& folder,
                                             juce::StringArray validNames,
                                             const FolderScanCounts& counts) const;
        void cacheResult(PatchFolderScanResult result);
        PatchFileExportResult validateMutatorExport(const juce::File& folder,
                                                    const MutationHistoryStore& store);
        PatchFileExportResult writeInitialSnapshot(const juce::File& folder,
                                                   const MutatorExportWriteArgs& args);
        PatchFileExportResult writeExportPatchFile(const juce::File& file,
                                                   const juce::uint8* packedData,
                                                   const MutatorExportWriteArgs& args);
        PatchFileExportResult writeRootEntry(const juce::File& rootDir,
                                             int rootIndex,
                                             const MutatorExportWriteArgs& args);
        PatchFileExportResult writeRetryEntries(const juce::File& rootDir,
                                                int rootIndex,
                                                const MutatorExportWriteArgs& args);
        PatchFileExportResult writeRootFolder(const juce::File& folder,
                                              int rootIndex,
                                              const MutatorExportWriteArgs& args);
        PatchFileExportResult writeAllRootFolders(const juce::File& folder,
                                                  const MutatorExportWriteArgs& args);
        PatchFileExportResult writeHistoryLayout(const juce::File& folder,
                                                 const MutatorExportWriteArgs& args);

        SysExDecoder& decoder_;
        PatchFolderScanResult lastScan_;
    };

} // namespace Core
