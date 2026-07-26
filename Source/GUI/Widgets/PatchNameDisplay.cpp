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

    PatchNameDisplay::~PatchNameDisplay()
    {
        detachOutsideClickListener();
    }

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

        // Fresh empty field — do not preload the current patch name.
        editBuffer_.clear();
        caretIndex_ = 0;
        caretOn_ = true;
        illegalCharPending_ = false;
        editing_ = true;

        grabKeyboardFocus();
        attachOutsideClickListener();
        startTimer(kCaretBlinkIntervalMs_);
        repaint();
    }

    void PatchNameDisplay::cancelEdit()
    {
        if (! editing_)
            return;

        stopTimer();
        detachOutsideClickListener();
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
        detachOutsideClickListener();
        editing_ = false;
        illegalCharPending_ = false;
        repaint();

        if (onCommit_)
            onCommit_(resolvedName);

        if (onEditEnded_)
            onEditEnded_();
    }

    void PatchNameDisplay::attachOutsideClickListener()
    {
        if (listeningForOutsideClicks_)
            return;

        juce::Desktop::getInstance().addGlobalMouseListener(this);
        listeningForOutsideClicks_ = true;
    }

    void PatchNameDisplay::detachOutsideClickListener()
    {
        if (! listeningForOutsideClicks_)
            return;

        juce::Desktop::getInstance().removeGlobalMouseListener(this);
        listeningForOutsideClicks_ = false;
    }

    int PatchNameDisplay::maxCaretIndex() const noexcept
    {
        if (editBuffer_.isEmpty())
            return 0;

        if (editBuffer_.length() >= kNameLength_)
            return kNameLength_ - 1;

        return editBuffer_.length();
    }

    void PatchNameDisplay::insertCharacterAtCaret(juce::juce_wchar character)
    {
        if (editBuffer_.length() >= kNameLength_)
            return;

        editBuffer_ = editBuffer_.substring(0, caretIndex_)
            + juce::String::charToString(character)
            + editBuffer_.substring(caretIndex_);
        ++caretIndex_;
        caretIndex_ = juce::jmin(caretIndex_, maxCaretIndex());
        restartCaretBlink();
    }

    void PatchNameDisplay::deleteCharacterBeforeCaret()
    {
        if (editBuffer_.isEmpty())
            return;

        // At max length the caret sits on the last character (no 9th slot). Backspace
        // must delete that last character, not the one before it.
        if (editBuffer_.length() == kNameLength_ && caretIndex_ == kNameLength_ - 1)
        {
            deleteCharacterAtCaret();
            return;
        }

        if (caretIndex_ <= 0)
            return;

        editBuffer_ = editBuffer_.substring(0, caretIndex_ - 1)
            + editBuffer_.substring(caretIndex_);
        --caretIndex_;
        restartCaretBlink();
    }

    void PatchNameDisplay::deleteCharacterAtCaret()
    {
        if (caretIndex_ >= editBuffer_.length())
            return;

        editBuffer_ = editBuffer_.substring(0, caretIndex_)
            + editBuffer_.substring(caretIndex_ + 1);
        caretIndex_ = juce::jmin(caretIndex_, maxCaretIndex());
        restartCaretBlink();
    }

    void PatchNameDisplay::moveCaret(int delta)
    {
        caretIndex_ = juce::jlimit(0, maxCaretIndex(), caretIndex_ + delta);
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

    void PatchNameDisplay::mouseDown(const juce::MouseEvent& e)
    {
        if (! editing_)
            return;

        // Commit when the click is outside this display rectangle (e.g. module header
        // or anywhere else in the UI). Clicks inside the afficheur keep editing.
        if (getScreenBounds().contains(e.getScreenPosition()))
            return;

        commitEdit();
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

        // Normal text-field editing: Backspace removes the char before the caret and
        // shifts the tail left; Delete removes at the caret. Cap remains 8 chars.
        if (key.getKeyCode() == juce::KeyPress::backspaceKey)
        {
            clearIllegalCharacterPending();
            deleteCharacterBeforeCaret();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::deleteKey)
        {
            clearIllegalCharacterPending();
            deleteCharacterAtCaret();
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
        // Keyboard focus leaving the display also commits (e.g. Tab). Outside clicks are
        // handled by the global mouse listener against this component's screen bounds.
        commitEdit();
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

        // Characters use natural pitch. Past-the-end caret exists only while length < 8.
        const float glyphWidth = juce::jmax(
            1.0f,
            juce::GlyphArrangement::getStringWidth(scaledFont, "M"));
        const float caretPadX = kCaretPadX_ * uiScale_;
        const float caretPadY = kCaretPadY_ * uiScale_;
        const float caretBottomTrim = kCaretBottomTrim_ * uiScale_;
        const float slotWidth = glyphWidth + 2.0f * caretPadX;
        const float slotGap = kSlotGap_ * uiScale_;
        const bool showEndCaretSlot = editBuffer_.length() < kNameLength_;
        const int drawnSlots = juce::jmax(1, editBuffer_.length() + (showEndCaretSlot ? 1 : 0));
        const float blockWidth = slotWidth * static_cast<float>(drawnSlots)
            + slotGap * static_cast<float>(juce::jmax(0, drawnSlots - 1));
        const float blockX = rowBounds.getCentreX() - 0.5f * blockWidth;
        const float fullCaretHeight = scaledFont.getHeight() + 2.0f * caretPadY;
        const float caretHeight = juce::jmax(1.0f, fullCaretHeight - caretBottomTrim);
        const float caretY = rowBounds.getCentreY() - 0.5f * fullCaretHeight;

        auto slotBoundsAt = [&](int index) -> juce::Rectangle<float>
        {
            const float slotX = blockX + static_cast<float>(index) * (slotWidth + slotGap);
            return { slotX, caretY, slotWidth, caretHeight };
        };

        for (int i = 0; i < editBuffer_.length(); ++i)
        {
            const auto character = editBuffer_.substring(i, i + 1);
            const bool isSpace = character == " ";
            const auto bounds = slotBoundsAt(i);
            const bool caretHere = (i == caretIndex_) && caretOn_;

            if (caretHere)
            {
                g.setColour(look_.text);
                g.fillRect(bounds);
                g.setColour(juce::Colour(ColourChart::kBlack));
                if (! isSpace)
                    g.drawText(character, bounds, juce::Justification::centred, false);
            }
            else if (! isSpace)
            {
                g.setColour(look_.text.withAlpha(kPrimaryIdleAlpha_));
                g.drawText(character, bounds, juce::Justification::centred, false);
            }
        }

        // Empty insertion caret after the last character — only while there is room to type.
        if (showEndCaretSlot && caretIndex_ == editBuffer_.length() && caretOn_)
        {
            g.setColour(look_.text);
            g.fillRect(slotBoundsAt(editBuffer_.length()));
        }
    }

    void PatchNameDisplay::drawSecondaryText(juce::Graphics& g, const juce::Rectangle<float>& rowBounds)
    {
        g.setColour(look_.secondaryText);
        g.setFont(scaledSecondaryFont());
        g.drawText(secondaryLabel_, rowBounds, juce::Justification::centred, false);
    }
}
