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

class BaseModalDialog
{
public:

    BaseModalDialog(const std::string& title);
    virtual ~BaseModalDialog();

    void UpdateModal(const gugu::DeltaTime& dt);
    bool IsClosed() const;

protected:

    virtual void UpdateModalImpl(const gugu::DeltaTime& dt) = 0;
    void CloseModalImpl();

private:

    std::string m_title;
    bool m_opening;
    bool m_closed;
};

}   //namespace gugu
