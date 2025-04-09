////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/SoundInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/AudioClip.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
SoundParameters::SoundParameters()
    : audioClip(nullptr)
    , audioClipId("")
    , mixerGroupInstance(nullptr)
    , mixerGroupId("")
    , volume(1.f)
    , pitchLowerOffset(0.f)
    , pitchUpperOffset(0.f)
    , group(0)
{
}

SoundInstance::SoundInstance()
    : m_mixerGroupInstance(nullptr)
    , m_volume(1.f)
    , m_group(0)
{
    Reset();
}

SoundInstance::~SoundInstance()
{
    Reset();
}

void SoundInstance::Reset()
{
    m_sfSound.stop();
    m_sfSound.setVolume(100);
    m_sfSound.setPitch(1);
    m_volume = 1.f;
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

void SoundInstance::SetAudioClip(AudioClip* audioClip)
{
    Reset();

    if (audioClip)
        m_sfSound.setBuffer(*audioClip->GetOrLoadSFSoundBuffer());
}

void SoundInstance::SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance)
{
    m_mixerGroupInstance = mixerGroupInstance;
    RecomputeMixedVolume();
}

void SoundInstance::SetVolume(float volume)
{
    m_volume = volume;
    RecomputeMixedVolume();
}

void SoundInstance::SetPitch(float pitch)
{
    m_sfSound.setPitch(pitch);
}

void SoundInstance::RecomputeMixedVolume()
{
    float volume = m_mixerGroupInstance == nullptr ? m_volume : m_mixerGroupInstance->ComputeMixedVolume(m_volume);
    m_sfSound.setVolume(volume * 100.f);
}

void SoundInstance::Play()
{
    m_sfSound.play();
}

}   // namespace gugu
