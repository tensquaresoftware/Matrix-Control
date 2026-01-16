#include "ParameterPanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Widgets/Label.h"
#include "GUI/Widgets/Slider.h"
#include "GUI/Widgets/ComboBox.h"
#include "GUI/Widgets/HorizontalSeparator.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

ParameterPanel::~ParameterPanel() = default;

ParameterPanel::ParameterPanel(Theme& theme,
                                WidgetFactory& factory,
                                const juce::String& parameterId,
                                ParameterType type,
                                juce::AudioProcessorValueTreeState& apvts)
    : parameterType_(type)
{
    if (type == ParameterType::None)
    {
        createSeparator(theme);
    }
    else
    {
        createParameterLabel(theme, factory, parameterId);
        createParameterWidget(theme, factory, parameterId, apvts);
        createSeparator(theme);
    }
}

void ParameterPanel::createParameterLabel(Theme& theme, WidgetFactory& factory, const juce::String& parameterId)
{
    label_ = std::make_unique<tss::Label>(
        theme,
        PluginDimensions::Widgets::Widths::Label::kPatchEditModule,
        PluginDimensions::Widgets::Heights::kLabel,
        factory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label_);
}

void ParameterPanel::createParameterWidget(Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts)
{
    if (parameterType_ == ParameterType::Slider)
        createSliderWidget(theme, factory, parameterId, apvts);
    else
        createComboBoxWidget(theme, factory, parameterId, apvts);
}

void ParameterPanel::createSliderWidget(Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts)
{
    slider_ = factory.createIntParameterSlider(parameterId, theme);
    sliderAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        parameterId,
        *slider_);
    addAndMakeVisible(*slider_);
}

void ParameterPanel::createComboBoxWidget(Theme& theme, WidgetFactory& factory, const juce::String& parameterId, juce::AudioProcessorValueTreeState& apvts)
{
    comboBox_ = factory.createChoiceParameterComboBox(
        parameterId,
        theme,
        PluginDimensions::Widgets::Widths::ComboBox::kPatchEditModule,
        PluginDimensions::Widgets::Heights::kComboBox);
    comboBoxAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        parameterId,
        *comboBox_);
    addAndMakeVisible(*comboBox_);
}

void ParameterPanel::createSeparator(Theme& theme)
{
    separator_ = std::make_unique<tss::HorizontalSeparator>(
        theme,
        PluginDimensions::Widgets::Widths::HorizontalSeparator::kPatchEditModule,
        PluginDimensions::Widgets::Heights::kHorizontalSeparator);
    addAndMakeVisible(*separator_);
}

void ParameterPanel::resized()
{
    const auto labelHeight = PluginDimensions::Widgets::Heights::kLabel;

    if (parameterType_ == ParameterType::None)
    {
        layoutSeparator(labelHeight);
    }
    else
    {
        int y = 0;

        layoutParameterLabel(y);
        layoutParameterWidget(y);
        y += labelHeight;
        layoutSeparator(y);
    }
}

void ParameterPanel::layoutParameterLabel(int y)
{
    const auto labelWidth = PluginDimensions::Widgets::Widths::Label::kPatchEditModule;
    const auto labelHeight = PluginDimensions::Widgets::Heights::kLabel;

    if (auto* label = label_.get())
        label->setBounds(0, y, labelWidth, labelHeight);
}

void ParameterPanel::layoutParameterWidget(int y)
{
    const auto labelWidth = PluginDimensions::Widgets::Widths::Label::kPatchEditModule;
    const auto sliderWidth = PluginDimensions::Widgets::Widths::Slider::kStandard;
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = PluginDimensions::Widgets::Widths::ComboBox::kPatchEditModule;
    const auto comboBoxHeight = PluginDimensions::Widgets::Heights::kComboBox;

    if (parameterType_ == ParameterType::Slider)
    {
        if (auto* slider = slider_.get())
            slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);
    }
    else
    {
        if (auto* comboBox = comboBox_.get())
            comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);
    }
}

void ParameterPanel::layoutSeparator(int y)
{
    const auto separatorWidth = PluginDimensions::Widgets::Widths::HorizontalSeparator::kPatchEditModule;
    const auto separatorHeight = PluginDimensions::Widgets::Heights::kHorizontalSeparator;

    if (auto* separator = separator_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void ParameterPanel::setTheme(Theme& theme)
{
    if (auto* label = label_.get())
        label->setTheme(theme);

    if (auto* slider = slider_.get())
        slider->setTheme(theme);

    if (auto* comboBox = comboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = separator_.get())
        separator->setTheme(theme);

    repaint();
}

int ParameterPanel::getTotalHeight() const
{
    return PluginDimensions::Widgets::Heights::kLabel + PluginDimensions::Widgets::Heights::kHorizontalSeparator;
}
