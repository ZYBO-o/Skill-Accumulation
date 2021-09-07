#include "BTree.h"

//搜索整个树，且有返回值
int getDepth(TreeNode* root) {
    //空节点情况
    if(root == nullptr) return 0;
    //非空
    //先求左子树深度，再右子树深度，最后取左右深度最大的数值 再+1 （加1是因为算上当前中间节点）就是目前节点为根节点的树的深度。
    int leftDepth = getDepth(root->left);       //左
    int rightDepth = getDepth(root->right);     //右
    int depth = 1 + max(leftDepth, rightDepth); //上
    return depth;
}
int maxDepth(TreeNode* root) {
    return getDepth(root);
}

//利用层次遍历的方法：
int maxDepth2(TreeNode* root) {
    if(!root) return 0;
    queue<TreeNode*> queue;
    queue.push(root);
    int count = 0;
    while(!queue.empty()) {
        int size = queue.size();
        for(int i = 0; i < size; ++i) {
            TreeNode* node = queue.front();
            queue.pop();
            if(node->left)
                queue.push(node->left);
            if(node->right)
                queue.push(node->right);
        }
        ++ count;
    }
    return count;
}


