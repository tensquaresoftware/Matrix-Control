#include "BankTransferProgressDialog.h"

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
        if (onCancelRequested_)
            onCancelRequested_();
    };
    cancelButton_.setMouseClickGrabsKeyboardFocus(false);
    addAndMakeVisible(cancelButton_);
}

BankTransferProgressDialog::~BankTransferProgressDialog() = default;

void BankTransferProgressDialog::prepareForShow(const juce::String& title,
                                                const juce::String& message,
                                                int totalSteps,
                                                std::function<void()> onCancelRequested)
{
    title_ = title;
    message_ = message;
    totalSteps_ = juce::jmax(1, totalSteps);
    completedSteps_ = 0;
    onCancelRequested_ = std::move(onCancelRequested);
    setCancelEnabled(static_cast<bool>(onCancelRequested_));
    repaint();
}

void BankTransferProgressDialog::setProgress(int completedSteps)
{
    completedSteps_ = juce::jlimit(0, totalSteps_, completedSteps);
    repaint();
}

void BankTransferProgressDialog::setMessage(const juce::String& message)
{
    message_ = message;
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

juce::Rectangle<int> BankTransferProgressDialog::getDialogBounds() const
{
    const int border = getBorderThickness();
    const int dialogWidth = juce::roundToInt(static_cast<float>(kDesignWidth) * uiScale_) + border * 2;
    const int dialogHeight = juce::roundToInt(static_cast<float>(kDesignHeight) * uiScale_)
                             + juce::roundToInt(static_cast<float>(kTitleBarHeight_) * uiScale_)
                             + border * 2;

    return getLocalBounds().withSizeKeepingCentre(dialogWidth, dialogHeight);
}

float BankTransferProgressDialog::getProgressFraction() const noexcept
{
    return juce::jlimit(0.0f, 1.0f, static_cast<float>(completedSteps_) / static_cast<float>(totalSteps_));
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

    const int padding = juce::roundToInt(12.0f * uiScale_);
    auto textArea = content.reduced(padding);
    textArea.removeFromBottom(juce::roundToInt(60.0f * uiScale_));

    g.setFont(skin_->getBaseFont().withHeight(skin_->getBaseFont().getHeight() * uiScale_));
    g.drawFittedText(message_, textArea, juce::Justification::topLeft, 2);

    auto barArea = content.reduced(padding);
    barArea.removeFromBottom(juce::roundToInt(36.0f * uiScale_));
    auto progressBar = barArea.removeFromBottom(juce::roundToInt(16.0f * uiScale_));

    g.setColour(skin_->getColour(SkinColourId::kBodyPanelBackground));
    g.fillRect(progressBar);

    auto fillBar = progressBar.withWidth(juce::roundToInt(
        static_cast<float>(progressBar.getWidth()) * getProgressFraction()));
    g.setColour(skin_->getColour(SkinColourId::kModuleHeaderLineBlue));
    g.fillRect(fillBar);

    g.setColour(skin_->getColour(SkinColourId::kDarkPanelText));
    g.drawRect(progressBar, 1);

    const int percent = juce::roundToInt(getProgressFraction() * 100.0f);
    g.drawText(juce::String(percent) + juce::String(PluginDisplayNames::Units::kPercent),
              progressBar,
              juce::Justification::centred,
              false);
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
