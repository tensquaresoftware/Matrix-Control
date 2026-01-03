#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
}

class FmTrackPanel : public juce::Component
{
public:
    explicit FmTrackPanel(tss::Theme& newTheme);
    ~FmTrackPanel() override = default;

    void paint(juce::Graphics&) override;
    void setTheme(tss::Theme& newTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 150;
    inline constexpr static int kHeight = 275;
    tss::Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FmTrackPanel)
};

