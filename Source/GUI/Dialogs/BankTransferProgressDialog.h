#pragma once

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
    class ISkin;
}

// Blocking modal overlay for Bank Utility EXPORT/IMPORT: title, waiting-text message, a linear
// progress bar, and a Cancel button. Modeled on MasterInitConfirmDialog (dim overlay + boxed
// dialog) but with a determinate progress readout instead of static confirm text.
class BankTransferProgressDialog : public juce::Component
{
public:
    static constexpr int kDesignWidth = 420;
    static constexpr int kDesignHeight = 120;

    explicit BankTransferProgressDialog(TSS::ISkin& skin);
    ~BankTransferProgressDialog() override;

    // onCancelRequested is empty while cancellation is not offered (e.g. while restoring a
    // snapshot after Cancel) — the Cancel button is disabled in that case.
    void prepareForShow(const juce::String& title,
                        const juce::String& message,
                        int totalSteps,
                        std::function<void()> onCancelRequested);
    void setProgress(int completedSteps);
    void setMessage(const juce::String& message);
    // Disables Cancel (e.g. once a restore-on-cancel is in flight and can no longer be aborted).
    void setCancelEnabled(bool enabled);

    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    int getBorderThickness() const;
    juce::Rectangle<int> getDialogBounds() const;
    float getProgressFraction() const noexcept;

    std::function<void()> onCancelRequested_;
    TSS::ISkin* skin_;
    juce::String title_;
    juce::String message_;
    int totalSteps_ = 1;
    int completedSteps_ = 0;
    float uiScale_ = 1.0f;

    juce::TextButton cancelButton_;

    inline constexpr static int kTitleBarHeight_ = 28;
    inline constexpr static int kBorderThickness_ = 4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BankTransferProgressDialog)
};
