#include <juce_core/juce_core.h>

#include "Core/MIDI/Queue/RealtimeQueuePressureMonitor.h"

class RealtimeQueuePressureMonitorTests : public juce::UnitTest
{
public:
    RealtimeQueuePressureMonitorTests() : juce::UnitTest("RealtimeQueuePressureMonitor") {}

    void runTest() override
    {
        testEngageRequiresSustainedDepth();
        testClearAfterSettle();
        testNoFlickerNearThreshold();
    }

private:
    void testEngageRequiresSustainedDepth()
    {
        beginTest("Alert engages only after sustained high realtime depth");

        Core::RealtimeQueuePressureMonitor monitor;
        expect(!monitor.isAlertActive());
        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth, 1000));
        expect(!monitor.isAlertActive());
        expect(monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth, 1033));
        expect(monitor.isAlertActive());
        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth + 10, 1066));
        expect(monitor.isAlertActive());
    }

    void testClearAfterSettle()
    {
        beginTest("Alert clears only after low depth settles");

        Core::RealtimeQueuePressureMonitor monitor;
        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth, 0));
        expect(monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth, 33));
        expect(monitor.isAlertActive());

        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kClearDepth, 100));
        expect(monitor.isAlertActive());

        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kClearDepth,
                               100 + Core::RealtimeQueuePressureMonitor::kSettleMs - 1));
        expect(monitor.isAlertActive());

        expect(monitor.update(Core::RealtimeQueuePressureMonitor::kClearDepth,
                              100 + Core::RealtimeQueuePressureMonitor::kSettleMs));
        expect(!monitor.isAlertActive());
    }

    void testNoFlickerNearThreshold()
    {
        beginTest("Depth bouncing below engage does not engage");

        Core::RealtimeQueuePressureMonitor monitor;
        expect(!monitor.update(Core::RealtimeQueuePressureMonitor::kEngageDepth - 1, 0));
        expect(!monitor.isAlertActive());
        expect(!monitor.update(0, 100));
        expect(!monitor.isAlertActive());
    }
};

static RealtimeQueuePressureMonitorTests realtimeQueuePressureMonitorTests;
