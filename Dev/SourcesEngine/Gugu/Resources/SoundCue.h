#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Audio/SoundInstance.h"

#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AudioMixerGroup;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SoundCue : public Resource
{
public:

    struct ClipEntry
    {
        AudioClip* audioClip = nullptr;
        std::string audioClipId;
        float volume = 1.f;
        float pitchLowerOffset = 0.f;
        float pitchUpperOffset = 0.f;
    };

public:

    SoundCue();
    virtual ~SoundCue();

    void SetMixerGroup(AudioMixerGroup* mixerGroup);
    AudioMixerGroup* GetMixerGroup() const;

    void SetVolumeAttenuation(float volumeAttenuation);
    float GetVolumeAttenuation() const;

    size_t GetSoundCount() const;
    bool GetClip(size_t index, ClipEntry& clipEntry) const;

    bool GetRandomSound(SoundParameters& parameters) const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    void RecomputeRuntimeSoundParameters();

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    std::vector<ClipEntry> m_audioClips;
    AudioMixerGroup* m_mixerGroup;
    float m_volumeAttenuation;

    // Runtime data.
    std::vector<SoundParameters> m_soundParameters;
};

}   // namespace gugu
