#include "FmTrackPanel.h"

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

FmTrackPanel::FmTrackPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
{
    // Module Name
    fmTrackModuleHeader = std::make_unique<tss::ModuleHeader>(
        inTheme, 
        widgetFactory.getGroupDisplayName(SynthDescriptors::ModuleIds::kFmTrack) );
    addAndMakeVisible(*fmTrackModuleHeader);

    // Standalone Widgets
    fmTrackInitButton = widgetFactory.createStandaloneButton(SynthDescriptors::StandaloneWidgetIds::kFmTrackInit, inTheme);
    fmTrackInitButton->onClick = [this]
    {
        apvts.state.setProperty(SynthDescriptors::StandaloneWidgetIds::kFmTrackInit,
                                juce::Time::getCurrentTime().toMilliseconds(),
                                nullptr);
    };
    addAndMakeVisible(*fmTrackInitButton);

    // FM Amount
    fmAmountLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmAmount));
    addAndMakeVisible(*fmAmountLabel);

    fmAmountSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmAmount, inTheme);
    fmAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmAmount,
        *fmAmountSlider);
    addAndMakeVisible(*fmAmountSlider);

    horizontalSeparator1 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator1);

    // FM Mod by ENV 3
    fmModByEnv3Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmModByEnv3));
    addAndMakeVisible(*fmModByEnv3Label);

    fmModByEnv3Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmModByEnv3, inTheme);
    fmModByEnv3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmModByEnv3,
        *fmModByEnv3Slider);
    addAndMakeVisible(*fmModByEnv3Slider);

    horizontalSeparator2 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator2);

    // FM Mod by Pressure
    fmModByPressureLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kFmModByPressure));
    addAndMakeVisible(*fmModByPressureLabel);

    fmModByPressureSlider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kFmModByPressure, inTheme);
    fmModByPressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kFmModByPressure,
        *fmModByPressureSlider);
    addAndMakeVisible(*fmModByPressureSlider);

    horizontalSeparator3 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator3);

    // Track Point 1
    trackPoint1Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint1));
    addAndMakeVisible(*trackPoint1Label);

    trackPoint1Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint1, inTheme);
    trackPoint1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint1,
        *trackPoint1Slider);
    addAndMakeVisible(*trackPoint1Slider);

    horizontalSeparator4 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator4);

    // Track Point 2
    trackPoint2Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint2));
    addAndMakeVisible(*trackPoint2Label);

    trackPoint2Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint2, inTheme);
    trackPoint2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint2,
        *trackPoint2Slider);
    addAndMakeVisible(*trackPoint2Slider);

    horizontalSeparator5 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator5);

    // Track Point 3
    trackPoint3Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint3));
    addAndMakeVisible(*trackPoint3Label);

    trackPoint3Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint3, inTheme);
    trackPoint3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint3,
        *trackPoint3Slider);
    addAndMakeVisible(*trackPoint3Slider);

    horizontalSeparator6 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator6);

    // Track Point 4
    trackPoint4Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint4));
    addAndMakeVisible(*trackPoint4Label);

    trackPoint4Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint4, inTheme);
    trackPoint4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint4,
        *trackPoint4Slider);
    addAndMakeVisible(*trackPoint4Slider);

    horizontalSeparator7 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator7);

    // Track Point 5
    trackPoint5Label = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackPoint5));
    addAndMakeVisible(*trackPoint5Label);

    trackPoint5Slider = widgetFactory.createIntParameterSlider(SynthDescriptors::ParameterIds::kTrackPoint5, inTheme);
    trackPoint5Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackPoint5,
        *trackPoint5Slider);
    addAndMakeVisible(*trackPoint5Slider);

    horizontalSeparator8 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator8);

    // Track Input
    trackInputLabel = std::make_unique<tss::Label>(
        inTheme, tss::Label::LabelWidth::PatchEditModule, 
        widgetFactory.getParameterDisplayName(SynthDescriptors::ParameterIds::kTrackInput));
    addAndMakeVisible(*trackInputLabel);

    trackInputComboBox = widgetFactory.createChoiceParameterComboBox(SynthDescriptors::ParameterIds::kTrackInput, inTheme);
    trackInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        SynthDescriptors::ParameterIds::kTrackInput,
        *trackInputComboBox);
    addAndMakeVisible(*trackInputComboBox);

    horizontalSeparator9 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator9);

    horizontalSeparator10 = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::PatchEditModule);
    addAndMakeVisible(*horizontalSeparator10);

    setSize(getWidth(), getHeight());
}

FmTrackPanel::~FmTrackPanel() = default;

void FmTrackPanel::paint(juce::Graphics& g)
{
        g.fillAll(theme->getPatchEditModulePanelBackgroundColour());
}

void FmTrackPanel::resized()
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

    if (auto* header = fmTrackModuleHeader.get())
        header->setBounds(0, y, moduleHeaderWidth, moduleHeaderHeight);

    if (auto* button = fmTrackInitButton.get())
        button->setBounds(panelWidth - buttonWidth, y, buttonWidth, buttonHeight);

    y += moduleHeaderHeight;

    if (auto* label = fmAmountLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = fmAmountSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator1.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = fmModByEnv3Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = fmModByEnv3Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator2.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = fmModByPressureLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = fmModByPressureSlider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator3.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackPoint1Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = trackPoint1Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator4.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackPoint2Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = trackPoint2Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator5.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackPoint3Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = trackPoint3Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator6.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackPoint4Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = trackPoint4Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator7.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackPoint5Label.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* slider = trackPoint5Slider.get())
        slider->setBounds(labelWidth, y, sliderWidth, sliderHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator8.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    if (auto* label = trackInputLabel.get())
        label->setBounds(0, y, labelWidth, labelHeight);

    if (auto* comboBox = trackInputComboBox.get())
        comboBox->setBounds(labelWidth, y, comboBoxWidth, comboBoxHeight);

    y += labelHeight;

    if (auto* separator = horizontalSeparator9.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);

    y += separatorHeight;

    y += labelHeight;

    if (auto* separator = horizontalSeparator10.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void FmTrackPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = fmTrackModuleHeader.get())
        header->setTheme(inTheme);

    if (auto* button = fmTrackInitButton.get())
        button->setTheme(inTheme);

    if (auto* label = fmAmountLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = fmAmountSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator1.get())
        separator->setTheme(inTheme);

    if (auto* label = fmModByEnv3Label.get())
        label->setTheme(inTheme);

    if (auto* slider = fmModByEnv3Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator2.get())
        separator->setTheme(inTheme);

    if (auto* label = fmModByPressureLabel.get())
        label->setTheme(inTheme);

    if (auto* slider = fmModByPressureSlider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator3.get())
        separator->setTheme(inTheme);

    if (auto* label = trackPoint1Label.get())
        label->setTheme(inTheme);

    if (auto* slider = trackPoint1Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator4.get())
        separator->setTheme(inTheme);

    if (auto* label = trackPoint2Label.get())
        label->setTheme(inTheme);

    if (auto* slider = trackPoint2Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator5.get())
        separator->setTheme(inTheme);

    if (auto* label = trackPoint3Label.get())
        label->setTheme(inTheme);

    if (auto* slider = trackPoint3Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator6.get())
        separator->setTheme(inTheme);

    if (auto* label = trackPoint4Label.get())
        label->setTheme(inTheme);

    if (auto* slider = trackPoint4Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator7.get())
        separator->setTheme(inTheme);

    if (auto* label = trackPoint5Label.get())
        label->setTheme(inTheme);

    if (auto* slider = trackPoint5Slider.get())
        slider->setTheme(inTheme);

    if (auto* separator = horizontalSeparator8.get())
        separator->setTheme(inTheme);

    if (auto* label = trackInputLabel.get())
        label->setTheme(inTheme);

    if (auto* comboBox = trackInputComboBox.get())
        comboBox->setTheme(inTheme);

    if (auto* separator = horizontalSeparator9.get())
        separator->setTheme(inTheme);

    if (auto* separator = horizontalSeparator10.get())
        separator->setTheme(inTheme);

    repaint();
}
