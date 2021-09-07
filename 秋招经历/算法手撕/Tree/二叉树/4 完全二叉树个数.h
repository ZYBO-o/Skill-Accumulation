#include "BTree.h"

//先求它的左子树的节点数量，再求的右子树的节点数量，最后取总和再加一 
int TreeNodes(TreeNode* root) {
    if(root == nullptr)
        return 0;
    int leftNodes = TreeNodes(root->left);
    int rightNodes = TreeNodes(root->right);

    return 1 + rightNodes + leftNodes;
}
int countNodes(TreeNode* root) {
    return TreeNodes(root);
}

//层次遍历实现：
int countNodes2(TreeNode* root) {
    if(root == nullptr) return 0;
    queue<TreeNode*> queue;
    queue.push(root);
    int count = 0;
    while(!queue.empty()) {
        int size = queue.size();
        for(int i = 0; i < size; ++i) {
            TreeNode* node = queue.front();
            queue.pop();
            ++count;
            if(node->left)
                queue.push(node->left);
            if(node->right)
                queue.push(node->right);
        }
    }
    return count;
}