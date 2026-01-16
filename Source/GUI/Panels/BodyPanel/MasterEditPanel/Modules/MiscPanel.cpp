#include "MiscPanel.h"

#include "../../../../Themes/Theme.h"
#include "../../../../Widgets/ModuleHeader.h"
#include "../../../../Widgets/Label.h"
#include "../../../../Widgets/Slider.h"
#include "../../../../Widgets/ComboBox.h"
#include "../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../Shared/PluginDescriptors.h"
#include "../../../../../Shared/PluginDimensions.h"
#include "../../../../Factories/WidgetFactory.h"

using tss::Theme;

MiscPanel::MiscPanel(Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : theme_(&theme)
    , apvts_(apvts)
{
    setupModuleHeader(theme, widgetFactory, PluginDescriptors::ModuleIds::kMisc);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMasterTune,
                                masterTuneLabel_, masterTuneSlider_, masterTuneAttachment_, horizontalSeparator1_);

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kMasterTranspose,
                                masterTransposeLabel_, masterTransposeSlider_, masterTransposeAttachment_, horizontalSeparator2_);
    masterTransposeSlider_->setUnit("st");

    setupIntParameterWithSlider(theme, widgetFactory,
                                PluginDescriptors::ParameterIds::kBendRange,
                                bendRangeLabel_, bendRangeSlider_, bendRangeAttachment_, horizontalSeparator3_);
    bendRangeSlider_->setUnit("st");

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kUnisonEnable,
                                     unisonEnableLabel_, unisonEnableComboBox_, unisonEnableAttachment_, horizontalSeparator4_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVolumeInvertEnable,
                                     volumeInvertEnableLabel_, volumeInvertEnableComboBox_, volumeInvertEnableAttachment_, horizontalSeparator5_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kBankLockEnable,
                                     bankLockEnableLabel_, bankLockEnableComboBox_, bankLockEnableAttachment_, horizontalSeparator6_);

    setupChoiceParameterWithComboBox(theme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kMemoryProtectEnable,
                                     memoryProtectEnableLabel_, memoryProtectEnableComboBox_, memoryProtectEnableAttachment_, horizontalSeparator7_);

    setSize(getWidth(), getHeight());
}

MiscPanel::~MiscPanel() = default;

void MiscPanel::paint(juce::Graphics& g)
{
    g.fillAll(theme_->getMasterEditPanelBackgroundColour());
}

void MiscPanel::resized()
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

    if (auto* header = miscModuleHeader_.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    y += moduleHeaderHeight;

    if (auto* label = masterTuneLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = masterTuneSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = masterTransposeLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = masterTransposeSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = bendRangeLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = bendRangeSlider_.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = unisonEnableLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = unisonEnableComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = volumeInvertEnableLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = volumeInvertEnableComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = bankLockEnableLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = bankLockEnableComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = memoryProtectEnableLabel_.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = memoryProtectEnableComboBox_.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7_.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void MiscPanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = miscModuleHeader_.get())
        header->setTheme(theme);

    if (auto* label = masterTuneLabel_.get())
        label->setTheme(theme);

    if (auto* slider = masterTuneSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator1_.get())
        separator->setTheme(theme);

    if (auto* label = masterTransposeLabel_.get())
        label->setTheme(theme);

    if (auto* slider = masterTransposeSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator2_.get())
        separator->setTheme(theme);

    if (auto* label = bendRangeLabel_.get())
        label->setTheme(theme);

    if (auto* slider = bendRangeSlider_.get())
        slider->setTheme(theme);

    if (auto* separator = horizontalSeparator3_.get())
        separator->setTheme(theme);

    if (auto* label = unisonEnableLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = unisonEnableComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator4_.get())
        separator->setTheme(theme);

    if (auto* label = volumeInvertEnableLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = volumeInvertEnableComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator5_.get())
        separator->setTheme(theme);

    if (auto* label = bankLockEnableLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = bankLockEnableComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator6_.get())
        separator->setTheme(theme);

    if (auto* label = memoryProtectEnableLabel_.get())
        label->setTheme(theme);

    if (auto* comboBox = memoryProtectEnableComboBox_.get())
        comboBox->setTheme(theme);

    if (auto* separator = horizontalSeparator7_.get())
        separator->setTheme(theme);

    repaint();
}

void MiscPanel::setupModuleHeader(Theme& theme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    miscModuleHeader_ = std::make_unique<tss::ModuleHeader>(
        theme, 
        widgetFactory.getGroupDisplayName(moduleId),
        PluginDimensions::Widgets::Widths::ModuleHeader::kMasterEditModule,
        PluginDimensions::Widgets::Heights::kModuleHeader,
        tss::ModuleHeader::ColourVariant::Orange);
    addAndMakeVisible(*miscModuleHeader_);
}

void MiscPanel::setupIntParameterWithSlider(Theme& theme, WidgetFactory& widgetFactory,
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

void MiscPanel::setupChoiceParameterWithComboBox(Theme& theme, WidgetFactory& widgetFactory,
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
