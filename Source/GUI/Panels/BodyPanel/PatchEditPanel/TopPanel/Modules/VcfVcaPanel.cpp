#include "VcfVcaPanel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/Button.h"
#include "../../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../../Shared/PluginDescriptors.h"
#include "../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

VcfVcaPanel::VcfVcaPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kVcfVca);
    setupInitButton(inTheme, widgetFactory, PluginDescriptors::StandaloneWidgetIds::kVcfVcaInit);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVcfBalance,
                                vcfBalanceLabel, vcfBalanceSlider, vcfBalanceAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVcfFequency,
                                vcfFrequencyLabel, vcfFrequencySlider, vcfFrequencyAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVcfFrequencyModByEnv1,
                                vcfFrequencyModByEnv1Label, vcfFrequencyModByEnv1Slider, vcfFrequencyModByEnv1Attachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVcfFrequencyModByPressure,
                                vcfFrequencyModByPressureLabel, vcfFrequencyModByPressureSlider, vcfFrequencyModByPressureAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVcfResonance,
                                vcfResonanceLabel, vcfResonanceSlider, vcfResonanceAttachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVca1Volume,
                                vca1VolumeLabel, vca1VolumeSlider, vca1VolumeAttachment, horizontalSeparator6);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVca1ModByVelocity,
                                vca1ModByVelocityLabel, vca1ModByVelocitySlider, vca1ModByVelocityAttachment, horizontalSeparator7);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kVca2ModByEnv2,
                                vca2ModByEnv2Label, vca2ModByEnv2Slider, vca2ModByEnv2Attachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVcfLevers,
                                     vcfLeversLabel, vcfLeversComboBox, vcfLeversAttachment, horizontalSeparator9);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kVcfKeyboardPortamento,
                                     vcfKeyboardPortamentoLabel, vcfKeyboardPortamentoComboBox, vcfKeyboardPortamentoAttachment, horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

VcfVcaPanel::~VcfVcaPanel() = default;

void VcfVcaPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void VcfVcaPanel::resized()
{
    const auto moduleHeaderHeight = tss::ModuleHeader::getHeight();
    const auto moduleHeaderWidth = tss::ModuleHeader::getWidth(tss::ModuleHeader::ModuleWidth::PatchEdit);
    const auto buttonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InitCopyPaste);
    const auto buttonHeight = tss::Button::getHeight();
    const auto labelWidth = tss::Label::getWidth(tss::Label::LabelWidth::PatchEditModule);
    const auto labelHeight = tss::Label::getHeight();
    const auto sliderWidth = tss::Slider::getWidth();
    const auto sliderHeight = tss::Slider::getHeight();
    const auto comboBoxWidth = tss::ComboBox::getWidth(tss::ComboBox::ComboBoxWidth::PatchEditModule);
    const auto comboBoxHeight = tss::ComboBox::getHeight();
    const auto separatorWidth = tss::HorizontalSeparator::getWidth(tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    const auto separatorHeight = tss::HorizontalSeparator::getHeight();
    const auto panelWidth = getWidth();

    int y = 0;

    if (auto* header = vcfVcaModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = vcfVcaInitButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = vcfBalanceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfBalanceSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyModByEnv1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencyModByEnv1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfFrequencyModByPressureLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfFrequencyModByPressureSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfResonanceLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vcfResonanceSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca1VolumeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca1VolumeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca1ModByVelocityLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca1ModByVelocitySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vca2ModByEnv2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = vca2ModByEnv2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfLeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vcfLeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = vcfKeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = vcfKeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void VcfVcaPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = vcfVcaModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = vcfVcaInitButton.get())
        button->setTheme(inTheme);

    if (auto* label = vcfBalanceLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfBalanceSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyModByEnv1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencyModByEnv1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfFrequencyModByPressureLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfFrequencyModByPressureSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfResonanceLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vcfResonanceSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = vca1VolumeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vca1VolumeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = vca1ModByVelocityLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = vca1ModByVelocitySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = vca2ModByEnv2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = vca2ModByEnv2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfLeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vcfLeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = vcfKeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = vcfKeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void VcfVcaPanel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    vcfVcaModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*vcfVcaModuleHeader);
}

void VcfVcaPanel::setupInitButton(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& initWidgetId)
{
    vcfVcaInitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    vcfVcaInitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*vcfVcaInitButton);
}

void VcfVcaPanel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
                                               const juce::String& parameterId,
                                               std::unique_ptr<tss::Label>& label,
                                               std::unique_ptr<tss::Slider>& slider,
                                               std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment,
                                               std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    slider = widgetFactory.createIntParameterSlider(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        parameterId,
        *slider);
    addAndMakeVisible(*slider);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*separator);
}

void VcfVcaPanel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
                                                    const juce::String& parameterId,
                                                    std::unique_ptr<tss::Label>& label,
                                                    std::unique_ptr<tss::ComboBox>& comboBox,
                                                    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& attachment,
                                                    std::unique_ptr<tss::HorizontalSeparator>& separator)
{
    label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(parameterId));
    addAndMakeVisible(*label);

    comboBox = widgetFactory.createChoiceParameterComboBox(parameterId, inTheme);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        parameterId,
        *comboBox);
    addAndMakeVisible(*comboBox);

    separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*separator);
}
