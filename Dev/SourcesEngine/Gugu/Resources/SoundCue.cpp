////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/SoundCue.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AudioClip.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/Math/Random.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SoundCue::SoundCue()
    : m_mixerGroup(nullptr)
{
}

SoundCue::~SoundCue()
{
    Unload();
}

size_t SoundCue::GetSoundCount() const
{
    return m_audioClips.size();
}

bool SoundCue::GetSound(size_t index, SoundParameters& parameters) const
{
    if (index < 0 || index >= m_audioClips.size())
        return false;
    
    parameters = m_audioClips[index];
    return true;
}

bool SoundCue::GetRandomSound(SoundParameters& parameters) const
{
    if (m_audioClips.empty())
        return false;

    size_t index = GetRandom(m_audioClips.size());
    parameters = m_audioClips[index];
    return true;
}

EResourceType::Type SoundCue::GetResourceType() const
{
    return EResourceType::SoundCue;
}

void SoundCue::GetDependencies(std::set<Resource*>& dependencies) const
{
    dependencies.insert(m_mixerGroup);

    for (auto& parameters : m_audioClips)
    {
        if (parameters.audioClip)
        {
            dependencies.insert(parameters.audioClip);
        }
    }
}

void SoundCue::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_mixerGroup == removedDependency)
    {
        m_mixerGroup = nullptr;

        for (auto& parameters : m_audioClips)
        {
            parameters.mixerGroupInstance = nullptr;
            parameters.mixerGroupId = "";
        }
    }

    for (auto& parameters : m_audioClips)
    {
        if (parameters.audioClip == removedDependency)
        {
            parameters.audioClip = nullptr;
            parameters.audioClipId = "";
        }
    }
}

void SoundCue::Unload()
{
    m_audioClips.clear();
    m_mixerGroup = nullptr;
}

bool SoundCue::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("SoundCue");
    if (!rootNode)
        return false;

    AudioMixerGroup* mixerGroup = GetResources()->GetAudioMixerGroup(rootNode.child("MixerGroup").attribute("source").as_string());

    for (pugi::xml_node clipNode = rootNode.child("Clips").child("Clip"); clipNode; clipNode = clipNode.next_sibling("Clip"))
    {
        if (AudioClip* audioClip = GetResources()->GetAudioClip(clipNode.attribute("source").as_string()))
        {
            // Warmup file for later use.
            audioClip->GetOrLoadSFSoundBuffer();

            SoundParameters parameters;
            parameters.audioClip = audioClip;
            parameters.audioClipId = audioClip->GetID();
            parameters.mixerGroupId = mixerGroup->GetID();
            parameters.volume = clipNode.attribute("volume").as_float(parameters.volume);
            parameters.pitchLowerOffset = clipNode.attribute("pitchLowerOffset").as_float(parameters.pitchLowerOffset);
            parameters.pitchUpperOffset = clipNode.attribute("pitchUpperOffset").as_float(parameters.pitchUpperOffset);

            m_audioClips.push_back(parameters);
        }
    }

    return true;
}

}   // namespace gugu
