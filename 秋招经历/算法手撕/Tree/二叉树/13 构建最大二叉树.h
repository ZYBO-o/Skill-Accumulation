#include "BTree.h"

TreeNode* traversal(vector<int>& nums) {
    //第一步：如果大小为0，直接返回空
    if(nums.size() == 0)
        return nullptr;
    //第二步：找到最大值作为结点
    auto rootVal = max_element(nums.begin(), nums.end());
    int delimit = 0;
  	//第三步： 找到切割点的位置
    for(delimit = 0; delimit < nums.size() - 1; ++delimit) {
        if(nums[delimit] == *rootVal)
            break;
    }
    TreeNode* root = new TreeNode(*rootVal);
    if(nums.size() == 1)
        return root;
    //第四步：切割数组，得到左数组与右数组
    vector<int> leftTree(nums.begin(), nums.begin() + delimit);
    vector<int> rightTree(nums.begin() + delimit + 1, nums.end());
  	//第五步：递归处理左区间和右区间
    root->left = traversal(leftTree);
    root->right = traversal(rightTree);
    return root;
}
TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
    return traversal(nums);
}
