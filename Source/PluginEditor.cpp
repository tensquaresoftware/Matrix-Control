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
    , buttonI(20, "I")
    , buttonC(20, "C")
    , buttonP(20, "P")
    , buttonUnlock(75, "UNLOCK")
    , buttonInit(35, "INIT")
    , buttonStore(40, "STORE")
    , comboBox1(McComboBox::Size::Normal)
    , comboBox2(McComboBox::Size::Large)
    , frequencyLabel("FREQUENCY")
    , frequencySlider(0.0)
    , syncLabel("SYNC")
    , syncComboBox(McComboBox::Size::Normal)
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
    
    buttonI.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(buttonI);
    
    buttonC.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(buttonC);
    
    buttonP.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(buttonP);
    
    buttonUnlock.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(buttonUnlock);
    
    buttonInit.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(buttonInit);
    
    buttonStore.setLookAndFeel(lookAndFeel.get());
    buttonStore.setEnabled(false);
    addAndMakeVisible(buttonStore);
    
    comboBox1.addItem("NONE", 1);
    comboBox1.addItem("ENV 1", 2);
    comboBox1.addItem("ENV 2", 3);
    comboBox1.addItem("ENV 3", 4);
    comboBox1.addItem("LFO 1", 5);
    comboBox1.addItem("LFO 2", 6);
    comboBox1.addItem("VIBRATO", 7);
    comboBox1.addItem("RAMP 1", 8);
    comboBox1.addItem("RAMP 2", 9);
    comboBox1.addItem("KEYBD", 10);
    comboBox1.addItem("PORTA", 11);
    comboBox1.addItem("TRACK", 12);
    comboBox1.addItem("KB GATE", 13);
    comboBox1.addItem("VELOCITY", 14);
    comboBox1.addItem("REL VEL", 15);
    comboBox1.addItem("PRESSURE", 16);
    comboBox1.addItem("PEDAL 1", 17);
    comboBox1.addItem("PEDAL 2", 18);
    comboBox1.addItem("LEVER 1", 19);
    comboBox1.addItem("LEVER 2", 20);
    comboBox1.addItem("LEVER 3", 21);
    comboBox1.setSelectedId(1);
    comboBox1.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(comboBox1);
    
    comboBox2.addItem("NONE", 1);
    comboBox2.addItem("DCO 1 FREQUENCY", 2);
    comboBox2.addItem("DCO 1 PULSE WIDTH", 3);
    comboBox2.addItem("DCO 1 WAVE SHAPE", 4);
    comboBox2.addItem("DCO 2 FREQUENCY", 5);
    comboBox2.addItem("DCO 2 PULSE WIDTH", 6);
    comboBox2.addItem("DCO 2 WAVE SHAPE", 7);
    comboBox2.addItem("DCO 1 / DCO 2 MIX", 8);
    comboBox2.addItem("VCF FM < DCO 1", 9);
    comboBox2.addItem("VCF FREQUENCY", 10);
    comboBox2.addItem("VCF RESONANCE", 11);
    comboBox2.addItem("VCA 1 VOLUME", 12);
    comboBox2.addItem("VCA 2 VOLUME", 13);
    comboBox2.addItem("ENV 1 DELAY", 14);
    comboBox2.addItem("ENV 1 ATTACK", 15);
    comboBox2.addItem("ENV 1 DECAY", 16);
    comboBox2.addItem("ENV 1 RELEASE", 17);
    comboBox2.addItem("ENV 1 AMPLITUDE", 18);
    comboBox2.addItem("ENV 2 DELAY", 19);
    comboBox2.addItem("ENV 2 ATTACK", 20);
    comboBox2.addItem("ENV 2 DECAY", 21);
    comboBox2.addItem("ENV 2 RELEASE", 22);
    comboBox2.addItem("ENV 2 AMPLITUDE", 23);
    comboBox2.addItem("ENV 3 DELAY", 24);
    comboBox2.addItem("ENV 3 ATTACK", 25);
    comboBox2.addItem("ENV 3 DECAY", 26);
    comboBox2.addItem("ENV 3 RELEASE", 27);
    comboBox2.addItem("ENV 3 AMPLITUDE", 28);
    comboBox2.addItem("LFO 1 SPEED", 29);
    comboBox2.addItem("LFO 1 AMPLITUDE", 30);
    comboBox2.addItem("LFO 2 SPEED", 31);
    comboBox2.addItem("LFO 2 AMPLITUDE", 32);
    comboBox2.addItem("PORTAMENTO RATE", 33);
    comboBox2.setSelectedId(1);
    comboBox2.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(comboBox2);
    
    frequencyLabel.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(frequencyLabel);
    
    frequencySlider.setRange(0.0, 63.0, 1.0);
    frequencySlider.setValue(0.0);
    frequencySlider.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(frequencySlider);
    
    syncLabel.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(syncLabel);
    
    syncComboBox.addItem("OFF", 1);
    syncComboBox.addItem("SOFT", 2);
    syncComboBox.addItem("MEDIUM", 3);
    syncComboBox.addItem("HARD", 4);
    syncComboBox.setSelectedId(1);
    syncComboBox.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(syncComboBox);
    
    themeLabel.setText("THEME");
    themeLabel.setLookAndFeel(lookAndFeel.get());
    addAndMakeVisible(themeLabel);
    
    themeComboBox.addItem("BLACK", 1);
    themeComboBox.addItem("CREAM", 2);
    themeComboBox.addItem("DEBUG", 3);
    themeComboBox.setSelectedId(1);
    themeComboBox.setLookAndFeel(lookAndFeel.get());
    themeComboBox.onChange = [this] { updateTheme(); };
    addAndMakeVisible(themeComboBox);
    
    setSize(500, 400);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(lookAndFeel->getGuiBackgroundColour());
}

void PluginEditor::resized()
{
    pluginVersionLabel.setBounds(5, getHeight() - 20, 100, 20);
    
    auto themeLabelWidth = 50;
    auto themeComboBoxWidth = 60;
    themeLabel.setBounds(5, 5, themeLabelWidth, 20);
    themeComboBox.setBounds(5 + themeLabelWidth, 5, themeComboBoxWidth, 20);
    
    auto startX = 5;
    auto startY = 70;
    auto sliderWidth = 60;
    auto sliderHeight = 20;
    auto spacing = 20;
    auto verticalSpacing = 30;
    
    testSlider1.setBounds(startX, startY, sliderWidth, sliderHeight);
    testSlider2.setBounds(startX + sliderWidth + spacing, startY, sliderWidth, sliderHeight);
    testSlider3.setBounds(startX + 2 * (sliderWidth + spacing), startY, 50, 18);
    testSlider4.setBounds(startX + 3 * (sliderWidth + spacing), startY, sliderWidth, sliderHeight);
    
    auto buttonsStartY = startY + verticalSpacing;
    auto buttonHeight = 20;
    auto buttonSpacing = 5;
    
    buttonI.setBounds(startX, buttonsStartY, 20, buttonHeight);
    buttonC.setBounds(startX + 20, buttonsStartY, 20, buttonHeight);
    buttonP.setBounds(startX + 40, buttonsStartY, 20, buttonHeight);
    
    auto rightButtonsStartX = startX + 40 + 20 + buttonSpacing;
    buttonUnlock.setBounds(rightButtonsStartX, buttonsStartY, 75, buttonHeight);
    buttonInit.setBounds(rightButtonsStartX + 75 + buttonSpacing, buttonsStartY, 35, buttonHeight);
    buttonStore.setBounds(rightButtonsStartX + 75 + buttonSpacing + 35 + buttonSpacing, buttonsStartY, 40, buttonHeight);
    
    auto combosStartY = buttonsStartY + buttonHeight + buttonSpacing + 10;
    comboBox1.setBounds(startX, combosStartY, 60, 20);
    comboBox2.setBounds(startX + 60 + buttonSpacing, combosStartY, 105, 20);
    
    auto frequencyStartY = combosStartY + 20 + 5;
    auto labelWidth = 90;
    frequencyLabel.setBounds(startX, frequencyStartY, labelWidth, 20);
    frequencySlider.setBounds(startX + labelWidth, frequencyStartY, sliderWidth, 20);
    
    auto syncStartY = frequencyStartY + 20 + 5;
    syncLabel.setBounds(startX, syncStartY, labelWidth, 20);
    syncComboBox.setBounds(startX + labelWidth, syncStartY, 60, 20);
}

void PluginEditor::updateTheme()
{
    auto selectedId = themeComboBox.getSelectedId();
    
    if (selectedId == 1)
    {
        lookAndFeel->setTheme(McLookAndFeel::Theme::Black);
    }
    else if (selectedId == 2)
    {
        lookAndFeel->setTheme(McLookAndFeel::Theme::Cream);
    }
    else if (selectedId == 3)
    {
        lookAndFeel->setTheme(McLookAndFeel::Theme::Debug);
    }
    
    updateAllWidgetsLookAndFeel();
    repaint();
}

void PluginEditor::updateAllWidgetsLookAndFeel()
{
    testSlider1.setLookAndFeel(lookAndFeel.get());
    testSlider1.repaint();
    testSlider2.setLookAndFeel(lookAndFeel.get());
    testSlider2.repaint();
    testSlider3.setLookAndFeel(lookAndFeel.get());
    testSlider3.repaint();
    testSlider4.setLookAndFeel(lookAndFeel.get());
    testSlider4.repaint();
    
    buttonI.setLookAndFeel(lookAndFeel.get());
    buttonI.repaint();
    buttonC.setLookAndFeel(lookAndFeel.get());
    buttonC.repaint();
    buttonP.setLookAndFeel(lookAndFeel.get());
    buttonP.repaint();
    buttonUnlock.setLookAndFeel(lookAndFeel.get());
    buttonUnlock.repaint();
    buttonInit.setLookAndFeel(lookAndFeel.get());
    buttonInit.repaint();
    buttonStore.setLookAndFeel(lookAndFeel.get());
    buttonStore.repaint();
    
    comboBox1.setLookAndFeel(lookAndFeel.get());
    comboBox1.repaint();
    comboBox2.setLookAndFeel(lookAndFeel.get());
    comboBox2.repaint();
    
    frequencyLabel.setLookAndFeel(lookAndFeel.get());
    frequencyLabel.repaint();
    frequencySlider.setLookAndFeel(lookAndFeel.get());
    frequencySlider.repaint();
    
    syncLabel.setLookAndFeel(lookAndFeel.get());
    syncLabel.repaint();
    syncComboBox.setLookAndFeel(lookAndFeel.get());
    syncComboBox.repaint();
    
    themeLabel.setLookAndFeel(lookAndFeel.get());
    themeLabel.repaint();
    themeComboBox.setLookAndFeel(lookAndFeel.get());
    themeComboBox.repaint();
}
