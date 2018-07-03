#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class RendererDemo;
}

namespace gugu
{
    class Element;
    class ElementSprite;
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

    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

protected:

    gugu::Element* m_root;

    RendererDemo* m_renderer;
    gugu::ElementSprite* m_twirl;
};

}   //namespace demoproject
