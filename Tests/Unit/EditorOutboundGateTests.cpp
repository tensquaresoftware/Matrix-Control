#include <juce_core/juce_core.h>

#include "Core/MIDI/EditorOutboundGate.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

class EditorOutboundGateTests : public juce::UnitTest
{
public:
    EditorOutboundGateTests() : juce::UnitTest("EditorOutboundGate") {}

    void runTest() override
    {
        using Type = MatrixDeviceTypes::Type;

        beginTest("isEditorOutboundAllowed — supported device only");
        expect(! Core::isEditorOutboundAllowed(false, Type::kMatrix1000));
        expect(! Core::isEditorOutboundAllowed(true, Type::kUnknown));
        expect(Core::isEditorOutboundAllowed(true, Type::kMatrix1000));
        expect(Core::isEditorOutboundAllowed(true, Type::kMatrix6));
        expect(Core::isEditorOutboundAllowed(true, Type::kMatrix6R));

        beginTest("maySendEditorProgramChange — blocked when undetected or Unknown");
        expect(! Core::maySendEditorProgramChange(false, Type::kMatrix1000));
        expect(! Core::maySendEditorProgramChange(true, Type::kUnknown));
        expect(Core::maySendEditorProgramChange(true, Type::kMatrix1000));

        beginTest("maySendEditorSysEx — blocks editor SysEx when undetected or Unknown");
        juce::MemoryBlock remoteEdit;
        {
            const juce::uint8 bytes[] = { 0xF0, 0x10, 0x06, 0x06, 0x00, 0x40, 0xF7 };
            remoteEdit.append(bytes, sizeof(bytes));
        }
        expect(! Core::maySendEditorSysEx(false, Type::kMatrix1000, remoteEdit));
        expect(! Core::maySendEditorSysEx(true, Type::kUnknown, remoteEdit));
        expect(Core::maySendEditorSysEx(true, Type::kMatrix1000, remoteEdit));

        beginTest("maySendEditorSysEx — Device Inquiry allowlisted while locked");
        const auto inquiry = SysExEncoder::encodeDeviceInquiry();
        expect(Core::isDeviceInquirySysEx(inquiry));
        expect(Core::maySendEditorSysEx(false, Type::kUnknown, inquiry));
        expect(Core::maySendEditorSysEx(true, Type::kUnknown, inquiry));
        expect(Core::maySendEditorSysEx(true, Type::kMatrix1000, inquiry));

        beginTest("maySendEditorProgramChange / SysEx — blocked when MIDI-unresponsive");
        expect(! Core::maySendEditorProgramChange(true, Type::kMatrix1000, true));
        expect(! Core::maySendEditorSysEx(true, Type::kMatrix1000, remoteEdit, true));
        expect(Core::maySendEditorSysEx(true, Type::kMatrix1000, inquiry, true));

        beginTest("isSectionLocked — undetected, Unknown, Compare, and unresponsive");
        expect(Core::isSectionLocked(false, Type::kMatrix1000, false, false));
        expect(Core::isSectionLocked(true, Type::kUnknown, false, false));
        expect(Core::isSectionLocked(true, Type::kMatrix1000, true, false));
        expect(Core::isSectionLocked(true, Type::kMatrix1000, false, true));
        expect(! Core::isSectionLocked(true, Type::kMatrix1000, false, false));
    }
};

static EditorOutboundGateTests editorOutboundGateTests;
