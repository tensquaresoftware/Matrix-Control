#pragma once

#include <functional>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Core/Actions/ActionExecutionHooks.h"
#include "Core/Actions/IActionHandler.h"
#include "Core/Init/InitTemplateLoader.h"
#include "Core/Init/MasterModuleInitService.h"
#include "Core/Services/ClipboardService.h"

namespace Core
{
    class ApvtsPatchMapper;
    class MatrixModBusParameterSysExDispatcher;
    class MatrixModInitService;
    class PatchModel;
    class PatchModuleInitService;
    class PatchParameterSysExDispatcher;

    class ModuleActionHandler final : public IActionHandler
    {
    public:
        using RefreshPasteMirrorsCallback = std::function<void()>;

        struct Dependencies
        {
            juce::AudioProcessorValueTreeState& apvts;
            PatchModel* patchModel = nullptr;
            ApvtsPatchMapper* apvtsPatchMapper = nullptr;
            ClipboardService* clipboardService = nullptr;
            MatrixModInitService* matrixModInitService = nullptr;
            MasterModuleInitService* masterModuleInitService = nullptr;
            PatchModuleInitService* patchModuleInitService = nullptr;
            PatchParameterSysExDispatcher* patchParameterSysExDispatcher = nullptr;
            MatrixModBusParameterSysExDispatcher* matrixModBusParameterSysExDispatcher = nullptr;
            RefreshPasteMirrorsCallback refreshPasteMirrors;
        };

        ModuleActionHandler(Dependencies dependencies, ActionExecutionHooks hooks);

        void handleAction(const juce::String& propertyId, const juce::var& newValue) override;

    private:
        bool handleMatrixModInit(const juce::String& propertyId);
        bool handleMasterModuleInit(const juce::String& propertyId);
        bool handleClipboardCopy(const juce::String& propertyId);
        bool handleClipboardPaste(const juce::String& propertyId);
        bool handlePatchModuleInit(const juce::String& propertyId);

        bool copyFullInternalPatch();
        bool copyMatrixModulationSection();
        bool copyPatchModule(PatchModuleKind moduleKind, bool shapeOnlyIntent);
        void clearEnvelopeShapeOnlyFlag();

        bool pasteMatrixModulationSection();
        bool pastePatchModule(PatchModuleKind moduleKind);
        void publishPasteBlockedFooter(const juce::String& sourceLabel,
                                       const juce::String& targetName);
        void pushMatrixModulationToApvtsAndSysEx();
        void pushModuleToApvtsAndSysEx(const juce::String& moduleGroupId);

        void propagateInitTemplateFooterMessage(const InitTemplateLoadResult& result);
        int parseMatrixModBusInitIndex(const juce::String& propertyId) const;
        bool hasClipboardPipeline() const;

        juce::AudioProcessorValueTreeState& apvts_;
        PatchModel* patchModel_;
        ApvtsPatchMapper* apvtsPatchMapper_;
        ClipboardService* clipboardService_;
        MatrixModInitService* matrixModInitService_;
        MasterModuleInitService* masterModuleInitService_;
        PatchModuleInitService* patchModuleInitService_;
        PatchParameterSysExDispatcher* patchParameterSysExDispatcher_;
        MatrixModBusParameterSysExDispatcher* matrixModBusParameterSysExDispatcher_;
        RefreshPasteMirrorsCallback refreshPasteMirrors_;
        ActionExecutionHooks hooks_;
    };

} // namespace Core
