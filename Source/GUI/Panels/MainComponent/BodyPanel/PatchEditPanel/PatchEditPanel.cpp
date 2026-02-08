#include "PatchEditPanel.h"

#include "TopPanel/TopPanel.h"
#include "TopPanel/Modules/FmTrackPanel.h"
#include "MiddlePanel/MiddlePanel.h"
#include "BottomPanel/BottomPanel.h"

#include "GUI/Themes/Skin.h"
#include "GUI/Widgets/SectionHeader.h"
#include "GUI/Panels/Reusable/BaseModulePanel.h"
#include "GUI/Panels/Reusable/ParameterPanel.h"
#include "GUI/Widgets/Slider.h"
#include "Shared/PluginDescriptors.h"
#include "Shared/PluginDimensions.h"
#include "GUI/Factories/WidgetFactory.h"


PatchEditPanel::~PatchEditPanel()
{
    for (auto* slider : trackPointSliders_)
    {
        if (slider != nullptr)
            slider->removeListener(this);
    }
}

PatchEditPanel::PatchEditPanel(tss::Skin& skin, WidgetFactory& widgetFactory, juce::AudioProcessorValueTreeState& apvts)
    : skin_(&skin)
    , sectionHeader_(std::make_unique<tss::SectionHeader>(
        skin, 
        PluginDimensions::Widgets::Widths::SectionHeader::kPatchEdit,
        PluginDimensions::Widgets::Heights::kSectionHeader,
        PluginDescriptors::getSectionDisplayName(PluginDescriptors::SectionIds::kPatchEdit)))
    , topPanel_(std::make_unique<TopPanel>(skin, widgetFactory, apvts))
    , middlePanel_(std::make_unique<MiddlePanel>(skin, apvts))
    , bottomPanel_(std::make_unique<BottomPanel>(skin, widgetFactory, apvts))
{
    setOpaque(false);
    addAndMakeVisible(*sectionHeader_);
    addAndMakeVisible(*topPanel_);
    addAndMakeVisible(*middlePanel_);
    addAndMakeVisible(*bottomPanel_);

    setupTrackPointSliderConnections();

    setSize(getWidth(), getHeight());
}

void PatchEditPanel::resized()
{
    const auto bounds = getLocalBounds();
    
    const auto sectionHeaderY = 0;
    sectionHeader_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + sectionHeaderY,
        bounds.getWidth(),
        PluginDimensions::Widgets::Heights::kSectionHeader
    );
    
    const auto topPanelY = sectionHeaderY + PluginDimensions::Widgets::Heights::kSectionHeader;
    topPanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + topPanelY,
        bounds.getWidth(),
        TopPanel::getHeight()
    );
    
    const auto middlePanelY = topPanelY + TopPanel::getHeight();
    middlePanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + middlePanelY,
        bounds.getWidth(),
        MiddlePanel::getHeight()
    );
    
    const auto bottomPanelY = middlePanelY + MiddlePanel::getHeight();
    bottomPanel_->setBounds(
        bounds.getX() + 0,
        bounds.getY() + bottomPanelY,
        bounds.getWidth(),
        BottomPanel::getHeight()
    );
}

void PatchEditPanel::setSkin(tss::Skin& skin)
{
    skin_ = &skin;

    if (auto* header = sectionHeader_.get())
        header->setSkin(skin);

    if (auto* panel = topPanel_.get())
        panel->setSkin(skin);

    if (auto* panel = middlePanel_.get())
        panel->setSkin(skin);

    if (auto* panel = bottomPanel_.get())
        panel->setSkin(skin);
}

void PatchEditPanel::setupTrackPointSliderConnections()
{
    if (topPanel_ == nullptr || middlePanel_ == nullptr)
        return;
    
    auto* fmTrackPanel = topPanel_->getFmTrackPanel();
    if (fmTrackPanel == nullptr)
        return;
    
    for (int i = 0; i < kTrackPointSliderCount_; ++i)
    {
        const size_t parameterPanelIndex = static_cast<size_t>(kTrackPointSliderStartIndex_ + i);
        
        if (auto* paramPanel = fmTrackPanel->getParameterPanelAt(parameterPanelIndex))
        {
            if (auto* slider = paramPanel->getSlider())
            {
                trackPointSliders_[static_cast<size_t>(i)] = slider;
                slider->addListener(this);
            }
        }
    }
    
    middlePanel_->getTrackGeneratorDisplay().setOnValueChanged([this](int pointIndex, int newValue)
    {
        if (pointIndex < 0 || pointIndex >= kTrackPointSliderCount_)
            return;
        
        if (auto* slider = trackPointSliders_[static_cast<size_t>(pointIndex)])
            slider->setValue(static_cast<double>(newValue), juce::sendNotificationSync);
    });
}

void PatchEditPanel::sliderValueChanged(juce::Slider* slider)
{
    if (middlePanel_ == nullptr)
        return;
    
    for (size_t i = 0; i < trackPointSliders_.size(); ++i)
    {
        if (trackPointSliders_[i] == slider)
        {
            const int value = static_cast<int>(slider->getValue());
            auto& display = middlePanel_->getTrackGeneratorDisplay();
            
            switch (static_cast<int>(i))
            {
                case 0: display.setTrackPoint1(value, false); break;
                case 1: display.setTrackPoint2(value, false); break;
                case 2: display.setTrackPoint3(value, false); break;
                case 3: display.setTrackPoint4(value, false); break;
                case 4: display.setTrackPoint5(value, false); break;
                default: break;
            }
            
            break;
        }
    }
}

