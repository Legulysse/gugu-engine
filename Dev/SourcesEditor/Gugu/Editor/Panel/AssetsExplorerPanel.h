#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include "Gugu/Core/DeltaTime.h"

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AssetsExplorerPanel : public BasePanel
{
public:

    AssetsExplorerPanel();
    virtual ~AssetsExplorerPanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) override;

    void RefreshContent(const std::string& projectAssetsPath);
    void ClearContent();

private:

    class TreeNode
    {
    public:

        std::string name;
        bool isFolder;
        std::vector<TreeNode*> children;
    };

    void RecursiveSortTreeNodes(TreeNode* node);
    void RecursiveDeleteTreeNodes(TreeNode* node);
    void DisplayTreeNode(TreeNode* node, int directoryFlags, int fileFlags, bool test_drag_and_drop, bool table, int depth, bool expandAll, bool collapseAll);

    static bool CompareTreeNodes(const TreeNode* left, const TreeNode* right);

private:

    TreeNode* m_rootNode;
};

}   //namespace gugu
