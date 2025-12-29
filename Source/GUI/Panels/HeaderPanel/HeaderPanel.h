#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Widgets/Button.h"
#include "../../Skin/SkinDimensions.h"

namespace tss
{
    class Skin;
    class Button;
}

using tss::Skin;
using tss::Button;

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(Skin& skin);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setSkin(Skin& skin);

    Button& getButtonBlack() { return buttonBlack; }
    Button& getButtonCream() { return buttonCream; }
    Button& getButtonDebug() { return buttonDebug; }
    Button& getButtonDisabled() { return buttonDisabled; }

    static int getHeight() { return tss::SkinDimensions::HeaderPanel::kHeight; }

private:

    Skin* skin;

    Button buttonBlack;
    Button buttonCream;
    Button buttonDebug;
    Button buttonDisabled;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

