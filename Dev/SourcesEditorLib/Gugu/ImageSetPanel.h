#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class RenderViewport;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImageSetPanel
{
public:

    ImageSetPanel();
    ~ImageSetPanel();

    void Update(const gugu::DeltaTime& dt);

private:

    RenderViewport* m_renderViewport;
};

}   //namespace gugu
