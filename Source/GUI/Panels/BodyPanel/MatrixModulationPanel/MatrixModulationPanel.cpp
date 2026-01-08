#include "MatrixModulationPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionHeader.h"
#include "../../../Widgets/ModulationBusHeader.h"
#include "../../../Widgets/Label.h"
#include "../../../Widgets/ComboBox.h"
#include "../../../Widgets/Slider.h"
#include "../../../Widgets/Button.h"
#include "../../../Widgets/HorizontalSeparator.h"
#include "../../../../Shared/SynthDescriptors.h"
#include "../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

MatrixModulationPanel::~MatrixModulationPanel() = default;

MatrixModulationPanel::MatrixModulationPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
    , sectionHeader(std::make_unique<tss::SectionHeader>(
        inTheme,
        tss::SectionHeader::SectionWidth::MatrixModulation,
        SynthDescriptors::getSectionDisplayName(SynthDescriptors::SectionIds::kMatrixModulation)))
    , modulationBusHeader(std::make_unique<tss::ModulationBusHeader>(inTheme))
{
    addAndMakeVisible(*sectionHeader);
    addAndMakeVisible(*modulationBusHeader);

    const auto parameterArrays = createModulationBusParameterArrays();

    createInitAllBussesButton(inTheme);

    for (int busNumber = 0; busNumber < SynthDescriptors::kModulationBusCount; ++busNumber)
    {
        createModulationBus(busNumber, widgetFactory, inTheme, parameterArrays);
    }

    setSize(getWidth(), getHeight());
}

std::array<const char*, SynthDescriptors::kModulationBusCount> MatrixModulationPanel::createBusIds() const
{
    return {
        SynthDescriptors::ModulationBusIds::kModulationBus0,
        SynthDescriptors::ModulationBusIds::kModulationBus1,
        SynthDescriptors::ModulationBusIds::kModulationBus2,
        SynthDescriptors::ModulationBusIds::kModulationBus3,
        SynthDescriptors::ModulationBusIds::kModulationBus4,
        SynthDescriptors::ModulationBusIds::kModulationBus5,
        SynthDescriptors::ModulationBusIds::kModulationBus6,
        SynthDescriptors::ModulationBusIds::kModulationBus7,
        SynthDescriptors::ModulationBusIds::kModulationBus8,
        SynthDescriptors::ModulationBusIds::kModulationBus9
    };
}

std::array<const char*, SynthDescriptors::kModulationBusCount> MatrixModulationPanel::createSourceParameterIds() const
{
    return {
        SynthDescriptors::ParameterIds::kModulationBus0Source,
        SynthDescriptors::ParameterIds::kModulationBus1Source,
        SynthDescriptors::ParameterIds::kModulationBus2Source,
        SynthDescriptors::ParameterIds::kModulationBus3Source,
        SynthDescriptors::ParameterIds::kModulationBus4Source,
        SynthDescriptors::ParameterIds::kModulationBus5Source,
        SynthDescriptors::ParameterIds::kModulationBus6Source,
        SynthDescriptors::ParameterIds::kModulationBus7Source,
        SynthDescriptors::ParameterIds::kModulationBus8Source,
        SynthDescriptors::ParameterIds::kModulationBus9Source
    };
}

std::array<const char*, SynthDescriptors::kModulationBusCount> MatrixModulationPanel::createAmountParameterIds() const
{
    return {
        SynthDescriptors::ParameterIds::kModulationBus0Amount,
        SynthDescriptors::ParameterIds::kModulationBus1Amount,
        SynthDescriptors::ParameterIds::kModulationBus2Amount,
        SynthDescriptors::ParameterIds::kModulationBus3Amount,
        SynthDescriptors::ParameterIds::kModulationBus4Amount,
        SynthDescriptors::ParameterIds::kModulationBus5Amount,
        SynthDescriptors::ParameterIds::kModulationBus6Amount,
        SynthDescriptors::ParameterIds::kModulationBus7Amount,
        SynthDescriptors::ParameterIds::kModulationBus8Amount,
        SynthDescriptors::ParameterIds::kModulationBus9Amount
    };
}

std::array<const char*, SynthDescriptors::kModulationBusCount> MatrixModulationPanel::createDestinationParameterIds() const
{
    return {
        SynthDescriptors::ParameterIds::kModulationBus0Destination,
        SynthDescriptors::ParameterIds::kModulationBus1Destination,
        SynthDescriptors::ParameterIds::kModulationBus2Destination,
        SynthDescriptors::ParameterIds::kModulationBus3Destination,
        SynthDescriptors::ParameterIds::kModulationBus4Destination,
        SynthDescriptors::ParameterIds::kModulationBus5Destination,
        SynthDescriptors::ParameterIds::kModulationBus6Destination,
        SynthDescriptors::ParameterIds::kModulationBus7Destination,
        SynthDescriptors::ParameterIds::kModulationBus8Destination,
        SynthDescriptors::ParameterIds::kModulationBus9Destination
    };
}

MatrixModulationPanel::ModulationBusParameterArrays MatrixModulationPanel::createModulationBusParameterArrays() const
{
    ModulationBusParameterArrays arrays;
    arrays.busIds = createBusIds();
    arrays.sourceParameterIds = createSourceParameterIds();
    arrays.amountParameterIds = createAmountParameterIds();
    arrays.destinationParameterIds = createDestinationParameterIds();
    return arrays;
}

void MatrixModulationPanel::createInitAllBussesButton(Theme& inTheme)
{
    initAllBussesButton = std::make_unique<tss::Button>(inTheme, tss::Button::ButtonWidth::InitCopyPaste, "I");
    initAllBussesButton->onClick = [this]
    {
        const auto busIds = createBusIds();
        for (const auto& busId : busIds)
        {
            apvts.state.setProperty(busId, juce::Time::getCurrentTime().toMilliseconds(), nullptr);
        }
    };
    addAndMakeVisible(*initAllBussesButton);
}

void MatrixModulationPanel::createModulationBus(int busNumber, WidgetFactory& widgetFactory, Theme& inTheme, const ModulationBusParameterArrays& parameterArrays)
{
    const auto busNumberAsSizeT = static_cast<size_t>(busNumber);
    auto& bus = modulationBuses[busNumberAsSizeT];

    createBusNumberLabel(bus, busNumber, inTheme);
    createSourceComboBox(bus, busNumberAsSizeT, widgetFactory, inTheme, parameterArrays);
    createAmountSlider(bus, busNumberAsSizeT, widgetFactory, inTheme, parameterArrays);
    createDestinationComboBox(bus, busNumberAsSizeT, inTheme, parameterArrays);
    createInitButton(bus, busNumberAsSizeT, inTheme, parameterArrays);
    createBusSeparator(bus, inTheme);
}

void MatrixModulationPanel::createBusNumberLabel(ModulationBus& bus, int busNumber, Theme& inTheme)
{
    bus.busNumberLabel = std::make_unique<tss::Label>(inTheme, tss::Label::LabelWidth::ModulationBusNumber, juce::String(busNumber));
    addAndMakeVisible(*bus.busNumberLabel);
}

void MatrixModulationPanel::createSourceComboBox(ModulationBus& bus, size_t busNumberAsSizeT, WidgetFactory& widgetFactory, Theme& inTheme, const ModulationBusParameterArrays& parameterArrays)
{
    bus.sourceComboBox = widgetFactory.createChoiceParameterComboBox(parameterArrays.sourceParameterIds[busNumberAsSizeT], inTheme);
    bus.sourceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, parameterArrays.sourceParameterIds[busNumberAsSizeT], *bus.sourceComboBox);
    addAndMakeVisible(*bus.sourceComboBox);
}

void MatrixModulationPanel::createAmountSlider(ModulationBus& bus, size_t busNumberAsSizeT, WidgetFactory& widgetFactory, Theme& inTheme, const ModulationBusParameterArrays& parameterArrays)
{
    bus.amountSlider = widgetFactory.createIntParameterSlider(parameterArrays.amountParameterIds[busNumberAsSizeT], inTheme);
    bus.amountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, parameterArrays.amountParameterIds[busNumberAsSizeT], *bus.amountSlider);
    addAndMakeVisible(*bus.amountSlider);
}

void MatrixModulationPanel::createDestinationComboBox(ModulationBus& bus, size_t busNumberAsSizeT, Theme& inTheme, const ModulationBusParameterArrays& parameterArrays)
{
    const auto& destinationDesc = SynthDescriptors::kModulationBusChoiceParameters[busNumberAsSizeT][1];
    bus.destinationComboBox = std::make_unique<tss::ComboBox>(inTheme, tss::ComboBox::ComboBoxWidth::MatrixModulationDestination);
    for (const auto& choice : destinationDesc.choices)
    {
        bus.destinationComboBox->addItem(choice, bus.destinationComboBox->getNumItems() + 1);
    }
    bus.destinationComboBox->setSelectedItemIndex(destinationDesc.defaultIndex);
    bus.destinationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, parameterArrays.destinationParameterIds[busNumberAsSizeT], *bus.destinationComboBox);
    addAndMakeVisible(*bus.destinationComboBox);
}

void MatrixModulationPanel::createInitButton(ModulationBus& bus, size_t busNumberAsSizeT, Theme& inTheme, const ModulationBusParameterArrays& parameterArrays)
{
    bus.initButton = std::make_unique<tss::Button>(inTheme, tss::Button::ButtonWidth::InitCopyPaste, "I");
    bus.initButton->onClick = [this, busId = parameterArrays.busIds[busNumberAsSizeT]]
    {
        apvts.state.setProperty(busId, juce::Time::getCurrentTime().toMilliseconds(), nullptr);
    };
    addAndMakeVisible(*bus.initButton);
}

void MatrixModulationPanel::createBusSeparator(ModulationBus& bus, Theme& inTheme)
{
    bus.separator = std::make_unique<tss::HorizontalSeparator>(inTheme, tss::HorizontalSeparator::SeparatorWidth::MatrixModulationBus);
    addAndMakeVisible(*bus.separator);
}

void MatrixModulationPanel::paint(juce::Graphics& g)
{
    if (auto* currentTheme = theme)
        g.fillAll(currentTheme->getMatrixModulationPanelBackgroundColour());
}

void MatrixModulationPanel::resized()
{
    int sectionHeaderHeight = 0;
    if (auto* header = sectionHeader.get())
    {
        header->setBounds(0, 0, getWidth(), header->getHeight());
        sectionHeaderHeight = header->getHeight();
    }

    int modulationBusHeaderHeight = 0;
    if (auto* busHeader = modulationBusHeader.get())
    {
        busHeader->setBounds(0, sectionHeaderHeight, busHeader->getWidth(), busHeader->getHeight());
        modulationBusHeaderHeight = busHeader->getHeight();
    }

    if (auto* initButton = initAllBussesButton.get())
    {
        const auto initAllButtonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InitCopyPaste);
        const auto initAllButtonHeight = tss::Button::getHeight();
        const auto initAllButtonX = getWidth() - initAllButtonWidth;
        initButton->setBounds(initAllButtonX, sectionHeaderHeight, initAllButtonWidth, initAllButtonHeight);
    }

    const auto busNumberLabelWidth = tss::Label::getWidth(tss::Label::LabelWidth::ModulationBusNumber);
    const auto busNumberLabelHeight = tss::Label::getHeight();

    const auto sourceComboBoxWidth = tss::ComboBox::getWidth(tss::ComboBox::ComboBoxWidth::MatrixModulationAmount);
    const auto sourceComboBoxHeight = tss::ComboBox::getHeight();
    
    const auto amountSliderWidth = tss::Slider::getWidth();
    const auto amountSliderHeight = tss::Slider::getHeight();

    const auto destinationComboBoxWidth = tss::ComboBox::getWidth(tss::ComboBox::ComboBoxWidth::MatrixModulationDestination);
    const auto destinationComboBoxHeight = tss::ComboBox::getHeight();

    const auto initButtonWidth = tss::Button::getWidth(tss::Button::ButtonWidth::InitCopyPaste);
    const auto initButtonHeight = tss::Button::getHeight();

    const auto separatorWidth = tss::HorizontalSeparator::getWidth(tss::HorizontalSeparator::SeparatorWidth::MatrixModulationBus);
    const auto separatorHeight = tss::HorizontalSeparator::getHeight();

    const auto modulationBusStartY = sectionHeaderHeight + modulationBusHeaderHeight;
    
    const auto widgetRowHeight = busNumberLabelHeight;

    int y = modulationBusStartY;

    for (int busNumber = 0; busNumber < SynthDescriptors::kModulationBusCount; ++busNumber)
    {
        const auto busNumberAsSizeT = static_cast<size_t>(busNumber);
        auto& bus = modulationBuses[busNumberAsSizeT];

        positionModulationBusWidgetsRow(bus, y, busNumberLabelWidth, busNumberLabelHeight, sourceComboBoxWidth, sourceComboBoxHeight, amountSliderWidth, amountSliderHeight, destinationComboBoxWidth, destinationComboBoxHeight, initButtonWidth, initButtonHeight);
        y += widgetRowHeight;

        positionModulationBusSeparator(bus, y, separatorWidth, separatorHeight);
        y += separatorHeight;
    }
}

void MatrixModulationPanel::positionModulationBusWidgetsRow(ModulationBus& bus, int y, int busNumberLabelWidth, int busNumberLabelHeight, int sourceComboBoxWidth, int sourceComboBoxHeight, int amountSliderWidth, int amountSliderHeight, int destinationComboBoxWidth, int destinationComboBoxHeight, int initButtonWidth, int initButtonHeight)
{
    int x = 0;

    if (auto* label = bus.busNumberLabel.get())
        label->setBounds(x, y, busNumberLabelWidth, busNumberLabelHeight);
    x += busNumberLabelWidth;

    if (auto* comboBox = bus.sourceComboBox.get())
        comboBox->setBounds(x, y, sourceComboBoxWidth, sourceComboBoxHeight);
    x += sourceComboBoxWidth + kSpacing;

    if (auto* slider = bus.amountSlider.get())
        slider->setBounds(x, y, amountSliderWidth, amountSliderHeight);
    x += amountSliderWidth + kSpacing;

    if (auto* comboBox = bus.destinationComboBox.get())
        comboBox->setBounds(x, y, destinationComboBoxWidth, destinationComboBoxHeight);
    x += destinationComboBoxWidth + kSpacing;

    if (auto* button = bus.initButton.get())
        button->setBounds(x, y, initButtonWidth, initButtonHeight);
}

void MatrixModulationPanel::positionModulationBusSeparator(ModulationBus& bus, int y, int separatorWidth, int separatorHeight)
{
    if (auto* separator = bus.separator.get())
        separator->setBounds(0, y, separatorWidth, separatorHeight);
}

void MatrixModulationPanel::setTheme(Theme& inTheme)
{
    theme = &inTheme;

    if (auto* header = sectionHeader.get())
        header->setTheme(inTheme);

    if (auto* busHeader = modulationBusHeader.get())
        busHeader->setTheme(inTheme);

    if (auto* button = initAllBussesButton.get())
        button->setTheme(inTheme);

    for (auto& bus : modulationBuses)
    {
        if (auto* label = bus.busNumberLabel.get())
            label->setTheme(inTheme);

        if (auto* comboBox = bus.sourceComboBox.get())
            comboBox->setTheme(inTheme);

        if (auto* slider = bus.amountSlider.get())
            slider->setTheme(inTheme);

        if (auto* comboBox = bus.destinationComboBox.get())
            comboBox->setTheme(inTheme);

        if (auto* button = bus.initButton.get())
            button->setTheme(inTheme);

        if (auto* separator = bus.separator.get())
            separator->setTheme(inTheme);
    }

    repaint();
}

