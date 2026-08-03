// Extracted from PatchMutatorEngine.cpp for modular maintenance.
// Construction, recipe, audition resolve, and editor/synth push.

#include "Core/Services/PatchMutator/PatchMutatorEngine.h"
#include "Core/Services/PatchMutator/PatchMutatorEngineInternal.h"

#include <cstring>

#include "Core/MIDI/MidiManager.h"
#include "Core/MIDI/SysEx/SysExEncoder.h"
#include "Core/Models/PatchModel.h"

using namespace PatchMutatorEngineInternal;

namespace Core
{

MutatorActionResult PatchMutatorEngine::makeWarningResult(const char* message)
{
    MutatorActionResult result;
    result.footerMessage = message;
    result.footerSeverity = kFooterSeverityWarning;
    return result;
}

MutatorActionResult PatchMutatorEngine::makeHistoryLimitResult()
{
    MutatorActionResult result;
    result.footerMessage = kHistoryLimitFooterMessage;
    result.footerSeverity = kFooterSeverityWarning;
    result.defragModalRequested = true;
    return result;
}

MutatorActionResult PatchMutatorEngine::makeSuccessResult()
{
    MutatorActionResult result;
    result.success = true;
    return result;
}

PatchMutatorEngine::PatchMutatorEngine(Dependencies dependencies, ActionExecutionHooks hooks)
    : patchModel_(dependencies.patchModel)
    , apvtsPatchMapper_(dependencies.apvtsPatchMapper)
    , patchNameSyncer_(dependencies.patchNameSyncer)
    , midiManager_(dependencies.midiManager)
    , apvts_(dependencies.apvts)
    , hooks_(std::move(hooks))
    , getCurrentPatchNumber_(std::move(dependencies.getCurrentPatchNumber))
    , getDeviceMemoryLimits_(std::move(dependencies.getDeviceMemoryLimits))
    , patchFileService_(dependencies.patchFileService)
    , sysExEncoder_(dependencies.sysExEncoder)
{
    jassert(patchModel_ != nullptr);
    jassert(apvtsPatchMapper_ != nullptr);
}

void PatchMutatorEngine::freezeExportBasename(const PatchModel& snapshot)
{
    if (! patchLoadContextProvider_)
        return;

    const auto context = patchLoadContextProvider_();
    historyStore_.setFrozenExportBasename(context.computeExportBasename(snapshot.getName()));
}

void PatchMutatorEngine::setPatchLoadContextProvider(std::function<PatchLoadContext()> provider)
{
    patchLoadContextProvider_ = std::move(provider);
}

void PatchMutatorEngine::refreshFrozenExportBasename(const juce::String& newPatchName)
{
    if (! historyStore_.hasFrozenExportBasename())
        return;

    if (! patchLoadContextProvider_)
        return;

    const auto context = patchLoadContextProvider_();
    historyStore_.setFrozenExportBasename(context.computeExportBasename(newPatchName));
}

void PatchMutatorEngine::forceExitCompare()
{
    auto& state = apvts_.state;
    if (! readBoolProperty(state, MutatorState::kCompareActive, false))
        return;

    state.setProperty(MutatorState::kCompareActive, false, nullptr);
    // Compare owned the INITIAL row while it was active — drop it with the lock.
    state.setProperty(MutatorState::kInitialSelected, false, nullptr);
    initialSelected_ = false;
    compareSavedMutateRootIndex_ = -1;
    compareSavedRetryIndex_ = MutationHistoryStore::kRootOnly;
    clearCompareLockedFooterIfPresent(apvts_);
}

void PatchMutatorEngine::auditionAfterHistoryMutation()
{
    PatchModel buffer;

    if (! historyStore_.isEmpty())
        buffer = resolveAuditionBuffer();
    else if (historyStore_.hasInitialSnapshot())
        buffer = historyStore_.getInitialSnapshot();
    else
        buffer = *patchModel_;

    if (candidateDiffersFromLive(buffer))
        pushResultToEditorAndSynth(buffer);
}

bool PatchMutatorEngine::canOfferInitialSelection() const
{
    return ! historyStore_.isEmpty() && historyStore_.hasInitialSnapshot();
}

void PatchMutatorEngine::applySelectionFromApvts()
{
    const auto& state = apvts_.state;

    // Compare keeps its own lock semantics: while it is active the origin is on screen but
    // the selection still belongs to the mutation Compare will restore.
    initialSelected_ = canOfferInitialSelection()
                       && readBoolProperty(state, MutatorState::kInitialSelected, false)
                       && ! readBoolProperty(state, MutatorState::kCompareActive, false);

    if (! state.hasProperty(MutatorState::kSelectedMutateRootIndex))
        return;

    const int mutateRootIndex = static_cast<int>(state.getProperty(MutatorState::kSelectedMutateRootIndex, -1));
    const int retryIndex = static_cast<int>(state.getProperty(MutatorState::kSelectedRetryIndex,
                                                               MutationHistoryStore::kRootOnly));

    if (mutateRootIndex < 0 || ! historyStore_.hasRoot(mutateRootIndex))
    {
        selectedRootIndex_ = -1;
        selectedRetryIndex_ = MutationHistoryStore::kRootOnly;
        return;
    }

    selectedRootIndex_ = mutateRootIndex;

    if (retryIndex == MutationHistoryStore::kRootOnly)
        selectedRetryIndex_ = MutationHistoryStore::kRootOnly;
    else if (historyStore_.hasRetry(mutateRootIndex, retryIndex))
        selectedRetryIndex_ = retryIndex;
    else
        selectedRetryIndex_ = MutationHistoryStore::kRootOnly;
}

void PatchMutatorEngine::setAuditionSelection(int rootIndex, int retryIndex)
{
    selectedRootIndex_ = rootIndex;
    selectedRetryIndex_ = retryIndex;
}

int PatchMutatorEngine::rootCount() const noexcept
{
    return historyStore_.rootCount();
}

std::optional<MutationEntry> PatchMutatorEngine::getEntry(int rootIndex, int retryIndex) const
{
    return historyStore_.getEntry(rootIndex, retryIndex);
}

std::optional<MutatorActionResult> PatchMutatorEngine::validateMutationRecipe(
    const MutationRecipe& recipe) const
{
    if (recipe.amountPercent == 0 || recipe.randomPercent == 0)
        return makeWarningResult(kNoOpRecipeFooterMessage);

    if (! recipe.hasAnyModuleEnabled())
        return makeWarningResult(kNoModuleScopeFooterMessage);

    return std::nullopt;
}

std::optional<MutatorActionResult> PatchMutatorEngine::validateRetryPreconditions(
    int& outRootIndex) const
{
    if (historyStore_.isEmpty())
        return makeWarningResult(kEmptyHistoryFooterMessage);

    // RETRY needs a mutation to branch from — the origin snapshot has no parent entry.
    if (initialSelected_)
        return makeWarningResult(kNoSelectionFooterMessage);

    const auto rootIndexOpt = resolveSelectedRootIndex();
    if (! rootIndexOpt.has_value() || ! historyStore_.hasRoot(*rootIndexOpt))
        return makeWarningResult(kNoSelectionFooterMessage);

    outRootIndex = *rootIndexOpt;

    if (historyStore_.isRetrySlotsFull(outRootIndex) || historyStore_.isRetryIndexExhausted(outRootIndex))
        return makeHistoryLimitResult();

    return std::nullopt;
}

bool PatchMutatorEngine::applyRecipeMutation(PatchModel& working, const MutationRecipe& recipe)
{
    rng_.setSeedRandomly();
    JuceRandomSource rngSource(rng_);
    return algorithm_.apply(working, recipe, rngSource);
}

MutationRecipe PatchMutatorEngine::buildRecipeFromApvts() const
{
    const auto& state = apvts_.state;

    MutationRecipe recipe;
    recipe.amountPercent = readPatchMutatorPercent(apvts_, PatchMutator::kAmount, 50);
    recipe.randomPercent = readPatchMutatorPercent(apvts_, PatchMutator::kRandom, 25);
    recipe.enableDco1 = readBoolProperty(state, PatchMutator::kEnableDco1, false);
    recipe.enableDco2 = readBoolProperty(state, PatchMutator::kEnableDco2, false);
    recipe.enableVcfVca = readBoolProperty(state, PatchMutator::kEnableVcfVca, false);
    recipe.enableFmTrack = readBoolProperty(state, PatchMutator::kEnableFmTrack, false);
    recipe.enableRampPortamento = readBoolProperty(state, PatchMutator::kEnableRampPortamento, false);
    recipe.enableEnvelope1 = readBoolProperty(state, PatchMutator::kEnableEnvelope1, false);
    recipe.enableEnvelope2 = readBoolProperty(state, PatchMutator::kEnableEnvelope2, false);
    recipe.enableEnvelope3 = readBoolProperty(state, PatchMutator::kEnableEnvelope3, false);
    recipe.enableLfo1 = readBoolProperty(state, PatchMutator::kEnableLfo1, false);
    recipe.enableLfo2 = readBoolProperty(state, PatchMutator::kEnableLfo2, false);
    recipe.enableMatrixMod = readBoolProperty(state, PatchMutator::kEnableMatrixMod, false);
    return recipe;
}

PatchModel PatchMutatorEngine::resolveAuditionBuffer()
{
    applySelectionFromApvts();

    if (historyStore_.isEmpty())
        return *patchModel_;

    // HISTORY INITIAL: the origin snapshot is what the user hears, so it is also what
    // MUTATE branches from.
    if (initialSelected_)
        return historyStore_.getInitialSnapshot();

    int rootIndex = selectedRootIndex_;
    int retryIndex = selectedRetryIndex_;

    if (rootIndex < 0)
    {
        const auto sortedRoots = historyStore_.getSortedRootIndices();
        if (sortedRoots.isEmpty())
            return *patchModel_;

        rootIndex = sortedRoots.getLast();
        retryIndex = MutationHistoryStore::kRootOnly;
    }

    if (const auto entry = historyStore_.getEntry(rootIndex, retryIndex))
    {
        PatchModel model;
        model.loadFrom(entry->result.data());
        return model;
    }

    if (const auto rootEntry = historyStore_.getEntry(rootIndex, MutationHistoryStore::kRootOnly))
    {
        PatchModel model;
        model.loadFrom(rootEntry->result.data());
        return model;
    }

    return *patchModel_;
}

std::optional<int> PatchMutatorEngine::resolveSelectedRootIndex() const
{
    if (historyStore_.isEmpty())
        return std::nullopt;

    if (selectedRootIndex_ >= 0)
        return selectedRootIndex_;

    const auto sortedRoots = historyStore_.getSortedRootIndices();
    if (sortedRoots.isEmpty())
        return std::nullopt;

    return sortedRoots.getLast();
}

std::optional<MutationEntry> PatchMutatorEngine::resolveSelectedEntryForRetry(int rootIndex) const
{
    if (selectedRetryIndex_ == MutationHistoryStore::kRootOnly)
        return historyStore_.getEntry(rootIndex, MutationHistoryStore::kRootOnly);

    if (const auto retryEntry = historyStore_.getEntry(rootIndex, selectedRetryIndex_))
        return retryEntry;

    return historyStore_.getEntry(rootIndex, MutationHistoryStore::kRootOnly);
}

bool PatchMutatorEngine::candidateDiffersFromLive(const PatchModel& candidate) const
{
    PatchModel stampedCandidate;
    stampedCandidate.loadFrom(candidate.data());
    stampedCandidate.setName(patchModel_->getName());

    return std::memcmp(stampedCandidate.data(), patchModel_->data(), PatchModel::kBufferSize) != 0;
}

void PatchMutatorEngine::pushResultToEditorAndSynth(const PatchModel& mutatedModel)
{
    // The live model + APVTS are the name SSOT — stamp it onto the pushed buffer so
    // History audition / Compare cannot resurrect a stale or Mxx name after a rename.
    PatchModel stampedModel;
    stampedModel.loadFrom(mutatedModel.data());
    stampedModel.setName(patchModel_->getName());

    std::memcpy(patchModel_->data(), stampedModel.data(), PatchModel::kBufferSize);
    pushPatchModelToApvtsWithSuppress(apvts_, hooks_, *apvtsPatchMapper_, patchNameSyncer_);

    if (midiManager_ == nullptr || ! getDeviceMemoryLimits_)
        return;

    const bool hasBankConcept = getDeviceMemoryLimits_().hasBankConcept();
    const juce::uint8 patchNumber = getCurrentPatchNumber_
        ? static_cast<juce::uint8>(juce::jlimit(0, 255, getCurrentPatchNumber_()))
        : 0;

    // Slot-write path (Matrix-6/6R) needs a patch number supplier; edit-buffer (M-1000) does not.
    if (! hasBankConcept && ! getCurrentPatchNumber_)
        return;

    midiManager_->sendFullPatchForAudition(patchModel_->data(), patchNumber, hasBankConcept);
}

bool PatchMutatorEngine::readBoolProperty(const juce::ValueTree& state,
                                          const juce::Identifier& propertyId,
                                          bool defaultValue)
{
    if (! state.hasProperty(propertyId))
        return defaultValue;

    return static_cast<bool>(state.getProperty(propertyId, defaultValue));
}

} // namespace Core
