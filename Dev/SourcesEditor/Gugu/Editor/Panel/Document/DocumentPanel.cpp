////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DocumentPanel::DocumentPanel(Resource* resource)
    : m_resource(resource)
    , m_dirty(false)
    , m_visible(false)
    , m_focused(false)
    , m_closing(false)
    , m_closed(false)
    , m_currentUndoStateIndex(0)
{
    m_resourceID = m_resource->GetID();
    m_title = m_resourceID;
}

DocumentPanel::~DocumentPanel()
{
}

void DocumentPanel::InitializePanel()
{
    SaveState();
}

void DocumentPanel::UpdatePanel(const DeltaTime& dt)
{
    bool wasVisible = m_visible;
    m_visible = false;
    m_focused = false;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar;
    if (m_dirty)
    {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }

    // TODO: do I need to push some id on the id stack, to avoid potential collisions ?
    bool isOpen = true;
    if (ImGui::Begin(m_title.c_str(), &isOpen, flags))
    {
        m_visible = true;

        if (m_visible && !wasVisible)
        {
            OnVisibilityChanged(true);
        }

        ImGuiFocusedFlags focusedFlags = ImGuiFocusedFlags_ChildWindows;
        if (ImGui::IsWindowFocused(focusedFlags))
        {
            m_focused = true;
        }

        UpdatePanelImpl(dt);
    }

    if (!m_visible && wasVisible)
    {
        OnVisibilityChanged(false);
    }

    ImGui::End();

    m_closing |= !isOpen;
}

void DocumentPanel::UpdateProperties(const DeltaTime& dt)
{
    UpdatePropertiesImpl(dt);
}

void DocumentPanel::UpdateHierarchy(const DeltaTime& dt)
{
    UpdateHierarchyImpl(dt);
}

bool DocumentPanel::Save()
{
    if (m_resource && SaveToFileImpl())
    {
        m_dirty = false;

        OnSaved();
        return true;
    }

    return false;
}

bool DocumentPanel::Undo()
{
    if (UndoState())
    {
        OnUndoRedo();
        return true;
    }

    return false;
}

bool DocumentPanel::Redo()
{
    if (RedoState())
    {
        OnUndoRedo();
        return true;
    }

    return false;
}

void DocumentPanel::SaveState()
{
    std::string state;
    if (SaveStateImpl(state))
    {
        // Erase history past the current index.
        if (m_currentUndoStateIndex + 1 < m_undoStates.size())
        {
            m_undoStates.resize(m_currentUndoStateIndex + 1);
        }

        m_undoStates.push_back(state);

        // Erase older states if reaching the limit (minimum of 2 states : initial state + last state).
        size_t maxUndoStateCount = 1 + GetEditor()->GetEditorConfig().maxUndoStateCount;
        maxUndoStateCount = Max<size_t>(2, maxUndoStateCount);

        if (m_undoStates.size() > maxUndoStateCount)
        {
            size_t eraseCount = m_undoStates.size() - maxUndoStateCount;
            StdVectorRemoveAt(m_undoStates, 1, eraseCount);
        }

        m_currentUndoStateIndex = m_undoStates.size() - 1;
    }
}

void DocumentPanel::ReloadCurrentState()
{
    if (!m_undoStates.empty() && LoadStateImpl(m_undoStates[m_currentUndoStateIndex]))
    {
        GetResources()->NotifyResourceUpdated(m_resource);
        GetResources()->UpdateResourceDependencies(m_resource);
    }
}

bool DocumentPanel::UndoState()
{
    if (!m_undoStates.empty() && m_currentUndoStateIndex > 0)
    {
        size_t newIndex = m_currentUndoStateIndex - 1;
        if (LoadStateImpl(m_undoStates[newIndex]))
        {
            GetResources()->NotifyResourceUpdated(m_resource);
            GetResources()->UpdateResourceDependencies(m_resource);

            m_currentUndoStateIndex = newIndex;
            m_dirty = true;
            return true;
        }
    }

    return false;
}

bool DocumentPanel::RedoState()
{
    if (!m_undoStates.empty() && m_currentUndoStateIndex + 1 < m_undoStates.size())
    {
        size_t newIndex = m_currentUndoStateIndex + 1;
        if (LoadStateImpl(m_undoStates[newIndex]))
        {
            GetResources()->NotifyResourceUpdated(m_resource);
            GetResources()->UpdateResourceDependencies(m_resource);

            m_currentUndoStateIndex = newIndex;
            m_dirty = true;
            return true;
        }
    }

    return false;
}

bool DocumentPanel::SaveToFileImpl()
{
    return m_resource->SaveToFile();
}

bool DocumentPanel::LoadStateImpl(const std::string& value)
{
    return m_resource->LoadFromString(value);
}

bool DocumentPanel::SaveStateImpl(std::string& result)
{
    return m_resource->SaveToString(result);
}

Resource* DocumentPanel::GetResource() const
{
    return m_resource;
}

const std::string& DocumentPanel::GetResourceID() const
{
    return m_resourceID;
}

bool DocumentPanel::IsSameResource(const std::string& resourceID) const
{
    return m_resourceID == resourceID;
}

bool DocumentPanel::IsFocused() const
{
    return m_focused;
}

void DocumentPanel::ForceFocus()
{
    ImGui::SetWindowFocus(m_title.c_str());
}

void DocumentPanel::RaiseDirty()
{
    GetResources()->NotifyResourceUpdated(m_resource);
    GetResources()->UpdateResourceDependencies(m_resource);

    SaveState();
    m_dirty = true;
}

bool DocumentPanel::IsDirty() const
{
    return m_dirty;
}

bool DocumentPanel::Close()
{
    if (m_closed)
        return true;

    if (m_dirty)
    {
        m_closing = true;
    }
    else
    {
        m_closing = false;
        m_closed = true;
    }

    return m_closed;
}

void DocumentPanel::CancelClosing()
{
    m_closing = false;
}

void DocumentPanel::ValidateClosing()
{
    if (m_closing)
    {
        m_closing = false;
        m_closed = true;
    }
}

bool DocumentPanel::IsClosing() const
{
    return m_closing;
}

bool DocumentPanel::IsClosed() const
{
    return m_closed;
}

}   //namespace gugu
