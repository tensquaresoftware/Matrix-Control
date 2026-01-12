#include "Dco1Panel.h"

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

Dco1Panel::Dco1Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, PluginDescriptors::ModuleIds::kDco1);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              PluginDescriptors::StandaloneWidgetIds::kDco1Init,
                              PluginDescriptors::StandaloneWidgetIds::kDco1Copy,
                              PluginDescriptors::StandaloneWidgetIds::kDco1Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kDco1Frequency,
                                dco1FrequencyLabel, dco1FrequencySlider, dco1FrequencyAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kDco1FrequencyModByLfo1,
                                dco1FrequencyModByLfo1Label, dco1FrequencyModByLfo1Slider, dco1FrequencyModByLfo1Attachment, horizontalSeparator2);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kDco1Sync,
                                     dco1SyncLabel, dco1SyncComboBox, dco1SyncAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kDco1PulseWidth,
                                dco1PulseWidthLabel, dco1PulseWidthSlider, dco1PulseWidthAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kDco1PulseWidthModByLfo2,
                                dco1PulseWidthModByLfo2Label, dco1PulseWidthModByLfo2Slider, dco1PulseWidthModByLfo2Attachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                PluginDescriptors::ParameterIds::kDco1WaveShape,
                                dco1WaveShapeLabel, dco1WaveShapeSlider, dco1WaveShapeAttachment, horizontalSeparator6);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kDco1WaveSelect,
                                     dco1WaveSelectLabel, dco1WaveSelectComboBox, dco1WaveSelectAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kDco1Levers,
                                     dco1LeversLabel, dco1LeversComboBox, dco1LeversAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kDco1KeyboardPortamento,
                                     dco1KeyboardPortamentoLabel, dco1KeyboardPortamentoComboBox, dco1KeyboardPortamentoAttachment, horizontalSeparator9);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     PluginDescriptors::ParameterIds::kDco1KeyClick,
                                     dco1KeyClickLabel, dco1KeyClickComboBox, dco1KeyClickAttachment, horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Dco1Panel::~Dco1Panel() = default;

void Dco1Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco1Panel::resized()
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

    if (auto* header = dco1ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = dco1PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = dco1CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = dco1InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = dco1FrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1FrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1FrequencyModByLfo1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1FrequencyModByLfo1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1SyncLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1SyncComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1PulseWidthLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1PulseWidthSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1PulseWidthModByLfo2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1PulseWidthModByLfo2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1WaveShapeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco1WaveShapeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1WaveSelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1WaveSelectComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1LeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1LeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1KeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1KeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco1KeyClickLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco1KeyClickComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Dco1Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = dco1ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = dco1InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco1CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco1PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = dco1FrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1FrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1FrequencyModByLfo1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1FrequencyModByLfo1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1SyncLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1SyncComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1PulseWidthLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1PulseWidthSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1PulseWidthModByLfo2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1PulseWidthModByLfo2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1WaveShapeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco1WaveShapeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1WaveSelectLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1WaveSelectComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1LeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1LeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1KeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1KeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = dco1KeyClickLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco1KeyClickComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Dco1Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    dco1ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*dco1ModuleHeader);
}

void Dco1Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                          const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    dco1InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    dco1InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1InitButton);

    dco1CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    dco1CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1CopyButton);

    dco1PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    dco1PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco1PasteButton);
}

void Dco1Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void Dco1Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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

