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

    //TODO: maybe I should use a const ref instead, and provide a Set ?
    ParticleSystemSettings* GetParticleSettings();

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::vector<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    ParticleSystemSettings m_particleSettings;
};

}   // namespace gugu
