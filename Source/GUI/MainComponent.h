#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Panels/HeaderPanel/HeaderPanel.h"
#include "Panels/MainPanel/MainPanel.h"
#include "Panels/FooterPanel/FooterPanel.h"

namespace tss
{
    class Skin;
}

using tss::Skin;

class MainComponent : public juce::Component
{
public:
    explicit MainComponent(Skin& skin);
    ~MainComponent() override = default;

    void resized() override;
    void setSkin(Skin& skin);

    HeaderPanel& getHeaderPanel() { return headerPanel; }
    MainPanel& getMainPanel() { return mainPanel; }
    FooterPanel& getFooterPanel() { return footerPanel; }

private:
    HeaderPanel headerPanel;
    MainPanel mainPanel;
    FooterPanel footerPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

