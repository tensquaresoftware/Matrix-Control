#include "PatchEditTopModulesPanel.h"

#include "Modules/Dco1Panel.h"
#include "Modules/Dco2Panel.h"
#include "Modules/VcfVcaPanel.h"
#include "Modules/FmTrackPanel.h"
#include "Modules/RampPortamentoPanel.h"

#include "GUI/Panels/MainComponent/BodyPanel/PatchEditPanel/PatchEditFiveColumnLayout.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "GUI/Factories/WidgetFactory.h"


PatchEditTopModulesPanel::~PatchEditTopModulesPanel() = default;

PatchEditTopModulesPanel::PatchEditTopModulesPanel(const Config& config)
    : rowDims_(config.rowDims)
    , width_(config.width)
    , height_(config.height)
    , skin_(&config.skin)
    , dco1Panel_(std::make_unique<Dco1Panel>(Dco1Panel::Config{
          .skin = config.skin,
          .width = rowDims_.childModuleWidth,
          .height = rowDims_.childModuleHeight,
          .widgetFactory = config.widgetFactory,
          .apvts = config.apvts,
          .moduleHeaderDims = config.moduleHeaderDims,
          .parameterCellDims = config.parameterCellDims}))
    , dco2Panel_(std::make_unique<Dco2Panel>(Dco2Panel::Config{
          .skin = config.skin,
          .width = rowDims_.childModuleWidth,
          .height = rowDims_.childModuleHeight,
          .widgetFactory = config.widgetFactory,
          .apvts = config.apvts,
          .moduleHeaderDims = config.moduleHeaderDims,
          .parameterCellDims = config.parameterCellDims}))
    , vcfVcaPanel_(std::make_unique<VcfVcaPanel>(config.skin, rowDims_.childModuleWidth, rowDims_.childModuleHeight, config.widgetFactory, config.apvts, config.moduleHeaderDims, config.parameterCellDims))
    , fmTrackPanel_(std::make_unique<FmTrackPanel>(FmTrackPanel::Config{
          .skin = config.skin,
          .width = rowDims_.childModuleWidth,
          .height = rowDims_.childModuleHeight,
          .widgetFactory = config.widgetFactory,
          .apvts = config.apvts,
          .moduleHeaderDims = config.moduleHeaderDims,
          .parameterCellDims = config.parameterCellDims}))
    , rampPortamentoPanel_(std::make_unique<RampPortamentoPanel>(config.skin, rowDims_.childModuleWidth, rowDims_.childModuleHeight, config.widgetFactory, config.apvts, config.moduleHeaderDims, config.parameterCellDims))
{
    setOpaque(false);
    addAndMakeVisible(*dco1Panel_);
    addAndMakeVisible(*dco2Panel_);
    addAndMakeVisible(*vcfVcaPanel_);
    addAndMakeVisible(*fmTrackPanel_);
    addAndMakeVisible(*rampPortamentoPanel_);

    setSize(width_, height_);
}

void PatchEditTopModulesPanel::resized()
{
    TSS::layoutPatchEditFiveColumns(
        getLocalBounds(),
        uiScale_,
        rowDims_.childModuleWidth,
        rowDims_.childModuleHeight,
        rowDims_.interModuleGap,
        {
            dco1Panel_.get(),
            dco2Panel_.get(),
            vcfVcaPanel_.get(),
            fmTrackPanel_.get(),
            rampPortamentoPanel_.get()
        });
}

void PatchEditTopModulesPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    TSS::propagateSkin(skin,
        dco1Panel_.get(),
        dco2Panel_.get(),
        vcfVcaPanel_.get(),
        fmTrackPanel_.get(),
        rampPortamentoPanel_.get());
}

void PatchEditTopModulesPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;
    
    uiScale_ = uiScale;
    
    if (dco1Panel_)
        dco1Panel_->setUiScale(uiScale_);
    if (dco2Panel_)
        dco2Panel_->setUiScale(uiScale_);
    if (vcfVcaPanel_)
        vcfVcaPanel_->setUiScale(uiScale_);
    if (fmTrackPanel_)
        fmTrackPanel_->setUiScale(uiScale_);
    if (rampPortamentoPanel_)
        rampPortamentoPanel_->setUiScale(uiScale_);
    
    resized();
    repaint();
}
