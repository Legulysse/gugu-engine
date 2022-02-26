#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/VisualEffects/ParticleSystemSettings.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ParticleEffect : public Resource
{
public:

    ParticleEffect();
    virtual ~ParticleEffect();

    ParticleSystemSettings* GetParticleSettings();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
    virtual bool SaveToFile() override;

protected:

    ParticleSystemSettings m_particleSettings;
};

}   // namespace gugu
