#include "Core/Services/PatchMutator/MutationPitchPolicy.h"

#include <cmath>
#include <iterator>

namespace Core
{

namespace
{
    // Relative DCO 1 ↔ DCO 2: unison, minor/major third, fourth, fifth, octave.
    // Sixths are intentionally out of Consonant (they land in Free or Dissonant).
    constexpr int kConsonantSemitones[] = { 0, 3, 4, 5, 7, 12 };
    // Seconds, tritone, sixths, sevenths.
    constexpr int kDissonantSemitones[] = { 1, 2, 6, 8, 9, 10, 11 };

    bool modePreservesDcoInterval(MutationMode mode) noexcept
    {
        return mode == MutationMode::kKindred || mode == MutationMode::kDrift;
    }

    float relativeRepickProbability(MutationMode mode) noexcept
    {
        switch (mode)
        {
            case MutationMode::kKindred: return MutationCalibration::kPitchRelativeRepickKindred;
            case MutationMode::kDrift:   return MutationCalibration::kPitchRelativeRepickDrift;
            case MutationMode::kWarp:
            case MutationMode::kWild:    return MutationCalibration::kPitchRelativeRepickWarpWild;
        }

        return MutationCalibration::kPitchRelativeRepickWarpWild;
    }

    juce::Array<int> buildRelativeIntervals(MutationPitchMode pitchMode, int octaveWindow)
    {
        const int windowSemitones = octaveWindow * kSemitonesPerOctave;
        juce::Array<int> relatives;

        const auto appendMultiples = [&relatives, windowSemitones](int baseSemitones)
        {
            if (baseSemitones == 0)
            {
                relatives.addIfNotAlreadyThere(0);
                return;
            }

            for (int delta = baseSemitones; delta <= windowSemitones; delta += kSemitonesPerOctave)
            {
                relatives.addIfNotAlreadyThere(delta);
                relatives.addIfNotAlreadyThere(-delta);
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

        return relatives;
    }

    // Absolute transposition set for a single writable DCO (unison is a no-op — skip it).
    juce::Array<int> buildAbsoluteJumpDeltas(MutationPitchMode pitchMode, int octaveWindow)
    {
        const int windowSemitones = octaveWindow * kSemitonesPerOctave;
        juce::Array<int> deltas;

        const auto* bases = pitchMode == MutationPitchMode::kConsonant
                                ? static_cast<const int*>(kConsonantSemitones)
                                : static_cast<const int*>(kDissonantSemitones);
        const int baseCount = pitchMode == MutationPitchMode::kConsonant
                                  ? static_cast<int>(std::size(kConsonantSemitones))
                                  : static_cast<int>(std::size(kDissonantSemitones));

        for (int i = 0; i < baseCount; ++i)
        {
            const int base = bases[i];
            if (base == 0)
                continue;

            for (int delta = base; delta <= windowSemitones; delta += kSemitonesPerOctave)
            {
                deltas.addIfNotAlreadyThere(delta);
                deltas.addIfNotAlreadyThere(-delta);
            }
        }

        return deltas;
    }

    bool valueInRange(int value, int minValue, int maxValue) noexcept
    {
        return value >= minValue && value <= maxValue;
    }

    bool relativeFitsAnchored(int dco1, int relative, int minValue, int maxValue) noexcept
    {
        return valueInRange(dco1, minValue, maxValue)
               && valueInRange(dco1 + relative, minValue, maxValue);
    }

    bool tryApplyRelative(MutationPitchPlan& plan,
                          const MutationPitchRequest& request,
                          int relative)
    {
        // Prefer anchoring DCO 1 (bass / reference), move DCO 2.
        if (relativeFitsAnchored(request.dco1Seed, relative, request.minValue, request.maxValue))
        {
            plan.hasDco1Value = true;
            plan.dco1Value = request.dco1Seed;
            plan.hasDco2Value = true;
            plan.dco2Value = request.dco1Seed + relative;
            return true;
        }

        // Else anchor DCO 2, move DCO 1.
        if (relativeFitsAnchored(request.dco2Seed - relative, relative, request.minValue, request.maxValue))
        {
            plan.hasDco1Value = true;
            plan.dco1Value = request.dco2Seed - relative;
            plan.hasDco2Value = true;
            plan.dco2Value = request.dco2Seed;
            return true;
        }

        return false;
    }

    int nearestLegalRelative(int currentRelative, const juce::Array<int>& legal)
    {
        int best = legal.getFirst();
        int bestDistance = std::abs(currentRelative - best);

        for (int i = 1; i < legal.size(); ++i)
        {
            const int candidate = legal.getUnchecked(i);
            const int distance = std::abs(currentRelative - candidate);
            if (distance < bestDistance)
            {
                best = candidate;
                bestDistance = distance;
            }
        }

        return best;
    }

    juce::Array<int> relativesThatFit(const juce::Array<int>& candidates,
                                      const MutationPitchRequest& request)
    {
        juce::Array<int> legal;

        for (const auto relative : candidates)
        {
            if (relativeFitsAnchored(request.dco1Seed, relative, request.minValue, request.maxValue)
                || relativeFitsAnchored(request.dco2Seed - relative, relative,
                                        request.minValue, request.maxValue))
            {
                legal.add(relative);
            }
        }

        return legal;
    }

    MutationPitchPlan planSingleOscillatorIntervalJump(const MutationPitchRequest& request,
                                                       IRandomSource& rng)
    {
        const int octaveWindow = juce::jlimit(MutationCalibration::kMinPitchOctaves,
                                              MutationCalibration::kMaxPitchOctaves,
                                              request.octaveWindow);
        const auto candidates = buildAbsoluteJumpDeltas(request.pitchMode, octaveWindow);
        const int seed = request.dco1Writable ? request.dco1Seed : request.dco2Seed;

        juce::Array<int> legal;
        for (const auto delta : candidates)
        {
            if (valueInRange(seed + delta, request.minValue, request.maxValue))
                legal.add(delta);
        }

        if (legal.isEmpty())
            return {};

        const int delta = legal[uniformRandomInt(rng, 0, legal.size() - 1)];
        MutationPitchPlan plan;

        if (request.dco1Writable)
        {
            plan.hasDco1Value = true;
            plan.dco1Value = seed + delta;
        }
        else
        {
            plan.hasDco2Value = true;
            plan.dco2Value = seed + delta;
        }

        return plan;
    }

    MutationPitchPlan planRelativeInterval(const MutationPitchRequest& request, IRandomSource& rng)
    {
        const int octaveWindow = juce::jlimit(MutationCalibration::kMinPitchOctaves,
                                              MutationCalibration::kMaxPitchOctaves,
                                              request.octaveWindow);
        const auto candidates = buildRelativeIntervals(request.pitchMode, octaveWindow);
        const auto legal = relativesThatFit(candidates, request);
        if (legal.isEmpty())
            return {};

        const int currentRelative = request.dco2Seed - request.dco1Seed;
        const bool currentIsLegal = legal.contains(currentRelative);
        const bool repick = rng.nextFloat() < relativeRepickProbability(request.mode);

        int targetRelative = currentRelative;

        if (repick || ! currentIsLegal)
        {
            if (repick)
                targetRelative = legal[uniformRandomInt(rng, 0, legal.size() - 1)];
            else
                targetRelative = nearestLegalRelative(currentRelative, legal);
        }
        else
        {
            // Kindred/Drift keeping an already-legal relative: no FREQUENCY write.
            return {};
        }

        MutationPitchPlan plan;
        if (tryApplyRelative(plan, request, targetRelative))
            return plan;

        for (const auto relative : legal)
        {
            if (relative == targetRelative)
                continue;

            if (tryApplyRelative(plan, request, relative))
                return plan;
        }

        return {};
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

    struct FreeJumpTargets
    {
        bool dco1 = false;
        bool dco2 = false;
    };

    void applyDeltaToPlan(MutationPitchPlan& plan,
                          const MutationPitchRequest& request,
                          int delta,
                          FreeJumpTargets targets)
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

    if (request.pitchMode == MutationPitchMode::kFree)
    {
        // Kindred/Drift still move both oscillators together so Free keeps the seed interval.
        const bool jointJump = request.dco1Writable
                               && request.dco2Writable
                               && modePreservesDcoInterval(request.mode);
        return planFreeJumps(request, jointJump, rng);
    }

    // Consonant / Dissonant: musical quality lives in the DCO 1 ↔ DCO 2 relative interval.
    if (request.dco1Writable && request.dco2Writable)
        return planRelativeInterval(request, rng);

    return planSingleOscillatorIntervalJump(request, rng);
}

} // namespace Core
