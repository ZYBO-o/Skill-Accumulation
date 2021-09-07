#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;
// 1 2 4 0 0 5 0 0 3 6 0 0 7 0 0 
class TreeNode {
public:
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

TreeNode* CreateTree() {
    TreeNode* root;
    int value;
    cin >> value;
    if(value == 0)
        return nullptr;
    root = new TreeNode(value);
    root->left = CreateTree();
    root->right = CreateTree();
    return root;
}

void PrintTree(TreeNode* root) {
    if(root != nullptr) {
        std::cout << root->val << " ";
        PrintTree(root->left);
        PrintTree(root->right);
    } 
}