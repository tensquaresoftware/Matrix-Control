#include <juce_core/juce_core.h>

#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardFeedbackResolver.h"
#include "Core/Services/ClipboardService.h"
#include "Shared/Definitions/PluginDescriptors.h"
#include "Shared/Definitions/PluginIDs.h"

namespace PatchEdit = PluginIDs::PatchEditSection;

class ClipboardFeedbackResolverTests : public juce::UnitTest
{
public:
    ClipboardFeedbackResolverTests() : juce::UnitTest("ClipboardFeedbackResolver") {}

    void runTest() override
    {
        testInactiveSessionYieldsNoBlink();
        testDco1CopyBlinksOtherDcoPasteOnly();
        testEnv1CopyBlinksEnv2AndEnv3Paste();
        testMatrixSamePatchCopyOnly();
        testMatrixCrossPatchUnlocksPaste();
        testFullPatchCrossPatchUnlocksPaste();
        testLfo1CopyBlinksLfo2PasteOnly();
    }

private:
    static const PluginDescriptors::IntParameterDescriptor& dco1Freq()
    {
        using namespace PluginDescriptors::PatchEditSection;
        for (const auto& descriptor : Dco1Module::kIntParameters)
        {
            if (descriptor.parameterId == PatchEdit::Dco1Module::ParameterWidgets::kFrequency)
                return descriptor;
        }

        jassertfalse;
        return Dco1Module::kIntParameters.front();
    }

    void testInactiveSessionYieldsNoBlink()
    {
        beginTest("inactive session — no blink roles");

        Core::PatchModel model;
        model.setValue(dco1Freq(), 10);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco1, model);

        const auto state = Core::resolveClipboardFeedback(clipboard, false, false);
        expect(!state.active);
        expect(!state.dco1Copy);
        expect(!state.dco2Paste);
    }

    void testDco1CopyBlinksOtherDcoPasteOnly()
    {
        beginTest("dco1 copy — source C + dco2 P, not dco1 P");

        Core::PatchModel model;
        model.setValue(dco1Freq(), 10);

        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Dco1, model);

        const auto state = Core::resolveClipboardFeedback(clipboard, true, false);
        expect(state.active);
        expect(state.dco1Copy);
        expect(!state.dco2Copy);
        expect(!state.dco1Paste);
        expect(state.dco2Paste);
        expect(!state.env1Paste);
    }

    void testEnv1CopyBlinksEnv2AndEnv3Paste()
    {
        beginTest("env1 copy — env2+env3 paste roles");

        Core::PatchModel model;
        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Env1, model);

        const auto state = Core::resolveClipboardFeedback(clipboard, true, false);
        expect(state.env1Copy);
        expect(!state.env1Paste);
        expect(state.env2Paste);
        expect(state.env3Paste);
        expect(!state.dco1Paste);
    }

    void testMatrixSamePatchCopyOnly()
    {
        beginTest("matrix copy same patch — C only");

        Core::PatchModel model;
        Core::ClipboardService clipboard;
        clipboard.copyMatrixModulation(model);

        const auto state = Core::resolveClipboardFeedback(clipboard, true, false);
        expect(state.matrixModulationCopy);
        expect(!state.matrixModulationPaste);
    }

    void testMatrixCrossPatchUnlocksPaste()
    {
        beginTest("matrix copy after other patch — C and P");

        Core::PatchModel model;
        Core::ClipboardService clipboard;
        clipboard.copyMatrixModulation(model);

        const auto state = Core::resolveClipboardFeedback(clipboard, true, true);
        expect(state.matrixModulationCopy);
        expect(state.matrixModulationPaste);
    }

    void testFullPatchCrossPatchUnlocksPaste()
    {
        beginTest("full patch after other patch — COPY and PASTE");

        Core::PatchModel model;
        Core::ClipboardService clipboard;
        clipboard.copyFullPatch(model);

        const auto samePatch = Core::resolveClipboardFeedback(clipboard, true, false);
        expect(samePatch.internalPatchesCopy);
        expect(!samePatch.internalPatchesPaste);

        const auto otherPatch = Core::resolveClipboardFeedback(clipboard, true, true);
        expect(otherPatch.internalPatchesCopy);
        expect(otherPatch.internalPatchesPaste);
    }

    void testLfo1CopyBlinksLfo2PasteOnly()
    {
        beginTest("lfo1 copy — source C + lfo2 P, not lfo1 P");

        Core::PatchModel model;
        Core::ClipboardService clipboard;
        clipboard.copyModule(Core::PatchModuleKind::Lfo1, model);

        const auto state = Core::resolveClipboardFeedback(clipboard, true, false);
        expect(state.lfo1Copy);
        expect(!state.lfo1Paste);
        expect(state.lfo2Paste);
        expect(!state.dco1Paste);
    }
};

static ClipboardFeedbackResolverTests clipboardFeedbackResolverTests;
