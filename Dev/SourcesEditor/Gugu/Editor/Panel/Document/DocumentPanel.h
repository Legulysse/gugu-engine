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
    virtual void UpdateHierarchy(const DeltaTime& dt) final;
    virtual void UpdateProperties(const DeltaTime& dt) final;

    bool Save();
    bool Undo();
    bool Redo();

    Resource* GetResource() const;
    const std::string& GetResourceID() const;
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

    virtual bool SaveToFileImpl();
    virtual bool LoadFromStringImpl(const std::string& value);
    virtual bool SaveToStringImpl(std::string& result);

    virtual void OnSaved() {}
    virtual void OnUndoRedo() {}

    virtual void OnVisibilityChanged(bool visible) {}

    virtual void UpdatePanelImpl(const DeltaTime& dt) = 0;
    virtual void UpdateHierarchyImpl(const DeltaTime& dt) {}
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) {}

protected:

    std::string m_resourceID;
    Resource* m_resource;

    bool m_visible;
    bool m_focused;
    bool m_closing;
    bool m_closed;

private:

    bool m_dirty;

    std::vector<std::string> m_undoStates;
    size_t m_currentUndoStateIndex;
};

}   //namespace gugu
