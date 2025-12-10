////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/SoundInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/AudioClip.h"
#include "Gugu/System/Memory.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SoundInstance::SoundInstance()
    : m_audioClip(nullptr)
    , m_mixerGroupInstance(nullptr)
    , m_sfSound(nullptr)
    , m_volume(1.f)
{
    // Default empty buffer.
    static const sf::SoundBuffer defaultEmptyBuffer;
    m_sfSound = new sf::Sound(defaultEmptyBuffer);

    Reset();
}

SoundInstance::~SoundInstance()
{
    SafeDelete(m_sfSound);  // This will call stop() in the sf::Sound destructor.
}

void SoundInstance::Reset()
{
    m_sfSound->stop();
    m_sfSound->setVolume(100.f);
    m_sfSound->setPitch(1.f);
    m_sfSound->setSpatializationEnabled(false);
    m_sfSound->setPosition(sf::Vector3f(0, 0, 0));

    m_audioClip = nullptr;
    m_volume = 1.f;
}

bool SoundInstance::UpdateStatus()
{
    if (m_audioClip == nullptr)
        return false;

    if (m_sfSound->getStatus() == sf::SoundSource::Status::Stopped)
    {
        Reset();
        return false;
    }

    // Return true if IsActive.
    return true;
}

bool SoundInstance::IsActive() const
{
    return m_audioClip != nullptr;
}

void SoundInstance::SetAudioClip(AudioClip* audioClip)
{
    Reset();

    if (audioClip)
    {
        sf::SoundBuffer* soundBuffer = audioClip->GetOrLoadSFSoundBuffer();
        if (soundBuffer)
        {
            m_sfSound->setBuffer(*soundBuffer);
            m_audioClip = audioClip;
        }
    }
}

AudioClip* SoundInstance::GetAudioClip() const
{
    return m_audioClip;
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
    m_sfSound->setPitch(pitch);
}

void SoundInstance::SetSpatialization(bool enabled, float minDistance, float attenuation)
{
    // Attenuation formula for spatialization.
    // 
    // https://www.sfml-dev.org/tutorials/3.0/audio/spatialization/#audio-sources
    // 
    // MinDistance   is the sound's minimum distance, set with setMinDistance
    // Attenuation   is the sound's attenuation, set with setAttenuation
    // Distance      is the distance between the sound and the listener
    // Volume factor is the calculated factor, in range[0 .. 1], that will be applied to the sound's volume
    // Volume factor = MinDistance / (MinDistance + Attenuation * (max(Distance, MinDistance) - MinDistance))

    m_sfSound->setSpatializationEnabled(enabled);
    m_sfSound->setMinDistance(minDistance);
    m_sfSound->setAttenuation(attenuation);
}

void SoundInstance::SetPosition(const Vector2f& position)
{
    m_sfSound->setPosition(sf::Vector3f(position.x, 0.f, position.y));
}

void SoundInstance::RecomputeMixedVolume()
{
    float volume = m_mixerGroupInstance == nullptr ? m_volume : m_mixerGroupInstance->ComputeMixedVolume(m_volume);
    m_sfSound->setVolume(volume * 100.f);
}

void SoundInstance::Play()
{
    m_sfSound->play();
}

}   // namespace gugu
