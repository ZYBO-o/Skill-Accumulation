#include<vector>
#include<iostream>
#include"BTree.h"
#include<unordered_map>
class Solution {
private:

void searchBST(TreeNode* cur, unordered_map<int, int>& map) { // 前序遍历
    if (cur == NULL) return ;
    map[cur->val]++; // 统计元素频率
    searchBST(cur->left, map);
    searchBST(cur->right, map);
    return ;
}
bool static cmp (const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}
public:
    vector<int> findMode(TreeNode* root) {
        unordered_map<int, int> map; // key:元素，value:出现频率
        vector<int> result;
        if (root == NULL) 
            return result;

        searchBST(root, map);

        vector<pair<int, int>> vec(map.begin(), map.end());

        sort(vec.begin(), vec.end(), cmp); // 给频率排个序

        result.push_back(vec[0].first);

        for (int i = 1; i < vec.size(); i++) { 
            // 取最高的放到result数组中
            if (vec[i].second == vec[0].second) 
                result.push_back(vec[i].first);
            else 
                break;
        }
        return result;
    }
};