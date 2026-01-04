#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
}

class PatchManagerPanel : public juce::Component
{
public:
    explicit PatchManagerPanel(tss::Theme& inTheme);
    ~PatchManagerPanel() override = default;

    void paint(juce::Graphics&) override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 415;

    tss::Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchManagerPanel)
};

