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
        };

        namespace Common
        {
            // Common background colour constants for all panels and widgets
            static constexpr juce::uint32 kCommonBackgroundBlack = ColourChart::kDarkGrey2;
            static constexpr juce::uint32 kCommonBackgroundCream = ColourChart::kCream;

            // Common disabled and interaction colour constants
            static constexpr juce::uint32 kBackgroundDisabled = ColourChart::kDarkGrey3;
            static constexpr juce::uint32 kBorderDisabled = ColourChart::kDarkGrey4;
            static constexpr juce::uint32 kDecorationDisabled = ColourChart::kDarkGrey5;
            static constexpr juce::uint32 kContentDisabled = ColourChart::kDarkGrey5;
            static constexpr juce::uint32 kContentHoover = ColourChart::kDarkGrey1;
        }

        namespace Gui
        {
            inline constexpr ColourElement kCommonBackground = {
                "CommonBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kGuiBackground = {
                "GuiBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };
        }

        namespace Panel
        {
            inline constexpr ColourElement kHeaderPanelBackground = {
                "HeaderPanelBackground",
                ColourChart::kLightGrey1,
                ColourChart::kLightGrey1
            };

            inline constexpr ColourElement kBodyPanelBackground = {
                "BodyPanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kFooterPanelBackground = {
                "FooterPanelBackground",
                ColourChart::kLightGrey1,
                ColourChart::kLightGrey1
            };

            inline constexpr ColourElement kPatchEditPanelBackground = {
                "PatchEditPanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kMatrixModulationPanelBackground = {
                "MatrixModulationPanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kPatchManagerPanelBackground = {
                "PatchManagerPanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kMasterEditPanelBackground = {
                "MasterEditPanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kPatchEditModulePanelBackground = {
                "PatchEditModulePanelBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };
        }

        namespace SectionHeader
        {
            inline constexpr ColourElement kContentArea = {
                "SectionHeaderContentArea",
                ColourChart::kTransparent,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kText = {
                "SectionHeaderText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kLineBlue = {
                "SectionHeaderLineBlue",
                ColourChart::kBlue,
                ColourChart::kBlue
            };

            inline constexpr ColourElement kLineOrange = {
                "SectionHeaderLineOrange",
                ColourChart::kOrange,
                ColourChart::kOrange
            };
        }

        namespace ModuleHeader
        {
            inline constexpr ColourElement kText = {
                "ModuleHeaderText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kLineBlue = {
                "ModuleHeaderLineBlue",
                ColourChart::kBlue,
                ColourChart::kBlue
            };

            inline constexpr ColourElement kLineOrange = {
                "ModuleHeaderLineOrange",
                ColourChart::kOrange,
                ColourChart::kOrange
            };
        }

        namespace GroupLabel
        {
            inline constexpr ColourElement kText = {
                "GroupLabelText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kLine = {
                "GroupLabelLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5
            };
        }

        namespace Label
        {
            inline constexpr ColourElement kBackground = {
                "LabelBackground",
                ColourChart::kTransparent,
                ColourChart::kTransparent
            };

            inline constexpr ColourElement kText = {
                "LabelText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1
            };
        }

        namespace VerticalSeparator
        {
            inline constexpr ColourElement kLine = {
                "VerticalSeparatorLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5
            };
        }

        namespace HorizontalSeparator
        {
            inline constexpr ColourElement kLine = {
                "HorizontalSeparatorLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5
            };
        }

        namespace Button
        {
            inline constexpr ColourElement kBackground = {
                "ButtonBackground",
                ColourChart::kBlack,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kBorder = {
                "ButtonBorder",
                ColourChart::kDarkGrey3,
                ColourChart::kDarkGrey4
            };

            inline constexpr ColourElement kText = {
                "ButtonText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1,
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "ButtonBackgroundHoover",
                ColourChart::kBlack,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kTextHoover = {
                "ButtonTextHoover",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1,
            };

            inline constexpr ColourElement kBackgroundClicked = {
                "ButtonBackgroundClicked",
                ColourChart::kBlack,
                ColourChart::kLightGrey2
            };

            inline constexpr ColourElement kTextClicked = {
                "ButtonTextClicked",
                ColourChart::kRed,
                ColourChart::kRed,
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ButtonBackgroundDisabled",
                Common::kBackgroundDisabled,
                Common::kBackgroundDisabled,
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ButtonBorderDisabled",
                Common::kBorderDisabled,
                Common::kBorderDisabled,
            };

            inline constexpr ColourElement kTextDisabled = {
                "ButtonTextDisabled",
                Common::kContentDisabled,
                Common::kContentDisabled,
            };
        }

        namespace Slider
        {
            inline constexpr ColourElement kBackground = {
                "SliderBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1
            };

            inline constexpr ColourElement kBorder = {
                "SliderBorder",
                ColourChart::kTransparent, // kGreen2,
                ColourChart::kTransparent // kGreen2,
            };

            inline constexpr ColourElement kTrack = {
                "SliderTrack",
                ColourChart::kGreen3,
                ColourChart::kGreen3
            };

            inline constexpr ColourElement kText = {
                "SliderText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };

            inline constexpr ColourElement kBorderFocus = {
                "SliderBorderFocus",
                ColourChart::kGreen2, // kGreen4
                ColourChart::kGreen2 // kGreen4
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "SliderBackgroundDisabled",
                Common::kBackgroundDisabled,
                Common::kBackgroundDisabled
            };

            inline constexpr ColourElement kBorderDisabled = {
                "SliderBorderDisabled",
                Common::kBorderDisabled,
                Common::kBorderDisabled
            };

            inline constexpr ColourElement kTrackDisabled = {
                "SliderTrackDisabled",
                Common::kDecorationDisabled,
                Common::kDecorationDisabled
            };

            inline constexpr ColourElement kTextDisabled = {
                "SliderTextDisabled",
                Common::kContentDisabled,
                Common::kContentDisabled,
            };
        }

        namespace ComboBox
        {
            inline constexpr ColourElement kBackground = {
                "ComboBoxBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1
            };

            inline constexpr ColourElement kBorder = {
                "ComboBoxBorder",
                ColourChart::kTransparent, // kGreen2
                ColourChart::kTransparent // kGreen2
            };

            inline constexpr ColourElement kTriangle = {
                "ComboBoxTriangle",
                ColourChart::kGreen3,
                ColourChart::kGreen3
            };

            inline constexpr ColourElement kText = {
                "ComboBoxText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };

            inline constexpr ColourElement kBorderFocus = {
                "ComboBoxBorderFocus",
                ColourChart::kGreen2, // kGreen4
                ColourChart::kGreen2 // kGreen4
            };

            inline constexpr ColourElement kBackgroundDisabled = {
                "ComboBoxBackgroundDisabled",
                Common::kBackgroundDisabled,
                Common::kBackgroundDisabled
            };

            inline constexpr ColourElement kBorderDisabled = {
                "ComboBoxBorderDisabled",
                Common::kBorderDisabled,
                Common::kBorderDisabled
            };

            inline constexpr ColourElement kTriangleDisabled = {
                "ComboBoxTriangleDisabled",
                Common::kDecorationDisabled,
                Common::kDecorationDisabled
            };

            inline constexpr ColourElement kTextDisabled = {
                "ComboBoxTextDisabled",
                Common::kContentDisabled,
                Common::kContentDisabled,
            };
        }

        namespace PopupMenu
        {
            inline constexpr ColourElement kBackground = {
                "PopupMenuBackground",
                ColourChart::kGreen1,
                ColourChart::kGreen1
            };

            inline constexpr ColourElement kBorder = {
                "PopupMenuBorder",
                ColourChart::kGreen4,
                ColourChart::kGreen4
            };

            inline constexpr ColourElement kSeparator = {
                "PopupMenuSeparator",
                ColourChart::kGreen4,
                ColourChart::kGreen4
            };

            inline constexpr ColourElement kText = {
                "PopupMenuText",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };

            inline constexpr ColourElement kBackgroundHoover = {
                "PopupMenuBackgroundHoover",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };

            inline constexpr ColourElement kTextHoover = {
                "PopupMenuTextHoover",
                Common::kContentHoover,
                Common::kContentHoover,
            };
        }

        namespace NumberBox
        {
            inline constexpr ColourElement kText = {
                "NumberBoxText",
                ColourChart::kRed,
                ColourChart::kRed,
            };

            inline constexpr ColourElement kDot = {
                "NumberBoxDot",
                ColourChart::kRed,
                ColourChart::kRed,
            };
        }

        namespace EnvelopeDisplay
        {
            inline constexpr ColourElement kBackground = {
                "EnvelopeDisplayBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kBorder = {
                "EnvelopeDisplayBorder",
                ColourChart::kDarkGrey3,
                ColourChart::kDarkGrey4
            };

            inline constexpr ColourElement kEnvelope = {
                "EnvelopeDisplayEnvelope",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };
        }

        namespace PatchNameDisplay
        {
            inline constexpr ColourElement kBackground = {
                "PatchNameDisplayBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kBorder = {
                "PatchNameDisplayBorder",
                ColourChart::kDarkGrey3,
                ColourChart::kDarkGrey4
            };

            inline constexpr ColourElement kText = {
                "PatchNameDisplayText",
                ColourChart::kRed,
                ColourChart::kRed,
            };
        }

        namespace TrackGeneratorDisplay
        {
            inline constexpr ColourElement kBackground = {
                "TrackGeneratorDisplayBackground",
                Common::kCommonBackgroundBlack,
                Common::kCommonBackgroundCream
            };

            inline constexpr ColourElement kBorder = {
                "TrackGeneratorDisplayBorder",
                ColourChart::kDarkGrey3,
                ColourChart::kDarkGrey4
            };

            inline constexpr ColourElement kShaper = {
                "TrackGeneratorDisplayShaper",
                ColourChart::kGreen4,
                ColourChart::kGreen4,
            };
        }
    }
}

