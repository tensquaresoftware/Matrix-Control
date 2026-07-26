#include "FooterPanel.h"

#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "Shared/Definitions/MatrixDeviceTypes.h"
#include "Shared/Definitions/PluginDisplayNames.h"

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

void FooterPanel::paintBadgeAndDetail(juce::Graphics& g,
                                      juce::Rectangle<int> bounds,
                                      const juce::String& badgeLabel,
                                      const juce::String& detailText,
                                      juce::Colour badgeFill,
                                      juce::Colour detailColour,
                                      const juce::Font& font) const
{
    const int badgeHeight = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.severityBadgeHeight), uiScale_);
    const int badgePad = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.severityBadgeHorizontalPadding), uiScale_);
    const int badgeGap = TSS::ScaledLayout::scaledInt(
        static_cast<float>(dimensions_.severityBadgeToMessageGap), uiScale_);
    const auto badgeFont = skin_->getBaseFontBold().withHeight(font.getHeight());

    g.setFont(badgeFont);

    const int labelWidth = juce::roundToInt(juce::GlyphArrangement::getStringWidth(badgeFont, badgeLabel));
    const int badgeWidth = juce::jmin(bounds.getWidth(), labelWidth + 2 * badgePad);
    const int badgeY = bounds.getCentreY() - badgeHeight / 2;
    const juce::Rectangle<int> badgeBounds {
        bounds.getX(),
        badgeY,
        badgeWidth,
        badgeHeight
    };

    g.setColour(badgeFill);
    g.fillRect(badgeBounds);

    g.setColour(skin_->getColour(SkinColourId::kFooterPanelBackground));
    g.drawText(badgeLabel, badgeBounds, juce::Justification::centred, false);

    bounds.removeFromLeft(badgeWidth + badgeGap);
    g.setFont(font);
    g.setColour(detailColour);
    g.drawFittedText(detailText,
                     bounds,
                     juce::Justification::centredLeft,
                     1,
                     1.0f);
}

void FooterPanel::paint(juce::Graphics& g)
{
    g.fillAll(skin_->getColour(SkinColourId::kFooterPanelBackground));

    const int padding = juce::jmax(1, juce::roundToInt(static_cast<float>(dimensions_.padding) * uiScale_));
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

    g.setColour(skin_->getColour(SkinColourId::kBodyPanelBackground).withAlpha(0.0f));
    g.fillRect(leftBand);
    g.fillRect(centreBand);
    g.fillRect(rightBand);

    const auto font = skin_->getBaseFont().withHeight(skin_->getBaseFont().getHeight() * uiScale_);
    const auto chromeGrey = skin_->getColour(SkinColourId::kFooterMessageInfo);

    if (! currentMessage.isEmpty() && currentSeverity != MessageSeverity::None)
    {
        paintBadgeAndDetail(g,
                            leftBand.reduced(padding, 0),
                            getSeverityPrefix(currentSeverity),
                            currentMessage,
                            getSeverityColour(currentSeverity),
                            chromeGrey,
                            font);
    }

    {
        const auto type = MatrixDeviceTypes::fromApvtsString(deviceType_);
        const bool deviceOk = deviceDetected_
            && MatrixDeviceTypes::isSupportedMatrixDevice(type);
        const auto badgeFill = deviceOk
            ? skin_->getColour(SkinColourId::kFooterMessageInfo)
            : skin_->getColour(SkinColourId::kFooterMessageError).withAlpha(0.8f);

        paintBadgeAndDetail(g,
                            rightBand.reduced(padding, 0),
                            PluginDisplayNames::FooterPanel::kDeviceLabel,
                            buildDeviceDetailText(),
                            badgeFill,
                            chromeGrey,
                            font);
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
            return skin_->getColour(SkinColourId::kFooterMessageError).withAlpha(0.8f);
        default:
            return skin_->getColour(SkinColourId::kDarkPanelText);
    }
}

juce::String FooterPanel::getSeverityPrefix(MessageSeverity severity) const
{
    switch (severity)
    {
        case MessageSeverity::None:
            return {};
        case MessageSeverity::Info:
            return PluginDisplayNames::FooterPanel::kSeverityInfoPrefix;
        case MessageSeverity::Success:
            return PluginDisplayNames::FooterPanel::kSeveritySuccessPrefix;
        case MessageSeverity::Warning:
            return PluginDisplayNames::FooterPanel::kSeverityWarningPrefix;
        case MessageSeverity::Error:
            return PluginDisplayNames::FooterPanel::kSeverityErrorPrefix;
        default:
            return {};
    }
}

juce::String FooterPanel::buildDeviceDetailText() const
{
    if (! deviceDetected_)
        return PluginDisplayNames::FooterPanel::kDeviceNotConnectedDetail;

    const auto type = MatrixDeviceTypes::fromApvtsString(deviceType_);
    if (! MatrixDeviceTypes::isSupportedMatrixDevice(type))
        return PluginDisplayNames::FooterPanel::kDeviceUnknownDetail;

    juce::String detail = MatrixDeviceTypes::toDisplayString(type).toUpperCase();
    if (deviceVersion_.isNotEmpty())
        detail += " (V" + deviceVersion_.toUpperCase() + ")";

    return detail;
}
