#include "BSTree.h"
#include <stack>
using TreeNode = BSTreeNode;



class Solution2 {
public:
    TreeNode* pre = NULL; // 用来记录前一个节点 
    bool isValidBST(TreeNode* root) {
        if (root == NULL) 
            return true;

        bool left = isValidBST(root->left);

        if (pre != NULL && pre->val >= root->val) 
            return false;

        pre = root; // 记录前一个节点

        bool right = isValidBST(root->right);
        
        return left && right;
    }
};
//=========================迭代====================
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        stack<TreeNode*> st;
        TreeNode* cur = root;
        TreeNode* pre = NULL; // 记录前一个节点
        while (cur != NULL || !st.empty()) {
            if (cur != NULL) {
                st.push(cur);
                cur = cur->left;                // 左
            } else {
                cur = st.top();                 // 中
                st.pop();
                if (pre != NULL && cur->val <= pre->val)
                return false;
                pre = cur; //保存前一个访问的结点

                cur = cur->right;               // 右
            }
        }
        return true;
    }
};