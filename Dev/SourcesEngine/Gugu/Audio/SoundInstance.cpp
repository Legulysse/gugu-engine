////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/SoundInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Sound.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
SoundParameters::SoundParameters()
{
    sound = nullptr;
    soundID = "";
    volume = 1.f;
    group = 0;
}

SoundInstance::SoundInstance()
{
    Reset();
}
/*
SoundInstance::SoundInstance(const SoundInstance&)
{
    Reset();
}*/

SoundInstance::~SoundInstance()
{
    Reset();
}

void SoundInstance::Reset()
{
    m_sfSound.stop();
    m_group = 0;
}

void SoundInstance::SetGroup(int _iGroup)
{
    m_group = _iGroup;
}

int SoundInstance::GetGroup() const
{
    return m_group;
}

void SoundInstance::SetSound(gugu::Sound* _pSound)
{
    Reset();

    if (_pSound)
        m_sfSound.setBuffer(*_pSound->GetSFSoundBuffer());
}

void SoundInstance::SetVolume(float _fVolume)
{
    m_sfSound.setVolume(_fVolume * 100.f);
}

void SoundInstance::Play()
{
    m_sfSound.play();
}

}   // namespace gugu
