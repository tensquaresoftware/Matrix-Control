#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Widgets/Button.h"

namespace tss
{
    class Theme;
    class Button;
}

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(tss::Theme& inTheme);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    tss::Button& getButtonBlack() { return blackThemeButton; }
    tss::Button& getButtonCream() { return creamThemeButton; }
    tss::Button& getButtonDebug() { return debugThemeButton; }
    tss::Button& getButtonDisabled() { return someDisabledButton; }

    static int getHeight() { return kHeight; }
    static int getButtonWidth() { return kButtonWidth; }
    static int getButtonHeight() { return kButtonHeight; }
    static int getButtonSpacing() { return kButtonSpacing; }

private:
    inline constexpr static int kHeight = 30;
    inline constexpr static int kButtonWidth = 70;
    inline constexpr static int kButtonHeight = 20;
    inline constexpr static int kButtonSpacing = 5;

    tss::Theme* theme;

    tss::Button blackThemeButton;
    tss::Button creamThemeButton;
    tss::Button debugThemeButton;
    tss::Button someDisabledButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

