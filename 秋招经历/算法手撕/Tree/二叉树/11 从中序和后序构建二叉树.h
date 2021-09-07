#include "BTree.h"

TreeNode* traversal(vector<int>& inorder, vector<int>& postorder) {
    
    if(postorder.size() == 0)                                                        //第一步，如果后序遍历的长度为0，直接返回空
        return nullptr;
    
    int rootValue = postorder[postorder.size() - 1];                                //第二步，将后序数组中的最后一个数组作为切割点
    TreeNode* root = new TreeNode(rootValue);
    //叶子结点
    if(postorder.size() == 1) return root;
    
    int delimiterIndex;                                                             //第三步，找到切割点
    for(delimiterIndex = 0; delimiterIndex < inorder.size(); ++delimiterIndex)
        if(inorder[delimiterIndex] == rootValue)
            break;
    
    vector<int> leftInorder(inorder.begin(), inorder.begin() + delimiterIndex);     //第四步：切割中序数组，得到 中序左数组和中序右数组 
    vector<int> rightInorder(inorder.begin() + delimiterIndex + 1, inorder.end());
    
    // postorder 舍弃末尾元素，因为这个元素就是中间节点，已经用过了
    postorder.resize(postorder.size() - 1);                                         //第五步：切割后序数组，得到 后序左数组和后序右数组
    vector<int> leftPost(postorder.begin(), postorder.begin() + leftInorder.size());
    vector<int> rightPost(postorder.begin() + leftInorder.size(), postorder.end());
                                                                                     //第六步：递归处理左区间和右区间
    root->left = traversal(leftInorder, leftPost);
    root->right = traversal(rightInorder, rightPost);
    return root;
}
TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    if(inorder.size() == 0 || postorder.size() == 0)
        return nullptr;
    return traversal(inorder, postorder);
}