#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"
#include "Gugu/Utility/StateMachine.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {
    
class Demo : public gugu::Application, public gugu::StateMachine, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppStep(const gugu::DeltaTime& dt) override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;
};

}   //namespace demoproject
