#include "PatchManagerActionHandlerTestSupport.h"

using namespace PatchManagerActionHandlerTestSupport;

class PatchManagerActionHandlerUnsavedChantier2Tests : public juce::UnitTest
{
public:
    PatchManagerActionHandlerUnsavedChantier2Tests()
        : juce::UnitTest("PatchManagerActionHandlerUnsavedChantier2")
    {
    }

    void runTest() override
    {
        testUnsavedGate_saveAsFromDeviceKeepsNotStored();
        testUnsavedGate_saveAsAfterDeviceEditKeepsNotStored();
        testUnsavedGate_cleanDeviceSaveAsDoesNotInventRamRisk();
        testUnsavedGate_tryPersistSaveOverwritesKnownSyx();
        testUnsavedGate_fileOriginWithoutPathResolvesSaveAs();
        testUnsavedGate_captureAfterSaveFromDeviceKeepsNotStored();
    }

private:
    void testUnsavedGate_saveAsFromDeviceKeepsNotStored()
    {
        beginTest("unsavedGate_saveAsFromDeviceKeepsNotStored");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 3, false);
        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());
        expect(harness.handler.isPatchNotStoredInRam());

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

        expect(harness.handler.tryPersistCurrentPatchFromUnsavedGate(
            Core::UnsavedEditPersistKind::kSaveAs));
        expect(harness.handler.isPatchNotStoredInRam());
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));
        expectEquals(
            static_cast<int>(harness.handler.resolveUnsavedEditPersistKind(false)),
            static_cast<int>(Core::UnsavedEditPersistKind::kStore));

        tempDir.deleteRecursively();
    }

    void testUnsavedGate_saveAsAfterDeviceEditKeepsNotStored()
    {
        beginTest("unsavedGate_saveAsAfterDeviceEditKeepsNotStored");

        // Device-origin dirty (STORE already done earlier / flag clear) then Save As:
        // must still mark not-STORED so leave warns (plan §4.5 / decision 5).
        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 3, false);
        harness.useSuccessfulDeviceDump();
        fireInternalPatchNavigation(harness, InternalPatches::kLoadNextPatch);
        expect(! harness.handler.isPatchNotStoredInRam());

        harness.model.setName("EDITED!!");
        harness.patchNameSyncer.bufferToApvts();
        expect(harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));

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

        expect(harness.handler.tryPersistCurrentPatchFromUnsavedGate(
            Core::UnsavedEditPersistKind::kSaveAs));
        expect(harness.handler.isPatchNotStoredInRam());
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));

        tempDir.deleteRecursively();
    }

    void testUnsavedGate_cleanDeviceSaveAsDoesNotInventRamRisk()
    {
        beginTest("unsavedGate_cleanDeviceSaveAsDoesNotInventRamRisk");

        // Clean ROM/device load (not INIT): Save As is an export copy only — do not mark
        // not-STORED, or OPEN after export falsely prompts Unsaved Patch.
        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 2, 0, false);
        harness.useSuccessfulDeviceDump();
        fireInternalPatchNavigation(harness, InternalPatches::kLoadNextPatch);
        expect(! harness.handler.isPatchNotStoredInRam());
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));

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

        expect(! harness.handler.isPatchNotStoredInRam());
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));

        tempDir.deleteRecursively();
    }

    void testUnsavedGate_tryPersistSaveOverwritesKnownSyx()
    {
        beginTest("unsavedGate_tryPersistSaveOverwritesKnownSyx");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile, 1, nullptr);
        simulateSelectPatchFileDispatch(harness);

        expect(harness.handler.hasUsableKnownSyxPath());
        expectEquals(
            static_cast<int>(harness.handler.resolveUnsavedEditPersistKind(true)),
            static_cast<int>(Core::UnsavedEditPersistKind::kSave));

        harness.model.setName("EDITED!!");
        harness.patchNameSyncer.bufferToApvts();
        expect(harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));

        expect(harness.handler.tryPersistCurrentPatchFromUnsavedGate(
            Core::UnsavedEditPersistKind::kSave));
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));
        expect(! harness.handler.isPatchNotStoredInRam());

        tempDir.deleteRecursively();
    }
    void testUnsavedGate_fileOriginWithoutPathResolvesSaveAs()
    {
        beginTest("unsavedGate_fileOriginWithoutPathResolvesSaveAs");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        const auto tempDir = createTempScanDir();
        expect(tempDir.createDirectory());
        copyFixturePatchToDir(tempDir, "Patch 71.syx");
        setupComputerPatchesScan(harness, tempDir);

        harness.proc.apvts.state.setProperty(
            ComputerPatches::StandaloneWidgets::kSelectPatchFile, 1, nullptr);
        simulateSelectPatchFileDispatch(harness);
        expect(harness.handler.hasUsableKnownSyxPath());

        // Parent folder gone → known path is no longer a usable Save target.
        expect(tempDir.deleteRecursively());
        expect(! harness.handler.hasUsableKnownSyxPath());
        expectEquals(
            static_cast<int>(harness.handler.resolveUnsavedEditPersistKind(true)),
            static_cast<int>(Core::UnsavedEditPersistKind::kSaveAs));
    }
    void testUnsavedGate_captureAfterSaveFromDeviceKeepsNotStored()
    {
        beginTest("unsavedGate_captureAfterSaveFromDeviceKeepsNotStored");

        HandlerHarness harness(Core::DeviceMemoryLimits::resolve(MatrixDeviceTypes::Type::kMatrix1000));
        initializePatchManagerState(harness.proc.apvts.state, 0, 3, false);
        harness.handler.handleAction(InternalPatches::kInitPatch, juce::var());
        expect(harness.handler.isPatchNotStoredInRam());

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

        expect(harness.dirtyPatchTracker.hasSnapshot());
        expect(! harness.dirtyPatchTracker.syncApvtsAndIsDirty(
            harness.mapper, harness.patchNameSyncer, harness.model));
        expect(harness.handler.isPatchNotStoredInRam());

        tempDir.deleteRecursively();
    }
};

static PatchManagerActionHandlerUnsavedChantier2Tests patchManagerActionHandlerUnsavedChantier2Tests;
