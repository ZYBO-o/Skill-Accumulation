#include "BTree.h"

//分别求出左右子树的高度，然后如果差值小于等于1，则返回当前二叉树的高度，否则则返回-1，表示已经不是二叉树了。
int getDepth(TreeNode* root) {
    int result;
    if(root == nullptr) return 0;
    int leftDepth = getDepth(root->left);
    int rightDepth = getDepth(root->right);
    if(leftDepth == -1)
        return -1;
    if(rightDepth == -1)
        return -1;
    if(abs(rightDepth - leftDepth) > 1)
        result = -1;
    else
        result = 1 + max(rightDepth , leftDepth);

    return result;
}
bool isBalanced(TreeNode* root) {
    return getDepth(root) == -1 ? false : true;
}