#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

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
};

}   //namespace gugu
