#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class AudioMixerGroup : public Resource
{
public:

    AudioMixerGroup();
    virtual ~AudioMixerGroup();

    float GetVolumeAttenuation() const;

    const std::vector<AudioMixerGroup*>& GetChildGroups() const;

    virtual EResourceType::Type GetResourceType() const override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    float m_volumeAttenuation;
    std::vector<AudioMixerGroup*> m_childGroups;
};

}   // namespace gugu
