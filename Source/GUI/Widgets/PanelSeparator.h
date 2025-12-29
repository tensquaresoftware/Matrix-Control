#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    class Skin;

    class PanelSeparator : public juce::Component
    {
    public:
        explicit PanelSeparator(Skin& newSkin);
        ~PanelSeparator() override = default;

        void setSkin(Skin& skin);

        void paint(juce::Graphics& g) override;

    private:
        void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);

        Skin* skin = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelSeparator)
    };
}

