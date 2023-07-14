////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/AssetsExplorerPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Core/ProjectSettings.h"
#include "Gugu/Editor/Modal/NewResourceDialog.h"
#include "Gugu/Editor/Modal/NewDirectoryDialog.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

#include <SFML/Window/Clipboard.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AssetsExplorerPanel::TreeNode::TreeNode()
{
}

AssetsExplorerPanel::TreeNode::~TreeNode()
{
    ClearStdVector(children);
}

AssetsExplorerPanel::AssetsExplorerPanel()
    : m_rootNode(nullptr)
    , m_dirtyContent(false)
{
    m_title = "Assets Explorer";
}

AssetsExplorerPanel::~AssetsExplorerPanel()
{
    ClearContent();
}

void AssetsExplorerPanel::ClearContent()
{
    m_dirtyContent = false;

    SafeDelete(m_rootNode);
}

void AssetsExplorerPanel::RaiseDirtyContent()
{
    m_dirtyContent = true;
}

void AssetsExplorerPanel::RefreshContent()
{
    m_dirtyContent = false;

    ClearContent();

    const std::string& projectAssetsPath = GetEditor()->GetProjectSettings()->projectAssetsPath;
    const std::string& editorAssetsPath = GetResources()->GetPathAssets();

    m_rootNode = new TreeNode;
    m_rootNode->isFolder = true;
    m_rootNode->name = "ROOT";
    m_rootNode->path = projectAssetsPath;

    // Build assets tree directory structure.
    std::vector<std::string> directories;
    GetDirectories(projectAssetsPath, directories, true);

    std::vector<std::string> tokens;
    for (const std::string& directory : directories)
    {
        TreeNode* currentDirectory = m_rootNode;

        // Hide the project path from the hierarchy.
        std::string directoryPath = directory;
        directoryPath.erase(0, projectAssetsPath.size());

        StdStringSplit(directoryPath, system::PathSeparator, tokens);

        std::string cumulatedPath = projectAssetsPath;
        for (std::string directoryName : tokens)
        {
            cumulatedPath = cumulatedPath + system::PathSeparator + directoryName;
            bool foundDirectory = false;

            for (size_t i = 0; i < currentDirectory->children.size(); ++i)
            {
                if (currentDirectory->children[i]->isFolder && currentDirectory->children[i]->name == directoryName)
                {
                    currentDirectory = currentDirectory->children[i];
                    foundDirectory = true;
                    break;
                }
            }

            if (!foundDirectory)
            {
                TreeNode* newDirectory = new TreeNode;
                newDirectory->isFolder = true;
                newDirectory->name = directoryName;
                newDirectory->path = cumulatedPath;
                currentDirectory->children.push_back(newDirectory);

                currentDirectory = newDirectory;
            }
        }
    }

    // Refresh assets tree structure with known Resources.
    std::vector<const ResourceInfo*> vecInfos;
    GetResources()->GetAllResourceInfos(vecInfos);

    for (const ResourceInfo* resourceInfo : vecInfos)
    {
        TreeNode* currentDirectory = m_rootNode;

        std::string_view resourcePath = resourceInfo->fileInfo.GetDirectoryPath_utf8();

        // Ignore Editor assets.
        if (PathStartsWith(resourcePath, editorAssetsPath))
        {
            continue;
        }

        // Hide the project path from the hierarchy.
        resourcePath.remove_prefix(projectAssetsPath.size());

        StdStringSplit(resourcePath, system::PathSeparator, tokens);

        std::string cumulatedPath = projectAssetsPath;
        for (std::string directoryName : tokens)
        {
            cumulatedPath = cumulatedPath + system::PathSeparator + directoryName;
            bool foundDirectory = false;

            for (size_t i = 0; i < currentDirectory->children.size(); ++i)
            {
                if (currentDirectory->children[i]->isFolder && currentDirectory->children[i]->name == directoryName)
                {
                    currentDirectory = currentDirectory->children[i];
                    foundDirectory = true;
                    break;
                }
            }

            if (!foundDirectory)
            {
                TreeNode* newDirectory = new TreeNode;
                newDirectory->isFolder = true;
                newDirectory->name = directoryName;
                newDirectory->path = cumulatedPath;
                currentDirectory->children.push_back(newDirectory);

                currentDirectory = newDirectory;
            }
        }

        TreeNode* newNode = new TreeNode;
        newNode->isFolder = false;
        newNode->name = resourceInfo->fileInfo.GetFileName_utf8();
        newNode->path = resourceInfo->fileInfo.GetDirectoryPath_utf8();
        newNode->resourceID = resourceInfo->resourceID;
        currentDirectory->children.push_back(newNode);
    }

    SortTreeNodeChildren(m_rootNode, true);
}

void AssetsExplorerPanel::CreateNewDirectory(TreeNode* parentNode)
{
    GetEditor()->OpenModalDialog(new NewDirectoryDialog(parentNode->path, "",
        [=](const std::string& validatedDirectoryName)
        {
            std::string validatedDirectoryPath = CombinePaths(parentNode->path, validatedDirectoryName);

            if (!DirectoryExists(validatedDirectoryPath) && EnsureDirectoryExists(validatedDirectoryPath))
            {
                TreeNode* newDirectory = new TreeNode;
                newDirectory->isFolder = true;
                newDirectory->name = validatedDirectoryName;
                newDirectory->path = validatedDirectoryPath;
                parentNode->children.push_back(newDirectory);

                SortTreeNodeChildren(parentNode, false);
            }
        }
    ));
}

void AssetsExplorerPanel::UpdatePanel(const DeltaTime& dt)
{
    if (m_dirtyContent)
    {
        RefreshContent();
    }

    if (ImGui::Begin(m_title.c_str(), false))
    {
        if (m_rootNode)
        {
            // Using those as a base value to create width/height that are factor of the size of our font
            const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
            const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
            static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

            static ImGuiTreeNodeFlags directoryFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
            static ImGuiTreeNodeFlags fileFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
            static bool test_drag_and_drop = true;
            static bool testTable = false;

            static bool test_config = false;
            ImGui::Checkbox("Test Config", &test_config);
            if (test_config)
            {
                ImGui::Spacing();
                ImGui::PushID("directoryFlags");
                ImGui::TextUnformatted("Directories :");
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", &directoryFlags, ImGuiTreeNodeFlags_OpenOnArrow);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &directoryFlags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_Framed", &directoryFlags, ImGuiTreeNodeFlags_Framed);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &directoryFlags, ImGuiTreeNodeFlags_SpanAvailWidth);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &directoryFlags, ImGuiTreeNodeFlags_SpanFullWidth);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_DefaultOpen", &directoryFlags, ImGuiTreeNodeFlags_DefaultOpen);
                ImGui::PopID();

                ImGui::Spacing();
                ImGui::PushID("fileFlags");
                ImGui::TextUnformatted("Files :");
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_NoTreePushOnOpen", &fileFlags, ImGuiTreeNodeFlags_NoTreePushOnOpen);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_Bullet", &fileFlags, ImGuiTreeNodeFlags_Bullet);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_Framed", &fileFlags, ImGuiTreeNodeFlags_Framed);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &fileFlags, ImGuiTreeNodeFlags_SpanAvailWidth);
                ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &fileFlags, ImGuiTreeNodeFlags_SpanFullWidth);
                ImGui::PopID();

                ImGui::Checkbox("Test tree node as drag source", &test_drag_and_drop);
            }

            ImGui::Spacing();
            ImGui::Checkbox("Show as Table", &testTable);
            ImGui::Spacing();

            if (!testTable)
            {
                ImGui::PushID("_ASSETS_TABLE");
                DisplayTreeNode(m_rootNode, directoryFlags, fileFlags, test_drag_and_drop, testTable, 0);
                ImGui::PopID();
            }
            else
            {
                if (ImGui::BeginTable("_ASSETS_TABLE", 3, tableFlags))
                {
                    // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                    ImGui::TableHeadersRow();

                    DisplayTreeNode(m_rootNode, directoryFlags, fileFlags, test_drag_and_drop, testTable, 0);

                    ImGui::EndTable();
                }
            }
        }
    }
    ImGui::End();
}

void AssetsExplorerPanel::SortTreeNodeChildren(TreeNode* rootNode, bool recursive)
{
    for (size_t i = 0; i < rootNode->children.size(); ++i)
    {
        SortTreeNodeChildren(rootNode->children[i], recursive);
    }

    std::sort(rootNode->children.begin(), rootNode->children.end(), AssetsExplorerPanel::CompareTreeNodes);
}

void AssetsExplorerPanel::CollapseNode(TreeNode* rootNode, bool collapseSelf, bool collapseChildren, bool recursive)
{
    if (!rootNode->isFolder)
        return;

    if (collapseSelf)
    {
        ImGui::GetStateStorage()->SetInt(ImGui::GetID(rootNode->name.c_str()), 0);
    }

    if (collapseChildren)
    {
        ImGui::PushID(rootNode->name.c_str());

        for (size_t i = 0; i < rootNode->children.size(); ++i)
        {
            CollapseNode(rootNode->children[i], true, recursive, recursive);
        }

        ImGui::PopID();
    }
}

void AssetsExplorerPanel::ExpandNode(TreeNode* rootNode, bool expandSelf, bool expandChildren, bool recursive)
{
    if (!rootNode->isFolder)
        return;

    if (expandSelf)
    {
        ImGui::GetStateStorage()->SetInt(ImGui::GetID(rootNode->name.c_str()), 1);
    }

    if (expandChildren)
    {
        ImGui::PushID(rootNode->name.c_str());

        for (size_t i = 0; i < rootNode->children.size(); ++i)
        {
            ExpandNode(rootNode->children[i], true, recursive, recursive);
        }

        ImGui::PopID();
    }
}

void AssetsExplorerPanel::DisplayTreeNode(TreeNode* node, int directoryFlags, int fileFlags, bool test_drag_and_drop, bool isTable, int depth)
{
    if (isTable)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
    }

    if (node->isFolder)
    {
        ImGuiTreeNodeFlags nodeFlags = directoryFlags;
        if (depth == 0)
        {
            nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        }

        bool isOpen = ImGui::TreeNodeEx(node->name.c_str(), nodeFlags);

        // Context menu.
        bool collapseAll = false;
        bool collapseChildren = false;
        bool expandAll = false;
        HandleDirectoryContextMenu(node, &collapseAll, &collapseChildren, &expandAll);

        // Drag and drop.
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }

        // Table view details.
        if (isTable)
        {
            ImGui::TableNextColumn();
            ImGui::TextDisabled("--");
            ImGui::TableNextColumn();
            ImGui::TextDisabled("--");
        }

        if (isOpen)
        {
            ++depth;
            for (size_t i = 0; i < node->children.size(); ++i)
            {
                DisplayTreeNode(node->children[i], directoryFlags, fileFlags, test_drag_and_drop, isTable, depth);
            }

            ImGui::TreePop();
        }

        if (collapseAll)
        {
            CollapseNode(node, true, true, true);
        }

        if (collapseChildren)
        {
            CollapseNode(node, false, true, true);
        }

        if (expandAll)
        {
            ExpandNode(node, true, true, true);
        }
    }
    else
    {
        ImGuiTreeNodeFlags nodeFlags = fileFlags;

        bool is_selected = false; // TODO: handle selection.
        if (is_selected)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::TreeNodeEx(node->name.c_str(), nodeFlags);

        // Open Document.
        if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))     // ImGui::IsMouseDoubleClicked(0) 
        {
            GetEditor()->OpenDocument(node->resourceID);

            // TODO: handle selection.
        }

        // Context menu.
        HandleFileContextMenu(node);

        // Drag and drop.
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }

        // Table view details.
        if (isTable)
        {
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("0 ko");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("resource");
        }
    }
}

void AssetsExplorerPanel::HandleDirectoryContextMenu(TreeNode* node, bool* collapseAll, bool* collapseChildren, bool* expandAll)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::BeginMenu("New..."))
        {
            if (ImGui::MenuItem("Folder"))
            {
                CreateNewDirectory(node);
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Datasheet"))
            {
                GetEditor()->OpenModalDialog(new NewResourceDialog(node->path, EResourceType::Datasheet));
            }

            ImGui::Separator();
            if (ImGui::MenuItem("ImageSet"))
            {
                GetEditor()->OpenModalDialog(new NewResourceDialog(node->path, EResourceType::ImageSet));
            }

            if (ImGui::MenuItem("AnimSet"))
            {
                GetEditor()->OpenModalDialog(new NewResourceDialog(node->path, EResourceType::AnimSet));
            }

            if (ImGui::MenuItem("Particles"))
            {
                GetEditor()->OpenModalDialog(new NewResourceDialog(node->path, EResourceType::ParticleEffect));
            }

            if (ImGui::MenuItem("ElementWidget"))
            {
                GetEditor()->OpenModalDialog(new NewResourceDialog(node->path, EResourceType::ElementWidget));
            }

            ImGui::EndMenu();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Open in Explorer"))
        {
            OpenFileExplorer(node->path);
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Collapse All"))
        {
            *collapseAll = true;
        }

        if (ImGui::MenuItem("Collapse Children"))
        {
            *collapseChildren = true;
        }

        if (ImGui::MenuItem("Expand All"))
        {
            *expandAll = true;
        }

        ImGui::EndPopup();
    }
}

void AssetsExplorerPanel::HandleFileContextMenu(TreeNode* node)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Copy ID"))
        {
            sf::Clipboard::setString(sf::String::fromUtf8(node->resourceID.begin(), node->resourceID.end()));
        }

        if (ImGui::MenuItem("Open in Explorer"))
        {
            OpenFileExplorer(node->path);
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Delete"))
        {
            if (GetEditor()->CloseDocument(node->resourceID, true))
            {
                if (GetResources()->DeleteResource(node->resourceID))
                {
                    GetEditor()->RefreshAssets();
                }
            }
        }

        ImGui::EndPopup();
    }
}

bool AssetsExplorerPanel::CompareTreeNodes(const TreeNode* left, const TreeNode* right)
{
    // Default comparator to get a clean file hierarchy (alphabetical order, folders before files).
    return (left->isFolder && !right->isFolder)
        || (left->isFolder == right->isFolder && left->name < right->name);
}

}   //namespace gugu
