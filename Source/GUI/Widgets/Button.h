#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class Button : public juce::Button
    {
    public:
        enum class ButtonWidth
        {
            InitCopyPaste,
            PatchManagerBankSelect,
            InternalPatchesUtility,
            ComputerPatchesFolderFiles,
            PatchManagerUnlock,
            HeaderPanelTheme
        };

        explicit Button(Theme& inTheme, ButtonWidth width, const juce::String& text = {});
        ~Button() override = default;

        void setTheme(Theme& inTheme);
        
        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        static constexpr int getWidth(ButtonWidth width)
        {
            switch (width)
            {
                case ButtonWidth::InitCopyPaste:
                    return kInitCopyPasteWidth;
                case ButtonWidth::PatchManagerBankSelect:
                    return kPatchManagerBankSelectWidth;
                case ButtonWidth::InternalPatchesUtility:
                case ButtonWidth::ComputerPatchesFolderFiles:
                    return kInternalPatchesUtilityWidth;
                case ButtonWidth::PatchManagerUnlock:
                    return kPatchManagerUnlockWidth;
                case ButtonWidth::HeaderPanelTheme:
                    return kHeaderPanelThemeWidth;
                default:
                    return kInitCopyPasteWidth;
            }
        }

        static constexpr int getHeight() { return kHeight; }

    private:
        inline constexpr static int kInitCopyPasteWidth = 20;
        inline constexpr static int kPatchManagerBankSelectWidth = 35;
        inline constexpr static int kInternalPatchesUtilityWidth = 35;
        inline constexpr static int kPatchManagerUnlockWidth = 75;
        inline constexpr static int kHeaderPanelThemeWidth = 70;
        inline constexpr static int kHeight = 20;
        inline constexpr static float kBorderThickness = 2.0f;

        Theme* theme = nullptr;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool enabled, bool isHighlighted, bool isDown);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Button)
    };
}

