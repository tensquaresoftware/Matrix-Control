#include "Core/PluginProcessor.h"
#include "PluginEditor.h"
#include "Themes/Theme.h"
#include "Panels/MainComponent/HeaderPanel/HeaderPanel.h"
#include "Panels/MainComponent/BodyPanel/BodyPanel.h"
#include "Panels/MainComponent/FooterPanel/FooterPanel.h"
#include "Factories/WidgetFactory.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
{
    theme = tss::Theme::create(tss::Theme::ColourVariant::Black);
    
    widgetFactory = std::make_unique<WidgetFactory>(pluginProcessor.getApvts());
    
    setOpaque(true);
    setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(true, true);
    
    setSize(getWidth(), getHeight());
    
    const auto editorWidth = getWidth();
    const auto editorHeight = getHeight();
    mainComponent = std::make_unique<MainComponent>(*theme, editorWidth, editorHeight, *widgetFactory, pluginProcessor.getApvts());
    addAndMakeVisible(*mainComponent);
    
    if (auto* component = mainComponent.get())
    {
        component->setBounds(getLocalBounds());
    }
    
    auto& headerPanel = mainComponent->getHeaderPanel();
    
    headerPanel.getSkinComboBox().onChange = [this, &headerPanel]
    {
        const auto selectedId = headerPanel.getSkinComboBox().getSelectedId();
        if (selectedId == 1)
        {
            theme = tss::Theme::create(tss::Theme::ColourVariant::Black);
            updateTheme();
        }
        else if (selectedId == 2)
        {
            theme = tss::Theme::create(tss::Theme::ColourVariant::Cream);
            updateTheme();
        }
    };
    
    headerPanel.getZoomComboBox().onChange = [&headerPanel]
    {
        const auto selectedId = headerPanel.getZoomComboBox().getSelectedId();
        float zoomFactor = 1.0f;
        
        switch (selectedId)
        {
            case 1: zoomFactor = 0.5f; break;   // 50%
            case 2: zoomFactor = 0.75f; break;  // 75%
            case 3: zoomFactor = 0.9f; break;   // 90%
            case 4: zoomFactor = 1.0f; break;   // 100%
            case 5: zoomFactor = 1.25f; break;  // 125%
            case 6: zoomFactor = 1.5f; break;   // 150%
            case 7: zoomFactor = 2.0f; break;   // 200%
            default: zoomFactor = 1.0f; break;
        }
        
        // TODO: Implement zoom functionality (Phase 7)
        // For now, just log the selected zoom factor
        juce::ignoreUnused(zoomFactor);
    };
    
    repaint();
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(theme->getBodyPanelBackgroundColour());
}

void PluginEditor::resized()
{
    if (auto* component = mainComponent.get())
        component->setBounds(getLocalBounds());
}

void PluginEditor::mouseDown(const juce::MouseEvent&)
{
    unfocusAllComponents();
}

void PluginEditor::updateTheme()
{
    if (auto* widget = mainComponent.get())
        widget->setTheme(*theme);
    repaint();
}

