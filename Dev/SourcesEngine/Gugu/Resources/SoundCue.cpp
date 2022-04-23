////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/SoundCue.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/External/PugiXmlWrap.h"
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

int SoundCue::GetSoundCount() const
{
    return (int)m_audioFiles.size();
}

bool SoundCue::GetSound(int _iIndex, SoundParameters& _kParameters) const
{
    if (m_audioFiles.empty() || _iIndex < 0 || _iIndex >= (int)m_audioFiles.size())
        return false;
    
    _kParameters = m_audioFiles[_iIndex];
    return true;
}

bool SoundCue::GetRandomSound(SoundParameters& _kParameters) const
{
    if (m_audioFiles.empty())
        return false;

    int iIndex = GetRandom(m_audioFiles.size());
    _kParameters = m_audioFiles[iIndex];
    return true;
}

EResourceType::Type SoundCue::GetResourceType() const
{
    return EResourceType::SoundCue;
}

void SoundCue::Unload()
{
}

bool SoundCue::LoadFromXml(const pugi::xml_document& document)
{
    pugi::xml_node nodeRoot = document.child("SoundCue");
    if (!nodeRoot)
        return false;

    Unload();

    pugi::xml_node oNodeFiles = nodeRoot.child("Files");
    if (!oNodeFiles)
        return false;

    for (pugi::xml_node oNodeFile = oNodeFiles.child("File"); oNodeFile; oNodeFile = oNodeFile.next_sibling("File"))
    {
        Sound* pSound = nullptr;
        pugi::xml_attribute oAttributeName = oNodeFile.attribute("name");
        if (oAttributeName)
            pSound = GetResources()->GetSound(oAttributeName.as_string());

        if (!pSound)
            continue;

        SoundParameters kParameters;
        kParameters.sound = pSound;
        kParameters.volume = 100.f;

        pugi::xml_attribute oAttributeVolume = oNodeFile.attribute("volume");
        if (oAttributeVolume)
            kParameters.volume = oAttributeVolume.as_float(kParameters.volume);

        m_audioFiles.push_back(kParameters);
    }

    return true;
}

}   // namespace gugu
