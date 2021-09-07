#include "BTree.h"

TreeNode* traversal(vector<int>& preorder, vector<int>& inorder) {
    //第一步，如果先序遍历的长度为0，直接返回空
    if(preorder.size() == 0)
        return nullptr;
    //第二步：将先序遍历的第一个元素作为切割点
    int rootValue = preorder[0];
    TreeNode* root = new TreeNode(rootValue);
    //如果是元素为1，直接作为叶子结点
    if(preorder.size() == 1) return root;
    //第三步： 找到在中序数组中切割点的位置
    int delimitIndex;
    for(delimitIndex = 0; delimitIndex < inorder.size(); ++delimitIndex) 
        if(inorder[delimitIndex] == rootValue)
            break;
    //第四步：切割中序数组，得到中序左数组与中序右数组
    vector<int> leftInder(inorder.begin(), inorder.begin() + delimitIndex);
    vector<int> rightInder(inorder.begin() + delimitIndex + 1, inorder.end());
    //第五步：切割先序数组，得到先序左数组与先序右数组
    preorder.erase(preorder.begin(), preorder.begin() + 1);
    vector<int> leftPreorder(preorder.begin(), preorder.begin() + leftInder.size());
    vector<int> rightPreorder(preorder.begin() + leftInder.size(), preorder.end());
    //第六步：递归处理左区间和右区间
    root->left = traversal(leftPreorder, leftInder);
    root->right = traversal(rightPreorder, rightInder);
    return root;
}
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    if(preorder.size() == 0 || inorder.size() == 0)
        return nullptr;
    return traversal(preorder, inorder);
}