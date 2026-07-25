#pragma once

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

#include "GUI/Looks/WidgetLooks.h"

namespace TSS
{
    // Paint-only Patch Name display + inline rename editor. Deliberately NOT a JUCE
    // TextEditor: the rename UX is a fixed 8-slot, uppercase, custom-caret editor closer
    // to a hardware LCD name editor (see the Patch Name inline edit spec).
    class PatchNameDisplay : public juce::Component,
                             private juce::Timer
    {
    public:
        explicit PatchNameDisplay(int width, int height, const PatchNameDisplayLook& look);
        ~PatchNameDisplay() override;

        void setLook(const PatchNameDisplayLook& look);
        void setUiScale(float uiScale);

        // Primary line — the user patch name (8 chars max, display form).
        void setPatchName(const juce::String& patchName);

        // Secondary line — Mxx / Mxx-Ryy / INITIAL. Empty = single centred name (no history/Compare).
        void setSecondaryLabel(const juce::String& secondaryLabel);

        // Gates the double-click affordance (ROM banks / Compare / etc. are not editable).
        void setEditable(bool editable);
        bool isEditable() const noexcept { return editable_; }

        void beginEdit();
        // Abandon: restores the previous name display, no commit callback fires.
        void cancelEdit();
        bool isEditing() const noexcept { return editing_; }

        void onCommit(std::function<void(juce::String)> callback);
        void onIllegalCharacter(std::function<void()> callback);
        // Fires when a pending illegal-character footer should clear mid-edit
        // (valid key, arrow move, backspace/delete) — not an edit-session end.
        void onIllegalCharacterCleared(std::function<void()> callback);
        // Fires once per edit session end (commit or cancel) — panel uses this to clear
        // any footer error message it may have shown for an illegal character.
        void onEditEnded(std::function<void()> callback);

        void paint(juce::Graphics& g) override;
        void mouseDoubleClick(const juce::MouseEvent& e) override;
        bool keyPressed(const juce::KeyPress& key) override;
        void focusLost(juce::Component::FocusChangeType cause) override;

        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        inline constexpr static int kBorderThickness_ = 2;
        inline constexpr static int kNameLength_ = 8;
        inline constexpr static int kCaretBlinkIntervalMs_ = 500;
        inline constexpr static float kPrimaryRowHeightRatio_ = 0.62f; // dual-line split of the fixed 72px box

        PatchNameDisplayLook look_{};
        int width_;
        int height_;
        juce::String patchName_;
        juce::String secondaryLabel_;
        float uiScale_ = 1.0f;

        bool editable_ = false;
        bool editing_ = false;
        bool illegalCharPending_ = false;
        juce::String editBuffer_;
        int caretIndex_ = 0;
        bool caretOn_ = true;

        std::function<void(juce::String)> onCommit_;
        std::function<void()> onIllegalCharacter_;
        std::function<void()> onIllegalCharacterCleared_;
        std::function<void()> onEditEnded_;

        void timerCallback() override;

        juce::String buildEditBufferFromCurrentName() const;
        void commitEdit();
        void insertCharacterAtCaret(juce::juce_wchar character);
        void moveCaret(int delta);
        void restartCaretBlink();
        void clearIllegalCharacterPending();

        void drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawSingleLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawDualLine(juce::Graphics& g, const juce::Rectangle<float>& bounds);
        void drawNameSlots(juce::Graphics& g, const juce::Rectangle<float>& rowBounds);
        void drawSecondaryText(juce::Graphics& g, const juce::Rectangle<float>& rowBounds);

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchNameDisplay)
    };
}
