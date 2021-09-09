#include"BTree.h"
#include<queue>
#include<iostream>
#include <vector>
#include<algorithm>

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int> > res;   vector<int> temp;
        if(root == NULL) return res;
        queue<TreeNode*> q; q.emplace(root);
        while(!q.empty()) {
            int len = q.size();
            while(len--) {
                TreeNode* node = q.front(); q.pop();
                temp.emplace_back(node->val);
                if(node->left) q.emplace(node->left);
                if(node->right) q.emplace(node->right);
            }
            res.emplace_back(temp); temp.clear();
        }
        for(int i = 1; i < res.size(); i += 2)
            reverse(res[i].begin(), res[i].end());
        return res;
    }
};