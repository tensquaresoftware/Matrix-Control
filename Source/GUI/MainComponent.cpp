#include "MainComponent.h"

#include "../Shared/WidgetFactory.h"

using tss::Skin;

MainComponent::MainComponent(Skin& skin, WidgetFactory& widgetFactory)
    : headerPanel(skin)
    , bodyPanel(skin, widgetFactory)
    , footerPanel(skin)
{
    addAndMakeVisible(headerPanel);
    addAndMakeVisible(bodyPanel);
    addAndMakeVisible(footerPanel);
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto headerHeight = HeaderPanel::getHeight();
    auto bodyPanelHeight = BodyPanel::getHeight();
    auto footerHeight = FooterPanel::getHeight();

    headerPanel.setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), headerHeight);
    bodyPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight, bounds.getWidth(), bodyPanelHeight);
    footerPanel.setBounds(bounds.getX(), bounds.getY() + headerHeight + bodyPanelHeight, bounds.getWidth(), footerHeight);
}

void MainComponent::setSkin(Skin& skin)
{
    headerPanel.setSkin(skin);
    bodyPanel.setSkin(skin);
    footerPanel.setSkin(skin);
}

