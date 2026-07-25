#include "FooterPanel.h"

#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"

using TSS::SkinColourId;

const juce::Identifier FooterPanel::kMessageTextId("uiMessageText");
const juce::Identifier FooterPanel::kMessageSeverityId("uiMessageSeverity");
const juce::Identifier FooterPanel::kDeviceDetectedId("deviceDetected");
const juce::Identifier FooterPanel::kDeviceTypeId(MatrixDeviceTypes::kApvtsPropertyName);
const juce::Identifier FooterPanel::kDeviceVersionId("deviceVersion");

FooterPanel::FooterPanel(TSS::ISkin& skin,
                         const FooterPanelDimensions& dimensions,
                         juce::AudioProcessorValueTreeState& apvtsRef)
    : dimensions_(dimensions)
    , skin_(&skin)
    , apvts(apvtsRef)
{
    setOpaque(true);
    apvts.state.addListener(this);
    syncFromApvtsState(apvts.state);
}

FooterPanel::~FooterPanel()
{
    apvts.state.removeListener(this);
}

void FooterPanel::paint(juce::Graphics& g)
{
    g.fillAll(skin_->getColour(SkinColourId::kFooterPanelBackground));

    const int padding = juce::jmax(1, juce::roundToInt(static_cast<float>(dimensions_.padding) * uiScale_));
    const int iconSize = juce::jmax(1, juce::roundToInt(static_cast<float>(dimensions_.iconSize) * uiScale_));
    const int bandHeight = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.bandHeight), uiScale_);
    const int bandVerticalInset = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.bandVerticalInset), uiScale_);
    const int patchEditW = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.patchEditPanelWidth), uiScale_);
    const int sharedW = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.sharedPanelWidth), uiScale_);
    const int masterEditW = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.masterEditPanelWidth), uiScale_);
    const int gap = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.interColumnGap), uiScale_);

    const auto area = getLocalBounds();
    const int bandY = area.getY() + bandVerticalInset;

    const juce::Rectangle<int> leftBand { area.getX(), bandY, patchEditW, bandHeight };
    const juce::Rectangle<int> centreBand { leftBand.getRight() + gap, bandY, sharedW, bandHeight };
    const juce::Rectangle<int> rightBand { centreBand.getRight() + gap, bandY, masterEditW, bandHeight };

    g.setColour(skin_->getColour(SkinColourId::kBodyPanelBackground));
    g.fillRect(leftBand);
    g.fillRect(centreBand);
    g.fillRect(rightBand);

    const auto font = skin_->getBaseFont().withHeight(skin_->getBaseFont().getHeight() * uiScale_);

    if (! currentMessage.isEmpty() && currentSeverity != MessageSeverity::None)
    {
        auto messageBounds = leftBand.reduced(padding, 0);
        g.setColour(getSeverityColour(currentSeverity));
        g.setFont(font);

        const juce::String icon = getSeverityIcon(currentSeverity);
        const int iconSlotWidth = iconSize + padding;
        if (icon.isNotEmpty() && messageBounds.getWidth() > iconSlotWidth)
        {
            const auto iconBounds = messageBounds.removeFromLeft(iconSlotWidth);
            g.drawText(icon, iconBounds, juce::Justification::centredLeft);
        }

        g.drawFittedText(currentMessage,
                         messageBounds,
                         juce::Justification::centredLeft,
                         1,
                         1.0f);
    }

    const auto identityText = buildDeviceIdentityText();
    if (identityText.isNotEmpty())
    {
        g.setColour(skin_->getColour(SkinColourId::kDarkPanelText));
        g.setFont(font);
        g.drawFittedText(identityText,
                         rightBand,
                         juce::Justification::centred,
                         1,
                         1.0f);
    }
}

void FooterPanel::resized()
{
}

void FooterPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
}

void FooterPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;
    repaint();
}

void FooterPanel::valueTreePropertyChanged(juce::ValueTree& tree,
                                          const juce::Identifier& property)
{
    if (property == kMessageTextId
        || property == kMessageSeverityId
        || property == kDeviceDetectedId
        || property == kDeviceTypeId
        || property == kDeviceVersionId)
    {
        syncFromApvtsState(tree);
        repaint();
    }
}

void FooterPanel::syncFromApvtsState(juce::ValueTree& tree)
{
    currentMessage = tree.getProperty(kMessageTextId, juce::String()).toString();
    currentSeverity = parseSeverity(tree.getProperty(kMessageSeverityId, juce::String()).toString());
    deviceDetected_ = static_cast<bool>(tree.getProperty(kDeviceDetectedId, false));
    deviceType_ = tree.getProperty(kDeviceTypeId, juce::String()).toString();
    deviceVersion_ = tree.getProperty(kDeviceVersionId, juce::String()).toString();
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
            return skin_->getColour(SkinColourId::kDarkPanelText);
        case MessageSeverity::Info:
            return skin_->getColour(SkinColourId::kFooterMessageInfo);
        case MessageSeverity::Success:
            return skin_->getColour(SkinColourId::kFooterMessageSuccess);
        case MessageSeverity::Warning:
            return skin_->getColour(SkinColourId::kFooterMessageWarning);
        case MessageSeverity::Error:
            return skin_->getColour(SkinColourId::kFooterMessageError);
        default:
            return skin_->getColour(SkinColourId::kDarkPanelText);
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

juce::String FooterPanel::buildDeviceIdentityText() const
{
    if (! deviceDetected_ || deviceType_.isEmpty())
        return {};

    juce::String identity = MatrixDeviceTypes::toDisplayString(
        MatrixDeviceTypes::fromApvtsString(deviceType_));
    if (deviceVersion_.isNotEmpty())
        identity += " · v" + deviceVersion_;

    return identity;
}
