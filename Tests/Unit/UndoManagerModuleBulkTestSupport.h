#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "Core/Actions/ModuleActionHandler.h"
#include "Core/Init/PatchModuleInitService.h"
#include "Core/MIDI/PatchParameterSysExDispatcher.h"
#include "Core/MIDI/SysEx/SysExDecoder.h"
#include "Core/MIDI/SysEx/SysExParser.h"
#include "Core/Models/ApvtsPatchMapper.h"
#include "Core/Models/PatchModel.h"
#include "Core/Services/ClipboardService.h"

namespace UndoManagerModuleBulkTestSupport
{

class UndoBulkTestAudioProcessor : public juce::AudioProcessor
{
public:
    explicit UndoBulkTestAudioProcessor(juce::AudioProcessorValueTreeState::ParameterLayout layout);

    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState apvts;

    const juce::String getName() const override;
    void prepareToPlay(double, int) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override;
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
};

bool moduleStateMatches(const Core::PatchModel& lhs,
                        const Core::PatchModel& rhs,
                        const juce::String& moduleGroupId);

juce::AudioProcessorValueTreeState::ParameterLayout makeEnvelopePasteLayout();
juce::AudioProcessorValueTreeState::ParameterLayout makeDco1Layout();

juce::File createTempTemplatesDir();
void copyInitFixtureToDir(const juce::File& dir);

struct ModuleBulkUndoHarnessBase : juce::ValueTree::Listener
{
    bool suppressPatchSysEx { false };
    int patchDispatchCount { 0 };
    Core::ApvtsPatchMapper* trackedApvtsMapper { nullptr };
    Core::PatchParameterSysExDispatcher* trackedPatchDispatcher { nullptr };

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
                                  const juce::Identifier& property) override;
};

struct PasteUndoHarness : ModuleBulkUndoHarnessBase
{
    UndoBulkTestAudioProcessor proc;
    Core::PatchModel model;
    Core::ApvtsPatchMapper apvtsMapper;
    Core::ClipboardService clipboard;
    Core::PatchParameterSysExDispatcher patchParameterSysExDispatcher;
    Core::ModuleActionHandler handler;

    explicit PasteUndoHarness(juce::AudioProcessorValueTreeState::ParameterLayout layout);
    ~PasteUndoHarness() override;
};

struct InitUndoHarness : ModuleBulkUndoHarnessBase
{
    UndoBulkTestAudioProcessor proc;
    Core::PatchModel model;
    Core::ApvtsPatchMapper apvtsMapper;
    SysExParser parser;
    SysExDecoder decoder;
    Core::InitTemplateLoader loader;
    Core::PatchParameterSysExDispatcher patchParameterSysExDispatcher;
    Core::PatchModuleInitService initService;
    Core::ModuleActionHandler handler;
    juce::File templatesFolder;

    InitUndoHarness(juce::AudioProcessorValueTreeState::ParameterLayout layout, const juce::File& folder);
    ~InitUndoHarness() override;
};

} // namespace UndoManagerModuleBulkTestSupport
