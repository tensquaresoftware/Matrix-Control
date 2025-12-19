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
    
    setSize(500, 400);
}

PluginEditor::~PluginEditor() = default;

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(theme->getGuiBackgroundColour());
}

void PluginEditor::resized()
{
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
    
    auto labelY = centerY + buttonHeight / 2 + 10;
    labelPortamentoRate.setBounds(startX, labelY, labelPortamentoRate.getWidth(), labelPortamentoRate.getHeight());
}

void PluginEditor::updateTheme()
{
    buttonBlack.repaint();
    buttonCream.repaint();
    buttonDebug.repaint();
    buttonDisabled.repaint();
    labelPortamentoRate.repaint();
    repaint();
}

