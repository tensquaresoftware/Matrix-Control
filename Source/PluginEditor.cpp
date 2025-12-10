#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
    , pluginVersionLabel("VersionLabel", "v0.0.5")
    , testSlider1(31.0)
    , testSlider2(0.0)
    , testSlider3(100.0)
    , testSlider4(50.0)
    , testSlider5(31.0)
    , testSlider6(77.0)
{
    lookAndFeel = std::make_unique<McLookAndFeel>();
    
    pluginVersionLabel.setFont(juce::Font(juce::FontOptions().withHeight(8.0f)));
    pluginVersionLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    pluginVersionLabel.setJustificationType(juce::Justification::bottomLeft);
    addAndMakeVisible(pluginVersionLabel);
    
    testSlider1.setRange(0.0, 63.0, 1.0);
    testSlider1.setValue(31.0);
    testSlider1.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(testSlider1);
    
    testSlider2.setRange(-63.0, 63.0, 1.0);
    testSlider2.setValue(0.0);
    testSlider2.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(testSlider2);
    
    testSlider3.setRange(0.0, 127.0, 1.0);
    testSlider3.setValue(100.0);
    testSlider3.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(testSlider3);
    
    testSlider4.setRange(0.0, 100.0, 1.0);
    testSlider4.setValue(50.0);
    testSlider4.setLookAndFeel(lookAndFeel.get());
    testSlider4.setEnabled(false);
    addAndMakeVisible(testSlider4);
    
    testSlider5.setRange(0.0, 63.0, 1.0);
    testSlider5.setValue(31.0);
    testSlider5.setLookAndFeel(lookAndFeel.get());
    testSlider5.onValueChange = [this]
    {
        testSlider1.setValue(testSlider5.getValue(), juce::dontSendNotification);
    };
    addAndMakeVisible(testSlider5);
    
    testSlider6.setRange(-128.0, 127.0, 1.0);
    testSlider6.setValue(77.0);
    testSlider6.setSize(200, 50);
    testSlider6.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(testSlider6);
    
    setSize(500, 400);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF202020));
}

void PluginEditor::resized()
{
    pluginVersionLabel.setBounds(0, getHeight() - 20, 100, 20);
    
    auto centreY = getHeight() / 2 - 10;
    auto sliderWidth = 60;
    auto sliderHeight = 20;
    auto spacing = 20;
    auto verticalSpacing = 30;
    auto startX = 50;
    
    testSlider1.setBounds(startX, centreY, sliderWidth, sliderHeight);
    testSlider2.setBounds(startX + sliderWidth + spacing, centreY, sliderWidth, sliderHeight);
    testSlider3.setBounds(startX + 2 * (sliderWidth + spacing), centreY, sliderWidth, sliderHeight);
    testSlider4.setBounds(startX + 3 * (sliderWidth + spacing), centreY, sliderWidth, sliderHeight);
    
    testSlider5.setBounds(startX, centreY + verticalSpacing, sliderWidth, sliderHeight);
    testSlider6.setBounds(startX + sliderWidth + spacing, centreY + verticalSpacing, 200, 50);
}
