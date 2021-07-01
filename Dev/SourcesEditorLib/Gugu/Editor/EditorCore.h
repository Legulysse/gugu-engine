#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AssetsExplorerPanel;
    class ImageSetPanel;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EditorCore
{
public:

    EditorCore();
    virtual ~EditorCore();

    void Start();
    void Stop();

    void Update(const gugu::DeltaTime& dt);

    void ResetPanels();

private:

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    AssetsExplorerPanel* m_assetsExplorerPanel;
    ImageSetPanel* m_imageSetPanel;
};

}   //namespace gugu
