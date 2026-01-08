#include "Dco2Panel.h"

#include "../../../../../Themes/Theme.h"
#include "../../../../../Widgets/ModuleHeader.h"
#include "../../../../../Widgets/Label.h"
#include "../../../../../Widgets/Slider.h"
#include "../../../../../Widgets/ComboBox.h"
#include "../../../../../Widgets/Button.h"
#include "../../../../../Widgets/HorizontalSeparator.h"
#include "../../../../../../Shared/SynthDescriptors.h"
#include "../../../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

Dco2Panel::Dco2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, SynthDescriptors::ModuleIds::kDco2);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              SynthDescriptors::StandaloneWidgetIds::kDco2Init,
                              SynthDescriptors::StandaloneWidgetIds::kDco2Copy,
                              SynthDescriptors::StandaloneWidgetIds::kDco2Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2Frequency,
                                dco2FrequencyLabel, dco2FrequencySlider, dco2FrequencyAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2FrequencyModByLfo1,
                                dco2FrequencyModByLfo1Label, dco2FrequencyModByLfo1Slider, dco2FrequencyModByLfo1Attachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2Detune,
                                dco2DetuneLabel, dco2DetuneSlider, dco2DetuneAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2PulseWidth,
                                dco2PulseWidthLabel, dco2PulseWidthSlider, dco2PulseWidthAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2PulseWidthModByLfo2,
                                dco2PulseWidthModByLfo2Label, dco2PulseWidthModByLfo2Slider, dco2PulseWidthModByLfo2Attachment, horizontalSeparator5);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kDco2WaveShape,
                                dco2WaveShapeLabel, dco2WaveShapeSlider, dco2WaveShapeAttachment, horizontalSeparator6);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kDco2WaveSelect,
                                     dco2WaveSelectLabel, dco2WaveSelectComboBox, dco2WaveSelectAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kDco2Levers,
                                     dco2LeversLabel, dco2LeversComboBox, dco2LeversAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kDco2KeyboardPortamento,
                                     dco2KeyboardPortamentoLabel, dco2KeyboardPortamentoComboBox, dco2KeyboardPortamentoAttachment, horizontalSeparator9);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kDco2KeyClick,
                                     dco2KeyClickLabel, dco2KeyClickComboBox, dco2KeyClickAttachment, horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Dco2Panel::~Dco2Panel() = default;

void Dco2Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Dco2Panel::resized()
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

    if (auto* header = dco2ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = dco2PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = dco2CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = dco2InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = dco2FrequencyLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2FrequencySlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2FrequencyModByLfo1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2FrequencyModByLfo1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2DetuneLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2DetuneSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2PulseWidthLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2PulseWidthSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2PulseWidthModByLfo2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2PulseWidthModByLfo2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2WaveShapeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = dco2WaveShapeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2WaveSelectLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2WaveSelectComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2LeversLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2LeversComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2KeyboardPortamentoLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2KeyboardPortamentoComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = dco2KeyClickLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = dco2KeyClickComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Dco2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = dco2ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = dco2InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco2CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = dco2PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = dco2FrequencyLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2FrequencySlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2FrequencyModByLfo1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2FrequencyModByLfo1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2DetuneLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2DetuneSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2PulseWidthLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2PulseWidthSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2PulseWidthModByLfo2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2PulseWidthModByLfo2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2WaveShapeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = dco2WaveShapeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2WaveSelectLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2WaveSelectComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2LeversLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2LeversComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2KeyboardPortamentoLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2KeyboardPortamentoComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* label = dco2KeyClickLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = dco2KeyClickComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Dco2Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    dco2ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*dco2ModuleHeader);
}

void Dco2Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                          const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    dco2InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    dco2InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2InitButton);

    dco2CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    dco2CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2CopyButton);

    dco2PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    dco2PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*dco2PasteButton);
}

void Dco2Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void Dco2Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
