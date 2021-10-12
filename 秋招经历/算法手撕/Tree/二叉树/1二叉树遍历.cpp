#include "BTree.h"

//// 1 2 4 0 0 5 0 0 3 6 0 0 7 0 0 
void preorderTraversal(TreeNode *root) {
    stack<TreeNode*> s;
    while(!s.empty() || root != nullptr) {
        while(root != nullptr) {
            cout << root->val << " ";
            s.push(root);
            root = root->left;
        }
        root = s.top();
        s.pop();
        root = root->right;
    }
    cout << endl;
}

void postorderTraversal(TreeNode *root) {
    stack<TreeNode *> s;
    TreeNode *prev = nullptr;
    while (!s.empty() || root!=nullptr) {
        while (root) {                           // 1.遍历到最左子节点
            s.push(root);
            root = root->left;
        }
        root = s.top(); 
        s.pop();
        if (root->right && root->right != prev) {// 2.遍历最左子节点的右子树(右子树存在 && 未访问过)
            s.push(root);                        // 重复压栈以记录当前路径分叉节点
            root = root->right;      
        } else {                                 // 后序：填充vec在node->left和node->right后面
            cout << root->val << " ";            // 注意：此时node的左右子树应均已完成访问 
            prev = root;                        // 避免重复访问右子树[记录当前节点便于下一步对比]
            root = nullptr;                     // 避免重复访问左子树[设空节点]
        }
    }
    cout << endl;
}

void levelorder(TreeNode* root) {
    queue<TreeNode*> queue;
    queue.push(root);
    while(!queue.empty()) {
        int size = queue.size();
        for(int i = 0; i < size; ++i){
            TreeNode* node = queue.front();
            queue.pop();
            cout << node->val << " ";
            if(node->left)  queue.push(node->left);
            if(node->right) queue.push(node->right);
        }
    }
    cout << endl;
}

int main() {
    TreeNode* root = CreateTree();
    PrintTree(root);cout << endl;
    preorderTraversal(root);
    postorderTraversal(root);
    levelorder(root);
}