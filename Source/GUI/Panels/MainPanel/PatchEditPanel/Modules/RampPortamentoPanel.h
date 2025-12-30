#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;
}

using tss::Skin;

class RampPortamentoPanel : public juce::Component
{
public:
    explicit RampPortamentoPanel(Skin& newSkin);
    ~RampPortamentoPanel() override = default;

    void paint(juce::Graphics&) override;
    void setSkin(Skin& newSkin);

private:
    Skin* skin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RampPortamentoPanel)
};

