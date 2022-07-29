#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include <string>
#include <vector>

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

    virtual void UpdatePanel(const DeltaTime& dt) final;
    virtual void UpdateProperties(const DeltaTime& dt) final;

    bool Save();
    bool Undo();
    bool Redo();

    bool IsSameResource(const std::string& resourceID) const;

    bool IsFocused() const;
    void ForceFocus();

    void RaiseDirty();
    bool IsDirty() const;

    bool Close();
    void CancelClosing();
    void ValidateClosing();
    bool IsClosing() const;
    bool IsClosed() const;

protected:

    void SaveState();
    bool UndoState();
    bool RedoState();

    virtual void OnSaved() {}
    virtual void OnUndoRedo() {}

    virtual void UpdatePanelImpl(const DeltaTime& dt) = 0;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) {}

protected:

    std::string m_resourceID;
    Resource* m_resource;

    bool m_focused;
    bool m_closing;
    bool m_closed;

private:

    bool m_dirty;

    std::vector<std::string> m_undoStates;
    size_t m_currentUndoStateIndex;
};

}   //namespace gugu
