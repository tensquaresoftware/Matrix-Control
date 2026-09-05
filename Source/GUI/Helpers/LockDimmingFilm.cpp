#include "GUI/Helpers/LockDimmingFilm.h"

#include "GUI/Helpers/LockDimmingFilmPolicy.h"

namespace TSS
{
    LockDimmingFilm::LockDimmingFilm()
    {
        setInterceptsMouseClicks(true, false);
        setOpaque(false);
    }

    void LockDimmingFilm::setHoles(std::vector<juce::Rectangle<int>> holes)
    {
        holes_ = std::move(holes);
        repaint();
    }

    void LockDimmingFilm::paint(juce::Graphics& g)
    {
        for (const auto& hole : holes_)
            g.excludeClipRegion(hole);

        g.setColour(juce::Colours::black.withAlpha(kFilmIntensity));
        g.fillRect(getLocalBounds());
    }

    bool LockDimmingFilm::hitTest(int x, int y)
    {
        return lockDimmingFilmBlocksPoint(holes_, x, y);
    }
}
