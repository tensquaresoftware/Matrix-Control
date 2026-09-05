#pragma once

#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
    /**
     * Full-bounds dimming overlay with geometric holes (no paint + click-through).
     * Intensity is a single named constant — calibrate visually via that constant only.
     */
    class LockDimmingFilm : public juce::Component
    {
    public:
        static constexpr float kFilmIntensity = 0.75f;

        LockDimmingFilm();
        ~LockDimmingFilm() override = default;

        void setHoles(std::vector<juce::Rectangle<int>> holes);
        void paint(juce::Graphics& g) override;
        bool hitTest(int x, int y) override;

    private:
        std::vector<juce::Rectangle<int>> holes_;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LockDimmingFilm)
    };
}
