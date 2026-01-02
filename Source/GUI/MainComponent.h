#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Panels/HeaderPanel/HeaderPanel.h"
#include "Panels/BodyPanel/BodyPanel.h"
#include "Panels/FooterPanel/FooterPanel.h"

namespace tss
{
    class Skin;
}

class WidgetFactory;

using tss::Skin;

class MainComponent : public juce::Component
{
public:
    MainComponent(Skin& skin, WidgetFactory& widgetFactory);
    ~MainComponent() override = default;

    void resized() override;
    void setSkin(Skin& skin);

    HeaderPanel& getHeaderPanel() { return headerPanel; }
    BodyPanel& getBodyPanel() { return bodyPanel; }
    FooterPanel& getFooterPanel() { return footerPanel; }

private:
    HeaderPanel headerPanel;
    BodyPanel bodyPanel;
    FooterPanel footerPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

