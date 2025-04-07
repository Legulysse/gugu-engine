#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class AudioMixerGroup : public Resource
{
public:

    AudioMixerGroup();
    virtual ~AudioMixerGroup();

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

protected:

    virtual void Unload() override;

protected:

};

}   // namespace gugu
