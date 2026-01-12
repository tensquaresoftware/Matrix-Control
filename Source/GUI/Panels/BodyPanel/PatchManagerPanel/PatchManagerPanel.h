#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;
    class SectionHeader;
}

class WidgetFactory;
class BankUtilityPanel;
class InternalPatchesPanel;
class ComputerPatchesPanel;

class PatchManagerPanel : public juce::Component
{
public:
    PatchManagerPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory);
    ~PatchManagerPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 375;

    tss::Theme* theme;

    std::unique_ptr<tss::SectionHeader> sectionHeader;
    std::unique_ptr<BankUtilityPanel> bankUtilityPanel;
    std::unique_ptr<InternalPatchesPanel> internalPatchesPanel;
    std::unique_ptr<ComputerPatchesPanel> computerPatchesPanel;

    void layoutSectionHeader(const juce::Rectangle<int>& bounds, int y);
    void layoutBankUtilityPanel(const juce::Rectangle<int>& bounds, int y);
    void layoutInternalPatchesPanel(const juce::Rectangle<int>& bounds, int y);
    void layoutComputerPatchesPanel(const juce::Rectangle<int>& bounds, int y);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchManagerPanel)
};

