#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Widgets/Button.h"

namespace tss
{
    class Theme;
    class Button;
}

class HeaderPanel : public juce::Component
{
public:
    explicit HeaderPanel(tss::Theme& theme);
    ~HeaderPanel() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    tss::Button& getButtonBlack() { return blackThemeButton_; }
    tss::Button& getButtonCream() { return creamThemeButton_; }
    tss::Button& getButtonDebug() { return debugThemeButton_; }
    tss::Button& getButtonDisabled() { return someDisabledButton_; }

    static int getHeight() { return kHeight_; }
    static int getButtonWidth() { return kButtonWidth_; }
    static int getButtonHeight() { return kButtonHeight_; }
    static int getButtonSpacing() { return kButtonSpacing_; }

private:
    inline constexpr static int kHeight_ = 30;
    inline constexpr static int kButtonWidth_ = 70;
    inline constexpr static int kButtonHeight_ = 20;
    inline constexpr static int kButtonSpacing_ = 5;

    tss::Theme* theme_;

    tss::Button blackThemeButton_;
    tss::Button creamThemeButton_;
    tss::Button debugThemeButton_;
    tss::Button someDisabledButton_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderPanel)
};

