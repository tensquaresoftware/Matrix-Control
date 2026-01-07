#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
}

class MatrixModulationPanel : public juce::Component
{
public:
    explicit MatrixModulationPanel(tss::Theme& inTheme);
    ~MatrixModulationPanel() override = default;

    void paint(juce::Graphics&) override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 325;

    tss::Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatrixModulationPanel)
};

