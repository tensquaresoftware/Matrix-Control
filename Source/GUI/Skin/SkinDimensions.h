#pragma once

namespace tss
{
    namespace SkinDimensions
    {
        namespace HeaderPanel
        {
            inline constexpr int kHeight = 30;
            inline constexpr int kButtonWidth = 70;
            inline constexpr int kButtonHeight = 20;
            inline constexpr int kButtonSpacing = 5;
        }

        namespace BodyPanel
        {
            inline constexpr int kHeight = 760;
            inline constexpr int kSpacing = 15;
        }

        namespace FooterPanel
        {
            inline constexpr int kHeight = 30;
        }

        namespace PatchEditPanel
        {
            inline constexpr int kWidth = 810;
            inline constexpr int kHeight = 315;
        }

        namespace MasterEditPanel
        {
            inline constexpr int kWidth = 150;
            inline constexpr int kHeight = 730;
        }

        namespace PatchEditModulePanel
        {
            inline constexpr int kWidth = 150;
            inline constexpr int kHeight = 275;
        }

        namespace Gui
        {
            inline constexpr int kWidth = 1325;
            inline constexpr int kHeight = HeaderPanel::kHeight + BodyPanel::kHeight + FooterPanel::kHeight;
        }

        namespace Widget
        {
            namespace SectionName
            {
                inline constexpr int kHeight = 30;
                inline constexpr float kContentHeight = 20.0f;
                inline constexpr float kLineHeight = 12.0f;
                inline constexpr float kLeftLineWidth = 20.0f;
                inline constexpr float kTextSpacing = 5.0f;
            }

            namespace ModuleName
            {
                inline constexpr int kHeight = 25;
                inline constexpr int kNormalWidth = 150;
                inline constexpr int kLargeWidth = 225;
                inline constexpr float kTextLeftPadding = 2.0f;
                inline constexpr float kTextAreaHeight = 20.0f;
                inline constexpr float kLineThickness = 3.0f;
            }

            namespace GroupLabel
            {
                inline constexpr int kHeight = 40;
                inline constexpr float kTopAreaHeight = 15.0f;
                inline constexpr float kContentHeight = 20.0f;
                inline constexpr float kTextSpacing = 20.0f;
                inline constexpr float kLineThickness = 1.0f;
            }

            namespace ParameterLabel
            {
                inline constexpr int kWidth = 90;
                inline constexpr int kHeight = 20;
                inline constexpr float kBackgroundPadding = 1.0f;
                inline constexpr float kTextLeftPadding = 2.0f;
            }

            namespace PanelSeparator
            {
                inline constexpr int kWidth = 30;
                inline constexpr int kHeight = 730;
                inline constexpr float kLineWidth = 4.0f;
            }

            namespace ParameterSeparator
            {
                inline constexpr int kWidth = 150;
                inline constexpr int kHeight = 5;
                inline constexpr float kLineThickness = 1.0f;
            }

            namespace Button
            {
                inline constexpr int kHeight = 20;
                inline constexpr int kDefaultWidth = 20;
                inline constexpr float kBorderThickness = 2.0f;
            }

            namespace FocusableWidget
            {
                inline constexpr float kBorderThickness = 1.0f;
            }

            namespace Slider
            {
                inline constexpr int kWidth = 60;
                inline constexpr int kHeight = 20;
                inline constexpr int kBackgroundWidth = 60;
                inline constexpr int kBackgroundHeight = 16;
                inline constexpr double kDragSensitivity = 0.5;
                inline constexpr double kShiftKeyStep = 10.0;
            }

            namespace ComboBox
            {
                inline constexpr int kNormalWidth = 60;
                inline constexpr int kLargeWidth = 105;
                inline constexpr int kHeight = 20;
                inline constexpr int kNormalBackgroundWidth = 60;
                inline constexpr int kLargeBackgroundWidth = 105;
                inline constexpr int kBackgroundHeight = 16;
                inline constexpr float kLeftPadding = 3.0f;
                inline constexpr float kRightPadding = 3.0f;
                inline constexpr float kBorderThickness = 1.0f;
                inline constexpr int kVerticalMargin = 4;
                inline constexpr float kTriangleBaseSize = 7.0f;
                inline constexpr float kTriangleHeightFactor = 0.8660254f; // √3 / 2
            }

            namespace PopupMenu
            {
                inline constexpr int kItemHeight = 20;
                inline constexpr float kBorderThickness = 1.0f;
                inline constexpr int kSeparatorWidth = 1;
                inline constexpr int kTextLeftPadding = 3;
                inline constexpr int kColumnThreshold = 10;
            }

            namespace EmptySlot
            {
                inline constexpr int kWidth = 150;
                inline constexpr int kHeight = 20;
            }
        }
    }
}
