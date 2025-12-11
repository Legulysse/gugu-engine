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

void AudioMixerGroup::SetVolumeAttenuation(float volumeAttenuation)
{
    m_volumeAttenuation = volumeAttenuation;
}

float AudioMixerGroup::GetVolumeAttenuation() const
{
    return m_volumeAttenuation;
}

void AudioMixerGroup::SetSpatializationParameters(const SpatializationParameters& parameters)
{
    m_spatializationParameters = parameters;
}

const SpatializationParameters& AudioMixerGroup::GetSpatializationParameters() const
{
    return m_spatializationParameters;
}

bool AudioMixerGroup::GetSpatializationParameters(SpatializationParameters& parameters) const
{
    if (m_spatializationParameters.override)
    {
        parameters = m_spatializationParameters;
        return true;
    }

    return false;
}

const std::vector<AudioMixerGroup*>& AudioMixerGroup::GetChildMixerGroups() const
{
    return m_childMixerGroups;
}

EResourceType::Type AudioMixerGroup::GetResourceType() const
{
    return EResourceType::AudioMixerGroup;
}

void AudioMixerGroup::GetDependencies(std::set<Resource*>& dependencies) const
{
    for (auto& childMixerGroup : m_childMixerGroups)
    {
        if (childMixerGroup)
        {
            dependencies.insert(childMixerGroup);
        }
    }
}

void AudioMixerGroup::OnDependencyRemoved(const Resource* removedDependency)
{
    for (auto& childMixerGroup : m_childMixerGroups)
    {
        if (childMixerGroup == removedDependency)
        {
            childMixerGroup = nullptr;
        }
    }
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

    if (pugi::xml_node spatializationNode = rootNode.child("SpatializationParameters"))
    {
        m_spatializationParameters.override = spatializationNode.child("Override").attribute("value").as_bool(m_spatializationParameters.override);
        m_spatializationParameters.attenuation = spatializationNode.child("Attenuation").attribute("value").as_float(m_spatializationParameters.attenuation);
        m_spatializationParameters.minDistance = spatializationNode.child("MinDistance").attribute("value").as_float(m_spatializationParameters.minDistance);
    }

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
    pugi::xml_node rootNode = document.append_child("AudioMixerGroup");
    rootNode.append_attribute("serializationVersion") = 1;

    rootNode.append_child("VolumeAttenuation").append_attribute("value").set_value(m_volumeAttenuation);

    if (m_spatializationParameters.override)
    {
        pugi::xml_node spatializationNode = rootNode.append_child("SpatializationParameters");
        spatializationNode.append_child("Override").append_attribute("value").set_value(m_spatializationParameters.override);
        spatializationNode.append_child("Attenuation").append_attribute("value").set_value(m_spatializationParameters.attenuation);
        spatializationNode.append_child("MinDistance").append_attribute("value").set_value(m_spatializationParameters.minDistance);
    }

    if (!m_childMixerGroups.empty())
    {
        pugi::xml_node ChildMixerGroupsNode = rootNode.append_child("ChildMixerGroups");
        for (const auto* childMixerGroup : m_childMixerGroups)
        {
            if (childMixerGroup)
            {
                ChildMixerGroupsNode.append_child("ChildMixerGroup").append_attribute("source").set_value(childMixerGroup->GetID().c_str());
            }
        }
    }

    return true;
}

}   // namespace gugu
