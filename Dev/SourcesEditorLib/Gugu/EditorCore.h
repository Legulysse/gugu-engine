#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

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

    void UpdateAssetsExplorer1();
    void UpdateAssetsExplorer2();

    void RecursiveSortTreeNodes(TreeNode* node);
    void DisplayTreeNode1(TreeNode* node, ImGuiTreeNodeFlags directoryFlags, ImGuiTreeNodeFlags fileFlags, bool test_drag_and_drop);
    void DisplayTreeNode2(TreeNode* node, ImGuiTreeNodeFlags directoryFlags, ImGuiTreeNodeFlags fileFlags, bool test_drag_and_drop);

    static bool CompareTreeNodes(const TreeNode* left, const TreeNode* right);

private:

    bool m_resetPanels;
    bool m_showSearchResults;
    bool m_showImGuiDemo;

    TreeNode* m_rootNode;
};

}   //namespace gugu
