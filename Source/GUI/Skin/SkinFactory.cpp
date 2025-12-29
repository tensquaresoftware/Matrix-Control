#include "SkinFactory.h"

namespace tss
{
    std::unique_ptr<Skin> SkinFactory::createSkin(Skin::ColourVariant colourVariant)
    {
        return std::make_unique<Skin>(colourVariant);
    }
}

