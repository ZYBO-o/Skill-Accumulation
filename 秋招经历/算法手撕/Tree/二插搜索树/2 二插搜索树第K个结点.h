#include"BSTree.h"
#include <stack>
using TreeNode = BSTreeNode;

class Solution2 {
public:
    int kthLargest(TreeNode* root, int &k) {
        if (!root) 
            return 0;
        int result = kthLargest(root->right, k);

        if (!--k) 
            return root->val;

        return k < 0 ? result : kthLargest(root->left, k);
    }
};



class Solution {
public:
    int kthLargest(TreeNode* root, int k) {
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur != nullptr || !stk.empty()) {
            if (cur != nullptr) {
                stk.push(cur);
                cur = cur -> right;
            } else {
                cur = stk.top();
                stk.pop();
                if(--k == 0) return cur -> val;
                cur = cur -> left;
            }
        }
        return -1;
    }
};