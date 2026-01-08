#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "ColourChart.h"

namespace tss
{
    namespace ThemeColours
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
                ColourChart::kBlack,
                ColourChart::kWhite,
                ColourChart::kBlack
            };
        }

        namespace Panel
        {
            inline constexpr ColourElement kHeaderPanelBackground = {
                "HeaderPanelBackground",
                ColourChart::kLightGrey1,
                ColourChart::kLightGrey1,
                ColourChart::kWhite
            };

            inline constexpr ColourElement kBodyPanelBackground = {
                "BodyPanelBackground",
                ColourChart::kDarkGrey2,
                ColourChart::kCream,
                ColourChart::kBlack
            };

            inline constexpr ColourElement kFooterPanelBackground = {
                "FooterPanelBackground",
                ColourChart::kLightGrey1,
                ColourChart::kLightGrey1,
                ColourChart::kWhite
            };

            inline constexpr ColourElement kPatchEditPanelBackground = {
                "PatchEditPanelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kMatrixModulationPanelBackground = {
                "MatrixModulationPanelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kPatchManagerPanelBackground = {
                "PatchManagerPanelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kMasterEditPanelBackground = {
                "MasterEditPanelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kPatchEditModulePanelBackground = {
                "PatchEditModulePanelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kOrange
            };
        }

        namespace SectionHeader
        {
            inline constexpr ColourElement kBase = {
                "SectionHeaderBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kContentArea = {
                "SectionHeaderContentArea",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "SectionHeaderText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kLineBlue = {
                "SectionHeaderLineBlue",
                ColourChart::kBlue,
                ColourChart::kBlue,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kLineOrange = {
                "SectionHeaderLineOrange",
                ColourChart::kOrange,
                ColourChart::kOrange,
                ColourChart::kYellow
            };
        }

        namespace ModuleHeader
        {
            inline constexpr ColourElement kBase = {
                "ModuleHeaderBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kText = {
                "ModuleHeaderText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kLineBlue = {
                "ModuleHeaderLineBlue",
                ColourChart::kBlue,
                ColourChart::kBlue,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kLineOrange = {
                "ModuleHeaderLineOrange",
                ColourChart::kOrange,
                ColourChart::kOrange,
                ColourChart::kYellow
            };
        }

        namespace GroupLabel
        {
            inline constexpr ColourElement kBase = {
                "GroupLabelBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "GroupLabelText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kLine = {
                "GroupLabelLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5,
                ColourChart::kYellow
            };
        }

        namespace Label
        {
            inline constexpr ColourElement kBase = {
                "LabelBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "LabelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kText = {
                "LabelText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1,
                ColourChart::kTransparent
            };
        }

        namespace VerticalSeparator
        {
            inline constexpr ColourElement kBase = {
                "VerticalSeparatorBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kLine = {
                "VerticalSeparatorLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5,
                ColourChart::kOrange
            };
        }

        namespace HorizontalSeparator
        {
            inline constexpr ColourElement kBase = {
                "HorizontalSeparatorBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kLine = {
                "HorizontalSeparatorLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5,
                ColourChart::kRed
            };
        }

        namespace Button
        {
            inline constexpr ColourElement kBase = {
                "ButtonBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "ButtonBackground",
                ColourChart::kBlack,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBorder = {
                "ButtonBorder",
                ColourChart::kDarkGrey3,
                ColourChart::kDarkGrey4,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kText = {
                "ButtonText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "ButtonBackgroundHoover",
                ColourChart::kDarkGrey1,
                ColourChart::kDarkGrey1,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kTextHoover = {
                "ButtonTextHoover",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundClicked = {
                "ButtonBackgroundClicked",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kTextClicked = {
                "ButtonTextClicked",
                ColourChart::kDarkGrey1,
                ColourChart::kBlack,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ButtonBackgroundDisabled",
                ColourChart::kBackgroundDisabled,
                ColourChart::kBackgroundDisabled,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ButtonBorderDisabled",
                ColourChart::kBorderDisabled,
                ColourChart::kBorderDisabled,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kTextDisabled = {
                "ButtonTextDisabled",
                ColourChart::kContentDisabled,
                ColourChart::kContentDisabled,
                ColourChart::kTransparent
            };
        }

        namespace Slider
        {
            inline constexpr ColourElement kBase = {
                "SliderBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "SliderBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "SliderBorder",
                ColourChart::kTransparent, // kGreen2,
                ColourChart::kTransparent, // kGreen2,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kTrack = {
                "SliderTrack",
                ColourChart::kGreen3,
                ColourChart::kGreen3,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "SliderText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderFocus = {
                "SliderBorderFocus",
                ColourChart::kGreen2, // kGreen4
                ColourChart::kGreen2, // kGreen4
                ColourChart::kOrange
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "SliderBackgroundDisabled",
                ColourChart::kBackgroundDisabled,
                ColourChart::kBackgroundDisabled,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBorderDisabled = {
                "SliderBorderDisabled",
                ColourChart::kBorderDisabled,
                ColourChart::kBorderDisabled,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kTrackDisabled = {
                "SliderTrackDisabled",
                ColourChart::kDecorationDisabled,
                ColourChart::kDecorationDisabled,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kTextDisabled = {
                "SliderTextDisabled",
                ColourChart::kContentDisabled,
                ColourChart::kContentDisabled,
                ColourChart::kTransparent
            };
        }

        namespace ComboBox
        {
            inline constexpr ColourElement kBase = {
                "ComboBoxBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "ComboBoxBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "ComboBoxBorder",
                ColourChart::kTransparent, // kGreen2
                ColourChart::kTransparent, // kGreen2
                ColourChart::kOrange
            };

            inline constexpr ColourElement kTriangle = {
                "ComboBoxTriangle",
                ColourChart::kGreen3,
                ColourChart::kGreen3,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "ComboBoxText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBorderFocus = {
                "ComboBoxBorderFocus",
                ColourChart::kGreen2, // kGreen4
                ColourChart::kGreen2, // kGreen4
                ColourChart::kOrange
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ComboBoxBackgroundDisabled",
                ColourChart::kBackgroundDisabled,
                ColourChart::kBackgroundDisabled,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ComboBoxBorderDisabled",
                ColourChart::kBorderDisabled,
                ColourChart::kBorderDisabled,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kTriangleDisabled = {
                "ComboBoxTriangleDisabled",
                ColourChart::kDecorationDisabled,
                ColourChart::kDecorationDisabled,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kTextDisabled = {
                "ComboBoxTextDisabled",
                ColourChart::kContentDisabled,
                ColourChart::kContentDisabled,
                ColourChart::kTransparent
            };
        }

        namespace PopupMenu
        {
            inline constexpr ColourElement kBase = {
                "PopupMenuBase",
                ColourChart::kTransparent,
                ColourChart::kTransparent,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBackground = {
                "PopupMenuBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1,
                ColourChart::kRed
            };

            inline constexpr ColourElement kBorder = {
                "PopupMenuBorder",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kSeparator = {
                "PopupMenuSeparator",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kYellow
            };

            inline constexpr ColourElement kText = {
                "PopupMenuText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "PopupMenuBackgroundHoover",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kTextHoover = {
                "PopupMenuTextHoover",
                ColourChart::kContentHoover,
                ColourChart::kContentHoover,
                ColourChart::kTransparent
            };
        }
    }
}

