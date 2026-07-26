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

        if (! editable_)
        {
            hoveredPrimary_ = false;
            if (editing_)
                cancelEdit();
        }

        repaint();
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

        // Fresh empty LCD buffer — do not preload the current patch name.
        editBuffer_ = buildEmptyEditBuffer();
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

    juce::String PatchNameDisplay::buildEmptyEditBuffer() const
    {
        return juce::String::repeatedString(" ", kNameLength_);
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

    void PatchNameDisplay::updateHoverFromPosition(juce::Point<float> position)
    {
        const bool wasHovered = hoveredPrimary_;

        if (! editable_ || editing_)
        {
            hoveredPrimary_ = false;
        }
        else
        {
            const auto layout = computeTextBlockLayout(getLocalBounds().toFloat());
            hoveredPrimary_ = layout.primaryRow.contains(position);
        }

        if (hoveredPrimary_ != wasHovered)
            repaint();
    }

    void PatchNameDisplay::mouseDoubleClick(const juce::MouseEvent& e)
    {
        if (! editable_)
            return;

        const auto layout = computeTextBlockLayout(getLocalBounds().toFloat());
        if (! layout.primaryRow.contains(e.position))
            return;

        beginEdit();
    }

    void PatchNameDisplay::mouseEnter(const juce::MouseEvent& e)
    {
        updateHoverFromPosition(e.position);
    }

    void PatchNameDisplay::mouseExit(const juce::MouseEvent&)
    {
        if (! hoveredPrimary_)
            return;

        hoveredPrimary_ = false;
        repaint();
    }

    void PatchNameDisplay::mouseMove(const juce::MouseEvent& e)
    {
        updateHoverFromPosition(e.position);
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

    juce::Font PatchNameDisplay::scaledPrimaryFont() const
    {
        return look_.font.withHeight(look_.font.getHeight() * uiScale_);
    }

    juce::Font PatchNameDisplay::scaledSecondaryFont() const
    {
        return look_.secondaryFont.withHeight(look_.secondaryFont.getHeight() * uiScale_);
    }

    juce::Colour PatchNameDisplay::primaryTextColour() const
    {
        const float alpha = (editable_ && hoveredPrimary_ && ! editing_)
            ? kPrimaryHoverAlpha_
            : kPrimaryIdleAlpha_;
        return look_.text.withAlpha(alpha);
    }

    PatchNameDisplay::TextBlockLayout PatchNameDisplay::computeTextBlockLayout(
        juce::Rectangle<float> bounds) const
    {
        TextBlockLayout layout;
        const auto primaryFont = scaledPrimaryFont();
        const float primaryHeight = primaryFont.getHeight();

        if (secondaryLabel_.isEmpty())
        {
            layout.primaryRow = bounds.withSizeKeepingCentre(bounds.getWidth(), primaryHeight);
            return layout;
        }

        const auto secondaryFont = scaledSecondaryFont();
        const float secondaryHeight = secondaryFont.getHeight();
        const float gap = kDualLineGap_ * uiScale_;
        const float contentHeight = primaryHeight + gap + secondaryHeight;
        const float top = bounds.getY() + 0.5f * (bounds.getHeight() - contentHeight);

        layout.hasSecondary = true;
        layout.primaryRow = { bounds.getX(), top, bounds.getWidth(), primaryHeight };
        layout.secondaryRow = {
            bounds.getX(),
            top + primaryHeight + gap,
            bounds.getWidth(),
            secondaryHeight
        };
        return layout;
    }

    void PatchNameDisplay::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds().toFloat();
        drawBackground(g, bounds);
        drawBorder(g, bounds);

        const auto layout = computeTextBlockLayout(bounds);
        drawNameSlots(g, layout.primaryRow);

        if (layout.hasSecondary)
            drawSecondaryText(g, layout.secondaryRow);
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

    void PatchNameDisplay::drawNameSlots(juce::Graphics& g, const juce::Rectangle<float>& rowBounds)
    {
        const auto scaledFont = scaledPrimaryFont();
        g.setFont(scaledFont);

        if (! editing_)
        {
            g.setColour(primaryTextColour());
            g.drawText(patchName_, rowBounds, juce::Justification::centred, false);
            return;
        }

        // Same font as the idle name; slot pitch stays close to natural string spacing
        // (only a thin caret pad around each glyph — not a full-width / 8 stretch).
        const float glyphWidth = juce::jmax(
            1.0f,
            juce::GlyphArrangement::getStringWidth(scaledFont, "M"));
        const float caretPadX = kCaretPadX_ * uiScale_;
        const float caretPadY = kCaretPadY_ * uiScale_;
        const float caretBottomTrim = kCaretBottomTrim_ * uiScale_;
        const float slotWidth = glyphWidth + 2.0f * caretPadX;
        const float slotGap = kSlotGap_ * uiScale_;
        const float blockWidth = slotWidth * static_cast<float>(kNameLength_)
            + slotGap * static_cast<float>(kNameLength_ - 1);
        const float blockX = rowBounds.getCentreX() - 0.5f * blockWidth;
        const float fullCaretHeight = scaledFont.getHeight() + 2.0f * caretPadY;
        const float caretHeight = juce::jmax(1.0f, fullCaretHeight - caretBottomTrim);
        // Keep the top edge; shorten only from the bottom for optical vertical balance.
        const float caretY = rowBounds.getCentreY() - 0.5f * fullCaretHeight;

        for (int i = 0; i < kNameLength_; ++i)
        {
            const float slotX = blockX + static_cast<float>(i) * (slotWidth + slotGap);
            const auto slotBounds = juce::Rectangle<float>(slotX, caretY, slotWidth, caretHeight);
            const auto character = editBuffer_.substring(i, i + 1);
            const bool isBlank = character.trim().isEmpty();
            const bool isBlinkingCaretSlot = (i == caretIndex_) && caretOn_;

            if (isBlinkingCaretSlot)
            {
                g.setColour(look_.text);
                g.fillRect(slotBounds);
                g.setColour(juce::Colour(ColourChart::kBlack));
                if (! isBlank)
                    g.drawText(character, slotBounds, juce::Justification::centred, false);
            }
            else if (! isBlank)
            {
                g.setColour(look_.text.withAlpha(kPrimaryIdleAlpha_));
                g.drawText(character, slotBounds, juce::Justification::centred, false);
            }
        }
    }

    void PatchNameDisplay::drawSecondaryText(juce::Graphics& g, const juce::Rectangle<float>& rowBounds)
    {
        g.setColour(look_.secondaryText);
        g.setFont(scaledSecondaryFont());
        g.drawText(secondaryLabel_, rowBounds, juce::Justification::centred, false);
    }
}
