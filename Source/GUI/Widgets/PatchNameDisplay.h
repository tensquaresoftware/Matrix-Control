#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Theme;

    class PatchNameDisplay : public juce::Component
    {
    public:
        explicit PatchNameDisplay(Theme& theme, int width, int height);
        ~PatchNameDisplay() override = default;

        void setTheme(Theme& theme);
        void setPatchName(const juce::String& patchName);
        
        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static int kVerticalPadding_ = 15;
        inline constexpr static float kFontHeight_ = 28.0f;

        Theme* theme_ = nullptr;
        int width_;
        int height_;
        juce::String patchName_;

        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawText(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    };
}
