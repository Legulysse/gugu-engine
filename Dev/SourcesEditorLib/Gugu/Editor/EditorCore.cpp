////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/EditorCore.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/AssetsExplorerPanel.h"
#include "Gugu/Editor/Panel/DatasheetPanel.h"
#include "Gugu/Editor/Panel/ImageSetPanel.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Resource.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_internal.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
EditorCore::EditorCore()
    : m_resetPanels(false)
    , m_showSearchResults(true)
    , m_showImGuiDemo(false)
    , m_assetsExplorerPanel(nullptr)
    , m_lastActiveDocument(nullptr)
    , m_datasheetParser(nullptr)
{
    // This constructor should stay empty.
    // Because it's a singleton, if a GetInstance() is called inside by another system but the constructor isn't finished,
    // the GetInstance will try to create an other instance (loop).
}

EditorCore::~EditorCore()
{
    // Because of the constructor problem, I prefer to let the destructor also empty.
}

void EditorCore::Init(const EditorConfig& editorConfig)
{
    m_editorConfig = editorConfig;

    // Register Inputs.
    ManagerInputs* inputs = GetInputs();
    inputs->RegisterInput("ResetPanels", inputs->BuildKeyboardEvent(sf::Keyboard::F1));
    inputs->RegisterInput("SaveDocument", inputs->BuildKeyboardEvent(sf::Keyboard::S, true, false, false));
    inputs->RegisterInput("SaveAllDocuments", inputs->BuildKeyboardEvent(sf::Keyboard::S, true, true, false));

    // Additional ImGui Setup.
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Does not seem supported by the SFML backend.
    io.ConfigWindowsResizeFromEdges = true;
    io.ConfigDockingWithShift = false;

    // Create the AssetsExplorer Panel.
    m_assetsExplorerPanel = new AssetsExplorerPanel;

    // Create the DatasheetParser.
    m_datasheetParser = new DatasheetParser;
    m_datasheetParser->ParseBinding(m_editorConfig.pathDatasheetBinding);
}

void EditorCore::Release()
{
    m_lastActiveDocument = nullptr;
    ClearStdVector(m_documentPanels);

    SafeDelete(m_assetsExplorerPanel);
    SafeDelete(m_datasheetParser);

    EditorCore::DeleteInstance();
}

bool EditorCore::OnSFEvent(const sf::Event& event)
{
    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("ResetPanels", event))
    {
        ResetPanels();
        return false;
    }
    else if (inputs->IsInputReleased("SaveAllDocuments", event))
    {
        SaveAllDocuments();
        return false;
    }
    else if (inputs->IsInputReleased("SaveDocument", event))
    {
        SaveActiveDocument();
        return false;
    }

    return true;
}

void EditorCore::Update(const DeltaTime& dt)
{
    // Main menu bar.
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Quit", "Alt+F4"))
            {
                CloseEditor();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Document"))
        {
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                SaveActiveDocument();
            }

            if (ImGui::MenuItem("Save All", "Ctrl+Shift+S"))
            {
                SaveAllDocuments();
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Imgui Demo"))
        {
            m_showImGuiDemo = !m_showImGuiDemo;
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
    bool resetDocuments = false;
    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL || m_resetPanels)
    {
        resetDocuments = true;
        m_resetPanels = false;

        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
        //ImGuiID dock_id_up = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, NULL, &dock_main_id);
        ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

        ImGui::DockBuilderDockWindow("Assets Explorer", dock_id_left);
        //ImGui::DockBuilderDockWindow("ImageSet Editor", dock_main_id);
        //ImGui::DockBuilderDockWindow("AnimSet Editor", dock_main_id);
        ImGui::DockBuilderDockWindow("Output Log", dock_id_down);
        ImGui::DockBuilderDockWindow("Search Results", dock_id_down);
        ImGui::DockBuilderDockWindow("Properties", dock_id_right);
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    // Update AssetsExplorer panel.
    m_assetsExplorerPanel->UpdatePanel(dt);

    //if (ImGui::Begin("AnimSet Editor", false))
    //{
    //    ImGui::Text("Here lies the future AnimSet Editor.");
    //}
    //ImGui::End();

    // Update Documents panels.
    for (DocumentPanel* document : m_documentPanels)
    {
        ImGui::SetNextWindowDockID(dockspace_id, resetDocuments ? ImGuiCond_Always  : ImGuiCond_FirstUseEver);
        document->UpdatePanel(dt);

        if (document->IsFocused())
        {
            m_lastActiveDocument = document;
        }
    }

    // Update Properties panel.
    if (ImGui::Begin("Properties", false))
    {
        if (m_lastActiveDocument)
        {
            m_lastActiveDocument->UpdateProperties(dt);
        }
    }
    ImGui::End();

    // Update OutputLog panel.
    ImGui::Begin("Output Log", false);
    ImGui::End();

    if (m_showSearchResults)
    {
        ImGui::Begin("Search Results", &m_showSearchResults);
        ImGui::End();
    }

    if (m_showImGuiDemo)
    {
        ImGui::ShowDemoWindow();
    }

    // End Dockspace Window.
    ImGui::End();
}

void EditorCore::OpenDocument(const std::string& resourceID)
{
    FileInfo resourceFileInfo;
    if (!GetResources()->GetResourceFileInfo(resourceID, resourceFileInfo))
        return;

    for (DocumentPanel* document : m_documentPanels)
    {
        if (document->IsSameResource(resourceID))
        {
            // TODO: Force focus on the existing document.
            return;
        }
    }

    DocumentPanel* newDocument = nullptr;

    // Check native resource types.
    EResourceType::Type resourceType = GetResources()->GetResourceType(resourceFileInfo);
    if (resourceType == EResourceType::ImageSet)
    {
        newDocument = new ImageSetPanel(resourceID);
    }
    else if (resourceType == EResourceType::Unknown)
    {
        // Check datasheets.
        if (m_datasheetParser->IsDatasheet(resourceFileInfo))
        {
            newDocument = new DatasheetPanel(resourceID, resourceFileInfo);
        }
    }

    if (newDocument)
    {
        m_documentPanels.push_back(newDocument);
    }
}

bool EditorCore::SaveActiveDocument()
{
    if (!m_lastActiveDocument)
        return false;

    return m_lastActiveDocument->Save();
}

bool EditorCore::SaveAllDocuments()
{
    bool result = true;
    for (DocumentPanel* document : m_documentPanels)
    {
        result &= document->Save();
    }

    return result;
}

void EditorCore::ResetPanels()
{
    m_resetPanels = true;
    m_showSearchResults = true;
}

void EditorCore::CloseEditor()
{
    // TODO: Only close the application in standalone mode, prefer hiding the editor when used as an overlay.
    GetEngine()->StopMainLoop();
}

DatasheetParser* EditorCore::GetDatasheetParser() const
{
    return m_datasheetParser;
}

EditorCore* GetEditor()
{
    return EditorCore::GetInstance();
}

}   //namespace gugu
