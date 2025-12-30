#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
}

using tss::Skin;

class Dco2Panel : public juce::Component
{
public:
    explicit Dco2Panel(Skin& newSkin);
    ~Dco2Panel() override = default;

    void paint(juce::Graphics&) override;
    void setSkin(Skin& newSkin);

private:
    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dco2Panel)
};

