#pragma once

#include <memory>
#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Panels/Reusable/ModulationBusPanel.h"

namespace tss
{
    class Theme;
    class SectionHeader;
    class ModulationBusHeader;
    class Button;
}

class WidgetFactory;

class MatrixModulationPanel : public juce::Component
{
public:
    MatrixModulationPanel(tss::Theme& theme, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts);
    ~MatrixModulationPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void setTheme(tss::Theme& theme);

    static int getWidth() { return PluginDimensions::Panels::MatrixModulationPanel::kWidth; }
    static int getHeight() { return PluginDimensions::Panels::MatrixModulationPanel::kHeight; }

private:
    struct ModulationBusParameterArrays
    {
        std::array<const char*, PluginDescriptors::kModulationBusCount> busIds;
        std::array<const char*, PluginDescriptors::kModulationBusCount> sourceParameterIds;
        std::array<const char*, PluginDescriptors::kModulationBusCount> amountParameterIds;
        std::array<const char*, PluginDescriptors::kModulationBusCount> destinationParameterIds;
    };

    std::array<const char*, PluginDescriptors::kModulationBusCount> createBusIds() const;
    std::array<const char*, PluginDescriptors::kModulationBusCount> createSourceParameterIds() const;
    std::array<const char*, PluginDescriptors::kModulationBusCount> createAmountParameterIds() const;
    std::array<const char*, PluginDescriptors::kModulationBusCount> createDestinationParameterIds() const;
    ModulationBusParameterArrays createModulationBusParameterArrays() const;

    void createInitAllBussesButton(tss::Theme& theme);

    tss::Theme* theme_;
    juce::AudioProcessorValueTreeState& apvts_;

    std::unique_ptr<tss::SectionHeader> sectionHeader_;
    std::unique_ptr<tss::ModulationBusHeader> modulationBusHeader_;
    std::unique_ptr<tss::Button> initAllBussesButton_;
    std::vector<std::unique_ptr<ModulationBusPanel>> modulationBuses_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatrixModulationPanel)
};

