#include <cstring>

#include <juce_core/juce_core.h>

#include "Core/Models/PackedFieldCodec.h"
#include "Core/Models/PatchModel.h"
#include "Core/MIDI/SysEx/SysExConstants.h"
#include "Core/Services/ClipboardService.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"

namespace PatchEdit = PluginIDs::PatchEditSection;

class ClipboardServiceTests : public juce::UnitTest
{
public:
    ClipboardServiceTests() : juce::UnitTest("ClipboardService") {}

    void runTest() override
    {
        testEnv1ToEnv3FullCopy();
        testEnvShapeOnlyLeavesNonShapeParams();
        testDco1ToDco2SkipsSyncAndDetune();
        testDco2ToDco1SkipsNoiseAndOff();
        testDco2ToDco1PreservesSync();
        testLfo1ToLfo2SkipsIncompatibleModSources();
        testLfo2ToLfo1SkipsIncompatibleModSources();
        testLfoSameModuleStillCopiesModSources();
        testMatrixModSnapshotIsolation();
        testFullPatchCaptures134Bytes();
        testModeReplacementModuleAfterFullPatch();
        testDco1ToEnv1Rejected();
        testClearReturnsToEmpty();
    }

private:

    static const PluginDescriptors::IntParameterDescriptor& dco1Int(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Dco1Module::kIntParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Dco1Module::kIntParameters.front();
    }

    static const PluginDescriptors::ChoiceParameterDescriptor& dco1Choice(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Dco1Module::kChoiceParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Dco1Module::kChoiceParameters.front();
    }

    static const PluginDescriptors::IntParameterDescriptor& dco2Int(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Dco2Module::kIntParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Dco2Module::kIntParameters.front();
    }

    static const PluginDescriptors::ChoiceParameterDescriptor& dco2Choice(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Dco2Module::kChoiceParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Dco2Module::kChoiceParameters.front();
    }

    static const PluginDescriptors::IntParameterDescriptor& envInt(int module,
                                                                   const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;

        const auto& params = module == 1 ? Envelope1Module::kIntParameters
                            : module == 2 ? Envelope2Module::kIntParameters
                                          : Envelope3Module::kIntParameters;

        for (const auto& descriptor : params)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return params.front();
    }

    static const PluginDescriptors::ChoiceParameterDescriptor& envChoice(int module,
                                                                         const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;

        const auto& params = module == 1 ? Envelope1Module::kChoiceParameters
                            : module == 2 ? Envelope2Module::kChoiceParameters
                                          : Envelope3Module::kChoiceParameters;

        for (const auto& descriptor : params)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return params.front();
    }

    static const PluginDescriptors::IntParameterDescriptor& lfo1Int(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Lfo1Module::kIntParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Lfo1Module::kIntParameters.front();
    }

    static const PluginDescriptors::IntParameterDescriptor& lfo2Int(const char* paramId)
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Lfo2Module::kIntParameters)
        {
            if (descriptor.parameterId == paramId)
                return descriptor;
        }

        jassertfalse;
        return Lfo2Module::kIntParameters.front();
    }

    static juce::uint8 byteAt(const Core::PatchModel& model, int sysExOffset)
    {
        const auto offset = Core::PackedFieldCodec::safeOffset(sysExOffset, Core::PatchModel::kBufferSize);
        return model.data()[offset];
    }

    void testEnv1ToEnv3FullCopy()
    {
        beginTest("env1ToEnv3 — full ENV interchange");

        Core::PatchModel source;
        source.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDelay), 11);
        source.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAttack), 22);
        source.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kSustain), 33);
        source.setChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kTriggerMode), 2);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Env1, source);

        Core::PatchModel target;
        expect(clipboard.canPasteModule(Core::PatchModuleKind::Env3));
        expect(clipboard.pasteModule(Core::PatchModuleKind::Env3, target));

        expectEquals(target.getValue(envInt(3, PatchEdit::Envelope3Module::ParameterWidgets::kDelay)), 11);
        expectEquals(target.getValue(envInt(3, PatchEdit::Envelope3Module::ParameterWidgets::kAttack)), 22);
        expectEquals(target.getValue(envInt(3, PatchEdit::Envelope3Module::ParameterWidgets::kSustain)), 33);
        expectEquals(target.getChoiceIndex(envChoice(3, PatchEdit::Envelope3Module::ParameterWidgets::kTriggerMode)), 2);
    }

    void testEnvShapeOnlyLeavesNonShapeParams()
    {
        beginTest("env shape-only — D/A/D/S/R only; leave amplitude/trigger");

        Core::PatchModel source;
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kDelay), 10);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kAttack), 20);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kDecay), 30);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kSustain), 40);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kRelease), 50);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kAmplitude), 60);
        source.setValue(envInt(2, PatchEdit::Envelope2Module::ParameterWidgets::kAmplitudeModByVelocity), 15);
        source.setChoiceIndex(envChoice(2, PatchEdit::Envelope2Module::ParameterWidgets::kTriggerMode), 2);
        source.setChoiceIndex(envChoice(2, PatchEdit::Envelope2Module::ParameterWidgets::kEnvelopeMode), 1);
        source.setChoiceIndex(envChoice(2, PatchEdit::Envelope2Module::ParameterWidgets::kLfo1Trigger), 1);

        Core::PatchModel target;
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDelay), 1);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAttack), 2);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDecay), 3);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kSustain), 4);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kRelease), 5);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAmplitude), 7);
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAmplitudeModByVelocity), -9);
        target.setChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kTriggerMode), 0);
        target.setChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kEnvelopeMode), 0);
        target.setChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kLfo1Trigger), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Env2, source, true);
        expect(clipboard.isEnvelopeShapeOnly());
        expect(clipboard.pasteModule(Core::PatchModuleKind::Env1, target));

        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDelay)), 10);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAttack)), 20);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDecay)), 30);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kSustain)), 40);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kRelease)), 50);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAmplitude)), 7);
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kAmplitudeModByVelocity)), -9);
        expectEquals(target.getChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kTriggerMode)), 0);
        expectEquals(target.getChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kEnvelopeMode)), 0);
        expectEquals(target.getChoiceIndex(envChoice(1, PatchEdit::Envelope1Module::ParameterWidgets::kLfo1Trigger)), 0);
    }

    void testDco1ToDco2SkipsSyncAndDetune()
    {
        beginTest("dco1ToDco2 — skip SYNC/DETUNE cross-mapping");

        Core::PatchModel source;
        source.setChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kSync), 2);
        source.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency), 41);

        Core::PatchModel target;
        target.setValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kDetune), 7);
        target.setValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kFrequency), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco1, source);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Dco2, target));

        expectEquals(target.getValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kDetune)), 7);
        expectEquals(target.getValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kFrequency)), 41);
    }

    void testDco2ToDco1SkipsNoiseAndOff()
    {
        beginTest("dco2ToDco1 — skip WAVE SELECT NOISE and KEYBD/PORTA OFF");

        Core::PatchModel source;
        source.setChoiceIndex(dco2Choice(PatchEdit::Dco2Module::ParameterWidgets::kWaveSelect), 4);
        source.setChoiceIndex(dco2Choice(PatchEdit::Dco2Module::ParameterWidgets::kKeyboardPortamento), 0);
        source.setValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kPulseWidth), 55);

        Core::PatchModel target;
        target.setChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kWaveSelect), 1);
        target.setChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kKeyboardPortamento), 1);
        target.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kPulseWidth), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco2, source);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Dco1, target));

        expectEquals(target.getChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kWaveSelect)), 1);
        expectEquals(target.getChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kKeyboardPortamento)), 1);
        expectEquals(target.getValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kPulseWidth)), 55);
    }

    void testDco2ToDco1PreservesSync()
    {
        beginTest("dco2ToDco1 — DCO1 SYNC unchanged on cross-paste");

        Core::PatchModel source;
        source.setValue(dco2Int(PatchEdit::Dco2Module::ParameterWidgets::kFrequency), 63);

        Core::PatchModel target;
        target.setChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kSync), 2);
        target.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco2, source);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Dco1, target));

        expectEquals(target.getChoiceIndex(dco1Choice(PatchEdit::Dco1Module::ParameterWidgets::kSync)), 2);
        expectEquals(target.getValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency)), 63);
    }

    void testLfo1ToLfo2SkipsIncompatibleModSources()
    {
        beginTest("lfo1ToLfo2 — skip Pressure/Ramp1 mods; copy shared params");

        Core::PatchModel source;
        source.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure), 17);
        source.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1), -25);
        source.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeed), 42);
        source.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitude), 33);

        Core::PatchModel target;
        target.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard), 9);
        target.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2), 11);
        target.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeed), 0);
        target.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitude), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Lfo1, source);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Lfo2, target));

        expectEquals(target.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard)), 9);
        expectEquals(target.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2)), 11);
        expectEquals(target.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeed)), 42);
        expectEquals(target.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitude)), 33);
    }

    void testLfo2ToLfo1SkipsIncompatibleModSources()
    {
        beginTest("lfo2ToLfo1 — skip Keyboard/Ramp2 mods; copy shared params");

        Core::PatchModel source;
        source.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard), 17);
        source.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2), -25);
        source.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeed), 42);
        source.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitude), 33);

        Core::PatchModel target;
        target.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure), 9);
        target.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1), 11);
        target.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeed), 0);
        target.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitude), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Lfo2, source);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Lfo1, target));

        expectEquals(target.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure)), 9);
        expectEquals(target.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1)), 11);
        expectEquals(target.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeed)), 42);
        expectEquals(target.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitude)), 33);
    }

    void testLfoSameModuleStillCopiesModSources()
    {
        beginTest("lfo same-module — Pressure/Ramp1 and Keyboard/Ramp2 still copy");

        Core::PatchModel sourceLfo1;
        sourceLfo1.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure), 17);
        sourceLfo1.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1), -25);

        Core::PatchModel targetLfo1;
        targetLfo1.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure), 0);
        targetLfo1.setValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1), 0);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Lfo1, sourceLfo1);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Lfo1, targetLfo1));
        expectEquals(targetLfo1.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kSpeedModByPressure)), 17);
        expectEquals(targetLfo1.getValue(lfo1Int(PatchEdit::Lfo1Module::ParameterWidgets::kAmplitudeModByRamp1)), -25);

        Core::PatchModel sourceLfo2;
        sourceLfo2.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard), 19);
        sourceLfo2.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2), -27);

        Core::PatchModel targetLfo2;
        targetLfo2.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard), 0);
        targetLfo2.setValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2), 0);

        clipboard.copyModule(Core::PatchModuleKind::Lfo2, sourceLfo2);
        expect(clipboard.pasteModule(Core::PatchModuleKind::Lfo2, targetLfo2));
        expectEquals(targetLfo2.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kSpeedModByKeyboard)), 19);
        expectEquals(targetLfo2.getValue(lfo2Int(PatchEdit::Lfo2Module::ParameterWidgets::kAmplitudeModByRamp2)), -27);
    }

    void testMatrixModSnapshotIsolation()
    {
        beginTest("matrixMod — snapshot isolation + round-trip");

        Core::PatchModel source;
        source.setName("SENTINEL");
        source.data()[20] = 0xAB;
        source.data()[103] = 0xCD;

        for (int bus = 0; bus < 10; ++bus)
        {
            const int base = 104 + 3 * bus;
            source.data()[static_cast<size_t>(base)] = static_cast<juce::uint8>(bus + 1);
            source.data()[static_cast<size_t>(base + 1)] = static_cast<juce::uint8>(bus + 10);
            source.data()[static_cast<size_t>(base + 2)] = static_cast<juce::uint8>(bus + 20);
        }

        Core::ClipboardService clipboard;
        clipboard.copyMatrixModulation(source);

        Core::PatchModel target;
        target.setName("KEEPNAME");
        target.data()[20] = 0x12;
        target.data()[103] = 0x34;
        expect(clipboard.pasteMatrixModulation(target));

        expectEquals(target.getName(), juce::String("KEEPNAME"));
        expectEquals(static_cast<int>(byteAt(target, 20)), 0x12);
        expectEquals(static_cast<int>(byteAt(target, 103)), 0x34);

        for (int bus = 0; bus < 10; ++bus)
        {
            const int base = 104 + 3 * bus;
            expectEquals(static_cast<int>(byteAt(target, base)), bus + 1);
            expectEquals(static_cast<int>(byteAt(target, base + 1)), bus + 10);
            expectEquals(static_cast<int>(byteAt(target, base + 2)), bus + 20);
        }
    }

    void testFullPatchCaptures134Bytes()
    {
        beginTest("fullPatch — captures entire 134-byte buffer");

        Core::PatchModel source;
        for (size_t i = 0; i < Core::PatchModel::kBufferSize; ++i)
            source.data()[i] = static_cast<juce::uint8>(i + 1);

        Core::ClipboardService clipboard;
        clipboard.copyFullPatch(source, "BANK 0 / PATCH 0");

        Core::PatchModel target;
        expect(clipboard.pasteFullPatch(target));
        expect(std::memcmp(source.data(), target.data(), Core::PatchModel::kBufferSize) == 0);
        expect(clipboard.getMode() == Core::ClipboardMode::FullPatch);
    }

    void testModeReplacementModuleAfterFullPatch()
    {
        beginTest("modeReplacement — module copy after full-patch copy switches mode");

        Core::PatchModel patch;
        patch.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency), 9);

        Core::ClipboardService clipboard;
        clipboard.copyFullPatch(patch, "BANK 0 / PATCH 0");
        expect(clipboard.canPasteFullPatch());
        expect(!clipboard.canPasteModule(Core::PatchModuleKind::Dco2));
        expect(!clipboard.getSourceModuleKind().has_value());

        clipboard.copyModule(Core::PatchModuleKind::Dco1, patch);
        expect(clipboard.getMode() == Core::ClipboardMode::Module);
        expect(clipboard.canPasteModule(Core::PatchModuleKind::Dco2));
        expect(!clipboard.canPasteFullPatch());
        expect(clipboard.getSourceModuleKind().has_value());
        expect(*clipboard.getSourceModuleKind() == Core::PatchModuleKind::Dco1);
    }

    void testDco1ToEnv1Rejected()
    {
        beginTest("dco1ToEnv1 — incompatible pair rejected");

        Core::PatchModel source;
        source.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency), 50);

        Core::PatchModel target;
        target.setValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDelay), 5);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco1, source);

        expect(!clipboard.canPasteModule(Core::PatchModuleKind::Env1));
        expect(!clipboard.pasteModule(Core::PatchModuleKind::Env1, target));
        expectEquals(target.getValue(envInt(1, PatchEdit::Envelope1Module::ParameterWidgets::kDelay)), 5);
    }

    void testClearReturnsToEmpty()
    {
        beginTest("clear — returns Empty and rejects paste");

        Core::PatchModel source;
        source.setValue(dco1Int(PatchEdit::Dco1Module::ParameterWidgets::kFrequency), 40);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco1, source);
        expect(clipboard.hasContent());

        clipboard.clear();
        expect(!clipboard.hasContent());
        expect(clipboard.getMode() == Core::ClipboardMode::Empty);
        expect(!clipboard.getSourceModuleKind().has_value());
        expect(!clipboard.canPasteModule(Core::PatchModuleKind::Dco2));
        expect(!clipboard.canPasteFullPatch());
        expect(!clipboard.canPasteMatrixModulation());
    }
};

static ClipboardServiceTests clipboardServiceTests;
