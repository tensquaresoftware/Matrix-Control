#include "ModulePanel.h"

#include "../../Widgets/ModuleName.h"
#include "../../Widgets/Button.h"
#include "../../Widgets/ParameterLabel.h"
#include "../../Widgets/Slider.h"
#include "../../Widgets/ComboBox.h"
#include "../../Widgets/ParameterSeparator.h"
#include "../../Skin/Skin.h"
#include "../../Skin/SkinDimensions.h"

using tss::Skin;
using tss::ModuleName;
using tss::Button;
using tss::ParameterLabel;
using tss::Slider;
using tss::ComboBox;
using tss::ParameterSeparator;

ModulePanel::ModulePanel(const juce::String& newModuleName,
                             const std::vector<juce::String>& newButtonLabels,
                             const std::vector<ParameterConfig>& newParameters,
                             Skin& newSkin)
    : skin(&newSkin)
    , moduleName(newModuleName)
    , buttonLabels(newButtonLabels)
    , parameters(newParameters)
{
    auto height = calculateHeight();
    setSize(kWidth, height);

    createHeader();

    for (size_t i = 0; i < parameters.size(); ++i)
    {
        createParameterRow(static_cast<int>(i), parameters[i]);
    }

        auto finalSeparator = std::make_unique<ParameterSeparator>(newSkin);
    addAndMakeVisible(*finalSeparator);
    separators.push_back(std::move(finalSeparator));
}

void ModulePanel::setSkin(Skin& newSkin)
{
        skin = &newSkin;

    if (name != nullptr)
    {
        name->setSkin(*skin);
    }

    for (auto& button : buttons)
    {
        if (button != nullptr)
        {
            button->setSkin(*skin);
        }
    }

    for (auto& label : labels)
    {
        if (label != nullptr)
        {
            label->setSkin(*skin);
        }
    }

    for (auto& control : controls)
    {
        if (auto* slider = dynamic_cast<Slider*>(control.get()))
        {
            slider->setSkin(*skin);
        }
        else if (auto* comboBox = dynamic_cast<ComboBox*>(control.get()))
        {
            comboBox->setSkin(*skin);
        }
    }

    for (auto& separator : separators)
    {
        if (separator != nullptr)
        {
            separator->setSkin(*skin);
        }
    }

    repaint();
}

void ModulePanel::resized()
{
    layoutHeader();
    layoutParameters();
}

void ModulePanel::paint(juce::Graphics& g)
{
        if (skin == nullptr)
        {
            return;
        }
        
        g.fillAll(skin->getPatchEditPanelBackgroundColour());
}

void ModulePanel::createHeader()
{
        name = std::make_unique<ModuleName>(*skin, moduleName, ModuleName::Size::Normal);
    addAndMakeVisible(*name);

    for (const auto& buttonLabel : buttonLabels)
    {
            auto button = std::make_unique<Button>(*skin, tss::SkinDimensions::Widget::Button::kDefaultWidth, buttonLabel);
        addAndMakeVisible(*button);
        buttons.push_back(std::move(button));
    }
}

void ModulePanel::createParameterRow(int /* index */, const ParameterConfig& config)
{
        auto label = std::make_unique<ParameterLabel>(*skin, config.label);
    addAndMakeVisible(*label);
    labels.push_back(std::move(label));

    std::unique_ptr<juce::Component> control;

    if (config.type == ParameterWidgetType::Slider)
    {
            auto slider = std::make_unique<Slider>(*skin, 0.0);
        control = std::move(slider);
    }
    else if (config.type == ParameterWidgetType::ComboBox)
    {
            auto comboBox = std::make_unique<ComboBox>(*skin, ComboBox::Size::Normal);
        control = std::move(comboBox);
    }

    if (control != nullptr)
    {
        addAndMakeVisible(*control);
        controls.push_back(std::move(control));
    }

        auto separator = std::make_unique<ParameterSeparator>(*skin);
    addAndMakeVisible(*separator);
    separators.push_back(std::move(separator));
}

void ModulePanel::layoutHeader()
{
    if (name == nullptr)
    {
        return;
    }

    auto headerBounds = getLocalBounds();
    headerBounds.setHeight(kHeaderHeight);

    name->setBounds(headerBounds);

    if (!buttons.empty())
    {
        int buttonWidth = tss::SkinDimensions::Widget::Button::kDefaultWidth;
        int buttonSpacing = tss::SkinDimensions::ModulePanel::kButtonSpacing;
        int totalButtonsWidth = static_cast<int>(buttons.size()) * buttonWidth + static_cast<int>(buttons.size() - 1) * buttonSpacing;

        if (totalButtonsWidth <= kWidth)
        {
            int x = kWidth - totalButtonsWidth;
            int y = headerBounds.getY();

            for (auto& button : buttons)
            {
                if (button != nullptr)
                {
                    button->setBounds(x, y, buttonWidth, kHeaderHeight);
                    x += buttonWidth + buttonSpacing;
                }
            }
        }
    }
}

void ModulePanel::layoutParameters()
{
    int parameterRowHeight = tss::SkinDimensions::ModulePanel::kParameterRowHeight;
    int labelWidth = tss::SkinDimensions::Widget::ParameterLabel::kWidth;
    int controlWidth = tss::SkinDimensions::Widget::Slider::kWidth;
    int separatorHeight = tss::SkinDimensions::Widget::ParameterSeparator::kHeight;
    int controlHeight = tss::SkinDimensions::Widget::ParameterLabel::kHeight;

    int y = kHeaderHeight;

    for (size_t i = 0; i < parameters.size(); ++i)
    {
        auto rowBounds = juce::Rectangle<int>(0, y, kWidth, parameterRowHeight);

        if (i < labels.size() && labels[i] != nullptr)
        {
            auto labelBounds = rowBounds.removeFromLeft(labelWidth);
            labelBounds.setHeight(controlHeight);
            labels[i]->setBounds(labelBounds);
        }

        if (i < controls.size() && controls[i] != nullptr)
        {
            auto controlBounds = rowBounds;
            controlBounds.setWidth(controlWidth);
            controlBounds.setHeight(controlHeight);
            controls[i]->setBounds(controlBounds);
        }

        if (i < separators.size() - 1 && separators[i] != nullptr)
        {
            auto separatorBounds = juce::Rectangle<int>(0, y + controlHeight, kWidth, separatorHeight);
            separators[i]->setBounds(separatorBounds);
        }

        y += parameterRowHeight;
    }

    if (!separators.empty() && separators.back() != nullptr)
    {
        auto finalSeparatorBounds = juce::Rectangle<int>(0, y, kWidth, separatorHeight);
        separators.back()->setBounds(finalSeparatorBounds);
    }
}

int ModulePanel::calculateHeight() const
{
    return (1 + static_cast<int>(parameters.size())) * kParameterRowHeight;
}

