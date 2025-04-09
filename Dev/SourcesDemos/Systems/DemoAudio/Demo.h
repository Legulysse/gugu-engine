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
    class ElementCheckbox;
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
        SoundBubbles01,
        SoundCueBubblesFive,
        SoundCueBubblesPitch,
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
        ListenerVolume,
        MasterVolume,
        MusicVolume,
        UIVolume,
        GameplayVolume,
    };

    void OnButtonClick(int buttonId);
    void OnSliderChanged(int sliderId, gugu::ElementSlider* slider);
    void OnCheckboxChanged(int sliderId, gugu::ElementCheckbox* checkbox);

protected:

    gugu::Element* m_root;
};

}   //namespace demoproject
