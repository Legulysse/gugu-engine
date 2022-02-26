#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ParticleEffect : public Resource
{
public:

    ParticleEffect();
    virtual ~ParticleEffect();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

protected:

};

}   // namespace gugu
