#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

#include <imgui.h>  // TODO: see if I can get rid of this include.

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EditorCore
{
public:

    EditorCore();
    virtual ~EditorCore();

    void Start();
    void Stop();

    void Update(const gugu::DeltaTime& dt);

    void ResetPanels();

private:

    class TreeNode
    {
    public:

        std::string name;
        bool isFolder;
        std::vector<TreeNode*> children;
    };

    void UpdateAssetsExplorer();

    void RecursiveSortTreeNodes(TreeNode* node);
    void DisplayTreeNode(TreeNode* node, ImGuiTreeNodeFlags directoryFlags, ImGuiTreeNodeFlags fileFlags, bool test_drag_and_drop, bool table, int depth, bool expandAll, bool collapseAll);

    static bool CompareTreeNodes(const TreeNode* left, const TreeNode* right);

private:

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    TreeNode* m_rootNode;

    std::string m_testFileOpen;
};

}   //namespace gugu
