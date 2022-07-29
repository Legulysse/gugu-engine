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

    virtual void UpdateProperties(const gugu::DeltaTime& dt) override;

protected:

    virtual void UpdatePanelImpl(const gugu::DeltaTime& dt) override;


private:

    RenderViewport* m_renderViewport;

    Texture* m_texture;
};

}   //namespace gugu
