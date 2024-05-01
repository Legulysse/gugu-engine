#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EditorApp : public Application, public EventListener
{
public:

    EditorApp();
    virtual ~EditorApp();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppUpdateImGui(const DeltaTime& dt) override;

    virtual bool OnMainWindowCloseEvent() override;

    virtual bool OnSFEvent(const sf::Event& event) override;
};

}   //namespace gugu
