#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

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
};

}   //namespace demoproject
