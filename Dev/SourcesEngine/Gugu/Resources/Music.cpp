////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Music.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/String.h"
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

    if (!_pMusic->openFromFile(GetFileInfo().GetFileSystemPath()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Music not found : {0}", GetFileInfo().GetFilePath_utf8()));
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

sf::Time Music::GetDuration() const
{
    //TODO: Check if I need to make something a bit cleaner
    sf::Music music;
    if (music.openFromFile(GetFileInfo().GetFileSystemPath()))
        return music.getDuration();
    return sf::Time::Zero;
}

}   // namespace gugu
