#include <juce_core/juce_core.h>

#include "GUI/Helpers/LockDimmingFilmPolicy.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

class LockDimmingFilmPolicyTests : public juce::UnitTest
{
public:
    LockDimmingFilmPolicyTests() : juce::UnitTest("LockDimmingFilmPolicy") {}

    void runTest() override
    {
        deviceLockNoSynthUsesHeaderFooterOnly();
        deviceLockUnsupportedUsesHeaderFooterOnly();
        deviceLockMidiUnresponsiveUsesHeaderFooterOnly();
        compareWithSupportedDeviceAddsCompareHole();
        deviceLockWinsOverCompareWithoutCompareHole();
        unlockTurnsFilmOff();
        filmHitTestBlocksOutsideHolesAndPassesInside();
    }

private:
    using Mode = TSS::LockDimmingFilmMode;
    using Type = MatrixDeviceTypes::Type;

    void deviceLockNoSynthUsesHeaderFooterOnly()
    {
        beginTest("Device lock — no synth → Header+Footer only, no COMPARE hole");
        const auto mode = TSS::resolveLockDimmingFilmMode(false, Type::kMatrix1000, false, false);
        expect(mode == Mode::kHeaderFooterOnly);
        expect(TSS::lockDimmingFilmIsActive(mode));
        expect(! TSS::lockDimmingFilmIncludesCompareHole(mode));
    }

    void deviceLockUnsupportedUsesHeaderFooterOnly()
    {
        beginTest("Device lock — unsupported Matrix member → Header+Footer only");
        const auto mode = TSS::resolveLockDimmingFilmMode(true, Type::kUnknown, false, false);
        expect(mode == Mode::kHeaderFooterOnly);
        expect(! TSS::lockDimmingFilmIncludesCompareHole(mode));
    }

    void deviceLockMidiUnresponsiveUsesHeaderFooterOnly()
    {
        beginTest("Device lock — MIDI unresponsive → Header+Footer only");
        const auto mode = TSS::resolveLockDimmingFilmMode(true, Type::kMatrix1000, false, true);
        expect(mode == Mode::kHeaderFooterOnly);
        expect(! TSS::lockDimmingFilmIncludesCompareHole(mode));
    }

    void compareWithSupportedDeviceAddsCompareHole()
    {
        beginTest("Compare + device OK → Header+Footer+COMPARE hole");
        const auto mode = TSS::resolveLockDimmingFilmMode(true, Type::kMatrix1000, true, false);
        expect(mode == Mode::kHeaderFooterCompare);
        expect(TSS::lockDimmingFilmIsActive(mode));
        expect(TSS::lockDimmingFilmIncludesCompareHole(mode));
    }

    void deviceLockWinsOverCompareWithoutCompareHole()
    {
        beginTest("Device lock + Compare → device wins, no COMPARE hole");
        const auto mode = TSS::resolveLockDimmingFilmMode(false, Type::kMatrix1000, true, false);
        expect(mode == Mode::kHeaderFooterOnly);
        expect(! TSS::lockDimmingFilmIncludesCompareHole(mode));

        const auto unsupportedWithCompare =
            TSS::resolveLockDimmingFilmMode(true, Type::kUnknown, true, false);
        expect(unsupportedWithCompare == Mode::kHeaderFooterOnly);
        expect(! TSS::lockDimmingFilmIncludesCompareHole(unsupportedWithCompare));

        const auto unresponsiveWithCompare =
            TSS::resolveLockDimmingFilmMode(true, Type::kMatrix1000, true, true);
        expect(unresponsiveWithCompare == Mode::kHeaderFooterOnly);
        expect(! TSS::lockDimmingFilmIncludesCompareHole(unresponsiveWithCompare));
    }

    void unlockTurnsFilmOff()
    {
        beginTest("Unlock — device OK, Compare off → film off");
        const auto mode = TSS::resolveLockDimmingFilmMode(true, Type::kMatrix1000, false, false);
        expect(mode == Mode::kOff);
        expect(! TSS::lockDimmingFilmIsActive(mode));
        expect(! TSS::lockDimmingFilmIncludesCompareHole(mode));
    }

    void filmHitTestBlocksOutsideHolesAndPassesInside()
    {
        beginTest("Film hit-test — block outside holes, pass through inside");
        const std::vector<juce::Rectangle<int>> holes {
            { 0, 0, 100, 20 },
            { 0, 200, 100, 20 },
            { 40, 100, 20, 10 }
        };

        expect(! TSS::lockDimmingFilmBlocksPoint(holes, 10, 10));   // header hole
        expect(! TSS::lockDimmingFilmBlocksPoint(holes, 50, 105));  // COMPARE hole
        expect(TSS::lockDimmingFilmBlocksPoint(holes, 50, 80));     // body under film
        expect(TSS::lockDimmingFilmBlocksPoint({}, 0, 0));          // no holes → full block
    }
};

static LockDimmingFilmPolicyTests lockDimmingFilmPolicyTests;
