#include "BTree.h"

using namespace std;

class Solution {
public:
    void dfs(TreeNode* root, vector<int>& res, int depth){
        if(root == nullptr)
            return;
        if(res.size() == depth) {
            res.push_back(root->val);
        }
        depth++;
        dfs(root->right, res, depth);
        dfs(root->left, res, depth);
    }
    vector<int> rightSideView(TreeNode* root) {
        vector<int> res;
        dfs(root, res, 0);
        return res;
    }
};