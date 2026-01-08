#include "Lfo2Panel.h"

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

Lfo2Panel::Lfo2Panel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    setupModuleHeader(inTheme, widgetFactory, SynthDescriptors::ModuleIds::kLfo2);
    setupInitCopyPasteButtons(inTheme, widgetFactory,
                              SynthDescriptors::StandaloneWidgetIds::kLfo2Init,
                              SynthDescriptors::StandaloneWidgetIds::kLfo2Copy,
                              SynthDescriptors::StandaloneWidgetIds::kLfo2Paste);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kLfo2Speed,
                                lfo2SpeedLabel, lfo2SpeedSlider, lfo2SpeedAttachment, horizontalSeparator1);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kLfo2SpeedModByKeyboard,
                                lfo2SpeedModByKeyboardLabel, lfo2SpeedModByKeyboardSlider, lfo2SpeedModByKeyboardAttachment, horizontalSeparator2);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kLfo2RetriggerPoint,
                                lfo2RetriggerPointLabel, lfo2RetriggerPointSlider, lfo2RetriggerPointAttachment, horizontalSeparator3);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kLfo2Amplitude,
                                lfo2AmplitudeLabel, lfo2AmplitudeSlider, lfo2AmplitudeAttachment, horizontalSeparator4);

    setupIntParameterWithSlider(inTheme, widgetFactory,
                                SynthDescriptors::ParameterIds::kLfo2AmplitudeModByRamp2,
                                lfo2AmplitudeModByRamp2Label, lfo2AmplitudeModByRamp2Slider, lfo2AmplitudeModByRamp2Attachment, horizontalSeparator5);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kLfo2Waveform,
                                     lfo2WaveformLabel, lfo2WaveformComboBox, lfo2WaveformAttachment, horizontalSeparator6);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kLfo2TriggerMode,
                                     lfo2TriggerModeLabel, lfo2TriggerModeComboBox, lfo2TriggerModeAttachment, horizontalSeparator7);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kLfo2Lag,
                                     lfo2LagLabel, lfo2LagComboBox, lfo2LagAttachment, horizontalSeparator8);

    setupChoiceParameterWithComboBox(inTheme, widgetFactory,
                                     SynthDescriptors::ParameterIds::kLfo2SampleInput,
                                     lfo2SampleInputLabel, lfo2SampleInputComboBox, lfo2SampleInputAttachment, horizontalSeparator9);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

Lfo2Panel::~Lfo2Panel() = default;

void Lfo2Panel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void Lfo2Panel::resized()
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

    if (auto* header = lfo2ModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = lfo2PasteButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    if (auto* button = lfo2CopyButton.get())
        button->setBounds(panelWidth - buttonWidth * 2, y, buttonWidth, buttonHeight);

    if (auto* button = lfo2InitButton.get())
        button->setBounds(panelWidth - buttonWidth * 3, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = lfo2SpeedLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2SpeedSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2SpeedModByKeyboardLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2SpeedModByKeyboardSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2RetriggerPointLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2RetriggerPointSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2AmplitudeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2AmplitudeSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2AmplitudeModByRamp2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = lfo2AmplitudeModByRamp2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2WaveformLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2TriggerModeLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2TriggerModeComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2LagLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2LagComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = lfo2SampleInputLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = lfo2SampleInputComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void Lfo2Panel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = lfo2ModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = lfo2InitButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo2CopyButton.get())
        button->setTheme(inTheme);

    if (auto* button = lfo2PasteButton.get())
        button->setTheme(inTheme);

    if (auto* label = lfo2SpeedLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2SpeedSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2SpeedModByKeyboardLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2SpeedModByKeyboardSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2RetriggerPointLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2RetriggerPointSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2AmplitudeLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2AmplitudeSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2AmplitudeModByRamp2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = lfo2AmplitudeModByRamp2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2WaveformLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* comboBox = lfo2WaveformComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2TriggerModeLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2TriggerModeComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2LagLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2LagComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = lfo2SampleInputLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = lfo2SampleInputComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}

void Lfo2Panel::setupModuleHeader(Theme& inTheme, WidgetFactory& widgetFactory, const juce::String& moduleId)
{
    lfo2ModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(moduleId));
    addAndMakeVisible(*lfo2ModuleHeader);
}

void Lfo2Panel::setupInitCopyPasteButtons(Theme& inTheme, WidgetFactory& widgetFactory,
                                         const juce::String& initWidgetId, const juce::String& copyWidgetId, const juce::String& pasteWidgetId)
{
    lfo2InitButton = widgetFactory.createStandaloneButton(initWidgetId, inTheme);
    lfo2InitButton->onClick = [this, initWidgetId]
    {
        apvts.state.setProperty(initWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2InitButton);

    lfo2CopyButton = widgetFactory.createStandaloneButton(copyWidgetId, inTheme);
    lfo2CopyButton->onClick = [this, copyWidgetId]
    {
        apvts.state.setProperty(copyWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2CopyButton);

    lfo2PasteButton = widgetFactory.createStandaloneButton(pasteWidgetId, inTheme);
    lfo2PasteButton->onClick = [this, pasteWidgetId]
    {
        apvts.state.setProperty(pasteWidgetId,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*lfo2PasteButton);
}

void Lfo2Panel::setupIntParameterWithSlider(Theme& inTheme, WidgetFactory& widgetFactory,
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

void Lfo2Panel::setupChoiceParameterWithComboBox(Theme& inTheme, WidgetFactory& widgetFactory,
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
