#pragma once

#include <memory>
#include <array>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "../../../../Shared/SynthDescriptors.h"

namespace tss
{
    class Theme;
    class SectionHeader;
    class ModulationBusHeader;
    class Label;
    class ComboBox;
    class Slider;
    class Button;
    class ParameterSeparator;
}

class WidgetFactory;

class MatrixModulationPanel : public juce::Component
{
public:
    MatrixModulationPanel(tss::Theme& inTheme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MatrixModulationPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& inTheme);

    static int getWidth() { return kWidth; }
    static int getHeight() { return kHeight; }

private:
    struct ModulationBus
    {
        std::unique_ptr<tss::Label> busNumberLabel;
        std::unique_ptr<tss::ComboBox> sourceComboBox;
        std::unique_ptr<tss::Slider> amountSlider;
        std::unique_ptr<tss::ComboBox> destinationComboBox;
        std::unique_ptr<tss::Button> initButton;
        std::unique_ptr<tss::ParameterSeparator> separator;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sourceAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> destinationAttachment;
    };

    struct ModulationBusParameterArrays
    {
        std::array<const char*, SynthDescriptors::kModulationBusCount> busIds;
        std::array<const char*, SynthDescriptors::kModulationBusCount> sourceParameterIds;
        std::array<const char*, SynthDescriptors::kModulationBusCount> amountParameterIds;
        std::array<const char*, SynthDescriptors::kModulationBusCount> destinationParameterIds;
    };

    inline constexpr static int kWidth = 275;
    inline constexpr static int kHeight = 325;
    inline constexpr static int kSpacing = 5;

        std::array<const char*, SynthDescriptors::kModulationBusCount> createBusIds() const;
    std::array<const char*, SynthDescriptors::kModulationBusCount> createSourceParameterIds() const;
    std::array<const char*, SynthDescriptors::kModulationBusCount> createAmountParameterIds() const;
    std::array<const char*, SynthDescriptors::kModulationBusCount> createDestinationParameterIds() const;    
    ModulationBusParameterArrays createModulationBusParameterArrays() const;
    
    void createInitAllBussesButton(tss::Theme& inTheme);
    void createModulationBus(int busNumber, WidgetFactory& widgetFactory, tss::Theme& inTheme, const ModulationBusParameterArrays& parameterArrays);
    
    void createBusNumberLabel(ModulationBus& bus, int busNumber, tss::Theme& inTheme);
    void createSourceComboBox(ModulationBus& bus, size_t busNumberAsSizeT, WidgetFactory& widgetFactory, tss::Theme& inTheme, const ModulationBusParameterArrays& parameterArrays);
    void createAmountSlider(ModulationBus& bus, size_t busNumberAsSizeT, WidgetFactory& widgetFactory, tss::Theme& inTheme, const ModulationBusParameterArrays& parameterArrays);
    void createDestinationComboBox(ModulationBus& bus, size_t busNumberAsSizeT, tss::Theme& inTheme, const ModulationBusParameterArrays& parameterArrays);
    void createInitButton(ModulationBus& bus, size_t busNumberAsSizeT, tss::Theme& inTheme, const ModulationBusParameterArrays& parameterArrays);
    void createBusSeparator(ModulationBus& bus, tss::Theme& inTheme);

    tss::Theme* theme;
    juce::AudioProcessorValueTreeState& apvts;

    std::unique_ptr<tss::SectionHeader> sectionHeader;
    std::unique_ptr<tss::ModulationBusHeader> modulationBusHeader;
    std::unique_ptr<tss::Button> initAllBussesButton;
    std::array<ModulationBus, SynthDescriptors::kModulationBusCount> modulationBuses;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatrixModulationPanel)
};

