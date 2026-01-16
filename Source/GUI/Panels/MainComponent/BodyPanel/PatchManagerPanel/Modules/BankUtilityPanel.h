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
    BankUtilityPanel(tss::Theme& theme, WidgetFactory& widgetFactory);
    ~BankUtilityPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return kWidth_; }
    static int getHeight() { return kHeight_; }

private:
    inline constexpr static int kWidth_ = 275;
    inline constexpr static int kHeight_ = 115;
    inline constexpr static int kTopPadding_ = 10;
    inline constexpr static int kSpacing_ = 5;

    tss::Theme* theme_;

    std::unique_ptr<tss::ModuleHeader> bankUtilityModuleHeader_;
    std::unique_ptr<tss::Label> bankSelectorLabel_;
    std::unique_ptr<tss::Button> unlockBankButton_;
    std::unique_ptr<tss::Button> selectBank0Button_;
    std::unique_ptr<tss::Button> selectBank1Button_;
    std::unique_ptr<tss::Button> selectBank2Button_;
    std::unique_ptr<tss::Button> selectBank3Button_;
    std::unique_ptr<tss::Button> selectBank4Button_;
    std::unique_ptr<tss::Button> selectBank5Button_;
    std::unique_ptr<tss::Button> selectBank6Button_;
    std::unique_ptr<tss::Button> selectBank7Button_;
    std::unique_ptr<tss::Button> selectBank8Button_;
    std::unique_ptr<tss::Button> selectBank9Button_;

    void setupModuleHeader(tss::Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId);
    void setupBankSelectorLabel(tss::Theme& theme);
    void setupSelectBankButtons(tss::Theme& theme, WidgetFactory& widgetFactory);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BankUtilityPanel)
};
