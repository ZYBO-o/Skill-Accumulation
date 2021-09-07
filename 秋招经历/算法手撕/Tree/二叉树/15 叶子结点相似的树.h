#include"BTree.h"

void traversal (TreeNode* node, vector<int>&nums) {
    //结束条件
    if(node->left == nullptr && node->right == nullptr)  {
        nums.push_back(node->val);
        return;
    }
    //单层递归
    if(node->left) 
      	traversal(node->left, nums);
    if(node->right)
      	traversal(node->right, nums);
    return; 
}
bool leafSimilar(TreeNode* root1, TreeNode* root2) {
    vector<int> root1num;  
    vector<int> root2num;
    traversal(root1,root1num);
    traversal(root2,root2num);
    bool flag = true;
    if(root1num != root2num)
      	flag = false;
    return flag;
}