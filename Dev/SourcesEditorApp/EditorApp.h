#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EditorApp : public gugu::Application, public gugu::EventListener
{
public:

    EditorApp();
    virtual ~EditorApp();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

    virtual bool OnMainWindowCloseEvent() override;

    virtual bool OnSFEvent(const sf::Event& event) override;
};

}   //namespace gugu
