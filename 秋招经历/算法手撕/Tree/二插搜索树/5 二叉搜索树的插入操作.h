#include"BSTree.h"

using TreeNode = BSTreeNode;

class Solution {
private:
    TreeNode* parent;// 记录遍历节点的父节点
    void traversal(TreeNode* cur, int val) {
        //终止条件就是找到遍历的节点为null的时候，就是要插入节点的位置了，并把插入的节点返回。
        if (cur == NULL) {
            TreeNode* node = new TreeNode(val);
            if (val > parent->val) 
                parent->right = node;
            else 
                parent->left = node;
            return;
        }
        parent = cur;
        if (cur->val > val) traversal(cur->left, val);
        if (cur->val < val) traversal(cur->right, val);
        return;
    }

public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        parent = new TreeNode(0);
        if (root == NULL) {
            root = new TreeNode(val);
        }
        traversal(root, val);
        return root;
    }
};

//==========================迭代法=========================
class Solution2 {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (root == NULL) {
            TreeNode* node = new TreeNode(val);
            return node;
        }
        TreeNode* cur = root;
        TreeNode* parent = root; // 这个很重要，需要记录上一个节点，否则无法赋值新节点
        
        while (cur != NULL) {
            parent = cur;
            if (cur->val > val) 
                cur = cur->left;
            else 
                cur = cur->right;
        }
        
        TreeNode* node = new TreeNode(val);
        if (val < parent->val) 
            parent->left = node;// 此时是用parent节点的进行赋值
        else 
            parent->right = node;
        return root;
    }
};