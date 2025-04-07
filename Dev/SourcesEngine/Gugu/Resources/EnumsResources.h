#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

namespace EResourceType
{
    enum Type
    {
        Unknown = 0,

        Texture,
        Font,
        Sound,
        Music,
        AudioClip,
        AudioMixerGroup,
        SoundCue,
        ImageSet,
        AnimSet,
        ParticleEffect,
        Datasheet,
        ElementWidget,

        Custom,
    };
}

enum class EResourceEvent : uint8
{
    ResourceUpdated,
    ResourceRemoved,
    DependencyUpdated,
    DependencyRemoved,
};

}   // namespace gugu
