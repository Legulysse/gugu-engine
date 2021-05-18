////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Music.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu//Engine.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Music::Music()
{
}

Music::~Music()
{
}

bool Music::LoadSFMusic(sf::Music* _pMusic)
{
    if (!_pMusic)
        return false;

    if (!_pMusic->openFromFile(GetFileInfoRef().GetPathName()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Music not found : {0}", GetFileInfoRef().GetPathName()));
        return false;
    }

    return true;
}

EResourceType::Type Music::GetResourceType() const
{
    return EResourceType::Music;
}

bool Music::LoadFromFile()
{
    return true;
}

DeltaTime Music::GetDuration() const
{
    //TODO: Check if I need to make something a bit cleaner
    sf::Music music;
    if (music.openFromFile(GetFileInfoRef().GetPathName()))
        return music.getDuration();
    return DeltaTime(0);
}

}   // namespace gugu
