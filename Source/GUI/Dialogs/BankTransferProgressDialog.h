#pragma once

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

namespace TSS
{
    class ISkin;
}

// Blocking modal overlay for Bank Utility EXPORT/IMPORT.
// Export uses a destination-folder + single progress lane layout.
// Import always shows both lanes at fixed height: primary (safety snapshot) active first,
// secondary (write/restore) grayed until beginSecondaryPhase activates it.
class BankTransferProgressDialog : public juce::Component
{
public:
    enum class ContentLayout
    {
        Export,
        Import
    };

    static constexpr int kDesignWidth = 420;
    static constexpr int kDesignHeightSingle = 160;
    // Dual = single + section gap (1em) + phase lane (label + 0.5em + bar), keeping the same
    // leftover gap above Cancel as the export modal.
    static constexpr int kDesignHeightDual = 212;

    explicit BankTransferProgressDialog(TSS::ISkin& skin);
    ~BankTransferProgressDialog() override;

    // detail is typically an absolute folder path (may be empty).
    void prepareForShow(const juce::String& title,
                        const juce::String& message,
                        const juce::String& detail,
                        int totalSteps,
                        std::function<void()> onCancelRequested,
                        ContentLayout layout = ContentLayout::Import);

    // Activates the secondary lane (import write / restore) and starts its progress at 0.
    void beginSecondaryPhase(const juce::String& message, int totalSteps);

    // Updates the active lane (primary until secondary is active, then secondary).
    void setProgress(int completedSteps);
    void setMessage(const juce::String& message);
    void setDetail(const juce::String& detail);
    void setCancelEnabled(bool enabled);

    void setSkin(TSS::ISkin& skin);
    void setUiScale(float uiScale);

    void paint(juce::Graphics& g) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress& key) override;

private:
    int getBorderThickness() const;
    int getDesignContentHeight() const noexcept;
    juce::Rectangle<int> getDialogBounds() const;
    void paintProgressBar(juce::Graphics& g,
                          juce::Rectangle<int> bounds,
                          float fraction,
                          bool enabled) const;
    // Shared folder block: label, path, then 1em before the next section.
    // Caller supplies a body that already starts 1em below the title (no extra top gap here).
    juce::Rectangle<int> paintFolderHeader(juce::Graphics& g,
                                           juce::Rectangle<int> body,
                                           const juce::Font& bodyFont,
                                           const juce::String& folderLabel) const;
    void paintPhaseLane(juce::Graphics& g,
                        juce::Rectangle<int>& body,
                        const juce::Font& bodyFont,
                        const juce::String& message,
                        int completedSteps,
                        int totalSteps,
                        bool enabled) const;
    void paintExportBody(juce::Graphics& g, juce::Rectangle<int> body, const juce::Font& bodyFont) const;
    void paintImportBody(juce::Graphics& g, juce::Rectangle<int> body, const juce::Font& bodyFont) const;

    std::function<void()> onCancelRequested_;
    TSS::ISkin* skin_;
    ContentLayout contentLayout_ = ContentLayout::Import;
    juce::String title_;
    juce::String detail_;

    juce::String primaryMessage_;
    int primaryTotalSteps_ = 1;
    int primaryCompletedSteps_ = 0;

    juce::String secondaryMessage_;
    int secondaryTotalSteps_ = 1;
    int secondaryCompletedSteps_ = 0;
    bool secondaryLaneActive_ = false;

    float uiScale_ = 1.0f;
    juce::TextButton cancelButton_;

    inline constexpr static int kTitleBarHeight_ = 28;
    inline constexpr static int kBorderThickness_ = 4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BankTransferProgressDialog)
};
