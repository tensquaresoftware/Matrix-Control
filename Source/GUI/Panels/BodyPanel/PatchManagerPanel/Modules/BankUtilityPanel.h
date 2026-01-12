#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class ModuleHeader;
    class Label;
    class Button;
}

class WidgetFactory;

class BankUtilityPanel : public juce::Component
{
public:
    BankUtilityPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory);
    ~BankUtilityPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 115;
    inline constexpr static int kTopPadding = 10;
    inline constexpr static int kSpacing = 5;

    tss::Theme* theme;

    std::unique_ptr<tss::ModuleHeader> bankUtilityModuleHeader;
    std::unique_ptr<tss::Label> bankSelectorLabel;
    std::unique_ptr<tss::Button> unlockBankButton;
    std::unique_ptr<tss::Button> selectBank0Button;
    std::unique_ptr<tss::Button> selectBank1Button;
    std::unique_ptr<tss::Button> selectBank2Button;
    std::unique_ptr<tss::Button> selectBank3Button;
    std::unique_ptr<tss::Button> selectBank4Button;
    std::unique_ptr<tss::Button> selectBank5Button;
    std::unique_ptr<tss::Button> selectBank6Button;
    std::unique_ptr<tss::Button> selectBank7Button;
    std::unique_ptr<tss::Button> selectBank8Button;
    std::unique_ptr<tss::Button> selectBank9Button;

    void setupModuleHeader(tss::Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupBankSelectorLabel(tss::Theme& inTheme);
    void setupSelectBankButtons(tss::Theme& inTheme, WidgetFactory& widgetFactory);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BankUtilityPanel)
};
