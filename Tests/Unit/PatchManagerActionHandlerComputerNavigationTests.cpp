#include "PatchManagerActionHandlerTestSupport.h"

using namespace PatchManagerActionHandlerTestSupport;

class PatchManagerActionHandlerComputerNavigationTests : public juce::UnitTest
{
public:
    PatchManagerActionHandlerComputerNavigationTests() : juce::UnitTest("PatchManagerActionHandlerComputerNavigation") {}

    void runTest() override
    {
        testLoadAdjacent_nextWraps();
        testLoadAdjacent_previousWraps();
        testLoadAdjacent_singleFileReloads();
        testLoadAdjacent_sentinelNoOp();
        testLoadAdjacent_enqueuesSysEx();
        testLoadAdjacent_staleScanNoOp();
        testLoadAdjacent_emptyListNoOp();
        testLoadSelected_invokesOnPatchLoaded();
    }

private:
    void testLoadAdjacent_nextWraps()
    {
        beginTest("loadAdjacent_nextWraps");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 12, false);
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 5.syx");
        copyFixturePatchToDir(tempDir, "Patch 66.syx");
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            3,
            nullptr);

        fireAdjacentNavigation(harness, ComputerPatches::StandaloneWidgets::kLoadNextPatchFile);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     1);
        expect(scanQueue(harness.queue).editBufferPatch);

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_previousWraps()
    {
        beginTest("loadAdjacent_previousWraps");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 12, false);
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 5.syx");
        copyFixturePatchToDir(tempDir, "Patch 66.syx");
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        fireAdjacentNavigation(harness, ComputerPatches::StandaloneWidgets::kLoadPreviousPatchFile);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     3);
        expect(scanQueue(harness.queue).editBufferPatch);

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_singleFileReloads()
    {
        beginTest("loadAdjacent_singleFileReloads");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 12, false);
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        fireAdjacentNavigation(harness, ComputerPatches::StandaloneWidgets::kLoadNextPatchFile);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     1);
        expect(scanQueue(harness.queue).editBufferPatch);

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_sentinelNoOp()
    {
        beginTest("loadAdjacent_sentinelNoOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            0,
            nullptr);

        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kLoadNextPatchFile, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     0);
        expect(harness.queue.isEmpty());

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_enqueuesSysEx()
    {
        beginTest("loadAdjacent_enqueuesSysEx");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 12, false);
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 5.syx");
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        fireAdjacentNavigation(harness, ComputerPatches::StandaloneWidgets::kLoadNextPatchFile);

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     2);
        expect(scanQueue(harness.queue).editBufferPatch);

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_staleScanNoOp()
    {
        beginTest("loadAdjacent_staleScanNoOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        expect(tempDir.deleteRecursively());

        harness.proc.apvts.state.setProperty("uiMessageText", juce::String(), nullptr);
        harness.proc.apvts.state.setProperty("uiMessageSeverity", juce::String(), nullptr);
        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kLoadNextPatchFile, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     1);
        expect(harness.queue.isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageSeverity").toString().isEmpty());

        tempDir.deleteRecursively();
    }

    void testLoadAdjacent_emptyListNoOp()
    {
        beginTest("loadAdjacent_emptyListNoOp");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        setupComputerPatchesScan(harness, tempDir);
        expectEquals(harness.patchFileService.getLastScanResult().validCount, 0);
        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile,
            1,
            nullptr);

        harness.proc.apvts.state.setProperty("uiMessageText", juce::String(), nullptr);
        harness.proc.apvts.state.setProperty("uiMessageSeverity", juce::String(), nullptr);
        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kLoadNextPatchFile, juce::var());

        expectEquals(static_cast<int>(harness.proc.apvts.state.getProperty(
                         ComputerPatches::StandaloneWidgets::kSelectPatchFile)),
                     1);
        expect(harness.queue.isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageText").toString().isEmpty());
        expect(harness.proc.apvts.state.getProperty("uiMessageSeverity").toString().isEmpty());

        tempDir.deleteRecursively();
    }

    void testLoadSelected_invokesOnPatchLoaded()
    {
        beginTest("loadSelected_invokesOnPatchLoaded");

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

        expect(! harness.patchLoadHookState->invoked);
        harness.handler.handleAction(ComputerPatches::StandaloneWidgets::kSelectPatchFile, juce::var());
        expect(harness.patchLoadHookState->invoked);

        tempDir.deleteRecursively();
    }
};

static PatchManagerActionHandlerComputerNavigationTests patchManagerActionHandlerComputerNavigationTests;
