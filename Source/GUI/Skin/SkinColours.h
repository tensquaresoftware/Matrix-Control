#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "SkinColourChart.h"

namespace tss
{
    namespace SkinColours
    {
        struct ColourElement
        {
            const char* elementName;
            juce::uint32 blackVariant;
            juce::uint32 creamVariant;
            juce::uint32 debugVariant;
        };

        namespace Gui
        {
            inline constexpr ColourElement kGuiBackground = {
                "GuiBackground",
                SkinColourChart::kBlack,
                SkinColourChart::kWhite,
                SkinColourChart::kBlack
            };
        }

        namespace Panel
        {
            inline constexpr ColourElement kHeaderPanelBackground = {
                "HeaderPanelBackground",
                SkinColourChart::kLightGrey1,
                SkinColourChart::kLightGrey1,
                SkinColourChart::kWhite
            };

            inline constexpr ColourElement kMainPanelBackground = {
                "MainPanelBackground",
                SkinColourChart::kDarkGrey2,
                SkinColourChart::kCream,
                SkinColourChart::kBlack
            };

            inline constexpr ColourElement kFooterPanelBackground = {
                "FooterPanelBackground",
                SkinColourChart::kLightGrey1,
                SkinColourChart::kLightGrey1,
                SkinColourChart::kWhite
            };

            inline constexpr ColourElement kPatchEditPanelBackground = {
                "PatchEditPanelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kMatrixModulationPanelBackground = {
                "MatrixModulationPanelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kPatchManagerPanelBackground = {
                "PatchManagerPanelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kMasterEditPanelBackground = {
                "MasterEditPanelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kPatchEditModulePanelBackground = {
                "PatchEditModulePanelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kOrange
            };
        }

        namespace SectionName
        {
            inline constexpr ColourElement kBase = {
                "SectionNameBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kContentArea = {
                "SectionNameContentArea",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "SectionNameText",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kLineBlue = {
                "SectionNameLineBlue",
                SkinColourChart::kBlue,
                SkinColourChart::kBlue,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kLineOrange = {
                "SectionNameLineOrange",
                SkinColourChart::kOrange,
                SkinColourChart::kOrange,
                SkinColourChart::kYellow
            };
        }

        namespace ModuleName
        {
            inline constexpr ColourElement kBase = {
                "ModuleNameBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kText = {
                "ModuleNameText",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kLineBlue = {
                "ModuleNameLineBlue",
                SkinColourChart::kBlue,
                SkinColourChart::kBlue,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kLineOrange = {
                "ModuleNameLineOrange",
                SkinColourChart::kOrange,
                SkinColourChart::kOrange,
                SkinColourChart::kYellow
            };
        }

        namespace GroupLabel
        {
            inline constexpr ColourElement kBase = {
                "GroupLabelBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "GroupLabelText",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kLine = {
                "GroupLabelLine",
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kYellow
            };
        }

        namespace ParameterLabel
        {
            inline constexpr ColourElement kBase = {
                "ParameterLabelBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "ParameterLabelBackground",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "ParameterLabelText",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kDarkGrey1,
                SkinColourChart::kTransparent
            };
        }

        namespace PanelSeparator
        {
            inline constexpr ColourElement kBase = {
                "PanelSeparatorBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kLine = {
                "PanelSeparatorLine",
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kOrange
            };
        }

        namespace ParameterSeparator
        {
            inline constexpr ColourElement kBase = {
                "ParameterSeparatorBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kLine = {
                "ParameterSeparatorLine",
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kDarkGrey5,
                SkinColourChart::kRed
            };
        }

        namespace Button
        {
            inline constexpr ColourElement kBase = {
                "ButtonBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "ButtonBackground",
                SkinColourChart::kBlack,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBorder = {
                "ButtonBorder",
                SkinColourChart::kDarkGrey3,
                SkinColourChart::kDarkGrey4,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kText = {
                "ButtonText",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kDarkGrey1,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "ButtonBackgroundHoover",
                SkinColourChart::kDarkGrey1,
                SkinColourChart::kDarkGrey1,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kTextHoover = {
                "ButtonTextHoover",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundClicked = {
                "ButtonBackgroundClicked",
                SkinColourChart::kLightGrey2,
                SkinColourChart::kLightGrey2,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kTextClicked = {
                "ButtonTextClicked",
                SkinColourChart::kDarkGrey1,
                SkinColourChart::kBlack,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ButtonBackgroundDisabled",
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ButtonBorderDisabled",
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kTextDisabled = {
                "ButtonTextDisabled",
                SkinColourChart::kContentDisabled,
                SkinColourChart::kContentDisabled,
                SkinColourChart::kTransparent
            };
        }

        namespace Slider
        {
            inline constexpr ColourElement kBase = {
                "SliderBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "SliderBackground",
                SkinColourChart::kGreen1,
                SkinColourChart::kGreen1,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "SliderBorder",
                SkinColourChart::kTransparent, // kGreen2,
                SkinColourChart::kTransparent, // kGreen2,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kTrack = {
                "SliderTrack",
                SkinColourChart::kGreen3,
                SkinColourChart::kGreen3,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "SliderText",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderFocus = {
                "SliderBorderFocus",
                SkinColourChart::kGreen2, // kGreen4
                SkinColourChart::kGreen2, // kGreen4
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "SliderBackgroundDisabled",
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBorderDisabled = {
                "SliderBorderDisabled",
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kTrackDisabled = {
                "SliderTrackDisabled",
                SkinColourChart::kDecorationDisabled,
                SkinColourChart::kDecorationDisabled,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kTextDisabled = {
                "SliderTextDisabled",
                SkinColourChart::kContentDisabled,
                SkinColourChart::kContentDisabled,
                SkinColourChart::kTransparent
            };
        }

        namespace ComboBox
        {
            inline constexpr ColourElement kBase = {
                "ComboBoxBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "ComboBoxBackground",
                SkinColourChart::kGreen1,
                SkinColourChart::kGreen1,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "ComboBoxBorder",
                SkinColourChart::kTransparent, // kGreen2
                SkinColourChart::kTransparent, // kGreen2
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kTriangle = {
                "ComboBoxTriangle",
                SkinColourChart::kGreen3,
                SkinColourChart::kGreen3,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "ComboBoxText",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderFocus = {
                "ComboBoxBorderFocus",
                SkinColourChart::kGreen2, // kGreen4
                SkinColourChart::kGreen2, // kGreen4
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ComboBoxBackgroundDisabled",
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kBackgroundDisabled,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ComboBoxBorderDisabled",
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kBorderDisabled,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kTriangleDisabled = {
                "ComboBoxTriangleDisabled",
                SkinColourChart::kDecorationDisabled,
                SkinColourChart::kDecorationDisabled,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kTextDisabled = {
                "ComboBoxTextDisabled",
                SkinColourChart::kContentDisabled,
                SkinColourChart::kContentDisabled,
                SkinColourChart::kTransparent
            };
        }

        namespace PopupMenu
        {
            inline constexpr ColourElement kBase = {
                "PopupMenuBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "PopupMenuBackground",
                SkinColourChart::kGreen1,
                SkinColourChart::kGreen1,
                SkinColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "PopupMenuBorder",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kOrange
            };

            inline constexpr ColourElement kSeparator = {
                "PopupMenuSeparator",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "PopupMenuText",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "PopupMenuBackgroundHoover",
                SkinColourChart::kGreen4,
                SkinColourChart::kGreen4,
                SkinColourChart::kTransparent
            };

            inline constexpr ColourElement kTextHoover = {
                "PopupMenuTextHoover",
                SkinColourChart::kContentHoover,
                SkinColourChart::kContentHoover,
                SkinColourChart::kTransparent
            };
        }

        namespace EmptySlot
        {
            inline constexpr ColourElement kBase = {
                "EmptySlotBase",
                SkinColourChart::kTransparent,
                SkinColourChart::kTransparent,
                SkinColourChart::kRed
            };
        }
    }
}

