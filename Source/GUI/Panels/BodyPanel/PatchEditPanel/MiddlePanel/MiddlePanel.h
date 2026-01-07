#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
}

class MiddlePanel : public juce::Component
{
public:
    MiddlePanel(tss::Theme& inTheme);
    ~MiddlePanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 810;
    inline constexpr static int kHeight = 110;

    tss::Theme* theme;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiddlePanel)
};

