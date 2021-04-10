////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/EditorCore.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"

#include <imgui.h>
#include <imgui_internal.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
EditorCore::EditorCore()
    : m_resetPanels(false)
    , m_showSearchResults(true)
{
}

EditorCore::~EditorCore()
{
}

void EditorCore::Start()
{
    // Additional ImGui Setup.
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Does not seem supported by the SFML backend.
    io.ConfigWindowsResizeFromEdges = true;
    io.ConfigDockingWithShift = false;
}

void EditorCore::Stop()
{
}

void EditorCore::Update(const DeltaTime& dt)
{
    // Main menu bar.
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Quit"))
            {
                // TODO: Only close the application in standalone mode, prefer hiding the editor when used as an overlay.
                GetEngine()->StopLooping();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Docking panels ids.
    const char* dockspaceWindowName = "EditorDockSpace Window";
    const ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");

    // Begin Dockspace Window.
    ImGuiWindowFlags window_flags = /*ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(dockspaceWindowName, false, window_flags);
    ImGui::PopStyleVar(3);

    // Editor panels preset.
    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL || m_resetPanels)
    {
        m_resetPanels = false;

        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
        ImGuiID dock_id_up = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, NULL, &dock_main_id);
        ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

        ImGui::DockBuilderDockWindow("Assets Explorer", dock_id_left);
        ImGui::DockBuilderDockWindow("ImageSet Editor", dock_main_id);
        ImGui::DockBuilderDockWindow("AnimSet Editor", dock_main_id);
        ImGui::DockBuilderDockWindow("Output Log", dock_id_down);
        ImGui::DockBuilderDockWindow("Search Results", dock_id_down);
        ImGui::DockBuilderDockWindow("Properties", dock_id_right);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::Begin("Assets Explorer", false);
    ImGui::End();

    if (ImGui::Begin("ImageSet Editor", false))
    {
        ImGui::Text("Here lies the future ImageSet Editor.");
    }
    ImGui::End();

    if (ImGui::Begin("AnimSet Editor", false))
    {
        ImGui::Text("Here lies the future AnimSet Editor.");
    }
    ImGui::End();

    ImGui::Begin("Output Log", false);
    ImGui::End();

    ImGui::Begin("Properties", false);
    ImGui::End();

    if (m_showSearchResults)
    {
        ImGui::Begin("Search Results", &m_showSearchResults);
        ImGui::End();
    }

    // End Dockspace Window.
    ImGui::End();
}

void EditorCore::ResetPanels()
{
    m_resetPanels = true;
    m_showSearchResults = true;
}

}   //namespace gugu
