#include "BTree.h"

int getDepth(TreeNode* root) {
    if(root == nullptr) return 0;
    int leftDepth = getDepth(root->left);
    int rightDepth = getDepth(root->right);
    //左子树为空，右子树不为空，则最小深度在右子树上
    if(root->left == nullptr && root->right != nullptr)
        return 1 + rightDepth;
    //右子树为空，左子树不为空，则最小深度在左子树上
    if(root->right == nullptr && root->left != nullptr) 
        return 1 + leftDepth;
    //如果左右子树都为空，则返回左右子树深度最小值 + 1 
    return 1 + min(rightDepth , leftDepth);
}

int minDepth(TreeNode* root) {
    return getDepth(root);
}

//层次遍历实现： 首次出现左右子树都为空时为最小深度
int minDepth2(TreeNode* root) {
    if(root == nullptr) return 0;
    queue<TreeNode*> queue;
    queue.push(root);
    int depth = 0;
    int flag = 1;
    while(!queue.empty() && flag) {
        int size = queue.size();
        for(int i = 0; i < size; i++) {
            TreeNode* node = queue.front();
            queue.pop();
          	//左右子树都为空，则已经到达最底层
            if(node->left == nullptr && node->right == nullptr) {
                flag = 0;
                break;
            }
            if(node->left)
                queue.push(node->left);
            if(node->right)
                queue.push(node->right);
        }
        ++ depth;
    }
    return depth;
}