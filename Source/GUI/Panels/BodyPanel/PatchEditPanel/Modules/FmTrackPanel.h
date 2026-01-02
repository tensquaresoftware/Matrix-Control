#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
}

using tss::Skin;

class FmTrackPanel : public juce::Component
{
public:
    explicit FmTrackPanel(Skin& newSkin);
    ~FmTrackPanel() override = default;

    void paint(juce::Graphics&) override;
    void setSkin(Skin& newSkin);

private:
    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmTrackPanel)
};

