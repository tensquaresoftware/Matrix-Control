#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , pluginProcessor(p)
    , buttonBlack(70, "BLACK")
    , buttonCream(70, "CREAM")
    , buttonDebug(70, "DEBUG")
    , buttonDisabled(70, "DISABLED")
    , labelPortamentoRate("PORTAMENTO RATE")
    , sliderPortamentoRate(40.0)
    , labelPortaVelocity("PORTA < VELOCITY")
    , sliderPortaVelocity(0.0)
    , labelFrequency("FREQUENCY")
    , sliderFrequency(100.0)
    , labelTrackInput("TRACK INPUT")
    , comboBoxTrackInput(McComboBox::Size::Normal)
    , labelLegatoPorta("LEGATO PORTA")
    , comboBoxLegatoPorta(McComboBox::Size::Normal)
    , comboBoxTopLeft(McComboBox::Size::Large)
    , comboBoxTopRight(McComboBox::Size::Large)
    , comboBoxBottomLeft(McComboBox::Size::Large)
    , comboBoxBottomRight(McComboBox::Size::Large)
{
    theme = std::make_unique<McTheme>();
    
    buttonBlack.setTheme(theme.get());
    buttonBlack.onClick = [this]
    {
        theme->setTheme(McTheme::Theme::Black);
        updateTheme();
    };
    addAndMakeVisible(buttonBlack);
    
    buttonCream.setTheme(theme.get());
    buttonCream.onClick = [this]
    {
        theme->setTheme(McTheme::Theme::Cream);
        updateTheme();
    };
    addAndMakeVisible(buttonCream);
    
    buttonDebug.setTheme(theme.get());
    buttonDebug.onClick = [this]
    {
        theme->setTheme(McTheme::Theme::Debug);
        updateTheme();
    };
    addAndMakeVisible(buttonDebug);
    
    buttonDisabled.setTheme(theme.get());
    buttonDisabled.setEnabled(false);
    addAndMakeVisible(buttonDisabled);
    
    labelPortamentoRate.setTheme(theme.get());
    addAndMakeVisible(labelPortamentoRate);
    
    sliderPortamentoRate.setRange(0.0, 63.0, 1.0);
    sliderPortamentoRate.setValue(40.0);
    sliderPortamentoRate.setTheme(theme.get());
    addAndMakeVisible(sliderPortamentoRate);
    
    labelPortaVelocity.setTheme(theme.get());
    addAndMakeVisible(labelPortaVelocity);
    
    sliderPortaVelocity.setRange(-63.0, 63.0, 1.0);
    sliderPortaVelocity.setValue(0.0);
    sliderPortaVelocity.setTheme(theme.get());
    addAndMakeVisible(sliderPortaVelocity);
    
    labelFrequency.setTheme(theme.get());
    addAndMakeVisible(labelFrequency);
    
    sliderFrequency.setRange(0.0, 127.0, 1.0);
    sliderFrequency.setValue(100.0);
    sliderFrequency.setEnabled(false);
    sliderFrequency.setTheme(theme.get());
    addAndMakeVisible(sliderFrequency);
    
    labelTrackInput.setTheme(theme.get());
    addAndMakeVisible(labelTrackInput);
    
    auto trackInputChoices = juce::StringArray({
        "NONE", "ENV 1", "ENV 2", "ENV 3", "LFO 1", "LFO 2", "VIBRATO", "RAMP 1", "RAMP 2", "KEYBD",
        "PORTA", "TRACK", "KB GATE", "VELOCITY", "REL VEL", "PRESSURE",
        "PEDAL 1", "PEDAL 2", "LEVER 1", "LEVER 2", "LEVER 3"
    });
    
    comboBoxTrackInput.addItemList(trackInputChoices, 1);
    comboBoxTrackInput.setSelectedItemIndex(0);
    comboBoxTrackInput.setTheme(theme.get());
    addAndMakeVisible(comboBoxTrackInput);
    
    labelLegatoPorta.setTheme(theme.get());
    addAndMakeVisible(labelLegatoPorta);
    
    auto legatoPortaChoices = juce::StringArray({
        "OFF", "ON"
    });
    
    comboBoxLegatoPorta.addItemList(legatoPortaChoices, 1);
    comboBoxLegatoPorta.setSelectedItemIndex(0);
    comboBoxLegatoPorta.setEnabled(false);
    comboBoxLegatoPorta.setTheme(theme.get());
    addAndMakeVisible(comboBoxLegatoPorta);
    
    separator1.setTheme(theme.get());
    addAndMakeVisible(separator1);
    
    separator2.setTheme(theme.get());
    addAndMakeVisible(separator2);
    
    separator3.setTheme(theme.get());
    addAndMakeVisible(separator3);
    
    separator4.setTheme(theme.get());
    addAndMakeVisible(separator4);
    
    separator5.setTheme(theme.get());
    addAndMakeVisible(separator5);
    
    auto comboBoxChoices = juce::StringArray({
        "NONE", "DCO 1 FREQUENCY", "DCO 1 PULSE WIDTH", "DCO 1 WAVE SHAPE",
        "DCO 2 FREQUENCY", "DCO 2 PULSE WIDTH", "DCO 2 WAVE SHAPE",
        "DCO 1 / DCO 2 MIX", "VCF FM < DCO 1", "VCF FREQUENCY", "VCF RESONANCE",
        "VCA 1 VOLUME", "VCA 2 VOLUME",
        "ENV 1 DELAY", "ENV 1 ATTACK", "ENV 1 DECAY", "ENV 1 RELEASE", "ENV 1 AMPLITUDE",
        "ENV 2 DELAY", "ENV 2 ATTACK", "ENV 2 DECAY", "ENV 2 RELEASE", "ENV 2 AMPLITUDE",
        "ENV 3 DELAY", "ENV 3 ATTACK", "ENV 3 DECAY", "ENV 3 RELEASE", "ENV 3 AMPLITUDE",
        "LFO 1 SPEED", "LFO 1 AMPLITUDE", "LFO 2 SPEED", "LFO 2 AMPLITUDE",
        "PORTAMENTO RATE"
    });
    
    comboBoxTopLeft.addItemList(comboBoxChoices, 1);
    comboBoxTopLeft.setSelectedItemIndex(0);
    comboBoxTopLeft.setTheme(theme.get());
    addAndMakeVisible(comboBoxTopLeft);
    
    comboBoxTopRight.addItemList(comboBoxChoices, 1);
    comboBoxTopRight.setSelectedItemIndex(0);
    comboBoxTopRight.setTheme(theme.get());
    addAndMakeVisible(comboBoxTopRight);
    
    comboBoxBottomLeft.addItemList(comboBoxChoices, 1);
    comboBoxBottomLeft.setSelectedItemIndex(0);
    comboBoxBottomLeft.setTheme(theme.get());
    addAndMakeVisible(comboBoxBottomLeft);
    
    comboBoxBottomRight.addItemList(comboBoxChoices, 1);
    comboBoxBottomRight.setSelectedItemIndex(0);
    comboBoxBottomRight.setTheme(theme.get());
    addAndMakeVisible(comboBoxBottomRight);
    
    setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(true, true);
    
    setSize(500, 400);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(theme->getGuiBackgroundColour());
}

void PluginEditor::resized()
{
    static constexpr int kMargin = 5;
    
    auto comboBoxWidth = comboBoxTopLeft.getWidth();
    auto comboBoxHeight = comboBoxTopLeft.getHeight();
    
    comboBoxTopLeft.setBounds(kMargin, kMargin, comboBoxWidth, comboBoxHeight);
    comboBoxTopRight.setBounds(getWidth() - comboBoxWidth - kMargin, kMargin, comboBoxWidth, comboBoxHeight);
    comboBoxBottomLeft.setBounds(kMargin, getHeight() - comboBoxHeight - kMargin, comboBoxWidth, comboBoxHeight);
    comboBoxBottomRight.setBounds(getWidth() - comboBoxWidth - kMargin, getHeight() - comboBoxHeight - kMargin, comboBoxWidth, comboBoxHeight);
    
    auto buttonWidth = 70;
    auto buttonHeight = 20;
    auto spacing = 5;
    auto totalWidth = 4 * buttonWidth + 3 * spacing;

    auto startX = (getWidth() - totalWidth) / 2;
    auto centerY = getHeight() / 2 - 100;
    
    buttonBlack.setBounds(startX, centerY - buttonHeight / 2, buttonWidth, buttonHeight);
    buttonCream.setBounds(startX + buttonWidth + spacing, centerY - buttonHeight / 2, buttonWidth, buttonHeight);
    buttonDebug.setBounds(startX + 2 * (buttonWidth + spacing), centerY - buttonHeight / 2, buttonWidth, buttonHeight);
    buttonDisabled.setBounds(startX + 3 * (buttonWidth + spacing), centerY - buttonHeight / 2, buttonWidth, buttonHeight);
    
    auto sliderX = startX + labelPortamentoRate.getWidth();
    auto currentY = centerY + buttonHeight / 2 + 10;
    
    labelPortamentoRate.setBounds(startX, currentY, labelPortamentoRate.getWidth(), labelPortamentoRate.getHeight());
    sliderPortamentoRate.setBounds(sliderX, currentY, sliderPortamentoRate.getWidth(), sliderPortamentoRate.getHeight());
    currentY += labelPortamentoRate.getHeight();
    separator1.setBounds(startX, currentY, separator1.getWidth(), separator1.getHeight());
    currentY += separator1.getHeight();
    
    labelPortaVelocity.setBounds(startX, currentY, labelPortaVelocity.getWidth(), labelPortaVelocity.getHeight());
    sliderPortaVelocity.setBounds(sliderX, currentY, sliderPortaVelocity.getWidth(), sliderPortaVelocity.getHeight());
    currentY += labelPortaVelocity.getHeight();
    separator2.setBounds(startX, currentY, separator2.getWidth(), separator2.getHeight());
    currentY += separator2.getHeight();
    
    labelFrequency.setBounds(startX, currentY, labelFrequency.getWidth(), labelFrequency.getHeight());
    sliderFrequency.setBounds(sliderX, currentY, sliderFrequency.getWidth(), sliderFrequency.getHeight());
    currentY += labelFrequency.getHeight();
    separator3.setBounds(startX, currentY, separator3.getWidth(), separator3.getHeight());
    currentY += separator3.getHeight();
    
    labelTrackInput.setBounds(startX, currentY, labelTrackInput.getWidth(), labelTrackInput.getHeight());
    comboBoxTrackInput.setBounds(sliderX, currentY, comboBoxTrackInput.getWidth(), comboBoxTrackInput.getHeight());
    currentY += labelTrackInput.getHeight();
    separator4.setBounds(startX, currentY, separator4.getWidth(), separator4.getHeight());
    currentY += separator4.getHeight();
    
    labelLegatoPorta.setBounds(startX, currentY, labelLegatoPorta.getWidth(), labelLegatoPorta.getHeight());
    comboBoxLegatoPorta.setBounds(sliderX, currentY, comboBoxLegatoPorta.getWidth(), comboBoxLegatoPorta.getHeight());
    currentY += labelLegatoPorta.getHeight();
    separator5.setBounds(startX, currentY, separator5.getWidth(), separator5.getHeight());
}

void PluginEditor::mouseDown(const juce::MouseEvent&)
{
    unfocusAllComponents();
}

void PluginEditor::updateTheme()
{
    buttonBlack.repaint();
    buttonCream.repaint();
    buttonDebug.repaint();
    buttonDisabled.repaint();
    labelPortamentoRate.repaint();
    sliderPortamentoRate.repaint();
    labelPortaVelocity.repaint();
    sliderPortaVelocity.repaint();
    labelFrequency.repaint();
    sliderFrequency.repaint();
    labelTrackInput.repaint();
    comboBoxTrackInput.repaint();
    labelLegatoPorta.repaint();
    comboBoxLegatoPorta.repaint();
    separator1.repaint();
    separator2.repaint();
    separator3.repaint();
    separator4.repaint();
    separator5.repaint();
    comboBoxTopLeft.repaint();
    comboBoxTopRight.repaint();
    comboBoxBottomLeft.repaint();
    comboBoxBottomRight.repaint();
    repaint();
}

