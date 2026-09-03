#include <juce_core/juce_core.h>

#include "Core/Services/UnsavedEditWarningPolicy.h"
#include "Shared/Definitions/PluginIDs.h"

namespace
{
    using namespace PluginIDs::Settings::UnsavedStatePolicy;

    struct CloseGateScenario
    {
        int policyId = kAlwaysWarn;
        bool isDirty = false;
        bool notStoredInRam = false;
        Core::UnsavedEditConfirmChoice gateChoice = Core::UnsavedEditConfirmChoice::kCancel;
        bool gateWired = true;
        bool persistSucceeded = true;
    };

    bool simulateSessionCloseGate(const CloseGateScenario& scenario)
    {
        if (! Core::UnsavedEditWarning::shouldPrompt(
                scenario.policyId, scenario.isDirty, scenario.notStoredInRam))
            return true;

        if (! scenario.gateWired)
            return true;

        switch (scenario.gateChoice)
        {
            case Core::UnsavedEditConfirmChoice::kCancel:
                return false;
            case Core::UnsavedEditConfirmChoice::kDiscard:
                return true;
            case Core::UnsavedEditConfirmChoice::kPersist:
                return scenario.persistSucceeded;
        }

        return false;
    }
}

class PluginProcessorCloseGateTests : public juce::UnitTest
{
public:
    PluginProcessorCloseGateTests() : juce::UnitTest("PluginProcessorCloseGate") {}

    void runTest() override
    {
        testIsAtRisk_dirtyOrNotStored();
        testCloseGate_neverWarnSkipsPrompt();
        testCloseGate_atRiskCancelBlocks();
        testCloseGate_atRiskDiscardProceeds();
        testCloseGate_persistFailBlocksClose();
        testCloseGate_headlessNoGateProceeds();
        testCloseGate_cleanProceedsWithoutGate();
    }

private:
    void testIsAtRisk_dirtyOrNotStored()
    {
        beginTest("isAtRisk — dirty or not STORED");

        expect(Core::UnsavedEditWarning::isAtRisk(true, false));
        expect(Core::UnsavedEditWarning::isAtRisk(false, true));
        expect(Core::UnsavedEditWarning::isAtRisk(true, true));
        expect(! Core::UnsavedEditWarning::isAtRisk(false, false));
    }

    void testCloseGate_neverWarnSkipsPrompt()
    {
        beginTest("close gate — never-warn proceeds without modal");

        CloseGateScenario scenario;
        scenario.policyId = kNeverWarn;
        scenario.isDirty = true;
        scenario.notStoredInRam = true;
        scenario.gateChoice = Core::UnsavedEditConfirmChoice::kCancel;

        expect(simulateSessionCloseGate(scenario));
    }

    void testCloseGate_atRiskCancelBlocks()
    {
        beginTest("close gate — Cancel blocks close");

        CloseGateScenario dirtyScenario;
        dirtyScenario.isDirty = true;
        dirtyScenario.gateChoice = Core::UnsavedEditConfirmChoice::kCancel;
        expect(! simulateSessionCloseGate(dirtyScenario));

        CloseGateScenario notStoredScenario;
        notStoredScenario.notStoredInRam = true;
        notStoredScenario.gateChoice = Core::UnsavedEditConfirmChoice::kCancel;
        expect(! simulateSessionCloseGate(notStoredScenario));
    }

    void testCloseGate_atRiskDiscardProceeds()
    {
        beginTest("close gate — Discard proceeds");

        CloseGateScenario scenario;
        scenario.isDirty = true;
        scenario.gateChoice = Core::UnsavedEditConfirmChoice::kDiscard;

        expect(simulateSessionCloseGate(scenario));
    }

    void testCloseGate_persistFailBlocksClose()
    {
        beginTest("close gate — Persist failure blocks close");

        CloseGateScenario scenario;
        scenario.isDirty = true;
        scenario.gateChoice = Core::UnsavedEditConfirmChoice::kPersist;
        scenario.persistSucceeded = false;

        expect(! simulateSessionCloseGate(scenario));
    }

    void testCloseGate_headlessNoGateProceeds()
    {
        beginTest("close gate — headless without modal gate proceeds");

        CloseGateScenario scenario;
        scenario.isDirty = true;
        scenario.gateWired = false;

        expect(simulateSessionCloseGate(scenario));
    }

    void testCloseGate_cleanProceedsWithoutGate()
    {
        beginTest("close gate — clean patch proceeds");

        CloseGateScenario scenario;
        scenario.gateChoice = Core::UnsavedEditConfirmChoice::kCancel;

        expect(simulateSessionCloseGate(scenario));
    }
};

static PluginProcessorCloseGateTests pluginProcessorCloseGateTests;
