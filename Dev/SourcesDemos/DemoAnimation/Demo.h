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
    class ElementSpriteAnimated;
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

    virtual void AppStep(const gugu::DeltaTime& dt) override;

protected:

    gugu::Element* m_root;
    gugu::ElementSpriteAnimated* m_movingAnimationA;
    gugu::ElementSpriteAnimated* m_movingAnimationB;
};

}   //namespace demoproject
