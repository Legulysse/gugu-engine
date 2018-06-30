////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/SoundCue.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu//Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Random.h"
#include "Gugu/External/PugiXmlWrap.h"
#include "Gugu/Misc/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SoundCue::SoundCue()
{
}

SoundCue::~SoundCue()
{
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

EResourceType::Type	SoundCue::GetResourceType() const
{
	return EResourceType::SoundCue;
}

bool SoundCue::LoadFromFile()
{
    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(GetFileInfoRef().GetPathName().c_str());
    if (!result)
        return false;

    pugi::xml_node oNodeAudioCue = oDoc.child("SoundCue");
    if (!oNodeAudioCue)
        return false;

    pugi::xml_node oNodeFiles = oNodeAudioCue.child("Files");
    if (!oNodeFiles)
        return false;

    for (pugi::xml_node oNodeFile = oNodeFiles.child("File"); oNodeFile; oNodeFile = oNodeFile.next_sibling("File"))
    {
        Sound* pSound = nullptr;
        pugi::xml_attribute oAttributeName = oNodeFile.attribute("Name");
        if (oAttributeName)
            pSound = GetResources()->GetSound(oAttributeName.as_string());

        if (!pSound)
            continue;

        SoundParameters kParameters;
        kParameters.sound = pSound;
        kParameters.volume = 100.f;

        pugi::xml_attribute oAttributeVolume = oNodeFile.attribute("Volume");
        if (oAttributeVolume)
            kParameters.volume = oAttributeVolume.as_float(kParameters.volume);

        m_audioFiles.push_back(kParameters);
    }

    return true;
}

}	// namespace gugu
