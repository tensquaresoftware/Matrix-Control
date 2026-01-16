#include "FooterPanel.h"

#include "../../Themes/Theme.h"

using tss::Theme;

const juce::Identifier FooterPanel::kMessageTextId("uiMessageText");
const juce::Identifier FooterPanel::kMessageSeverityId("uiMessageSeverity");

FooterPanel::FooterPanel(Theme& inTheme, juce::AudioProcessorValueTreeState& apvtsRef)
    : theme(&inTheme)
    , apvts(apvtsRef)
{
    // Écouter les changements de l'APVTS
    apvts.state.addListener(this);
    
    // Initialiser avec le message actuel s'il existe
    const auto messageTextVar = apvts.state.getProperty(kMessageTextId, juce::String());
    const auto severityStrVar = apvts.state.getProperty(kMessageSeverityId, juce::String());
    
    const juce::String messageText = messageTextVar.toString();
    const juce::String severityStr = severityStrVar.toString();
    
    if (messageText.isNotEmpty())
    {
        currentMessage = messageText;
        currentSeverity = parseSeverity(severityStr);
    }
}

FooterPanel::~FooterPanel()
{
    apvts.state.removeListener(this);
}

void FooterPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme->getFooterPanelBackgroundColour());
    
    if (currentMessage.isEmpty() || currentSeverity == MessageSeverity::None)
        return;
    
    auto bounds = getLocalBounds().reduced(kPadding_);
    
    // Couleur du texte selon la sévérité
    g.setColour(getSeverityColour(currentSeverity));
    g.setFont(theme->getBaseFont());
    
    // Icône (optionnel)
    const juce::String icon = getSeverityIcon(currentSeverity);
    if (icon.isNotEmpty())
    {
        const auto iconBounds = bounds.removeFromLeft(kIconSize_ + kPadding_);
        g.drawText(icon, iconBounds, juce::Justification::centredLeft);
    }
    
    // Message
    g.drawText(currentMessage, bounds, juce::Justification::centredLeft);
}

void FooterPanel::resized()
{
}

void FooterPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;
    repaint();
}

void FooterPanel::valueTreePropertyChanged(juce::ValueTree& tree,
                                          const juce::Identifier& property)
{
    if (property == kMessageTextId || property == kMessageSeverityId)
    {
        const auto messageText = tree.getProperty(kMessageTextId, juce::String()).toString();
        const auto severityStr = tree.getProperty(kMessageSeverityId, juce::String()).toString();
        
        currentMessage = messageText;
        currentSeverity = parseSeverity(severityStr);
        
        repaint();
    }
}

FooterPanel::MessageSeverity FooterPanel::parseSeverity(const juce::String& severityStr) const
{
    if (severityStr == "info")
        return MessageSeverity::Info;
    if (severityStr == "success")
        return MessageSeverity::Success;
    if (severityStr == "warning")
        return MessageSeverity::Warning;
    if (severityStr == "error")
        return MessageSeverity::Error;
    
    return MessageSeverity::None;
}

juce::Colour FooterPanel::getSeverityColour(MessageSeverity severity) const
{
    switch (severity)
    {
        case MessageSeverity::None:
            return theme->getLabelTextColour();
        case MessageSeverity::Info:
            return juce::Colour(0xFF808080);  // Gris
        case MessageSeverity::Success:
            return juce::Colour(0xFF00FF00);  // Vert
        case MessageSeverity::Warning:
            return juce::Colour(0xFFFF8800);  // Orange
        case MessageSeverity::Error:
            return juce::Colour(0xFFFF0000);  // Rouge
        default:
            return theme->getLabelTextColour();
    }
}

juce::String FooterPanel::getSeverityIcon(MessageSeverity severity) const
{
    switch (severity)
    {
        case MessageSeverity::None:
            return juce::String();
        case MessageSeverity::Info:
            return "ℹ";
        case MessageSeverity::Success:
            return "✓";
        case MessageSeverity::Warning:
            return "⚠";
        case MessageSeverity::Error:
            return "✗";
        default:
            return juce::String();
    }
}

