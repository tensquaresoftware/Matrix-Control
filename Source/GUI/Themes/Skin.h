#pragma once

#include <map>
#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include "SkinValues.h"

namespace tss
{
    class Skin
    {
    public:
        enum class ColourVariant
        {
            Black,
            Cream
        };

        explicit Skin(ColourVariant variant);
        ~Skin() = default;

        static std::unique_ptr<Skin> create(ColourVariant variant);

        ColourVariant getColourVariant() const { return currentVariant_; }

        float getValue(SkinValueId valueId) const;
        juce::Colour getColour(SkinColourId colourId) const;

        void setValue(SkinValueId valueId, float value);
        void setColour(SkinColourId colourId, juce::Colour colour);

        juce::Font getBaseFont() const;

    private:
        void initializeDefaultValues();
        void initializeDefaultColours();
        void initializeBlackVariantColours();
        void initializeCreamVariantColours();

        ColourVariant currentVariant_;
        std::map<SkinValueId, float> values_;
        std::map<SkinColourId, juce::Colour> colours_;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Skin)
    };
}
