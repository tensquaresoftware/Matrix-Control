#include "BankTransferProgressDialog.h"

#include "GUI/Helpers/TextFitHelpers.h"
#include "GUI/Skins/Skin.h"
#include "Shared/Definitions/PluginDisplayNames.h"

using TSS::SkinColourId;

namespace
{
    constexpr juce::uint32 kDialogBorderColour = 0xff5E5E5E;
}

BankTransferProgressDialog::BankTransferProgressDialog(TSS::ISkin& skin)
    : skin_(&skin)
    , cancelButton_(PluginDisplayNames::Dialogs::BankTransferProgress::kCancel)
{
    setOpaque(false);
    setInterceptsMouseClicks(true, true);
    setWantsKeyboardFocus(true);

    cancelButton_.onClick = [this]
    {
        if (onCancelRequested_ && cancelButton_.isEnabled())
            onCancelRequested_();
    };
    cancelButton_.setMouseClickGrabsKeyboardFocus(false);
    addAndMakeVisible(cancelButton_);
}

BankTransferProgressDialog::~BankTransferProgressDialog() = default;

void BankTransferProgressDialog::prepareForShow(PrepareForShowArgs args)
{
    using namespace PluginDisplayNames::Dialogs::BankTransferProgress;
    using namespace PluginDisplayNames::PatchManagerSection::BankUtilityModule;

    title_ = std::move(args.title);
    detail_ = std::move(args.detail);
    primaryMessage_ = std::move(args.message);
    primaryTotalSteps_ = juce::jmax(1, args.totalSteps);
    primaryCompletedSteps_ = 0;
    secondaryLaneActive_ = false;
    secondaryCompletedSteps_ = 0;

    // Prefer explicit layout; fall back to title so existing call sites keep working.
    contentLayout_ = args.layout;
    if (args.layout == ContentLayout::Import
        && title_ == juce::String(kExportTitle))
        contentLayout_ = ContentLayout::Export;

    if (contentLayout_ == ContentLayout::Import)
    {
        // Fixed dual-lane height from the start; secondary stays grayed until activated.
        secondaryMessage_ = juce::String(kImportingWritingMessage);
        secondaryTotalSteps_ = primaryTotalSteps_;
    }
    else
    {
        secondaryMessage_.clear();
        secondaryTotalSteps_ = 1;
    }

    onCancelRequested_ = std::move(args.onCancelRequested);
    setCancelEnabled(static_cast<bool>(onCancelRequested_));
    resized();
    repaint();
}

void BankTransferProgressDialog::beginSecondaryPhase(const juce::String& message, int totalSteps)
{
    primaryCompletedSteps_ = primaryTotalSteps_;
    secondaryLaneActive_ = true;
    secondaryMessage_ = message;
    secondaryTotalSteps_ = juce::jmax(1, totalSteps);
    secondaryCompletedSteps_ = 0;
    resized();
    repaint();
}

void BankTransferProgressDialog::setProgress(int completedSteps)
{
    if (secondaryLaneActive_)
        secondaryCompletedSteps_ = juce::jlimit(0, secondaryTotalSteps_, completedSteps);
    else
        primaryCompletedSteps_ = juce::jlimit(0, primaryTotalSteps_, completedSteps);

    repaint();
}

void BankTransferProgressDialog::setMessage(const juce::String& message)
{
    if (secondaryLaneActive_)
        secondaryMessage_ = message;
    else
        primaryMessage_ = message;

    repaint();
}

void BankTransferProgressDialog::setDetail(const juce::String& detail)
{
    detail_ = detail;
    repaint();
}

void BankTransferProgressDialog::setCancelEnabled(bool enabled)
{
    cancelButton_.setEnabled(enabled);
}

void BankTransferProgressDialog::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    repaint();
}

void BankTransferProgressDialog::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;
    resized();
    repaint();
}

int BankTransferProgressDialog::getBorderThickness() const
{
    return juce::roundToInt(static_cast<float>(kBorderThickness_) * uiScale_);
}

int BankTransferProgressDialog::getDesignContentHeight() const noexcept
{
    return contentLayout_ == ContentLayout::Import ? kDesignHeightDual : kDesignHeightSingle;
}

juce::Rectangle<int> BankTransferProgressDialog::getDialogBounds() const
{
    const int border = getBorderThickness();
    const int dialogWidth = juce::roundToInt(static_cast<float>(kDesignWidth) * uiScale_) + border * 2;
    const int dialogHeight = juce::roundToInt(static_cast<float>(getDesignContentHeight()) * uiScale_)
                             + juce::roundToInt(static_cast<float>(kTitleBarHeight_) * uiScale_)
                             + border * 2;

    return getLocalBounds().withSizeKeepingCentre(dialogWidth, dialogHeight);
}

void BankTransferProgressDialog::paintProgressBar(juce::Graphics& g,
                                                    juce::Rectangle<int> bounds,
                                                    float fraction,
                                                    bool enabled) const
{
    g.setColour(skin_->getColour(enabled ? SkinColourId::kSliderTrackEnabled
                                         : SkinColourId::kSliderTrackDisabled));
    g.fillRect(bounds);

    if (enabled)
    {
        auto fillBar = bounds.withWidth(
            juce::roundToInt(static_cast<float>(bounds.getWidth()) * fraction));
        g.setColour(skin_->getColour(SkinColourId::kSliderValueBarEnabled));
        g.fillRect(fillBar);
    }

    const int percent = enabled ? juce::roundToInt(fraction * 100.0f) : 0;
    g.setColour(skin_->getColour(enabled ? SkinColourId::kSliderTextEnabled
                                         : SkinColourId::kSliderTextDisabled));
    g.drawText(juce::String(percent) + juce::String(PluginDisplayNames::Units::kPercent),
               bounds,
               juce::Justification::centred,
               false);
}

juce::Rectangle<int> BankTransferProgressDialog::paintFolderHeader(juce::Graphics& g,
                                                                   juce::Rectangle<int> body,
                                                                   const juce::Font& bodyFont,
                                                                   const juce::String& folderLabel) const
{
    const float em = bodyFont.getHeight();
    const int gap1em = juce::roundToInt(em);
    const int lineHeight = juce::jmax(1, juce::roundToInt(em));

    g.setFont(bodyFont);
    g.setColour(skin_->getColour(SkinColourId::kDarkPanelText));

    {
        auto labelLine = body.removeFromTop(lineHeight);
        g.drawText(folderLabel, labelLine, juce::Justification::centredLeft, false);
    }

    if (detail_.isNotEmpty())
    {
        auto pathLine = body.removeFromTop(lineHeight);
        const auto fittedPath = TSS::TextFitHelpers::fitWithAsciiEllipsis(
            detail_, bodyFont, static_cast<float>(pathLine.getWidth()), true);
        g.drawText(fittedPath, pathLine, juce::Justification::centredLeft, false);
    }

    body.removeFromTop(gap1em);
    return body;
}

void BankTransferProgressDialog::paintPhaseLane(juce::Graphics& g, const PhaseLanePaintArgs& args) const
{
    const float em = args.bodyFont.getHeight();
    const int gapHalfEm = juce::roundToInt(em * 0.5f);
    const int lineHeight = juce::jmax(1, juce::roundToInt(em));
    const int barHeight = juce::roundToInt(16.0f * uiScale_);

    g.setFont(args.bodyFont);
    g.setColour(args.enabled ? skin_->getColour(SkinColourId::kDarkPanelText)
                             : skin_->getColour(SkinColourId::kSliderTextDisabled));

    {
        auto progressLabel = args.body.removeFromTop(lineHeight);
        g.drawText(args.message, progressLabel, juce::Justification::centredLeft, false);
    }

    args.body.removeFromTop(gapHalfEm);

    auto progressBar = args.body.removeFromTop(barHeight);
    const float fraction = juce::jlimit(
        0.0f,
        1.0f,
        static_cast<float>(args.completedSteps)
            / static_cast<float>(juce::jmax(1, args.totalSteps)));
    paintProgressBar(g, progressBar, fraction, args.enabled);
}

void BankTransferProgressDialog::paintExportBody(juce::Graphics& g,
                                                   juce::Rectangle<int> body,
                                                   const juce::Font& bodyFont) const
{
    using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

    body = paintFolderHeader(g, body, bodyFont, juce::String(kDestinationFolderLabel));
    paintPhaseLane(g,
                   PhaseLanePaintArgs {
                       body,
                       bodyFont,
                       primaryMessage_,
                       primaryCompletedSteps_,
                       primaryTotalSteps_,
                       true });
}

void BankTransferProgressDialog::paintImportBody(juce::Graphics& g,
                                                   juce::Rectangle<int> body,
                                                   const juce::Font& bodyFont) const
{
    using namespace PluginDisplayNames::Dialogs::BankTransferProgress;

    body = paintFolderHeader(g, body, bodyFont, juce::String(kSourceFolderLabel));
    paintPhaseLane(g,
                   PhaseLanePaintArgs {
                       body,
                       bodyFont,
                       primaryMessage_,
                       primaryCompletedSteps_,
                       primaryTotalSteps_,
                       true });

    body.removeFromTop(juce::roundToInt(bodyFont.getHeight()));
    paintPhaseLane(g,
                   PhaseLanePaintArgs {
                       body,
                       bodyFont,
                       secondaryMessage_,
                       secondaryCompletedSteps_,
                       secondaryTotalSteps_,
                       secondaryLaneActive_ });
}

void BankTransferProgressDialog::paint(juce::Graphics& g)
{
    g.fillAll(skin_->getColour(SkinColourId::kBodyPanelBackground).withAlpha(0.85f));

    const auto dialogBounds = getDialogBounds();
    const int border = getBorderThickness();

    g.setColour(juce::Colour(kDialogBorderColour));
    g.fillRect(dialogBounds);

    auto inner = dialogBounds.reduced(border);
    const int titleBarHeight = juce::roundToInt(static_cast<float>(kTitleBarHeight_) * uiScale_);
    auto titleBar = inner.removeFromTop(titleBarHeight);
    auto content = inner;

    g.setColour(skin_->getColour(SkinColourId::kHeaderPanelBackground));
    g.fillRect(titleBar);
    g.fillRect(content);

    g.setColour(skin_->getColour(SkinColourId::kDarkPanelText));
    g.setFont(skin_->getBaseFontBold().withHeight(skin_->getBaseFontBold().getHeight() * uiScale_));
    g.drawText(title_, titleBar, juce::Justification::centred, false);

    // Custom modal scheme: exactly 1em under the title, then content (no extra top padding).
    const auto bodyFont = skin_->getBaseFont().withHeight(skin_->getBaseFont().getHeight() * uiScale_);
    const int gapUnderTitle = juce::roundToInt(bodyFont.getHeight());
    const int padX = juce::roundToInt(12.0f * uiScale_);
    const int bottomReserve = juce::roundToInt(40.0f * uiScale_);

    auto body = content;
    body.removeFromTop(gapUnderTitle);
    body = body.withTrimmedLeft(padX).withTrimmedRight(padX);
    body.removeFromBottom(bottomReserve);

    if (contentLayout_ == ContentLayout::Export)
        paintExportBody(g, body, bodyFont);
    else
        paintImportBody(g, body, bodyFont);
}

void BankTransferProgressDialog::resized()
{
    auto inner = getDialogBounds().reduced(getBorderThickness());
    inner.removeFromTop(juce::roundToInt(static_cast<float>(kTitleBarHeight_) * uiScale_));

    const int padding = juce::roundToInt(12.0f * uiScale_);
    const int buttonHeight = juce::roundToInt(24.0f * uiScale_);
    const int buttonWidth = juce::roundToInt(72.0f * uiScale_);

    auto buttonRow = inner.reduced(padding).removeFromBottom(buttonHeight);
    cancelButton_.setBounds(buttonRow.removeFromRight(buttonWidth));
}

bool BankTransferProgressDialog::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::escapeKey && cancelButton_.isEnabled() && onCancelRequested_)
    {
        onCancelRequested_();
        return true;
    }

    return false;
}
