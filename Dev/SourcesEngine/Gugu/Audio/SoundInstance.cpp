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
    pitchLowerOffset = 0.f;
    pitchUpperOffset = 0.f;
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
    m_sfSound.setVolume(100);
    m_sfSound.setPitch(1);
    m_group = 0;
}

void SoundInstance::SetGroup(int group)
{
    m_group = group;
}

int SoundInstance::GetGroup() const
{
    return m_group;
}

void SoundInstance::SetSound(gugu::Sound* sound)
{
    Reset();

    if (sound)
        m_sfSound.setBuffer(*sound->GetSFSoundBuffer());
}

void SoundInstance::SetVolume(float volume)
{
    m_sfSound.setVolume(volume * 100.f);
}

void SoundInstance::SetPitch(float pitch)
{
    m_sfSound.setPitch(pitch);
}

void SoundInstance::Play()
{
    m_sfSound.play();
}

}   // namespace gugu
