////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DocumentPanel::DocumentPanel(Resource* resource)
    : m_resource(resource)
    , m_dirty(false)
    , m_focused(false)
    , m_closing(false)
    , m_closed(false)
{
    m_resourceID = resource->GetID();
    m_title = m_resourceID;
}

DocumentPanel::~DocumentPanel()
{
}

void DocumentPanel::UpdatePanel(const DeltaTime& dt)
{
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
        if (ImGui::IsWindowFocused())
        {
            m_focused = true;
        }

        UpdatePanelImpl(dt);
    }
    ImGui::End();

    m_closing |= !isOpen;
}

bool DocumentPanel::Save()
{
    if (SaveImpl())
    {
        if (m_resource && m_resource->SaveToFile())
        {
            m_dirty = false;
            return true;
        }
    }

    return false;
}

bool DocumentPanel::Undo()
{
    UndoImpl();
    return true;
}

bool DocumentPanel::SaveImpl()
{
    return true;
}

bool DocumentPanel::UndoImpl()
{
    return true;
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

bool DocumentPanel::IsDirty() const
{
    return m_dirty;
}

bool DocumentPanel::IsClosing() const
{
    return m_closing;
}

bool DocumentPanel::IsClosed() const
{
    return m_closed;
}

void DocumentPanel::CancelClosing()
{
    m_closing = false;
}

void DocumentPanel::ValidateClosing()
{
    m_closed = m_closing;
    m_closing = false;
}

}   //namespace gugu
