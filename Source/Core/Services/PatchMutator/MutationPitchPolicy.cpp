#include "Core/Services/PatchMutator/MutationPitchPolicy.h"

namespace Core
{

namespace
{
    // Perfect fifth / fourth, major and minor third, major and minor sixth, octave.
    constexpr int kConsonantSemitones[] = { 3, 4, 5, 7, 8, 9, 12 };
    // Minor and major second, tritone, minor and major seventh.
    constexpr int kDissonantSemitones[] = { 1, 2, 6, 10, 11 };

    bool modePreservesDcoInterval(MutationMode mode) noexcept
    {
        return mode == MutationMode::kKindred || mode == MutationMode::kDrift;
    }

    juce::Array<int> buildIntervalDeltas(MutationPitchMode pitchMode, int octaveWindow)
    {
        const int windowSemitones = octaveWindow * kSemitonesPerOctave;
        juce::Array<int> deltas;

        const auto appendMultiples = [&deltas, windowSemitones](int baseSemitones)
        {
            for (int delta = baseSemitones; delta <= windowSemitones; delta += kSemitonesPerOctave)
            {
                deltas.addIfNotAlreadyThere(delta);
                deltas.addIfNotAlreadyThere(-delta);
            }
        };

        if (pitchMode == MutationPitchMode::kConsonant)
        {
            for (const auto semitones : kConsonantSemitones)
                appendMultiples(semitones);
        }
        else
        {
            for (const auto semitones : kDissonantSemitones)
                appendMultiples(semitones);
        }

        return deltas;
    }

    bool deltaFitsSeed(int delta, int seed, int minValue, int maxValue) noexcept
    {
        const int target = seed + delta;
        return target >= minValue && target <= maxValue;
    }

    // The legal jump window is measured from the current value, so a DCO near the floor
    // simply has more room upward than downward.
    juce::Array<int> legalDeltasForSeeds(const juce::Array<int>& candidates,
                                         const MutationPitchRequest& request,
                                         bool includeDco1,
                                         bool includeDco2)
    {
        juce::Array<int> legal;

        for (const auto delta : candidates)
        {
            if (includeDco1 && ! deltaFitsSeed(delta, request.dco1Seed, request.minValue, request.maxValue))
                continue;

            if (includeDco2 && ! deltaFitsSeed(delta, request.dco2Seed, request.minValue, request.maxValue))
                continue;

            legal.add(delta);
        }

        return legal;
    }

    int freeDeltaForSeeds(const MutationPitchRequest& request,
                          bool includeDco1,
                          bool includeDco2,
                          IRandomSource& rng)
    {
        int lowest = -request.freeSpread;
        int highest = request.freeSpread;

        if (includeDco1)
        {
            lowest = juce::jmax(lowest, request.minValue - request.dco1Seed);
            highest = juce::jmin(highest, request.maxValue - request.dco1Seed);
        }

        if (includeDco2)
        {
            lowest = juce::jmax(lowest, request.minValue - request.dco2Seed);
            highest = juce::jmin(highest, request.maxValue - request.dco2Seed);
        }

        if (lowest > highest)
            return 0;

        return uniformRandomInt(rng, lowest, highest);
    }

    struct PitchTargets
    {
        bool dco1 = false;
        bool dco2 = false;
    };

    void applyDeltaToPlan(MutationPitchPlan& plan,
                          const MutationPitchRequest& request,
                          int delta,
                          PitchTargets targets)
    {
        if (targets.dco1)
        {
            plan.hasDco1Value = true;
            plan.dco1Value = juce::jlimit(request.minValue, request.maxValue, request.dco1Seed + delta);
        }

        if (targets.dco2)
        {
            plan.hasDco2Value = true;
            plan.dco2Value = juce::jlimit(request.minValue, request.maxValue, request.dco2Seed + delta);
        }
    }

    MutationPitchPlan planFreeJumps(const MutationPitchRequest& request, bool jointJump, IRandomSource& rng)
    {
        MutationPitchPlan plan;

        if (request.freeSpread <= 0)
            return plan;

        if (jointJump)
        {
            applyDeltaToPlan(plan, request, freeDeltaForSeeds(request, true, true, rng), { true, true });
            return plan;
        }

        if (request.dco1Writable)
            applyDeltaToPlan(plan, request, freeDeltaForSeeds(request, true, false, rng), { true, false });

        if (request.dco2Writable)
            applyDeltaToPlan(plan, request, freeDeltaForSeeds(request, false, true, rng), { false, true });

        return plan;
    }

    MutationPitchPlan planIntervalJumps(const MutationPitchRequest& request,
                                        bool jointJump,
                                        IRandomSource& rng)
    {
        const int octaveWindow = juce::jlimit(MutationCalibration::kMinPitchOctaves,
                                              MutationCalibration::kMaxPitchOctaves,
                                              request.octaveWindow);
        const auto candidates = buildIntervalDeltas(request.pitchMode, octaveWindow);

        const auto pickLegalDelta = [&candidates, &request, &rng](bool includeDco1, bool includeDco2)
        {
            const auto legal = legalDeltasForSeeds(candidates, request, includeDco1, includeDco2);
            if (legal.isEmpty())
                return 0;

            return legal[uniformRandomInt(rng, 0, legal.size() - 1)];
        };

        MutationPitchPlan plan;

        if (jointJump)
        {
            applyDeltaToPlan(plan, request, pickLegalDelta(true, true), { true, true });
            return plan;
        }

        if (request.dco1Writable)
            applyDeltaToPlan(plan, request, pickLegalDelta(true, false), { true, false });

        if (request.dco2Writable)
            applyDeltaToPlan(plan, request, pickLegalDelta(false, true), { false, true });

        return plan;
    }
} // namespace

bool pitchPreserveBlocksMutation(MutationPitchMode pitchMode) noexcept
{
    return pitchMode == MutationPitchMode::kPreserve;
}

MutationPitchPlan planPitchJumps(const MutationPitchRequest& request, IRandomSource& rng)
{
    if (pitchPreserveBlocksMutation(request.pitchMode))
        return {};

    if (! request.dco1Writable && ! request.dco2Writable)
        return {};

    if (request.maxValue <= request.minValue)
        return {};

    // Kindred and Drift move both oscillators together so the seed's interval survives.
    const bool jointJump = request.dco1Writable
                           && request.dco2Writable
                           && modePreservesDcoInterval(request.mode);

    if (request.pitchMode == MutationPitchMode::kFree)
        return planFreeJumps(request, jointJump, rng);

    return planIntervalJumps(request, jointJump, rng);
}

} // namespace Core
