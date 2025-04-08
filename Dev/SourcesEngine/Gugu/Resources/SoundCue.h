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
    class AudioMixerGroup;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SoundCue : public Resource
{
public:

    SoundCue();
    virtual ~SoundCue();
    
    size_t GetSoundCount() const;
    bool GetSound(size_t index, SoundParameters& parameters) const;
    bool GetRandomSound(SoundParameters& parameters) const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;

protected:

    std::vector<SoundParameters> m_audioFiles;
    AudioMixerGroup* m_mixerGroup;
};

}   // namespace gugu
