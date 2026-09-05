#include <juce_core/juce_core.h>

#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/MIDI/SysEx/SysExDecoder.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Core/MIDI/SysEx/SysExParser.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/PatchFileService.h"
#include "Core/Services/PatchMutator/MutationHistoryStore.h"
#include "PatchFileServiceTestSupport.h"

class PatchFileServiceMutatorExportTests : public juce::UnitTest
{
public:
    PatchFileServiceMutatorExportTests()
        : juce::UnitTest("PatchFileServiceMutatorExport")
        , decoder_(parser_)
        , service_(decoder_)
    {
    }

    void runTest() override
    {
        exportMutatorHistory_emptyStore_fails();
        exportMutatorHistory_writesInitialAndRoot();
        exportMutatorHistory_writesRetries();
        exportMutatorHistory_gapIndices();
        exportMutatorHistory_nameBytesMatch();
        exportMutatorHistory_initialAlsoStampedWithUserName();
        exportMutatorHistory_nonWritableFolder_fails();
        exportMutatorHistory_roundTripValidates();

        exportSession_createsFolderWithLayout();
        resolveKeepSessionFolder_indexesSuffix();
        exportSession_overwriteClearsExisting();
    }

private:
    SysExParser parser_;
    SysExDecoder decoder_;
    SysExEncoder encoder_;
    Core::PatchFileService service_;

    juce::File createTempScanDir()
    {
        return PatchFileServiceTestSupport::createTempDir(*this, "MatrixControlPatchFileServiceMutator");
    }

    juce::String decodedPatchName(const juce::File& file)
    {
        juce::uint8 packed[SysExConstants::kPatchPackedDataSize] = {};
        expect(service_.loadPatchSysExFile(file, packed).success);
        Core::PatchModel model;
        model.loadFrom(packed);
        return model.getName();
    }

    bool folderContainsExactSyxName(const juce::File& folder, const juce::String& exactFileName)
    {
        for (const auto& child : folder.findChildFiles(juce::File::findFiles, false, "*.syx"))
            if (child.getFileName() == exactFileName)
                return true;

        return false;
    }

    void exportMutatorHistory_emptyStore_fails()
    {
        beginTest("exportMutatorHistory_emptyStore_fails");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "WARMPAD");

        expect(! result.success);
        expect(result.errorMessage.isNotEmpty());

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_writesInitialAndRoot()
    {
        beginTest("exportMutatorHistory_writesInitialAndRoot");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        auto initial = PatchFileServiceTestSupport::makeDistinctBuffer(301);
        initial.setName("MY PATCH");
        store.setInitialSnapshot(initial);
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 302),
                                PatchFileServiceTestSupport::makeParentBuffer(302)));

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "MY PATCH");

        expect(result.success);
        expect(result.filesWritten >= 2);
        expect(tempDir.getChildFile("INITIAL.syx").existsAsFile());
        expect(folderContainsExactSyxName(tempDir, "INITIAL.syx"));
        expect(tempDir.getChildFile("M00.syx").existsAsFile());
        expect(! tempDir.getChildFile("M00").isDirectory());

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_writesRetries()
    {
        beginTest("exportMutatorHistory_writesRetries");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 310),
                                PatchFileServiceTestSupport::makeParentBuffer(310)));
        expect(store.insertRetry(0, 0,
                                 PatchFileServiceTestSupport::namedResult(0, 0, 311),
                                 PatchFileServiceTestSupport::makeParentBuffer(311)));
        expect(store.insertRetry(0, 1,
                                 PatchFileServiceTestSupport::namedResult(0, 1, 312),
                                 PatchFileServiceTestSupport::makeParentBuffer(312)));

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "WARMPAD");

        expect(result.success);
        expect(tempDir.getChildFile("M00.syx").existsAsFile());
        expect(tempDir.getChildFile("M00-R00.syx").existsAsFile());
        expect(tempDir.getChildFile("M00-R01.syx").existsAsFile());
        expect(! tempDir.getChildFile("M00").isDirectory());

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_gapIndices()
    {
        beginTest("exportMutatorHistory_gapIndices");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 320),
                                PatchFileServiceTestSupport::makeParentBuffer(320)));
        expect(store.insertRoot(5,
                                PatchFileServiceTestSupport::namedResult(
                                    5, Core::MutationHistoryStore::kRootOnly, 325),
                                PatchFileServiceTestSupport::makeParentBuffer(325)));
        expect(store.insertRoot(99,
                                PatchFileServiceTestSupport::namedResult(
                                    99, Core::MutationHistoryStore::kRootOnly, 399),
                                PatchFileServiceTestSupport::makeParentBuffer(399)));

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "WARMPAD");

        expect(result.success);
        expect(tempDir.getChildFile("M00.syx").existsAsFile());
        expect(tempDir.getChildFile("M05.syx").existsAsFile());
        expect(tempDir.getChildFile("M99.syx").existsAsFile());
        expect(! tempDir.getChildFile("M01.syx").existsAsFile());
        expect(! tempDir.getChildFile("M02.syx").existsAsFile());
        expect(! tempDir.getChildFile("M00").isDirectory());
        expect(! tempDir.getChildFile("M05").isDirectory());
        expect(! tempDir.getChildFile("M99").isDirectory());

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_nameBytesMatch()
    {
        beginTest("exportMutatorHistory_nameBytesMatch");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        expect(store.insertRoot(5,
                                PatchFileServiceTestSupport::namedResult(
                                    5, Core::MutationHistoryStore::kRootOnly, 335),
                                PatchFileServiceTestSupport::makeParentBuffer(335)));
        expect(store.insertRetry(5, 2,
                                 PatchFileServiceTestSupport::namedResult(5, 2, 352),
                                 PatchFileServiceTestSupport::makeParentBuffer(352)));

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "WARMPAD");
        expect(result.success);

        expectEquals(decodedPatchName(tempDir.getChildFile("M05.syx")),
                     juce::String("WARMPAD"));
        expectEquals(decodedPatchName(tempDir.getChildFile("M05-R02.syx")),
                     juce::String("WARMPAD"));

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_initialAlsoStampedWithUserName()
    {
        beginTest("exportMutatorHistory_initialAlsoStampedWithUserName");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        auto initial = PatchFileServiceTestSupport::makeDistinctBuffer(340);
        initial.setName("MY PATCH");
        store.setInitialSnapshot(initial);
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 341),
                                PatchFileServiceTestSupport::makeParentBuffer(341)));

        const auto result = service_.exportMutatorHistory(tempDir, store, encoder_, "COLDPAD");
        expect(result.success);

        expectEquals(decodedPatchName(tempDir.getChildFile("INITIAL.syx")), juce::String("COLDPAD"));

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_nonWritableFolder_fails()
    {
        beginTest("exportMutatorHistory_nonWritableFolder_fails");

        Core::MutationHistoryStore store;
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 350),
                                PatchFileServiceTestSupport::makeParentBuffer(350)));

        const auto missing = juce::File::getSpecialLocation(juce::File::tempDirectory)
                                 .getChildFile("MatrixControlMissingExportFolder");

        const auto result = service_.exportMutatorHistory(missing, store, encoder_, "WARMPAD");

        expect(! result.success);
        expect(result.errorMessage.isNotEmpty());
    }

    void exportSession_createsFolderWithLayout()
    {
        beginTest("exportSession_createsFolderWithLayout");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;
        auto initial = PatchFileServiceTestSupport::makeDistinctBuffer(401);
        initial.setName("MY PATCH");
        store.setInitialSnapshot(initial);
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 402),
                                PatchFileServiceTestSupport::makeParentBuffer(402)));

        const auto sessionFolder = tempDir.getChildFile("OB-VOX @ B8-P25");
        const Core::MutatorHistorySessionExportArgs args { sessionFolder, store, encoder_, false, "OB-VOX" };
        const auto result = service_.exportMutatorHistorySession(args);

        expect(result.success);
        expect(sessionFolder.getChildFile("INITIAL.syx").existsAsFile());
        expect(folderContainsExactSyxName(sessionFolder, "INITIAL.syx"));
        expect(sessionFolder.getChildFile("M00.syx").existsAsFile());
        expect(! sessionFolder.getChildFile("M00").isDirectory());

        tempDir.deleteRecursively();
    }

    void resolveKeepSessionFolder_indexesSuffix()
    {
        beginTest("resolveKeepSessionFolder_indexesSuffix");

        const auto tempDir = createTempScanDir();
        const juce::String basename = "OB-VOX @ B3-P38";

        expectEquals(Core::PatchFileService::resolveKeepSessionFolder(tempDir, basename).getFileName(),
                     basename);

        expect(tempDir.getChildFile(basename).createDirectory());
        expectEquals(Core::PatchFileService::resolveKeepSessionFolder(tempDir, basename).getFileName(),
                     basename + "-2");

        expect(tempDir.getChildFile(basename + "-2").createDirectory());
        expectEquals(Core::PatchFileService::resolveKeepSessionFolder(tempDir, basename).getFileName(),
                     basename + "-3");

        tempDir.deleteRecursively();
    }

    void exportSession_overwriteClearsExisting()
    {
        beginTest("exportSession_overwriteClearsExisting");

        const auto tempDir = createTempScanDir();
        const auto sessionFolder = tempDir.getChildFile("B0-P00");
        expect(sessionFolder.createDirectory());
        const auto stale = sessionFolder.getChildFile("stale.txt");
        expect(stale.replaceWithText("stale"));
        expect(sessionFolder.getChildFile("Initial.syx").replaceWithText("legacy-initial"));
        const auto legacyRootDir = sessionFolder.getChildFile("M00");
        expect(legacyRootDir.createDirectory());
        expect(legacyRootDir.getChildFile("M00.syx").replaceWithText("legacy-nested"));

        Core::MutationHistoryStore store;
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 410),
                                PatchFileServiceTestSupport::makeParentBuffer(410)));

        const Core::MutatorHistorySessionExportArgs args { sessionFolder, store, encoder_, true, "WARMPAD" };
        const auto result = service_.exportMutatorHistorySession(args);

        expect(result.success);
        expect(! stale.existsAsFile());
        expect(sessionFolder.getChildFile("M00.syx").existsAsFile());
        expect(folderContainsExactSyxName(sessionFolder, "M00.syx"));
        expect(! sessionFolder.getChildFile("M00").isDirectory());
        expect(! folderContainsExactSyxName(sessionFolder, "Initial.syx"));

        tempDir.deleteRecursively();
    }

    void exportMutatorHistory_roundTripValidates()
    {
        beginTest("exportMutatorHistory_roundTripValidates");

        const auto tempDir = createTempScanDir();
        Core::MutationHistoryStore store;

        auto initial = PatchFileServiceTestSupport::makeDistinctBuffer(360);
        initial.setName("INITNAME");
        store.setInitialSnapshot(initial);
        expect(store.insertRoot(0,
                                PatchFileServiceTestSupport::namedResult(
                                    0, Core::MutationHistoryStore::kRootOnly, 361),
                                PatchFileServiceTestSupport::makeParentBuffer(361)));
        expect(store.insertRetry(0, 0,
                                 PatchFileServiceTestSupport::namedResult(0, 0, 362),
                                 PatchFileServiceTestSupport::makeParentBuffer(362)));

        const auto exportResult = service_.exportMutatorHistory(tempDir, store, encoder_, "INITNAME");
        expect(exportResult.success);
        expectEquals(exportResult.filesWritten, 3);

        juce::MemoryBlock initialSysEx;
        expect(tempDir.getChildFile("INITIAL.syx").loadFileAsData(initialSysEx));
        PatchFileServiceTestSupport::expectEditBufferPatchHeader(*this, initialSysEx);

        const auto rootScan = service_.scanFolder(tempDir);
        expect(rootScan.folderUsable);
        expectEquals(rootScan.validCount, 3);
        expectEquals(rootScan.invalidCount, 0);

        tempDir.deleteRecursively();
    }
};

static PatchFileServiceMutatorExportTests patchFileServiceMutatorExportTests;
