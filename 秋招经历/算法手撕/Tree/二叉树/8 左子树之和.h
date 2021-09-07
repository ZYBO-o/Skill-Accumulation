#include "BTree.h"

//当遇到左叶子节点的时候，记录数值，然后通过递归求取左子树左叶子之和，和 右子树左叶子之和，相加便是整个树的左叶子之和。
int sumOfLeftLeaves(TreeNode* root) {
    if(root == nullptr) return 0;
    int LeftLeaves = sumOfLeftLeaves(root->left);
    int RightLeaves = sumOfLeftLeaves(root->right);
    int midValue = 0;
    if(root->left != nullptr && root->left->left == nullptr && root->left->right == nullptr)
        midValue = root->left->val;
    return midValue + LeftLeaves + RightLeaves;
}