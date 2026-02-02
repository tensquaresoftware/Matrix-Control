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
        updateThemeCache();
        updateTextWidthCache();
    }

    void NumberBox::setTheme(Theme& theme)
    {
        theme_ = &theme;
        updateThemeCache();
        invalidateCache();
        repaint();
    }

    void NumberBox::setValue(int newValue)
    {
        if (currentValue_ != newValue)
        {
            currentValue_ = newValue;
            updateTextWidthCache();
            invalidateCache();
            repaint();
        }
    }

    void NumberBox::setShowDot(bool show)
    {
        if (showDot_ != show)
        {
            showDot_ = show;
            invalidateCache();
            repaint();
        }
    }

    void NumberBox::paint(juce::Graphics& g)
    {
        if (theme_ == nullptr)
            return;

        if (!cacheValid_)
            regenerateCache();

        if (cachedImage_.isValid())
        {
            g.drawImage(cachedImage_, getLocalBounds().toFloat(),
                       juce::RectanglePlacement::stretchToFit);
        }
    }

    void NumberBox::resized()
    {
        invalidateCache();
    }

    void NumberBox::regenerateCache()
    {
        const auto width = getWidth();
        const auto height = getHeight();

        if (width <= 0 || height <= 0)
            return;

        const float pixelScale = getPixelScale();
        const int imageWidth = juce::roundToInt(width * pixelScale);
        const int imageHeight = juce::roundToInt(height * pixelScale);

        // Create HiDPI image at physical resolution
        cachedImage_ = juce::Image(juce::Image::ARGB, imageWidth, imageHeight, true);
        juce::Graphics g(cachedImage_);
        
        // Scale graphics context to match physical resolution
        g.addTransform(juce::AffineTransform::scale(pixelScale));

        const auto bounds = juce::Rectangle<float>(0.0f, 0.0f, 
                                                    static_cast<float>(width), 
                                                    static_cast<float>(height));

        g.setColour(cachedBackgroundColour_);
        g.fillRect(bounds);

        g.setColour(cachedBorderColour_);
        g.drawRect(bounds, kBorderThickness_);

        g.setColour(cachedTextColour_);
        g.setFont(cachedFont_);
        g.drawText(cachedValueText_, bounds, juce::Justification::centred, false);

        if (showDot_)
        {
            const auto dotPosition = calculateDotPosition(bounds, cachedTextWidth_);

            g.setColour(cachedDotColour_);
            g.fillEllipse(dotPosition.x, dotPosition.y, kDotRadius_ * 2.0f, kDotRadius_ * 2.0f);
        }

        cacheValid_ = true;
    }

    void NumberBox::invalidateCache()
    {
        cacheValid_ = false;
    }

    void NumberBox::updateThemeCache()
    {
        if (theme_ == nullptr)
            return;

        cachedBackgroundColour_ = theme_->getButtonBackgroundColourOn();
        cachedBorderColour_ = getBorderColour();
        cachedTextColour_ = theme_->getNumberBoxTextColour();
        cachedDotColour_ = theme_->getNumberBoxDotColour();
        cachedFont_ = theme_->getBaseFont();
    }

    void NumberBox::updateTextWidthCache()
    {
        cachedValueText_ = juce::String(currentValue_);
        
        if (theme_ == nullptr)
        {
            cachedTextWidth_ = 0.0f;
            return;
        }

        juce::GlyphArrangement glyphArrangement;
        glyphArrangement.addLineOfText(cachedFont_, cachedValueText_, 0.0f, 0.0f);
        cachedTextWidth_ = glyphArrangement.getBoundingBox(0, -1, true).getWidth();
    }

    float NumberBox::getPixelScale() const
    {
        const auto* display = juce::Desktop::getInstance()
                                  .getDisplays()
                                  .getDisplayForRect(getScreenBounds());
        const float displayScale = display != nullptr ? static_cast<float>(display->scale) : 1.0f;
        return displayScale;
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

    juce::Point<float> NumberBox::calculateDotPosition(const juce::Rectangle<float>& bounds, float textWidth) const
    {
        const auto textRight = bounds.getCentreX() + textWidth * 0.5f;
        const auto baselineY = bounds.getCentreY() + cachedFont_.getHeight() * 0.5f - cachedFont_.getDescent();

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
