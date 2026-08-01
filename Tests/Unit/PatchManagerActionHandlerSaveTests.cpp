#include "PatchManagerActionHandlerTestSupport.h"

using namespace PatchManagerActionHandlerTestSupport;

class PatchManagerActionHandlerSaveTests : public juce::UnitTest
{
public:
    PatchManagerActionHandlerSaveTests() : juce::UnitTest("PatchManagerActionHandlerSave") {}

    void runTest() override
    {
        testSaveAs_writesAndRescans();
        testSave_overwritesSelectedFile();
        testSave_selectSentinelNoOp();
        testSaveAs_cancelledNoWrite();
        testSave_noSysEx();
        testSave_unusableFolderNoOp();
        testSaveAs_noSysEx();
        testSave_preservesPatchEditName();
    }

private:
    void testSaveAs_writesAndRescans()
    {
        beginTest("saveAs_writesAndRescans");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();

        const auto revisionBefore = harness.proc.apvts.state.getProperty(
            ComputerPatches::StateProperties::kScanRevision);

        harness.pickSaveFileCallback = [&tempDir](juce::File, juce::String stem) {
            return tempDir.getChildFile(stem + ".syx");
        };

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchAs, juce::var());

        const auto savedFile = tempDir.getChildFile("PATCH.syx");
        expect(savedFile.existsAsFile());
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString()
               == FooterMessages::formatSaveSuccess(savedFile.getFileName()));
        expect(harness.proc.apvts.state.getProperty("uiMessageSeverity").toString() == "info");
        expect(harness.proc.apvts.state.hasProperty(ComputerPatches::StateProperties::kScanRevision));
        expect(harness.proc.apvts.state.getProperty(ComputerPatches::StateProperties::kScanRevision)
               != revisionBefore);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
            1);

        juce::MemoryBlock savedSysEx;
        expect(savedFile.loadFileAsData(savedSysEx));
        expect(harness.decoder.validatePatchSysExMessage(savedSysEx));

        tempDir.deleteRecursively();
    }

    void testSave_overwritesSelectedFile()
    {
        beginTest("save_overwritesSelectedFile");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        const auto target = tempDir.getChildFile("Patch 71.syx");
        const auto sizeBefore = target.getSize();

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchFile, juce::var());

        expect(target.existsAsFile());
        expect(target.getSize() > 0);
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString()
               == FooterMessages::formatSaveSuccess(target.getFileName()));
        expectEquals(static_cast<int>(harness.patchFileService.getLastScanResult().validCount), 1);
        expect(sizeBefore > 0);

        tempDir.deleteRecursively();
    }

    void testSave_selectSentinelNoOp()
    {
        beginTest("save_selectSentinelNoOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            0,
            nullptr);

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchFile, juce::var());

        expectEquals(tempDir.getNumberOfChildFiles(juce::File::findFiles), 0);
        expect(! harness.proc.apvts.state.getProperty("uiMessageText").toString().startsWith("Saved "));

        tempDir.deleteRecursively();
    }

    void testSaveAs_cancelledNoWrite()
    {
        beginTest("saveAs_cancelledNoWrite");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();

        const auto revisionBefore = harness.proc.apvts.state.getProperty(
            ComputerPatches::StateProperties::kScanRevision);
        const auto patchNameBefore = harness.proc.apvts.state.getProperty(PatchNameIds::kPatchName).toString();
        const auto selectBefore = static_cast<int>(harness.proc.apvts.state.getProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile));
        harness.pickSaveFileCallback = [](juce::File, juce::String) { return juce::File(); };

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchAs, juce::var());

        expectEquals(tempDir.getNumberOfChildFiles(juce::File::findFiles), 1);
        expect(harness.proc.apvts.state.getProperty(ComputerPatches::StateProperties::kScanRevision)
               == revisionBefore);
        expect(harness.proc.apvts.state.getProperty(PatchNameIds::kPatchName).toString() == patchNameBefore);
        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
            selectBefore);

        tempDir.deleteRecursively();
    }

    void testSave_noSysEx()
    {
        beginTest("save_noSysEx");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchFile, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(! queued.patchData);
        expect(harness.queue.isEmpty());

        tempDir.deleteRecursively();
    }

    void testSave_unusableFolderNoOp()
    {
        beginTest("save_unusableFolderNoOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        expect(tempDir.deleteRecursively());

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchFile, juce::var());

        expect(! harness.proc.apvts.state.getProperty("uiMessageText").toString().startsWith("Saved "));
    }

    void testSaveAs_noSysEx()
    {
        beginTest("saveAs_noSysEx");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();

        harness.pickSaveFileCallback = [&tempDir](juce::File, juce::String stem) {
            return tempDir.getChildFile(stem + ".syx");
        };

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchAs, juce::var());

        const auto queued = scanQueue(harness.queue);
        expect(! queued.patchData);
        expect(harness.queue.isEmpty());

        tempDir.deleteRecursively();
    }

    void testSave_preservesPatchEditName()
    {
        beginTest("save_preservesPatchEditName");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StateProperties::kFolderPath,
            tempDir.getFullPathName(),
            nullptr);
        harness.handler.rescanPersistedComputerPatchesFolder();

        harness.model.setName("*'CANOPY");
        harness.proc.apvts.state.setProperty(PatchNameIds::kPatchName, "*'CANOPY", nullptr);

        harness.pickSaveFileCallback = [&tempDir](juce::File, juce::String) {
            return tempDir.getChildFile("MY-PATCH.syx");
        };

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSavePatchAs, juce::var());

        expect(harness.model.getName() == "*'CANOPY");
        expect(harness.proc.apvts.state.getProperty(PatchNameIds::kPatchName).toString() == "*'CANOPY");

        const auto savedFile = tempDir.getChildFile("MY-PATCH.syx");
        expect(savedFile.existsAsFile());
        juce::MemoryBlock savedSysEx;
        expect(savedFile.loadFileAsData(savedSysEx));
        expect(harness.decoder.validatePatchSysExMessage(savedSysEx));
        expect(savedFile.getFileNameWithoutExtension() == "MY-PATCH");

        tempDir.deleteRecursively();
    }
};

static PatchManagerActionHandlerSaveTests patchManagerActionHandlerSaveTests;
