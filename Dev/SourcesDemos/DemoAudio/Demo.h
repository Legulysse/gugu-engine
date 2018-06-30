#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Demo : public gugu::Application, public gugu::EventListener
{
public:

	Demo();
	virtual ~Demo();

    virtual	void AppStart() override;
    virtual	void AppStop() override;

protected:

	enum EButton
	{
		Exit,
		SoundBubbles,
		TrackColors,
		TrackAbyss,
		TrackMighty3310Fight,
		TrackMighty3310Boss,
        TrackMighty3310Final,
        PlaylistMighty3310,
		StopAll,
	};

	void OnButtonClick(int _eButton);

protected:

	gugu::Element* m_root;
};

}   //namespace demoproject
