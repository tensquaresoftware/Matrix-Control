#pragma once

#include <vector>
#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include "../../Skin/SkinDimensions.h"

namespace tss
{
    class Skin;
    class ModuleName;
    class Button;
    class ParameterLabel;
    class Slider;
    class ComboBox;
    class ParameterSeparator;
}

using tss::Skin;
using tss::ModuleName;
using tss::Button;
using tss::ParameterLabel;
using tss::Slider;
using tss::ComboBox;
using tss::ParameterSeparator;

enum class ParameterWidgetType
{
    Slider,
    ComboBox
};

struct ParameterConfig
{
    juce::String label;
    juce::String id;
    ParameterWidgetType type;
};

class ModulePanel : public juce::Component
{
public:
    explicit ModulePanel(const juce::String& moduleName,
                           const std::vector<juce::String>& buttonLabels,
                           const std::vector<ParameterConfig>& parameters,
                           Skin& newSkin);
    ~ModulePanel() override = default;

    void setSkin(Skin& skin);
    void resized() override;
    void paint(juce::Graphics& g) override;

protected:
    virtual void createHeader();
    virtual void createParameterRow(int index, const ParameterConfig& config);

private:
    void layoutHeader();
    void layoutParameters();
    int calculateHeight() const;

    static constexpr int kWidth = tss::SkinDimensions::ModulePanel::kWidth;
    static constexpr int kHeaderHeight = tss::SkinDimensions::ModulePanel::kHeaderHeight;
    static constexpr int kParameterRowHeight = tss::SkinDimensions::ModulePanel::kParameterRowHeight;

    Skin* skin;
    std::unique_ptr<ModuleName> name;
    std::vector<std::unique_ptr<Button>> buttons;
    std::vector<std::unique_ptr<ParameterLabel>> labels;
    std::vector<std::unique_ptr<juce::Component>> controls;
    std::vector<std::unique_ptr<ParameterSeparator>> separators;

    juce::String moduleName;
    std::vector<juce::String> buttonLabels;
    std::vector<ParameterConfig> parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulePanel)
};

