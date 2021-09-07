#include "BTree.h"

int maxLen = INT_MIN;
int maxleftValue;

void traversal(TreeNode* node, int leftLen) {
    if(node->left == nullptr && node->right == nullptr) {
        if(leftLen > maxLen) {
            maxLen = leftLen;
            maxleftValue = node->val;
        }
        return; 
    }
    if(node->left) {
        leftLen++;
        traversal(node->left, leftLen);
        leftLen--;
    } 
    if(node->right) {
        leftLen++;
        traversal(node->right, leftLen);
        leftLen--;
    }
    return ;
}
int findBottomLeftValue(TreeNode* root) {
    traversal(root, 0);
    return maxleftValue;
}

























