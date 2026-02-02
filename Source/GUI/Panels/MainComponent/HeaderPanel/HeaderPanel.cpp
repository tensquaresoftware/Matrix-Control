#include "HeaderPanel.h"

#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/ComboBox.h"
#include "GUI/Themes/Theme.h"

using tss::Theme;

HeaderPanel::HeaderPanel(Theme& theme)
    : theme_(&theme)
    , skinLabel_(theme, kLabelWidth_, kControlHeight_, "SKIN:")
    , skinComboBox_(theme, kComboBoxWidth_, kControlHeight_, tss::ComboBox::Style::ButtonLike)
    , zoomLabel_(theme, kLabelWidth_, kControlHeight_, "ZOOM:")
    , zoomComboBox_(theme, kComboBoxWidth_, kControlHeight_, tss::ComboBox::Style::ButtonLike)
{
    setOpaque(true);
    
    addAndMakeVisible(skinLabel_);
    
    skinComboBox_.addItem("Black", 1);
    skinComboBox_.addItem("Cream", 2);
    skinComboBox_.setSelectedId(1, juce::dontSendNotification);
    addAndMakeVisible(skinComboBox_);
    
    addAndMakeVisible(zoomLabel_);
    
    zoomComboBox_.addItem("50%", 1);
    zoomComboBox_.addItem("75%", 2);
    zoomComboBox_.addItem("90%", 3);
    zoomComboBox_.addItem("100%", 4);
    zoomComboBox_.addItem("125%", 5);
    zoomComboBox_.addItem("150%", 6);
    zoomComboBox_.addItem("200%", 7);
    zoomComboBox_.setSelectedId(4, juce::dontSendNotification);
    addAndMakeVisible(zoomComboBox_);
}

void HeaderPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getHeaderPanelBackgroundColour());
}

void HeaderPanel::resized()
{
    const auto bounds = getLocalBounds();
    const auto spacing = getSpacing();
    const auto controlY = (getHeight() - kControlHeight_) / 2;
    
    int currentX = spacing;
    
    skinLabel_.setBounds(
        bounds.getX() + currentX,
        bounds.getY() + controlY,
        kLabelWidth_,
        kControlHeight_
    );
    currentX += kLabelWidth_ + spacing;
    
    skinComboBox_.setBounds(
        bounds.getX() + currentX,
        bounds.getY() + controlY,
        kComboBoxWidth_,
        kControlHeight_
    );
    currentX += kComboBoxWidth_ + spacing * 2;
    
    zoomLabel_.setBounds(
        bounds.getX() + currentX,
        bounds.getY() + controlY,
        kLabelWidth_,
        kControlHeight_
    );
    currentX += kLabelWidth_ + spacing;
    
    zoomComboBox_.setBounds(
        bounds.getX() + currentX,
        bounds.getY() + controlY,
        kComboBoxWidth_,
        kControlHeight_
    );
}

void HeaderPanel::setTheme(Theme& theme)
{
    theme_ = &theme;
    skinLabel_.setTheme(theme);
    skinComboBox_.setTheme(theme);
    zoomLabel_.setTheme(theme);
    zoomComboBox_.setTheme(theme);
}

