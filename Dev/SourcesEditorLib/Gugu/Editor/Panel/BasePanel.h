#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class BasePanel
{
public:

    BasePanel();
    virtual ~BasePanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) = 0;

    bool IsFocused() const;

protected:

    bool m_focused;
};

}   //namespace gugu
