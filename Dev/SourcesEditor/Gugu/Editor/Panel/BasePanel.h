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

    virtual void UpdatePanel(const DeltaTime& dt) = 0;

protected:

    std::string m_title;
};

}   //namespace gugu
