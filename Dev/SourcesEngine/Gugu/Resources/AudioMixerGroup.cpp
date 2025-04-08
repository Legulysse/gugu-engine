////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/AudioMixerGroup.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioMixerGroup::AudioMixerGroup()
    : m_volumeAttenuation(1.f)
{
}

AudioMixerGroup::~AudioMixerGroup()
{
    Unload();
}

EResourceType::Type AudioMixerGroup::GetResourceType() const
{
    return EResourceType::AudioMixerGroup;
}

float AudioMixerGroup::GetVolumeAttenuation() const
{
    return m_volumeAttenuation;
}

const std::vector<AudioMixerGroup*>& AudioMixerGroup::GetChildMixerGroups() const
{
    return m_childMixerGroups;
}

void AudioMixerGroup::Unload()
{
    m_volumeAttenuation = 1.f;
    m_childMixerGroups.clear();
}

bool AudioMixerGroup::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("AudioMixerGroup");
    if (!rootNode)
        return false;

    m_volumeAttenuation = rootNode.child("VolumeAttenuation").attribute("value").as_float(m_volumeAttenuation);

    for (pugi::xml_node childMixerGroupNode = rootNode.child("ChildMixerGroups").child("ChildMixerGroup"); childMixerGroupNode; childMixerGroupNode = childMixerGroupNode.next_sibling("ChildMixerGroup"))
    {
        auto childMixerGroup = GetResources()->GetAudioMixerGroup(childMixerGroupNode.attribute("source").as_string());
        if (childMixerGroup)
        {
            m_childMixerGroups.push_back(childMixerGroup);
        }
    }

    return true;
}

bool AudioMixerGroup::SaveToXml(pugi::xml_document& document) const
{
    return true;
}

}   // namespace gugu
