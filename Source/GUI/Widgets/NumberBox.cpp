#include "NumberBox.h"

#include <cstdlib>
#include <memory>

#include "GUI/Layout/ScaledDrawing.h"
#include "GUI/Skins/ColourChart.h"
#include "Shared/Definitions/PluginDisplayNames.h"

namespace TSS
{
    NumberBox::NumberBox(const NumberBoxLook& look, const Config& config)
        : look_(look)
        , height_(config.height)
        , minValue_(config.minValue)
        , maxValue_(config.maxValue)
        , editable_(config.editable)
    {
        setOpaque(true);
        setSize(config.width, height_);
        updateValueText();
    }

    void NumberBox::setLook(const NumberBoxLook& look)
    {
        look_ = look;
        applyEditorAppearance();
        repaint();
    }

    void NumberBox::setUiScale(float uiScale)
    {
        if (juce::approximatelyEqual(uiScale_, uiScale))
            return;

        uiScale_ = uiScale;
        applyEditorAppearance();
        layoutEditor();
        repaint();
    }

    void NumberBox::setRange(int minValue, int maxValue)
    {
        minValue_ = minValue;
        maxValue_ = maxValue;
        setValue(currentValue_);
        updateValueText();
        repaint();
    }

    void NumberBox::setValue(int newValue)
    {
        const int clampedValue = juce::jlimit(minValue_, maxValue_, newValue);
        
        if (currentValue_ != clampedValue)
        {
            currentValue_ = clampedValue;
            updateValueText();
            repaint();
            
            if (onValueChanged_)
                onValueChanged_(clampedValue);
        }
    }
    
    void NumberBox::setOnValueChanged(ValueChangedCallback callback)
    {
        onValueChanged_ = std::move(callback);
    }

    void NumberBox::setDisplayState(DisplayState state)
    {
        if (displayState_ == state)
            return;

        displayState_ = state;
        updateValueText();
        repaint();
    }

    void NumberBox::setFocusHighlight(bool focused)
    {
        if (focusHighlight_ == focused)
            return;

        focusHighlight_ = focused;
        repaint();
    }

    void NumberBox::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds().toFloat();

        // Match editor fill under the stroke so any inset gap uses the same colour as the field.
        g.setColour(editor_ != nullptr ? look_.editorBackground : look_.background);
        g.fillRect(bounds);

        g.setColour(getBorderColour());
        g.drawRect(bounds, borderStrokeThickness());

        if (editor_ != nullptr || cachedValueText_.isEmpty())
            return;

        g.setColour(getTextColour());
        g.setFont(scaledDisplayFont());
        g.drawText(cachedValueText_, bounds, juce::Justification::centred, false);
    }

    void NumberBox::resized()
    {
        layoutEditor();
        repaint();
    }

    int NumberBox::digitCount() const
    {
        int digits = 1;
        for (int remaining = std::abs(maxValue_) / 10; remaining > 0; remaining /= 10)
            ++digits;

        return digits;
    }

    void NumberBox::updateValueText()
    {
        switch (displayState_)
        {
            case DisplayState::kUnavailable:
                cachedValueText_.clear();
                return;

            case DisplayState::kUndefined:
                cachedValueText_ = juce::String::repeatedString(
                    juce::String::charToString(PluginDisplayNames::Widgets::NumberBox::kUndefinedValueDigit),
                    digitCount());
                return;

            case DisplayState::kValue:
            default:
                cachedValueText_ = juce::String(currentValue_).paddedLeft('0', digitCount());
                return;
        }
    }

    void NumberBox::mouseDoubleClick(const juce::MouseEvent&)
    {
        if (! editable_ || ! isEnabled()
            || displayState_ == DisplayState::kUnavailable
            || displayState_ == DisplayState::kUndefined)
            return;

        showEditor();
    }

    juce::Colour NumberBox::getBorderColour() const
    {
        return isEnabled() ? look_.borderOn : look_.borderOff;
    }

    juce::Colour NumberBox::getTextColour() const
    {
        // Red marks navigation focus only; an undefined coordinate stays neutral.
        const bool useFocusColour = focusHighlight_ && displayState_ == DisplayState::kValue;
        return useFocusColour ? look_.textFocus : look_.text;
    }

    juce::Font NumberBox::scaledDisplayFont() const
    {
        return look_.font.withHeight(look_.font.getHeight() * uiScale_);
    }

    float NumberBox::borderStrokeThickness() const
    {
        return ScaledDrawing::snappedStrokeThicknessFromDesign(
            static_cast<float>(kBorderThickness_),
            uiScale_,
            ScaledDrawing::systemDisplayScaleForComponent(*this),
            ScaledDrawing::StrokeSnapPolicy::kRound);
    }

    int NumberBox::editorBorderInset() const
    {
        return juce::jmax(1, juce::roundToInt(borderStrokeThickness()));
    }

    void NumberBox::layoutEditor()
    {
        if (editor_ == nullptr)
            return;

        editor_->setBounds(getLocalBounds().reduced(editorBorderInset()));
    }

    void NumberBox::applyEditorAppearance()
    {
        if (editor_ == nullptr)
            return;

        const auto editorFont = scaledDisplayFont();
        editor_->setFont(editorFont);
        editor_->applyFontToAllText(editorFont);
        editor_->setColour(juce::TextEditor::backgroundColourId, look_.editorBackground);
        editor_->setColour(juce::TextEditor::textColourId, look_.editorText);
        editor_->setColour(juce::TextEditor::highlightColourId, look_.editorSelectionBackground);
        editor_->setColour(juce::TextEditor::highlightedTextColourId, look_.editorText);
        editor_->setColour(juce::TextEditor::outlineColourId, juce::Colour(ColourChart::kTransparent));
        editor_->setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(ColourChart::kTransparent));
    }

    void NumberBox::showEditor()
    {
        if (editor_ != nullptr)
            return;

        editor_ = std::make_unique<juce::TextEditor>();
        layoutEditor();
        editor_->setText(juce::String(currentValue_), false);
        editor_->setJustification(juce::Justification::centred);
        applyEditorAppearance();

        editor_->setBorder(juce::BorderSize<int>(0));
        editor_->setIndents(0, 0);
        editor_->setInputRestrictions(0, "0123456789");

        editor_->onReturnKey = [this] { handleEditorReturn(); };
        editor_->onEscapeKey = [this] { hideEditor(); };
        editor_->onFocusLost = [this] { hideEditor(); };

        addAndMakeVisible(*editor_);
        editor_->grabKeyboardFocus();
        editor_->selectAll();
    }

    void NumberBox::hideEditor()
    {
        if (editor_ == nullptr)
            return;

        removeChildComponent(editor_.get());
        editor_.reset();
        repaint();
    }

    void NumberBox::handleEditorReturn()
    {
        if (editor_ == nullptr)
            return;

        const auto text = editor_->getText();
        
        if (text.isEmpty())
        {
            hideEditor();
            return;
        }
        
        const int rawValue = text.getIntValue();
        const int clampedValue = juce::jlimit(minValue_, maxValue_, rawValue);

        setValue(clampedValue);
        hideEditor();
    }
}
