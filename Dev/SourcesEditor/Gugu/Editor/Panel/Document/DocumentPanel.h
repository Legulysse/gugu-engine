#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DocumentPanel : public BasePanel
{
public:

    DocumentPanel();
    virtual ~DocumentPanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) override;

    virtual void UpdatePanelImpl(const gugu::DeltaTime& dt) = 0;
    virtual void UpdateProperties(const gugu::DeltaTime& dt) = 0;
    virtual bool Save() = 0;

    bool IsSameResource(const std::string& resourceID) const;

    bool IsFocused() const;
    void ForceFocus();

    bool IsClosed() const;

protected:

    std::string m_resourceID;
    bool m_dirty;
    bool m_focused;
    bool m_closed;
};

}   //namespace gugu
