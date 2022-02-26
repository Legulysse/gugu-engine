#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
}

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
    void ForceFocus();

protected:

    std::string m_title;
    bool m_focused;
};

}   //namespace gugu
