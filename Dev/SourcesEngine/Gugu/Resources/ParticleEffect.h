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

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    ParticleSystemSettings m_particleSettings;
};

}   // namespace gugu
