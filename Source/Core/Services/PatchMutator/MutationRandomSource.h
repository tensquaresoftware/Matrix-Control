#pragma once

#include <juce_core/juce_core.h>

namespace Core
{

    // Injectable randomness so golden vectors can pin an exact draw sequence.
    struct IRandomSource
    {
        virtual ~IRandomSource() = default;
        virtual float nextFloat() = 0;
        virtual int nextInt(int rangeSize) = 0;
    };

    class SeededRandom final : public IRandomSource
    {
    public:
        explicit SeededRandom(juce::uint32 seed) { rng_.setSeed(seed); }

        float nextFloat() override { return rng_.nextFloat(); }
        int nextInt(int rangeSize) override { return rng_.nextInt(rangeSize); }

    private:
        juce::Random rng_;
    };

    class JuceRandomSource final : public IRandomSource
    {
    public:
        explicit JuceRandomSource(juce::Random& rng) : rng_(rng) {}

        float nextFloat() override { return rng_.nextFloat(); }
        int nextInt(int rangeSize) override { return rng_.nextInt(rangeSize); }

    private:
        juce::Random& rng_;
    };

    inline int uniformRandomInt(IRandomSource& rng, int lo, int hi)
    {
        jassert(lo <= hi);
        return lo + rng.nextInt(hi - lo + 1);
    }

} // namespace Core
