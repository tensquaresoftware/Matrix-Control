#include "BodyPanel.h"

#include "GUI/Layout/ScaledLayout.h"
#include "GUI/Skins/ISkin.h"
#include "GUI/Skins/SkinHelpers.h"
#include "GUI/Widgets/ModuleHeader.h"
#include "GUI/Helpers/CompareLockBinder.h"
#include "PatchEditPanel/PatchEditPanel.h"
#include "MasterEditPanel/MasterEditPanel.h"
#include "SharedPanel/SharedPanel.h"
#include "GUI/Factories/WidgetFactory.h"

BodyPanel::BodyPanel(TSS::ISkin& skin,
                     const GuiLayoutDimensions& layoutDimensions,
                     WidgetFactory& widgetFactory,
                     juce::AudioProcessorValueTreeState& apvts,
                     const Core::PatchFileService& patchFileService)
    : dims_(layoutDimensions.body)
    , skin_(&skin)
{
    setOpaque(false);
    patchEditPanel_ = std::make_unique<PatchEditPanel>(
        skin, dims_.patchEdit, layoutDimensions.patchEditParameterCell, layoutDimensions.patchEditModuleHeader, widgetFactory, apvts);
    addAndMakeVisible(*patchEditPanel_);

    sharedPanel_ = std::make_unique<SharedPanel>(skin, dims_.shared, widgetFactory, apvts, patchFileService);
    addAndMakeVisible(*sharedPanel_);

    masterEditPanel_ = std::make_unique<MasterEditPanel>(skin, dims_.masterEdit, widgetFactory, apvts);
    addAndMakeVisible(*masterEditPanel_);

    compareLockBinder_ = std::make_unique<TSS::CompareLockBinder>(
        apvts,
        std::vector<juce::Component*>{ patchEditPanel_.get(), masterEditPanel_.get() });
}

BodyPanel::~BodyPanel() = default;

void BodyPanel::paint(juce::Graphics&)
{
}

void BodyPanel::resized()
{
    auto area = getLocalBounds();
    const float sf = uiScale_;

    const int patchEditPanelWidth  = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.patchEditWidth), sf);
    const int patchEditPanelHeight = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.patchEditHeight), sf);
    const int sharedColumnW        = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.sharedColumnWidth), sf);
    const int masterEditW          = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.masterEditWidth), sf);
    const int masterEditH          = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.masterEditHeight), sf);
    const int sharedColumnHeight   = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.sharedColumnHeight), sf);
    const int interColumnGap       = TSS::ScaledLayout::scaledInt(static_cast<float>(dims_.interColumnGap), sf);

    const auto patchEditBounds = area.removeFromLeft(patchEditPanelWidth);
    patchEditPanel_->setBounds(patchEditBounds.getX(), 0, patchEditBounds.getWidth(), patchEditPanelHeight);

    area.removeFromLeft(interColumnGap);

    const auto sharedBounds = area.removeFromLeft(sharedColumnW);
    sharedPanel_->setBounds(sharedBounds.getX(), 0, sharedBounds.getWidth(), sharedColumnHeight);

    area.removeFromLeft(interColumnGap);

    const auto masterEditBounds = area.removeFromLeft(masterEditW);
    masterEditPanel_->setBounds(masterEditBounds.getX(), 0, masterEditBounds.getWidth(), masterEditH);
}

void BodyPanel::setSkin(TSS::ISkin& skin)
{
    skin_ = &skin;
    TSS::propagateSkin(skin,
        patchEditPanel_.get(),
        sharedPanel_.get(),
        masterEditPanel_.get());
}

void BodyPanel::setUiScale(float uiScale)
{
    if (juce::approximatelyEqual(uiScale_, uiScale))
        return;
    
    uiScale_ = uiScale;
    
    if (patchEditPanel_)
        patchEditPanel_->setUiScale(uiScale_);
    if (sharedPanel_)
        sharedPanel_->setUiScale(uiScale_);
    if (masterEditPanel_)
        masterEditPanel_->setUiScale(uiScale_);
    
    resized();
    repaint();
}

void BodyPanel::setBusReorderHandler(BusReorderHandler handler)
{
    if (sharedPanel_ != nullptr)
        sharedPanel_->setBusReorderHandler(std::move(handler));
}

void BodyPanel::setMasterInitConfirmationGate(TSS::ModuleHeader::InitConfirmationGate gate)
{
    if (masterEditPanel_ != nullptr)
        masterEditPanel_->setInitConfirmationGate(std::move(gate));
}
