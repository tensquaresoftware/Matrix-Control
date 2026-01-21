#include "MainComponent.h"

#include "GUI/Factories/WidgetFactory.h"
#include "GUI/Themes/Theme.h"

using tss::Theme;

MainComponent::MainComponent(Theme& theme, int width, int height, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , headerPanel(theme)
    , bodyPanel(theme, widgetFactory, apvts)
    , footerPanel(theme, apvts)
{
    setOpaque(true);
    setSize(width, height);
    
    addAndMakeVisible(headerPanel);
    addAndMakeVisible(bodyPanel);
    addAndMakeVisible(footerPanel);
}

void MainComponent::paint(juce::Graphics& g)
{
    if (theme_ != nullptr)
        g.fillAll(theme_->getGuiBackgroundColour());
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto y = 0;
    
    layoutHeaderPanel(bounds, y);
    y += HeaderPanel::getHeight();
    
    layoutBodyPanel(bounds, y);
    y += BodyPanel::getHeight();
    
    layoutFooterPanel(bounds, y);
}

void MainComponent::layoutHeaderPanel(juce::Rectangle<int> bounds, int y)
{
    headerPanel.setBounds(
        bounds.getX(),
        bounds.getY() + y,
        bounds.getWidth(),
        HeaderPanel::getHeight()
    );
}

void MainComponent::layoutBodyPanel(juce::Rectangle<int> bounds, int y)
{
    bodyPanel.setBounds(
        bounds.getX(),
        bounds.getY() + y,
        bounds.getWidth(),
        BodyPanel::getHeight()
    );
}

void MainComponent::layoutFooterPanel(juce::Rectangle<int> bounds, int y)
{
    footerPanel.setBounds(
        bounds.getX(),
        bounds.getY() + y,
        bounds.getWidth(),
        FooterPanel::getHeight()
    );
}

void MainComponent::setTheme(Theme& theme)
{
    theme_ = &theme;
    headerPanel.setTheme(theme);
    bodyPanel.setTheme(theme);
    footerPanel.setTheme(theme);
    repaint();
}

