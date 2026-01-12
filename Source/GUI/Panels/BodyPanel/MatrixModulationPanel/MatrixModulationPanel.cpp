#include "MatrixModulationPanel.h"

#include "../../../Themes/Theme.h"
#include "../../../Widgets/SectionHeader.h"
#include "../../../Widgets/ModulationBusHeader.h"
#include "../../../Widgets/Label.h"
#include "../../../Widgets/ComboBox.h"
#include "../../../Widgets/Slider.h"
#include "../../../Widgets/Button.h"
#include "../../../Widgets/HorizontalSeparator.h"
#include "../../../../Shared/PluginDescriptors.h"
#include "../../../../GUI/Factories/WidgetFactory.h"

using tss::Theme;

MatrixModulationPanel::~MatrixModulationPanel() = default;

MatrixModulationPanel::MatrixModulationPanel(Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& inApvts)
    : theme(&inTheme)
    , apvts(inApvts)
    , sectionHeader(std::make_unique<tss::SectionHeader>(
        inTheme,
        tss::SectionHeader::SectionWidth::MatrixModulation,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kMatrixModulation)))
    , modulationBusHeader(std::make_unique<tss::ModulationBusHeader>(inTheme))
{
    addAndMakeVisible(*sectionHeader);
    addAndMakeVisible(*modulationBusHeader);

    const auto parameterArrays = createModulationBusParameterArrays();

    createInitAllBussesButton(inTheme);

    for (int busNumber = 0; busNumber < PluginDescriptors::kModulationBusCount; ++busNumber)
    {
        createModulationBus(busNumber, widgetFactory, inTheme, parameterArrays);
    }

    setSize(getWidth(), getHeight());
}

std::array<const char*, PluginDescriptors::kModulationBusCount> MatrixModulationPanel::createBusIds() const
{
    return {
        PluginDescriptors::ModulationBusIds::kModulationBus0,
        PluginDescriptors::ModulationBusIds::kModulationBus1,
        PluginDescriptors::ModulationBusIds::kModulationBus2,
        PluginDescriptors::ModulationBusIds::kModulationBus3,
        PluginDescriptors::ModulationBusIds::kModulationBus4,
        PluginDescriptors::ModulationBusIds::kModulationBus5,
        PluginDescriptors::ModulationBusIds::kModulationBus6,
        PluginDescriptors::ModulationBusIds::kModulationBus7,
        PluginDescriptors::ModulationBusIds::kModulationBus8,
        PluginDescriptors::ModulationBusIds::kModulationBus9
    };
}

std::array<const char*, PluginDescriptors::kModulationBusCount> MatrixModulationPanel::createSourceParameterIds() const
{
    return {
        PluginDescriptors::ParameterIds::kModulationBus0Source,
        PluginDescriptors::ParameterIds::kModulationBus1Source,
        PluginDescriptors::ParameterIds::kModulationBus2Source,
        PluginDescriptors::ParameterIds::kModulationBus3Source,
        PluginDescriptors::ParameterIds::kModulationBus4Source,
        PluginDescriptors::ParameterIds::kModulationBus5Source,
        PluginDescriptors::ParameterIds::kModulationBus6Source,
        PluginDescriptors::ParameterIds::kModulationBus7Source,
        PluginDescriptors::ParameterIds::kModulationBus8Source,
        PluginDescriptors::ParameterIds::kModulationBus9Source
    };
}

std::array<const char*, PluginDescriptors::kModulationBusCount> MatrixModulationPanel::createAmountParameterIds() const
{
    return {
        PluginDescriptors::ParameterIds::kModulationBus0Amount,
        PluginDescriptors::ParameterIds::kModulationBus1Amount,
        PluginDescriptors::ParameterIds::kModulationBus2Amount,
        PluginDescriptors::ParameterIds::kModulationBus3Amount,
        PluginDescriptors::ParameterIds::kModulationBus4Amount,
        PluginDescriptors::ParameterIds::kModulationBus5Amount,
        PluginDescriptors::ParameterIds::kModulationBus6Amount,
        PluginDescriptors::ParameterIds::kModulationBus7Amount,
        PluginDescriptors::ParameterIds::kModulationBus8Amount,
        PluginDescriptors::ParameterIds::kModulationBus9Amount
    };
}

std::array<const char*, PluginDescriptors::kModulationBusCount> MatrixModulationPanel::createDestinationParameterIds() const
{
    return {
        PluginDescriptors::ParameterIds::kModulationBus0Destination,
        PluginDescriptors::ParameterIds::kModulationBus1Destination,
        PluginDescriptors::ParameterIds::kModulationBus2Destination,
        PluginDescriptors::ParameterIds::kModulationBus3Destination,
        PluginDescriptors::ParameterIds::kModulationBus4Destination,
        PluginDescriptors::ParameterIds::kModulationBus5Destination,
        PluginDescriptors::ParameterIds::kModulationBus6Destination,
        PluginDescriptors::ParameterIds::kModulationBus7Destination,
        PluginDescriptors::ParameterIds::kModulationBus8Destination,
        PluginDescriptors::ParameterIds::kModulationBus9Destination
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
    const auto& destinationDesc = PluginDescriptors::kModulationBusChoiceParameters[busNumberAsSizeT][1];
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

    for (int busNumber = 0; busNumber < PluginDescriptors::kModulationBusCount; ++busNumber)
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

