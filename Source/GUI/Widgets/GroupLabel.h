#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class GroupLabel : public juce::Component
    {
    public:
        enum class GroupWidth
        {
            InternalPatchesBrowser,
            InternalPatchesUtility,
            ComputerPatchesBrowser,
            ComputerPatchesFolderAndFiles
        };

        explicit GroupLabel(Theme& inTheme, GroupWidth width, const juce::String& text = juce::String());
        ~GroupLabel() override = default;

        void setTheme(Theme& inTheme);

        void setText(const juce::String& newText);
        juce::String getText() const { return labelText; }

        void paint(juce::Graphics& g) override;

        static constexpr int getWidth(GroupWidth width)
        {
            switch (width)
            {
                case GroupWidth::InternalPatchesBrowser:
                    return kInternalPatchesBrowserWidth;
                case GroupWidth::InternalPatchesUtility:
                    return kInternalPatchesUtilityWidth;
                case GroupWidth::ComputerPatchesBrowser:
                    return kComputerPatchesBrowserWidth;
                case GroupWidth::ComputerPatchesFolderAndFiles:
                    return kComputerPatchesFolderAndFilesWidth;
                default:
                    return kInternalPatchesBrowserWidth;
            }
        }

    private:
        inline constexpr static int kInternalPatchesBrowserWidth = 115;
        inline constexpr static int kInternalPatchesUtilityWidth = 155;
        inline constexpr static int kComputerPatchesBrowserWidth = 140;
        inline constexpr static int kComputerPatchesFolderAndFilesWidth = 130;
        inline constexpr static int kHeight = 40;
        inline constexpr static float kTopAreaHeight = 15.0f;
        inline constexpr static float kContentHeight = 20.0f;
        inline constexpr static float kTextSpacing = 20.0f;
        inline constexpr static float kLineThickness = 1.0f;

        Theme* theme = nullptr;
        juce::String labelText;

        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLeftLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawRightLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        juce::Rectangle<float> getContentArea(const juce::Rectangle<float>& bounds) const;
        float calculateTextWidth() const;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroupLabel)
    };
}

