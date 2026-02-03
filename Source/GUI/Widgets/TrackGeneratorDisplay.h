#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class TrackGeneratorDisplay : public juce::Component
    {
    public:
        explicit TrackGeneratorDisplay(Skin& skin, int width, int height);
        ~TrackGeneratorDisplay() override = default;

        void setSkin(Skin& skin);
        
        void paint(juce::Graphics& g) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static int kVerticalPadding_ = 15;
        inline constexpr static float kTriangleBase_ = 10.0f;

        Skin* skin_ = nullptr;
        int width_;
        int height_;

        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawTriangle(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackGeneratorDisplay)
    };
}
