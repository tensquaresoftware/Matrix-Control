#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Panels/HeaderPanel/HeaderPanel.h"
#include "Panels/BodyPanel/BodyPanel.h"
#include "Panels/FooterPanel/FooterPanel.h"

namespace tss
{
    class Theme;
}

class WidgetFactory;

using tss::Theme;

class MainComponent : public juce::Component
{
public:
    MainComponent(Theme& theme, WidgetFactory& widgetFactory);
    ~MainComponent() override = default;

    void resized() override;
    void setTheme(Theme& theme);

    HeaderPanel& getHeaderPanel() { return headerPanel; }
    BodyPanel& getBodyPanel() { return bodyPanel; }
    FooterPanel& getFooterPanel() { return footerPanel; }

private:
    HeaderPanel headerPanel;
    BodyPanel bodyPanel;
    FooterPanel footerPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

