////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/SoundCue.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Sound.h"
#include "Gugu/Math/Random.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SoundCue::SoundCue()
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
    if (m_audioFiles.empty() || index < 0 || index >= m_audioFiles.size())
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
    for (auto& audioFile : m_audioFiles)
    {
        if (audioFile.sound == removedDependency)
        {
            audioFile.sound = nullptr;
        }
    }
}

void SoundCue::Unload()
{
    m_audioFiles.clear();
}

bool SoundCue::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node nodeRoot = document.child("SoundCue");
    if (!nodeRoot)
        return false;

    pugi::xml_node nodeFiles = nodeRoot.child("Files");
    if (!nodeFiles)
        return false;

    for (pugi::xml_node nodeFile = nodeFiles.child("File"); nodeFile; nodeFile = nodeFile.next_sibling("File"))
    {
        Sound* sound = nullptr;
        pugi::xml_attribute attributeName = nodeFile.attribute("name");
        if (attributeName)
            sound = GetResources()->GetSound(attributeName.as_string());

        if (!sound)
            continue;

        SoundParameters parameters;
        parameters.soundID = attributeName.as_string();
        parameters.sound = sound;
        parameters.volume = nodeFile.attribute("volume").as_float(parameters.volume);
        parameters.pitchLowerOffset = nodeFile.attribute("pitchLowerOffset").as_float(parameters.pitchLowerOffset);
        parameters.pitchUpperOffset = nodeFile.attribute("pitchUpperOffset").as_float(parameters.pitchUpperOffset);

        m_audioFiles.push_back(parameters);
    }

    return true;
}

}   // namespace gugu
