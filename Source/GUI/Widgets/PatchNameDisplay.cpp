#include "PatchNameDisplay.h"

#include "GUI/Layout/ScaledDrawing.h"
#include "GUI/Skins/ColourChart.h"
#include "Core/Services/PatchFileNameSanitizer.h"
#include "Core/Services/PatchNameEditRules.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace TSS
{
    PatchNameDisplay::PatchNameDisplay(int width, int height, const PatchNameDisplayLook& look)
        : look_(look)
        , width_(width)
        , height_(height)
        , patchName_(PluginDisplayNames::PatchEditSection::PatchNameModule::StandaloneWidgets::kDefaultPatchName)
    {
        setOpaque(false);
        setSize(width_, height_);
        setWantsKeyboardFocus(true);
    }

    PatchNameDisplay::~PatchNameDisplay() = default;

    void PatchNameDisplay::setLook(const PatchNameDisplayLook& look)
    {
        look_ = look;
        repaint();
    }

    void PatchNameDisplay::setUiScale(float uiScale)
    {
        if (juce::approximatelyEqual(uiScale_, uiScale))
            return;
        
        uiScale_ = uiScale;
        repaint();
    }

    void PatchNameDisplay::setPatchName(const juce::String& patchName)
    {
        patchName_ = patchName;
        repaint();
    }

    void PatchNameDisplay::setSecondaryLabel(const juce::String& secondaryLabel)
    {
        if (secondaryLabel_ == secondaryLabel)
            return;

        secondaryLabel_ = secondaryLabel;
        repaint();
    }

    void PatchNameDisplay::setEditable(bool editable)
    {
        if (editable_ == editable)
            return;

        editable_ = editable;

        if (! editable_ && editing_)
            cancelEdit();
    }

    void PatchNameDisplay::onCommit(std::function<void(juce::String)> callback)
    {
        onCommit_ = std::move(callback);
    }

    void PatchNameDisplay::onIllegalCharacter(std::function<void()> callback)
    {
        onIllegalCharacter_ = std::move(callback);
    }

    void PatchNameDisplay::onIllegalCharacterCleared(std::function<void()> callback)
    {
        onIllegalCharacterCleared_ = std::move(callback);
    }

    void PatchNameDisplay::onEditEnded(std::function<void()> callback)
    {
        onEditEnded_ = std::move(callback);
    }

    void PatchNameDisplay::beginEdit()
    {
        if (! editable_ || editing_)
            return;

        editBuffer_ = buildEditBufferFromCurrentName();
        caretIndex_ = 0;
        caretOn_ = true;
        illegalCharPending_ = false;
        editing_ = true;

        grabKeyboardFocus();
        startTimer(kCaretBlinkIntervalMs_);
        repaint();
    }

    void PatchNameDisplay::cancelEdit()
    {
        if (! editing_)
            return;

        stopTimer();
        editing_ = false;
        illegalCharPending_ = false;
        repaint();

        if (onEditEnded_)
            onEditEnded_();
    }

    void PatchNameDisplay::commitEdit()
    {
        if (! editing_)
            return;

        const auto resolvedName = Core::PatchNameEditRules::resolveCommittedPatchName(editBuffer_, patchName_);

        stopTimer();
        editing_ = false;
        illegalCharPending_ = false;
        repaint();

        if (onCommit_)
            onCommit_(resolvedName);

        if (onEditEnded_)
            onEditEnded_();
    }

    juce::String PatchNameDisplay::buildEditBufferFromCurrentName() const
    {
        auto buffer = patchName_.toUpperCase().substring(0, kNameLength_);

        while (buffer.length() < kNameLength_)
            buffer += ' ';

        return buffer;
    }

    void PatchNameDisplay::insertCharacterAtCaret(juce::juce_wchar character)
    {
        editBuffer_ = editBuffer_.replaceSection(caretIndex_, 1, juce::String::charToString(character));
        caretIndex_ = juce::jmin(kNameLength_ - 1, caretIndex_ + 1);
        restartCaretBlink();
    }

    void PatchNameDisplay::moveCaret(int delta)
    {
        caretIndex_ = juce::jlimit(0, kNameLength_ - 1, caretIndex_ + delta);
        clearIllegalCharacterPending();
        restartCaretBlink();
    }

    void PatchNameDisplay::clearIllegalCharacterPending()
    {
        if (! illegalCharPending_)
            return;

        illegalCharPending_ = false;
        if (onIllegalCharacterCleared_)
            onIllegalCharacterCleared_();
    }

    void PatchNameDisplay::restartCaretBlink()
    {
        caretOn_ = true;
        stopTimer();
        startTimer(kCaretBlinkIntervalMs_);
        repaint();
    }

    void PatchNameDisplay::timerCallback()
    {
        caretOn_ = ! caretOn_;
        repaint();
    }

    void PatchNameDisplay::mouseDoubleClick(const juce::MouseEvent&)
    {
        beginEdit();
    }

    bool PatchNameDisplay::keyPressed(const juce::KeyPress& key)
    {
        if (! editing_)
            return false;

        if (key == juce::KeyPress::returnKey)
        {
            commitEdit();
            return true;
        }

        if (key == juce::KeyPress::escapeKey)
        {
            cancelEdit();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::leftKey)
        {
            moveCaret(-1);
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::rightKey)
        {
            moveCaret(1);
            return true;
        }

        // LCD-style overwrite editor: Backspace/Delete clear a slot to a space
        // (they must not fall through as "illegal characters").
        if (key.getKeyCode() == juce::KeyPress::backspaceKey)
        {
            clearIllegalCharacterPending();
            if (caretIndex_ > 0)
                --caretIndex_;
            editBuffer_ = editBuffer_.replaceSection(caretIndex_, 1, " ");
            restartCaretBlink();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::deleteKey)
        {
            clearIllegalCharacterPending();
            editBuffer_ = editBuffer_.replaceSection(caretIndex_, 1, " ");
            restartCaretBlink();
            return true;
        }

        const auto rawCharacter = key.getTextCharacter();
        if (rawCharacter == 0)
            return false; // non-printable key (Tab, function keys...) — let default handling occur

        const auto typedCharacter = juce::CharacterFunctions::toUpperCase(rawCharacter);

        if (! Core::PatchFileNameSanitizer::isAllowedMatrixChar(typedCharacter))
        {
            illegalCharPending_ = true;
            if (onIllegalCharacter_)
                onIllegalCharacter_();
            return true;
        }

        clearIllegalCharacterPending();
        insertCharacterAtCaret(typedCharacter);
        return true;
    }

    void PatchNameDisplay::focusLost(juce::Component::FocusChangeType)
    {
        cancelEdit();
    }

    void PatchNameDisplay::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds().toFloat();
        drawBackground(g, bounds);
        drawBorder(g, bounds);

        if (secondaryLabel_.isEmpty())
            drawSingleLine(g, bounds);
        else
            drawDualLine(g, bounds);
    }

    void PatchNameDisplay::drawBackground(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(look_.background);
        g.fillRect(bounds);
    }

    void PatchNameDisplay::drawBorder(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        g.setColour(look_.border);
        const float systemDisplayScale = ScaledDrawing::systemDisplayScaleForComponent(*this);
        const float borderThickness = ScaledDrawing::snappedStrokeThicknessFromDesign(
            static_cast<float>(kBorderThickness_),
            uiScale_,
            systemDisplayScale,
            ScaledDrawing::StrokeSnapPolicy::kRound);
        g.drawRect(bounds, borderThickness);
    }

    void PatchNameDisplay::drawSingleLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        drawNameSlots(g, bounds);
    }

    void PatchNameDisplay::drawDualLine(juce::Graphics& g, const juce::Rectangle<float>& bounds)
    {
        auto workingBounds = bounds;
        const auto primaryRow = workingBounds.removeFromTop(bounds.getHeight() * kPrimaryRowHeightRatio_);
        const auto secondaryRow = workingBounds;

        drawNameSlots(g, primaryRow);
        drawSecondaryText(g, secondaryRow);
    }

    void PatchNameDisplay::drawNameSlots(juce::Graphics& g, const juce::Rectangle<float>& rowBounds)
    {
        const auto scaledFont = look_.font.withHeight(look_.font.getHeight() * uiScale_);
        g.setFont(scaledFont);

        if (! editing_)
        {
            g.setColour(look_.text);
            g.drawText(patchName_, rowBounds, juce::Justification::centred, false);
            return;
        }

        const float slotWidth = rowBounds.getWidth() / static_cast<float>(kNameLength_);

        for (int i = 0; i < kNameLength_; ++i)
        {
            const auto slotBounds = rowBounds
                .withX(rowBounds.getX() + slotWidth * static_cast<float>(i))
                .withWidth(slotWidth);
            const auto character = editBuffer_.substring(i, i + 1);
            const bool isBlinkingCaretSlot = (i == caretIndex_) && caretOn_;

            if (isBlinkingCaretSlot)
            {
                g.setColour(look_.text);
                g.fillRect(slotBounds);
                g.setColour(juce::Colour(ColourChart::kBlack));
            }
            else
            {
                g.setColour(look_.text);
            }

            g.drawText(character, slotBounds, juce::Justification::centred, false);
        }
    }

    void PatchNameDisplay::drawSecondaryText(juce::Graphics& g, const juce::Rectangle<float>& rowBounds)
    {
        g.setColour(look_.secondaryText);
        g.setFont(look_.secondaryFont.withHeight(look_.secondaryFont.getHeight() * uiScale_));
        g.drawText(secondaryLabel_, rowBounds, juce::Justification::centred, false);
    }
}
