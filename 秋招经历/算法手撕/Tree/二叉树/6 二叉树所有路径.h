#include "BTree.h"

vector<string> res;
vector<int> path;
void backtraversal(TreeNode* root) {
    //先将根结点插入
    path.push_back(root->val);

    //结束条件
    if(root->left == nullptr && root->right == nullptr) {
        string rootpath;
        for(int i = 0; i < path.size() - 1; ++i) {
            rootpath += to_string(path[i]);
            rootpath += "->";
        }
        rootpath += to_string(path[path.size() - 1]);
        res.push_back(rootpath);
        return;
    }
    //回溯逻辑
    if(root->left) {
        backtraversal(root->left);
        path.pop_back();
    }
    if(root->right) {
        backtraversal(root->right);
        path.pop_back();
    }
}

vector<string> binaryTreePaths(TreeNode* root) {
    res.clear();
    path.clear();
    backtraversal(root);
    return res;
}