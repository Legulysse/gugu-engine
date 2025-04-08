#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Music;
    class AudioMixerGroupInstance;
}

namespace sf
{
    class Music;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class MusicInstance
{
public:

    MusicInstance();
    ~MusicInstance();

    void Reset();

    void SetMusic(Music* music, bool loop);
    bool IsSet() const;
    Music* GetMusic() const;

    void SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance);
    void SetVolume(float volume);

    void SetFadeCoeff(float fadeCoeff);
    float GetFadeCoeff() const;

    void RecomputeMixedVolume();

    void Play();
    void Pause();
    void Stop();

    sf::Time GetDuration() const;
    sf::Time GetPlayOffset() const;
    void SetPlayOffset(const sf::Time& offset);
    //TODO: Investigate loop points feature

    bool IsPaused() const;
    bool IsFinished() const;

private:

    Music* m_music;
    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Music* m_sfMusic;
    float m_volume;
    float m_fadeCoeff;
};

}   // namespace gugu
