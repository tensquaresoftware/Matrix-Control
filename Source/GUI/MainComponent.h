#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Panels/MainComponent/HeaderPanel/HeaderPanel.h"
#include "Panels/MainComponent/BodyPanel/BodyPanel.h"
#include "Panels/MainComponent/FooterPanel/FooterPanel.h"

namespace tss
{
    class Theme;
}

class WidgetFactory;

class MainComponent : public juce::Component
{
public:
    MainComponent(tss::Theme& theme, int width, int height, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MainComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    HeaderPanel& getHeaderPanel() { return headerPanel; }
    BodyPanel& getBodyPanel() { return bodyPanel; }
    FooterPanel& getFooterPanel() { return footerPanel; }

private:
    void layoutHeaderPanel(juce::Rectangle<int> bounds, int y);
    void layoutBodyPanel(juce::Rectangle<int> bounds, int y);
    void layoutFooterPanel(juce::Rectangle<int> bounds, int y);

    tss::Theme* theme_;

    HeaderPanel headerPanel;
    BodyPanel bodyPanel;
    FooterPanel footerPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

