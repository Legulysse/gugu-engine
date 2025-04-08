////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/SoundCue.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Sound.h"
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
    return m_audioFiles.size();
}

bool SoundCue::GetSound(size_t index, SoundParameters& parameters) const
{
    if (index < 0 || index >= m_audioFiles.size())
        return false;
    
    parameters = m_audioFiles[index];
    return true;
}

bool SoundCue::GetRandomSound(SoundParameters& parameters) const
{
    if (m_audioFiles.empty())
        return false;

    size_t index = GetRandom(m_audioFiles.size());
    parameters = m_audioFiles[index];
    return true;
}

EResourceType::Type SoundCue::GetResourceType() const
{
    return EResourceType::SoundCue;
}

void SoundCue::GetDependencies(std::set<Resource*>& dependencies) const
{
    dependencies.insert(m_mixerGroup);

    for (auto& audioFile : m_audioFiles)
    {
        if (audioFile.sound)
        {
            dependencies.insert(audioFile.sound);
        }
    }
}

void SoundCue::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_mixerGroup == removedDependency)
    {
        m_mixerGroup = nullptr;

        for (auto& audioFile : m_audioFiles)
        {
            audioFile.mixerGroupInstance = nullptr;
            audioFile.mixerGroupID = "";
        }
    }

    for (auto& audioFile : m_audioFiles)
    {
        if (audioFile.sound == removedDependency)
        {
            audioFile.sound = nullptr;
            audioFile.soundID = "";
        }
    }
}

void SoundCue::Unload()
{
    m_audioFiles.clear();
    m_mixerGroup = nullptr;
}

bool SoundCue::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node nodeRoot = document.child("SoundCue");
    if (!nodeRoot)
        return false;

    AudioMixerGroup* mixerGroup = GetResources()->GetAudioMixerGroup(nodeRoot.child("MixerGroup").attribute("source").as_string());

    for (pugi::xml_node nodeFile = nodeRoot.child("Clips").child("Clip"); nodeFile; nodeFile = nodeFile.next_sibling("File"))
    {
        if (Sound* sound = GetResources()->GetSound(nodeFile.attribute("source").as_string()))
        {
            SoundParameters parameters;
            parameters.sound = sound;
            parameters.soundID = sound->GetID();
            parameters.mixerGroupID = mixerGroup->GetID();
            parameters.volume = nodeFile.attribute("volume").as_float(parameters.volume);
            parameters.pitchLowerOffset = nodeFile.attribute("pitchLowerOffset").as_float(parameters.pitchLowerOffset);
            parameters.pitchUpperOffset = nodeFile.attribute("pitchUpperOffset").as_float(parameters.pitchUpperOffset);

            m_audioFiles.push_back(parameters);
        }
    }

    return true;
}

}   // namespace gugu
