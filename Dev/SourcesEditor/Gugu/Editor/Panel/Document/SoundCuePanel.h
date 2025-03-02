#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class SoundCue;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SoundCuePanel : public DocumentPanel
{
public:

    SoundCuePanel(SoundCue* resource);
    virtual ~SoundCuePanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;
    virtual bool SaveStateImpl(std::string& result) override;

private:

    SoundCue* m_soundCue;
};

}   //namespace gugu
