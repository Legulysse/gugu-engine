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
    class DeltaTime;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AssetsExplorerPanel : public BasePanel
{
public:

    AssetsExplorerPanel();
    virtual ~AssetsExplorerPanel();

    virtual void UpdatePanel(const DeltaTime& dt) override;

    void RaiseDirtyContent();
    void RefreshContent();
    void ClearContent();

private:

    class TreeNode
    {
    public:

        TreeNode();
        ~TreeNode();

        std::string name;
        std::string path;
        bool isFolder;
        std::vector<TreeNode*> children;

        std::string resourceID;
    };

    void CreateNewDirectory(TreeNode* parentNode);

    void SortTreeNodeChildren(TreeNode* rootNode, bool recursive);
    void DisplayTreeNode(TreeNode* node, int directoryFlags, int fileFlags, bool test_drag_and_drop, bool table, int depth, bool expandAll, bool collapseAll);

    void HandleDirectoryContextMenu(TreeNode* node);
    void HandleFileContextMenu(TreeNode* node);

    static bool CompareTreeNodes(const TreeNode* left, const TreeNode* right);

private:

    TreeNode* m_rootNode;
    bool m_dirtyContent;
};

}   //namespace gugu
