#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/Skin/SkinFactory.h"
#include "Shared/WidgetFactory.h"
#include "Shared/ApvtsFactory.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
{
    validateSynthDescriptorsAtStartup();
    
    skin = tss::SkinFactory::createSkin(tss::Skin::ColourVariant::Black);
    
    widgetFactory = std::make_unique<WidgetFactory>(pluginProcessor.getApvts());
    mainComponent = std::make_unique<MainComponent>(*skin, *widgetFactory);
    addAndMakeVisible(*mainComponent);
    
    auto& headerPanel = mainComponent->getHeaderPanel();
    headerPanel.getButtonBlack().onClick = [this]
    {
        skin = tss::SkinFactory::createSkin(tss::Skin::ColourVariant::Black);
        updateSkin();
    };
    
    headerPanel.getButtonCream().onClick = [this]
    {
        skin = tss::SkinFactory::createSkin(tss::Skin::ColourVariant::Cream);
        updateSkin();
    };
    
    headerPanel.getButtonDebug().onClick = [this]
    {
        skin = tss::SkinFactory::createSkin(tss::Skin::ColourVariant::Debug);
        updateSkin();
    };
    
    setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(true, true);
    
    setSize(tss::SkinDimensions::Gui::kWidth, tss::SkinDimensions::Gui::kHeight);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(skin->getGuiBackgroundColour());
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

void PluginEditor::updateSkin()
{
    if (mainComponent != nullptr)
    {
        mainComponent->setSkin(*skin);
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

