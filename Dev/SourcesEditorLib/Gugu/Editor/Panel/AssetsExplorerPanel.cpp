////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/AssetsExplorerPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AssetsExplorerPanel::AssetsExplorerPanel()
    : m_rootNode(nullptr)
{
    m_title = "Assets Explorer";

    // Refresh assets tree structure.
    std::vector<const gugu::ResourceInfo*> vecInfos;
    gugu::GetResources()->GetAllResourceInfos(vecInfos);

    m_rootNode = new TreeNode;
    m_rootNode->isFolder = true;
    m_rootNode->name = "ROOT";

    std::string assetsPath = gugu::GetResources()->GetPathAssets();

    std::vector<std::string> tokens;
    for (const gugu::ResourceInfo* resourceInfo : vecInfos)
    {
        TreeNode* currentDirectory = m_rootNode;

        std::string resourcePath = resourceInfo->fileInfo.GetPath();
        if (resourcePath == assetsPath)
        {
            resourcePath = "";
        }
        else
        {
            resourcePath.erase(0, assetsPath.size());
        }

        StdStringSplit(resourcePath, "/", tokens);
        for (std::string directoryName : tokens)
        {
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
                currentDirectory->children.push_back(newDirectory);

                currentDirectory = newDirectory;
            }
        }

        TreeNode* newNode = new TreeNode;
        newNode->isFolder = false;
        newNode->name = resourceInfo->fileInfo.GetName();
        currentDirectory->children.push_back(newNode);
    }

    RecursiveSortTreeNodes(m_rootNode);
}

AssetsExplorerPanel::~AssetsExplorerPanel()
{
    RecursiveDeleteTreeNodes(m_rootNode);
    SafeDelete(m_rootNode);
}

void AssetsExplorerPanel::UpdatePanel(const gugu::DeltaTime& dt)
{
    if (ImGui::Begin(m_title.c_str(), false))
    {
        // Using those as a base value to create width/height that are factor of the size of our font
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        static ImGuiTreeNodeFlags directoryFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
        static ImGuiTreeNodeFlags fileFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
        static bool test_drag_and_drop = true;
        static bool testTable = false;

        bool expandAll = false;
        bool collapseAll = false;

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
        expandAll = ImGui::Button("Expand All");
        collapseAll = ImGui::Button("Collapse All");
        ImGui::Spacing();

        if (!testTable)
        {
            ImGui::PushID("AssetsTable");
            DisplayTreeNode(m_rootNode, directoryFlags, fileFlags, test_drag_and_drop, testTable, 0, expandAll, collapseAll);
            ImGui::PopID();
        }
        else
        {
            if (ImGui::BeginTable("AssetsTable", 3, tableFlags))
            {
                // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
                ImGui::TableHeadersRow();

                DisplayTreeNode(m_rootNode, directoryFlags, fileFlags, test_drag_and_drop, testTable, 0, expandAll, collapseAll);

                ImGui::EndTable();
            }
        }
    }
    ImGui::End();
}

void AssetsExplorerPanel::RecursiveSortTreeNodes(TreeNode* node)
{
    for (size_t i = 0; i < node->children.size(); ++i)
    {
        RecursiveSortTreeNodes(node->children[i]);
    }

    std::sort(node->children.begin(), node->children.end(), AssetsExplorerPanel::CompareTreeNodes);
}

void AssetsExplorerPanel::RecursiveDeleteTreeNodes(TreeNode* node)
{
    for (size_t i = 0; i < node->children.size(); ++i)
    {
        RecursiveDeleteTreeNodes(node->children[i]);
        SafeDelete(node->children[i]);
    }
}

void AssetsExplorerPanel::DisplayTreeNode(TreeNode* node, int directoryFlags, int fileFlags, bool test_drag_and_drop, bool isTable, int depth, bool expandAll, bool collapseAll)
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

        if (expandAll)
        {
            ImGui::SetNextItemOpen(true);
        }
        else if (collapseAll)
        {
            ImGui::SetNextItemOpen(false);
        }

        bool isOpen = ImGui::TreeNodeEx(node->name.c_str(), nodeFlags);

        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }

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
                DisplayTreeNode(node->children[i], directoryFlags, fileFlags, test_drag_and_drop, isTable, depth, expandAll, collapseAll);
            }

            ImGui::TreePop();
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

        if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
        {
            GetEditor()->OpenDocument(node->name);
        }
        else if (ImGui::IsItemClicked())
        {
            // TODO: handle selection.
        }

        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("This is a drag and drop source");
            ImGui::EndDragDropSource();
        }

        if (isTable)
        {
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("0 ko");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("resource");
        }
    }
}

bool AssetsExplorerPanel::CompareTreeNodes(const TreeNode* left, const TreeNode* right)
{
    // Default comparator to get a clean file hierarchy (alphabetical order, folders before files).
    return (left->isFolder && !right->isFolder)
        || (left->isFolder == right->isFolder && left->name < right->name);
}

}   //namespace gugu
