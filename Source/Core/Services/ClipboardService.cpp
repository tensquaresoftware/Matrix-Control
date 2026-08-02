#include "ClipboardService.h"

#include <cstring>

#include "Core/Models/PackedFieldCodec.h"
#include "Core/Services/ClipboardModulePasteHelpers.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace Core
{

bool ClipboardService::hasContent() const noexcept
{
    return mode_ != ClipboardMode::Empty;
}

ClipboardMode ClipboardService::getMode() const noexcept
{
    return mode_;
}

std::optional<PatchModuleKind> ClipboardService::getSourceModuleKind() const noexcept
{
    if (mode_ != ClipboardMode::Module)
        return std::nullopt;

    return sourceModuleKind_;
}

bool ClipboardService::isEnvelopeShapeOnly() const noexcept
{
    return mode_ == ClipboardMode::Module && envelopeShapeOnly_;
}

juce::String ClipboardService::getFullPatchSourceLabel() const
{
    if (mode_ != ClipboardMode::FullPatch)
        return {};

    return fullPatchSourceLabel_;
}

void ClipboardService::copyModule(PatchModuleKind source, const PatchModel& model, bool envelopeShapeOnly)
{
    mode_ = ClipboardMode::Module;
    sourceModuleKind_ = source;
    envelopeShapeOnly_ = envelopeShapeOnly && isEnvelopeModule(source);
    fullPatchSourceLabel_.clear();
    moduleSnapshot_.intValues.clear();
    moduleSnapshot_.choiceIndices.clear();

    const auto views = ClipboardModulePaste::descriptorsFor(source);

    for (const auto& descriptor : views.intParams)
    {
        if (envelopeShapeOnly_ && ! isEnvelopeShapeDisplayName(descriptor.displayName))
            continue;

        moduleSnapshot_.intValues[descriptor.parameterId] = model.getValue(descriptor);
    }

    if (! envelopeShapeOnly_)
    {
        for (const auto& descriptor : views.choiceParams)
            moduleSnapshot_.choiceIndices[descriptor.parameterId] = model.getChoiceIndex(descriptor);
    }
}

void ClipboardService::copyFullPatch(const PatchModel& model, const juce::String& sourceLabel)
{
    mode_ = ClipboardMode::FullPatch;
    envelopeShapeOnly_ = false;
    fullPatchSourceLabel_ = sourceLabel;
    std::memcpy(fullPatchSnapshot_.data(), model.data(), fullPatchSnapshot_.size());
}

void ClipboardService::copyMatrixModulation(const PatchModel& model)
{
    mode_ = ClipboardMode::MatrixModulation;
    envelopeShapeOnly_ = false;
    fullPatchSourceLabel_.clear();

    const auto offset = PackedFieldCodec::safeOffset(
        static_cast<int>(kMatrixModSnapshotOffset),
        PatchModel::kBufferSize);

    std::memcpy(matrixModSnapshot_.data(), model.data() + offset, kMatrixModSnapshotSize);
}

void ClipboardService::clear() noexcept
{
    mode_ = ClipboardMode::Empty;
    sourceModuleKind_ = PatchModuleKind::Dco1;
    envelopeShapeOnly_ = false;
    fullPatchSourceLabel_.clear();
    moduleSnapshot_.intValues.clear();
    moduleSnapshot_.choiceIndices.clear();
    fullPatchSnapshot_.fill(0);
    matrixModSnapshot_.fill(0);
}

bool ClipboardService::canPasteModule(PatchModuleKind target) const noexcept
{
    if (mode_ != ClipboardMode::Module)
        return false;

    return areModulesCompatible(sourceModuleKind_, target);
}

bool ClipboardService::canPasteFullPatch() const noexcept
{
    return mode_ == ClipboardMode::FullPatch;
}

bool ClipboardService::canPasteMatrixModulation() const noexcept
{
    return mode_ == ClipboardMode::MatrixModulation;
}

bool ClipboardService::pasteModule(PatchModuleKind target, PatchModel& model)
{
    if (!canPasteModule(target))
        return false;

    if (sourceModuleKind_ == target)
        pasteModuleDirect(target, model);
    else if (isEnvelopeModule(sourceModuleKind_) && isEnvelopeModule(target))
        pasteEnvelopeModule(sourceModuleKind_, target, model);
    else if (isDcoModule(sourceModuleKind_) && isDcoModule(target))
        pasteDcoModule(sourceModuleKind_, target, model);
    else if (isLfoModule(sourceModuleKind_) && isLfoModule(target))
        pasteLfoModule(sourceModuleKind_, target, model);

    return true;
}

bool ClipboardService::pasteFullPatch(PatchModel& model)
{
    if (!canPasteFullPatch())
        return false;

    model.loadFrom(fullPatchSnapshot_.data());
    return true;
}

bool ClipboardService::pasteMatrixModulation(PatchModel& model)
{
    if (!canPasteMatrixModulation())
        return false;

    const auto offset = PackedFieldCodec::safeOffset(
        static_cast<int>(kMatrixModSnapshotOffset),
        PatchModel::kBufferSize);

    std::memcpy(model.data() + offset, matrixModSnapshot_.data(), kMatrixModSnapshotSize);
    return true;
}

bool ClipboardService::isEnvelopeModule(PatchModuleKind kind) noexcept
{
    return kind == PatchModuleKind::Env1 || kind == PatchModuleKind::Env2 || kind == PatchModuleKind::Env3;
}

bool ClipboardService::isEnvelopeShapeDisplayName(const juce::String& displayName) noexcept
{
    using namespace PluginDisplayNames::PatchEditSection::Envelope1Module::ParameterWidgets;

    return displayName == kDelay
        || displayName == kAttack
        || displayName == kDecay
        || displayName == kSustain
        || displayName == kRelease;
}

bool ClipboardService::isDcoModule(PatchModuleKind kind) noexcept
{
    return kind == PatchModuleKind::Dco1 || kind == PatchModuleKind::Dco2;
}

bool ClipboardService::isLfoModule(PatchModuleKind kind) noexcept
{
    return kind == PatchModuleKind::Lfo1 || kind == PatchModuleKind::Lfo2;
}

bool ClipboardService::areModulesCompatible(PatchModuleKind source, PatchModuleKind target) noexcept
{
    if (isEnvelopeModule(source) && isEnvelopeModule(target))
        return true;

    if (isDcoModule(source) && isDcoModule(target))
        return true;

    if (isLfoModule(source) && isLfoModule(target))
        return true;

    return false;
}

void ClipboardService::pasteModuleDirect(PatchModuleKind target, PatchModel& model)
{
    const auto views = ClipboardModulePaste::descriptorsFor(target);

    for (const auto& descriptor : views.intParams)
    {
        const auto it = moduleSnapshot_.intValues.find(descriptor.parameterId);
        if (it != moduleSnapshot_.intValues.end())
            model.setValue(descriptor, it->second);
    }

    for (const auto& descriptor : views.choiceParams)
    {
        const auto it = moduleSnapshot_.choiceIndices.find(descriptor.parameterId);
        if (it != moduleSnapshot_.choiceIndices.end())
            model.setChoiceIndex(descriptor, it->second);
    }
}

void ClipboardService::pasteEnvelopeModule(PatchModuleKind source,
                                           PatchModuleKind target,
                                           PatchModel& model)
{
    const auto sourceViews = ClipboardModulePaste::descriptorsFor(source);
    const auto targetViews = ClipboardModulePaste::descriptorsFor(target);
    const ClipboardModulePaste::CrossModulePasteArgs args {
        source, target, sourceViews, targetViews,
        moduleSnapshot_.intValues, moduleSnapshot_.choiceIndices, model
    };

    ClipboardModulePaste::pasteMatchedByDisplayName(args);
}

void ClipboardService::pasteDcoModule(PatchModuleKind source, PatchModuleKind target, PatchModel& model)
{
    const auto sourceViews = ClipboardModulePaste::descriptorsFor(source);
    const auto targetViews = ClipboardModulePaste::descriptorsFor(target);
    const ClipboardModulePaste::CrossModulePasteArgs args {
        source, target, sourceViews, targetViews,
        moduleSnapshot_.intValues, moduleSnapshot_.choiceIndices, model
    };

    ClipboardModulePaste::pasteDcoByDisplayName(args);
}

void ClipboardService::pasteLfoModule(PatchModuleKind source, PatchModuleKind target, PatchModel& model)
{
    const auto sourceViews = ClipboardModulePaste::descriptorsFor(source);
    const auto targetViews = ClipboardModulePaste::descriptorsFor(target);
    const ClipboardModulePaste::CrossModulePasteArgs args {
        source, target, sourceViews, targetViews,
        moduleSnapshot_.intValues, moduleSnapshot_.choiceIndices, model
    };

    ClipboardModulePaste::pasteLfoByDisplayName(args);
}

} // namespace Core
