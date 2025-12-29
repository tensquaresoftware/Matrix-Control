#pragma once

#include <memory>

#include "Skin.h"

namespace tss
{
    class SkinFactory
    {
    public:
        static std::unique_ptr<Skin> createSkin(Skin::ColourVariant colourVariant);

    private:
        SkinFactory() = delete;
    };
}

