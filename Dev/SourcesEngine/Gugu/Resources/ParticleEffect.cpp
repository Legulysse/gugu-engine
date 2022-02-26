////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ParticleEffect.h"

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ParticleEffect::ParticleEffect()
{
}

ParticleEffect::~ParticleEffect()
{
}

EResourceType::Type ParticleEffect::GetResourceType() const
{
    return EResourceType::ParticleEffect;
}

bool ParticleEffect::LoadFromFile()
{
    return true;
}

}   // namespace gugu
