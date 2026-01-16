#include "VibratoPanel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../Shared/PluginDimensions.h"
#include "../../../../../Factories/WidgetFactory.h"

using tss::Theme;

VibratoPanel::VibratoPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kVibrato);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoSpeed,
                                vibratoSpeedLabel_, vibratoSpeedSlider_, vibratoSpeedAttachment_, horizontalSeparator1_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoWaveform,
                                     vibratoWaveformLabel_, vibratoWaveformComboBox_, vibratoWaveformAttachment_, horizontalSeparator2_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoAmplitude,
                                vibratoAmplitudeLabel_, vibratoAmplitudeSlider_, vibratoAmplitudeAttachment_, horizontalSeparator3_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoSpeedModSource,
                                     vibratoSpeedModSourceLabel_, vibratoSpeedModSourceComboBox_, vibratoSpeedModSourceAttachment_, horizontalSeparator4_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoSpeedModAmount,
                                vibratoSpeedModAmountLabel_, vibratoSpeedModAmountSlider_, vibratoSpeedModAmountAttachment_, horizontalSeparator5_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVibratoAmpModSource,
                                     vibratoAmpModSourceLabel_, vibratoAmpModSourceComboBox_, vibratoAmpModSourceAttachment_, horizontalSeparator6_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVibratoAmpModAmount,
                                vibratoAmpModAmountLabel_, vibratoAmpModAmountSlider_, vibratoAmpModAmountAttachment_, horizontalSeparator7_);

    setSize(getWidth(), getHeight());
}

VibratoPanel::~VibratoPanel() = default;

void VibratoPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getMasterEditPanelBackgroundColour());
}

void VibratoPanel::resized()
{
    const auto moduleHeaderHeight = PluginDimensions::Widgets::Heights::kModuleHeader;
    const auto moduleHeaderWidth = PluginDimensions::Widgets::Widths::ModuleHeader::kMasterEditModule;
    const auto labelWidth = PluginDimensions::Widgets::Widths::Label::kMasterEditModule;
    const auto labelHeight = PluginDimensions::Widgets::Heights::kLabel;
    const auto sliderWidth = PluginDimensions::Widgets::Widths::Slider::kStandard;
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = PluginDimensions::Widgets::Widths::ComboBox::kMasterEditModule;
    const auto comboBoxHeight = PluginDimensions::Widgets::Heights::kComboBox;
    const auto separatorWidth = PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule;
    const auto separatorHeight = PluginDimensions::Widgets::Heights::kHorizontalSeparator;

    int y = 0;

    if (auto* header = vibratoModuleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = vibratoSpeedLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoSpeedSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoWaveformLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoWaveformComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmplitudeLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoAmplitudeSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoSpeedModSourceLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoSpeedModSourceComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoSpeedModAmountLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoSpeedModAmountSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmpModSourceLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vibratoAmpModSourceComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vibratoAmpModAmountLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vibratoAmpModAmountSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void VibratoPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = vibratoModuleHeader_.get())
        header->setTheme(theme);

    if (auto* label = vibratoSpeedLabel_.get())
        label->setTheme(theme);

    if (auto* slider = vibratoSpeedSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator1_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoWaveformLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = vibratoWaveformComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator2_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoAmplitudeLabel_.get())
        label->setTheme(theme);

    if (auto* slider = vibratoAmplitudeSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator3_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoSpeedModSourceLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = vibratoSpeedModSourceComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator4_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoSpeedModAmountLabel_.get())
        label->setTheme(theme);

    if (auto* slider = vibratoSpeedModAmountSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator5_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoAmpModSourceLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = vibratoAmpModSourceComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator6_.get())
        separator->setTheme(theme);

    if (auto* label = vibratoAmpModAmountLabel_.get())
        label->setTheme(theme);

    if (auto* slider = vibratoAmpModAmountSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator7_.get())
        separator->setTheme(theme);

    repaint();
}

void VibratoPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    vibratoModuleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*vibratoModuleHeader_);
}

void VibratoPanel::setupIntParameterWithSlider(Theme& theme, WidgetFactory& widgetFactory,
                                              const juce::String& parameterId,
                                              std::unique_ptr<tss::Label>& label,
                                              std::unique_ptr<tss::Slider>& slider,
                                              std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                              std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        theme, 
        PluginDimensions::Widgets::Widths::Label::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kLabel,
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    slider = widgetFactory.createIntParameterSlider(parameterId, theme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts_,
        parameterId,
        *slider);
    addAndMakeVisible(*slider);

    separator = std::make_unique<tss::HorizontalSeparator>(
        theme, 
        PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kHorizontalSeparator);
    addAndMakeVisible(*separator);
}

void VibratoPanel::setupChoiceParameterWithComboBox(Theme& theme, WidgetFactory& widgetFactory,
                                                    const juce::String& parameterId,
                                                    std::unique_ptr<tss::Label>& label,
                                                    std::unique_ptr<tss::ComboBox>& comboBox,
                                                    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                                    std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        theme, 
        PluginDimensions::Widgets::Widths::Label::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kLabel,
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    comboBox = widgetFactory.createChoiceParameterComboBox(
        parameterId, 
        theme,
        PluginDimensions::Widgets::Widths::ComboBox::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kComboBox);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts_,
        parameterId,
        *comboBox);
    addAndMakeVisible(*comboBox);

    separator = std::make_unique<tss::HorizontalSeparator>(
        theme, 
        PluginDimensions::Widgets::Widths::HorizontalSeparator::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kHorizontalSeparator);
    addAndMakeVisible(*separator);
}
