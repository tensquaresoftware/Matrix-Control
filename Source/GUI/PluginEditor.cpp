#include "../Core/PluginProcessor.h"
#include "PluginEditor.h"
#include "Themes/Theme.h"
#include "Panels/HeaderPanel/HeaderPanel.h"
#include "Panels/BodyPanel/BodyPanel.h"
#include "Panels/FooterPanel/FooterPanel.h"
#include "Factories/WidgetFactory.h"
#include "../Core/Factories/ApvtsFactory.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
{
    validateSynthDescriptorsAtStartup();
    
    theme = tss::Theme::create(tss::Theme::ColourVariant::Black);
    
    widgetFactory = std::make_unique<WidgetFactory>(pluginProcessor.getApvts());
    mainComponent = std::make_unique<MainComponent>(*theme, *widgetFactory);
    addAndMakeVisible(*mainComponent);
    
    auto& headerPanel = mainComponent->getHeaderPanel();
    headerPanel.getButtonBlack().onClick = [this]
    {
        theme = tss::Theme::create(tss::Theme::ColourVariant::Black);
        updateTheme();
    };
    
    headerPanel.getButtonCream().onClick = [this]
    {
        theme = tss::Theme::create(tss::Theme::ColourVariant::Cream);
        updateTheme();
    };
    
    headerPanel.getButtonDebug().onClick = [this]
    {
        theme = tss::Theme::create(tss::Theme::ColourVariant::Debug);
        updateTheme();
    };
    
    setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(true, true);
    
    setSize(1325, HeaderPanel::kHeight + BodyPanel::kHeight + FooterPanel::kHeight);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(theme->getGuiBackgroundColour());
}

void PluginEditor::resized()
{
    if (mainComponent != nullptr)
        mainComponent->setBounds(getLocalBounds());
}

void PluginEditor::mouseDown(const juce::MouseEvent&)
{
    unfocusAllComponents();
}

void PluginEditor::updateTheme()
{
    if (mainComponent != nullptr)
    {
        mainComponent->setTheme(*theme);
    }
    repaint();
}

void PluginEditor::validateSynthDescriptorsAtStartup()
{
    auto validationResult = ApvtsFactory::validateSynthDescriptors();
    if (!validationResult.isValid)
    {
        DBG("SynthDescriptors validation failed:");
        for (const auto& error : validationResult.errors)
        {
            DBG("  ERROR: " + error);
        }
        jassertfalse;
    }
}

