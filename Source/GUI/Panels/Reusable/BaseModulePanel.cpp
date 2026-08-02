#include "BaseModulePanel.h"

#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Panels/Reusable/ModulePanelConfigBuilder.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Widgets/ParameterCell.h"
#include "GUI/Factories/WidgetFactory.h"

namespace
{
TSS::ModuleHeader::ButtonSet toHeaderButtonSet(ModulePanelButtonSet buttonSet)
{
    return (buttonSet == ModulePanelButtonSet::InitCopyPaste)
        ? TSS::ModuleHeader::ButtonSet::InitCopyPaste
        : TSS::ModuleHeader::ButtonSet::InitOnly;
}

TSS::ModuleHeader::ColumnLayout toHeaderColumnLayout(ModulePanelModuleType moduleType)
{
    return (moduleType == ModulePanelModuleType::PatchEdit)
        ? TSS::ModuleHeader::ColumnLayout::PatchEdit
        : TSS::ModuleHeader::ColumnLayout::MasterEdit;
}

ParameterCell::ModuleType toParameterCellModuleType(ModulePanelModuleType moduleType)
{
    return (moduleType == ModulePanelModuleType::PatchEdit)
        ? ParameterCell::ModuleType::PatchEdit
        : ParameterCell::ModuleType::MasterEdit;
}
} // namespace

BaseModulePanel::BaseModulePanel(const Config& config)
    : skin_(&config.skin)
    , apvts_(config.apvts)
    , moduleType_(config.layout.moduleType)
    , moduleHeaderDims_(config.moduleHeaderDims)
    , parameterCellDims_(config.parameterCellDims)
{
    const auto moduleConfig = buildModulePanelConfig(config.layout);

    setOpaque(false);
    createHeader(config, moduleConfig);
    createParameterCells(config, moduleConfig);
    setSize(config.width, config.height);
}

void BaseModulePanel::createHeader(const Config& panelConfig, const ModulePanelConfig& moduleConfig)
{
    const TSS::ModuleHeader::WithActionsSpec spec {
        panelConfig.skin,
        panelConfig.widgetFactory,
        panelConfig.apvts,
        panelConfig.moduleHeaderDims,
        moduleConfig.moduleId,
        toHeaderColumnLayout(moduleConfig.moduleType),
        toHeaderButtonSet(moduleConfig.buttonSet),
        moduleConfig.initWidgetId,
        moduleConfig.copyWidgetId,
        moduleConfig.pasteWidgetId,
        moduleConfig.pasteEnabledPropertyId,
        moduleConfig.moduleType == ModulePanelModuleType::MasterEdit,
        initConfirmationGate_
    };
    moduleHeader_ = std::make_unique<TSS::ModuleHeader>(spec);
    addAndMakeVisible(*moduleHeader_);
}

void BaseModulePanel::createParameterCells(const Config& panelConfig,
                                           const ModulePanelConfig& moduleConfig)
{
    const auto modType = toParameterCellModuleType(moduleConfig.moduleType);

    for (const auto& paramConfig : moduleConfig.parameters)
    {
        parameterCells_.push_back(std::make_unique<ParameterCell>(ParameterCell::Config{
            .skin = panelConfig.skin,
            .factory = panelConfig.widgetFactory,
            .parameterId = paramConfig.parameterId,
            .type = paramConfig.parameterType,
            .moduleType = modType,
            .apvts = apvts_,
            .dimensions = panelConfig.parameterCellDims}));
        addAndMakeVisible(*parameterCells_.back());
    }
}

BaseModulePanel::~BaseModulePanel() = default;

void BaseModulePanel::resized()
{
    auto bounds = getLocalBounds();

    if (auto* header = moduleHeader_.get())
    {
        const int headerHeight = TSS::ScaledLayout::scaledInt(
            static_cast<float>(moduleHeaderDims_.height), uiScale_);
        header->setBounds(bounds.removeFromTop(headerHeight));
    }

    const size_t paramCount = parameterCells_.size();
    if (paramCount == 0)
        return;

    const int rowHeight = TSS::ScaledLayout::scaledInt(
        static_cast<float>(parameterCellDims_.rowHeight), uiScale_);

    for (size_t i = 0; i < paramCount; ++i)
    {
        if (auto* cell = parameterCells_[i].get())
            cell->setBounds(bounds.removeFromTop(rowHeight));
        else
            bounds.removeFromTop(rowHeight);
    }
}

void BaseModulePanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    TSS::propagateSkin(skin, moduleHeader_.get());

    for (auto& cell : parameterCells_)
        TSS::propagateSkin(skin, cell.get());
}

void BaseModulePanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;

    uiScale_ = uiScale;

    if (moduleHeader_)
        moduleHeader_->setUiScale(uiScale_);

    for (auto& cell : parameterCells_)
    {
        if (cell)
            cell->setUiScale(uiScale_);
    }

    resized();
    repaint();
}

void BaseModulePanel::setInitConfirmationGate(TSS::ModuleHeader::InitConfirmationGate gate)
{
    initConfirmationGate_ = std::move(gate);
    if (moduleHeader_)
        moduleHeader_->setInitConfirmationGate(initConfirmationGate_);
}

ParameterCell* BaseModulePanel::getParameterCellAt(size_t index)
{
    if (index >= parameterCells_.size())
        return nullptr;

    return parameterCells_[index].get();
}
