#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class TexturePanel : public DocumentPanel
{
public:

    TexturePanel(Texture* resource);
    virtual ~TexturePanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    Texture* m_texture;
};

}   //namespace gugu
