#include <juce_core/juce_core.h>

#include "Core/MIDI/DeviceInquiryCaptureFilter.h"
#include "Core/MIDI/Transport/MidiReceiver.h"

namespace
{
juce::MemoryBlock bytes(std::initializer_list<juce::uint8> data)
{
    return juce::MemoryBlock(data.begin(), data.size());
}

const auto kInquiryRequestEcho = bytes({ 0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 });

const auto kMatrix1000Identity = bytes({
    0xF0, 0x7E, 0x00, 0x06, 0x02, 0x10, 0x06, 0x00, 0x02, 0x00, 0x20, 0x31, 0x32, 0x30, 0xF7
});
} // namespace

class DeviceInquiryCaptureFilterTests : public juce::UnitTest
{
public:
    DeviceInquiryCaptureFilterTests() : juce::UnitTest("DeviceInquiryCaptureFilter") {}

    void runTest() override
    {
        testRejectsInquiryRequestEcho();
        testAcceptsMatrix1000Identity();
        testRejectsMissingF7AndForeignManufacturer();
        testFilterKeepsArmedUntilIdentity();
        testUnfilteredOneShotConsumesFirstSysEx();
    }

private:
    void testRejectsInquiryRequestEcho()
    {
        beginTest("isDeviceInquiryIdentityReply — rejects Universal Device Inquiry request echo");

        expect(! Core::isDeviceInquiryIdentityReply(kInquiryRequestEcho));
        expect(! Core::isDeviceInquiryIdentityReply(bytes({ 0xF0, 0x7E, 0x00, 0x06, 0x02 })));
    }

    void testAcceptsMatrix1000Identity()
    {
        beginTest("isDeviceInquiryIdentityReply — accepts lab M-1000 identity frame");

        expect(Core::isDeviceInquiryIdentityReply(kMatrix1000Identity));
    }

    void testRejectsMissingF7AndForeignManufacturer()
    {
        beginTest("isDeviceInquiryIdentityReply — rejects missing F7 and non-Oberheim identity");

        auto missingF7 = kMatrix1000Identity;
        missingF7[missingF7.getSize() - 1] = 0x00;
        expect(! Core::isDeviceInquiryIdentityReply(missingF7));

        auto foreign = kMatrix1000Identity;
        auto* data = static_cast<juce::uint8*>(foreign.getData());
        data[5] = 0x41; // non-Oberheim manufacturer
        expect(! Core::isDeviceInquiryIdentityReply(foreign));
    }

    void testFilterKeepsArmedUntilIdentity()
    {
        beginTest("armOneShotSysExCapture — filter ignores echo then delivers Device ID");

        MidiReceiver receiver;
        int deliveries = 0;
        juce::MemoryBlock last;

        receiver.armOneShotSysExCapture(
            [&](const juce::MemoryBlock& sysex)
            {
                ++deliveries;
                last = sysex;
            },
            [](const juce::MemoryBlock& sysex)
            {
                return Core::isDeviceInquiryIdentityReply(sysex);
            });

        receiver.feedCompleteSysEx(kInquiryRequestEcho);
        expectEquals(deliveries, 0);

        receiver.feedCompleteSysEx(kMatrix1000Identity);
        expectEquals(deliveries, 1);
        expect(last == kMatrix1000Identity);

        // One-shot cleared after match — a second identity must not fire again.
        receiver.feedCompleteSysEx(kMatrix1000Identity);
        expectEquals(deliveries, 1);
    }

    void testUnfilteredOneShotConsumesFirstSysEx()
    {
        beginTest("armOneShotSysExCapture — without filter, first SysEx wins (patch path)");

        MidiReceiver receiver;
        int deliveries = 0;

        receiver.armOneShotSysExCapture(
            [&](const juce::MemoryBlock&)
            {
                ++deliveries;
            });

        receiver.feedCompleteSysEx(kInquiryRequestEcho);
        expectEquals(deliveries, 1);

        receiver.feedCompleteSysEx(kMatrix1000Identity);
        expectEquals(deliveries, 1);
    }
};

static DeviceInquiryCaptureFilterTests deviceInquiryCaptureFilterTests;
