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
    , m_volumeAttenuation(1.f)
{
}

SoundCue::~SoundCue()
{
    Unload();
}

void SoundCue::SetMixerGroup(AudioMixerGroup* mixerGroup)
{
    m_mixerGroup = mixerGroup;

    RecomputeRuntimeSoundParameters();
}

AudioMixerGroup* SoundCue::GetMixerGroup() const
{
    return m_mixerGroup;
}

void SoundCue::SetVolumeAttenuation(float volumeAttenuation)
{
    m_volumeAttenuation = volumeAttenuation;

    RecomputeRuntimeSoundParameters();
}

float SoundCue::GetVolumeAttenuation() const
{
    return m_volumeAttenuation;
}

size_t SoundCue::GetSoundCount() const
{
    return m_audioClips.size();
}

bool SoundCue::GetClip(size_t index, ClipEntry& clipEntry) const
{
    if (index < 0 || index >= m_audioClips.size())
        return false;
    
    clipEntry = m_audioClips[index];
    return true;
}

void SoundCue::RecomputeRuntimeSoundParameters()
{
    m_soundParameters.clear();

    for (const auto& clip : m_audioClips)
    {
        // Only use readable clips.
        if (clip.audioClip)
        {
            // Warmup file for later use.
            clip.audioClip->GetOrLoadSFSoundBuffer();

            SoundParameters parameters;
            parameters.audioClip = clip.audioClip;
            parameters.audioClipId = clip.audioClipId;
            parameters.mixerGroupId = m_mixerGroup == nullptr ? "" : m_mixerGroup->GetID();
            parameters.volume = clip.volume * m_volumeAttenuation;
            parameters.pitchLowerOffset = clip.pitchLowerOffset;
            parameters.pitchUpperOffset = clip.pitchUpperOffset;
            m_soundParameters.push_back(parameters);
        }
    }
}

bool SoundCue::GetRandomSound(SoundParameters& parameters) const
{
    if (m_soundParameters.empty())
        return false;

    size_t index = GetRandom(m_soundParameters.size());
    parameters = m_soundParameters[index];
    return true;
}

EResourceType::Type SoundCue::GetResourceType() const
{
    return EResourceType::SoundCue;
}

void SoundCue::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (m_mixerGroup)
    {
        dependencies.insert(m_mixerGroup);
    }

    for (auto& clip : m_audioClips)
    {
        if (clip.audioClip)
        {
            dependencies.insert(clip.audioClip);
        }
    }
}

void SoundCue::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_mixerGroup == removedDependency)
    {
        m_mixerGroup = nullptr;
    }

    for (auto& clip : m_audioClips)
    {
        if (clip.audioClip == removedDependency)
        {
            clip.audioClip = nullptr;
            clip.audioClipId = "";
        }
    }

    RecomputeRuntimeSoundParameters();
}

void SoundCue::Unload()
{
    m_audioClips.clear();
    m_mixerGroup = nullptr;

    m_soundParameters.clear();
}

bool SoundCue::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("SoundCue");
    if (!rootNode)
        return false;

    m_mixerGroup = GetResources()->GetAudioMixerGroup(rootNode.child("MixerGroup").attribute("source").as_string());
    m_volumeAttenuation = rootNode.child("VolumeAttenuation").attribute("value").as_float(m_volumeAttenuation);

    for (pugi::xml_node clipNode = rootNode.child("Clips").child("Clip"); clipNode; clipNode = clipNode.next_sibling("Clip"))
    {
        AudioClip* audioClip = GetResources()->GetAudioClip(clipNode.attribute("source").as_string());

        ClipEntry clip;
        clip.audioClip = audioClip;
        clip.audioClipId = audioClip == nullptr ? "" : audioClip->GetID();
        clip.volume = clipNode.attribute("volume").as_float(clip.volume);
        clip.pitchLowerOffset = clipNode.attribute("pitchLowerOffset").as_float(clip.pitchLowerOffset);
        clip.pitchUpperOffset = clipNode.attribute("pitchUpperOffset").as_float(clip.pitchUpperOffset);

        m_audioClips.push_back(clip);
    }

    RecomputeRuntimeSoundParameters();

    return true;
}

bool SoundCue::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node rootNode = document.append_child("SoundCue");
    rootNode.append_attribute("serializationVersion") = 1;

    rootNode.append_child("MixerGroup").append_attribute("source").set_value((!m_mixerGroup) ? "" : m_mixerGroup->GetID().c_str());
    rootNode.append_child("VolumeAttenuation").append_attribute("value").set_value(m_volumeAttenuation);

    if (!m_audioClips.empty())
    {
        pugi::xml_node clipsNode = rootNode.append_child("Clips");
        for (const auto& clip : m_audioClips)
        {
            if (clip.audioClip)
            {
                pugi::xml_node clipNode = clipsNode.append_child("Clip");
                clipNode.append_attribute("source").set_value(clip.audioClipId.c_str());
                clipNode.append_attribute("volume").set_value(clip.volume);

                if (clip.pitchLowerOffset != 0.f)
                {
                    clipNode.append_attribute("pitchLowerOffset").set_value(clip.pitchLowerOffset);
                }

                if (clip.pitchUpperOffset != 0.f)
                {
                    clipNode.append_attribute("pitchUpperOffset").set_value(clip.pitchUpperOffset);
                }
            }
        }
    }

    return true;
}

}   // namespace gugu
