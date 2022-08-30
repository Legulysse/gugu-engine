#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Audio/SoundInstance.h"

#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Sound;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SoundCue : public Resource
{
public:

    SoundCue();
    virtual ~SoundCue();
    
    int     GetSoundCount   () const;
    bool    GetSound        (int _iIndex, SoundParameters& _kParameters) const;
    bool    GetRandomSound  (SoundParameters& _kParameters) const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;

protected:

    std::vector<SoundParameters> m_audioFiles;
};

}   // namespace gugu
