#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace tss
{
    namespace SkinFonts
    {
        inline juce::Font getDefaultFont()
        {
            juce::Font font(juce::FontOptions().withHeight(14.0f));
            
            auto availableTypefaces = juce::Font::findAllTypefaceNames();
            
            for (const auto& name : availableTypefaces)
            {
                if (name.containsIgnoreCase("PT Sans Narrow"))
                {
                    font = juce::Font(juce::FontOptions(name, 14.0f, juce::Font::plain));
                    break;
                }
            }
            
            return font;
        }

        inline juce::Font getSectionNameFont()
        {
            auto font = getDefaultFont();
            return font.withHeight(18.0f).boldened();
        }

        inline juce::Font getModuleNameFont()
        {
            auto font = getDefaultFont();
            return font.withHeight(16.0f).boldened();
        }

        inline juce::Font getGroupLabelFont()
        {
            return getDefaultFont();
        }

        inline juce::Font getParameterLabelFont()
        {
            return getDefaultFont();
        }

        inline juce::Font getButtonFont()
        {
            return getDefaultFont();
        }

        inline juce::Font getSliderFont()
        {
            return getDefaultFont();
        }

        inline juce::Font getComboBoxFont()
        {
            return getDefaultFont();
        }

        inline juce::Font getPopupMenuFont()
        {
            return getDefaultFont();
        }
    }
}

