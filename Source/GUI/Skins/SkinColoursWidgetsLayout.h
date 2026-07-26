#pragma once

#include "SkinColoursCommon.h"

namespace TSS::SkinColours
{
    namespace Widgets
    {
        namespace SectionHeader
        {
            inline constexpr ColourElement kText = {
                "SectionHeaderText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1
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
                ColourChart::kDarkGrey1
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
                ColourChart::kDarkGrey1
            };

            inline constexpr ColourElement kLine = {
                "GroupLabelLine",
                ColourChart::kDarkGrey5,
                ColourChart::kDarkGrey5
            };
        }

        namespace Label
        {
            inline constexpr ColourElement kText = {
                "LabelText",
                ColourChart::kLightGrey2,
                ColourChart::kDarkGrey1
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

        namespace DarkPanel
        {
            // Light text on dark Header/Footer/dialog chrome — must not follow Cream body LabelText.
            inline constexpr ColourElement kText = {
                "DarkPanelText",
                ColourChart::kLightGrey2,
                ColourChart::kLightGrey2
            };
        }

        namespace FooterPanel
        {
            // Trial chrome grey for footer message body / INFO badges.
            inline constexpr ColourElement kMessageInfo = {
                "FooterMessageInfo",
                ColourChart::kDarkGrey6,
                ColourChart::kDarkGrey6
            };

            inline constexpr ColourElement kMessageWarning = {
                "FooterMessageWarning",
                ColourChart::kOrange,
                ColourChart::kOrange
            };

            inline constexpr ColourElement kMessageError = {
                "FooterMessageError",
                ColourChart::kRed,
                ColourChart::kRed
            };
        }
    }
}
