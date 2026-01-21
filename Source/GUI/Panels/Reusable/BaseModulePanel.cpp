#include "BaseModulePanel.h"

#include "GUI/Themes/Theme.h"
#include "GUI/Panels/Reusable/ModuleHeaderPanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "GUI/Factories/WidgetFactory.h"

using tss::Theme;

BaseModulePanel::BaseModulePanel(Theme& theme,
                                 WidgetFactory& widgetFactory,
                                 juce::AudioProcessorValueTreeState& apvts,
                                 const ModulePanelConfig& config,
                                 int width,
                                 int height)
    : theme_(&theme)
    , apvts_(apvts)
    , moduleType_(config.moduleType)
{
    setOpaque(true);
    
    ModuleHeaderPanel::ButtonSet buttonSet = (config.buttonSet == ModulePanelButtonSet::InitCopyPaste)
        ? ModuleHeaderPanel::ButtonSet::InitCopyPaste
        : ModuleHeaderPanel::ButtonSet::InitOnly;
    
    ModuleHeaderPanel::ModuleType moduleType = (config.moduleType == ModulePanelModuleType::PatchEdit)
        ? ModuleHeaderPanel::ModuleType::PatchEdit
        : ModuleHeaderPanel::ModuleType::MasterEdit;
    
    moduleHeaderPanel_ = std::make_unique<ModuleHeaderPanel>(
        theme,
        widgetFactory,
        config.moduleId,
        buttonSet,
        moduleType,
        apvts_,
        config.initWidgetId,
        config.copyWidgetId,
        config.pasteWidgetId);
    addAndMakeVisible(*moduleHeaderPanel_);

    for (const auto& paramConfig : config.parameters)
    {
        ParameterPanel::ParameterType paramType = ParameterPanel::ParameterType::None;
        if (paramConfig.parameterType == ModulePanelParameterType::Slider)
            paramType = ParameterPanel::ParameterType::Slider;
        else if (paramConfig.parameterType == ModulePanelParameterType::ComboBox)
            paramType = ParameterPanel::ParameterType::ComboBox;
        
        ParameterPanel::ModuleType modType = (config.moduleType == ModulePanelModuleType::PatchEdit)
            ? ParameterPanel::ModuleType::PatchEdit
            : ParameterPanel::ModuleType::MasterEdit;
        
        parameterPanels_.push_back(std::make_unique<ParameterPanel>(
            theme,
            widgetFactory,
            paramConfig.parameterId,
            paramType,
            modType,
            apvts_));
        addAndMakeVisible(*parameterPanels_.back());
    }

    setSize(width, height);
}

BaseModulePanel::~BaseModulePanel() = default;

void BaseModulePanel::paint(juce::Graphics& g)
{
    if (theme_ == nullptr)
    {
        return;
    }

    const auto backgroundColour = (moduleType_ == ModulePanelModuleType::PatchEdit)
        ? theme_->getPatchEditModulePanelBackgroundColour()
        : theme_->getMasterEditPanelBackgroundColour();

    g.fillAll(backgroundColour);
}

void BaseModulePanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeaderPanel_.get())
        header->setBounds(bounds.removeFromTop(header->getHeight()));

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setBounds(bounds.removeFromTop(paramPanel->getTotalHeight()));
}

void BaseModulePanel::setTheme(Theme& theme)
{
    theme_ = &theme;

    if (auto* header = moduleHeaderPanel_.get())
        header->setTheme(theme);

    for (auto& paramPanel : parameterPanels_)
        if (paramPanel != nullptr)
            paramPanel->setTheme(theme);

    repaint();
}
