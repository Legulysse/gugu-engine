#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AudioMixerGroup;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AudioMixerGroupPanel : public DocumentPanel
{
public:

    AudioMixerGroupPanel(AudioMixerGroup* resource);
    virtual ~AudioMixerGroupPanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

private:

    AudioMixerGroup* m_mixerGroup;
};

}   //namespace gugu
