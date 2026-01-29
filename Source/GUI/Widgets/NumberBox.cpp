#include "NumberBox.h"

#include <memory>

#include "GUI/Themes/Theme.h"

namespace tss
{
    NumberBox::NumberBox(Theme& theme, int width, bool editable)
        : theme_(&theme)
        , editable_(editable)
    {
        setOpaque(true);
        setSize(width, kHeight_);
    }

    void NumberBox::setTheme(Theme& theme)
    {
        theme_ = &theme;
    }

    void NumberBox::setValue(int newValue)
    {
        if (currentValue_ != newValue)
        {
            currentValue_ = newValue;
            repaint();
        }
    }

    void NumberBox::setShowDot(bool show)
    {
        if (showDot_ != show)
        {
            showDot_ = show;
            repaint();
        }
    }

    void NumberBox::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        const auto bounds = getLocalBounds().toFloat();
        const auto text = juce::String(currentValue_);

        g.setColour(theme_->getButtonBackgroundColourOn());
        g.fillRect(bounds);

        g.setColour(getBorderColour());
        g.drawRect(bounds, kBorderThickness_);

        g.setColour(theme_->getNumberBoxTextColour());
        g.setFont(theme_->getBaseFont());
        g.drawText(text, bounds, juce::Justification::centred, false);

        if (showDot_)
        {
            const auto textWidth = calculateTextWidth(text);
            const auto dotPosition = calculateDotPosition(bounds, textWidth);

            g.setColour(theme_->getNumberBoxDotColour());
            g.fillEllipse(dotPosition.x, dotPosition.y, kDotRadius_ * 2.0f, kDotRadius_ * 2.0f);
        }
    }

    void NumberBox::mouseDoubleClick(const juce::MouseEvent&)
    {
        if (!editable_ || !isEnabled())
            return;

        showEditor();
    }

    juce::Colour NumberBox::getBorderColour() const
    {
        if (!isEnabled())
            return theme_->getButtonBorderColourOff();

        return theme_->getButtonBorderColourOn();
    }

    float NumberBox::calculateTextWidth(const juce::String& text) const
    {
        const auto font = theme_->getBaseFont();
        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(font, text, 0.0f, 0.0f);
        return glyphArrangement.getBoundingBox(0, -1, true).getWidth();
    }

    juce::Point<float> NumberBox::calculateDotPosition(const juce::Rectangle<float>& bounds, float textWidth) const
    {
        const auto font = theme_->getBaseFont();
        const auto textRight = bounds.getCentreX() + textWidth * 0.5f;
        const auto baselineY = bounds.getCentreY() + font.getHeight() * 0.5f - font.getDescent();

        return { textRight + kDotXOffset_, baselineY - kDotRadius_ };
    }

    void NumberBox::showEditor()
    {
        if (editor_ != nullptr)
            return;

        const auto baseFont = theme_->getBaseFont();
        const auto editorFont = baseFont.withStyle(juce::Font::bold)
                                        .withHeight(baseFont.getHeight() + kEditorFontSizeIncrease_);

        editor_ = std::make_unique<juce::TextEditor>();
        editor_->setBounds(getLocalBounds());
        editor_->setText(juce::String(currentValue_), false);
        editor_->setFont(editorFont);
        editor_->setJustification(juce::Justification::centred);
        
        editor_->setColour(juce::TextEditor::backgroundColourId, theme_->getNumberBoxEditorBackgroundColour());
        editor_->setColour(juce::TextEditor::textColourId, theme_->getNumberBoxEditorTextColour());
        editor_->setColour(juce::TextEditor::highlightColourId, theme_->getNumberBoxEditorSelectionBackgroundColour());
        editor_->setColour(juce::TextEditor::highlightedTextColourId, theme_->getNumberBoxEditorTextColour());
        editor_->setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        editor_->setColour(juce::TextEditor::focusedOutlineColourId, juce::Colours::transparentBlack);
        
        editor_->setBorder(juce::BorderSize<int>(0));
        editor_->setIndents(0, 0);
        editor_->setInputRestrictions(0, "0123456789-");

        editor_->onReturnKey = [this] { handleEditorReturn(); };
        editor_->onEscapeKey = [this] { hideEditor(); };
        editor_->onFocusLost = [this] { hideEditor(); };

        addAndMakeVisible(*editor_);
        editor_->grabKeyboardFocus();
        editor_->selectAll();
        editor_->applyFontToAllText(editorFont);
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
        const auto newValue = text.getIntValue();

        setValue(newValue);
        hideEditor();
    }
}
