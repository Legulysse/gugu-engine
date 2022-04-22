#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Resource;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DocumentPanel : public BasePanel
{
public:

    DocumentPanel(Resource* resource);
    virtual ~DocumentPanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) override;
    virtual void UpdateProperties(const gugu::DeltaTime& dt) = 0;

    bool Save();
    bool Undo();

    bool IsSameResource(const std::string& resourceID) const;

    bool IsFocused() const;
    void ForceFocus();

    bool IsDirty() const;

    bool IsClosing() const;
    bool IsClosed() const;
    void CancelClosing();
    void ValidateClosing();

protected:

    virtual bool SaveImpl();
    virtual bool UndoImpl();
    virtual void UpdatePanelImpl(const gugu::DeltaTime& dt) = 0;

protected:

    std::string m_resourceID;
    Resource* m_resource;

    bool m_dirty;
    bool m_focused;
    bool m_closing;
    bool m_closed;
};

}   //namespace gugu
