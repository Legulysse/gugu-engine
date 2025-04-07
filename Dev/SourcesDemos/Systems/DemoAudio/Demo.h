#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSlider;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Demo : public gugu::Application, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;

protected:

    enum EButton
    {
        Exit,
        SoundBubblesFive,
        SoundBubblesPitch,
        TrackColors,
        TrackAbyss,
        TrackMighty3310Fight,
        TrackMighty3310Boss,
        TrackMighty3310Final,
        PlaylistMighty3310,
        StopAll,
    };

    enum ESlider
    {
        MasterVolume,
    };

    void OnButtonClick(int _eButton);
    void OnSliderChanged(int sliderId, gugu::ElementSlider* slider);

protected:

    gugu::Element* m_root;
};

}   //namespace demoproject
