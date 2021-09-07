#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
using namespace std;

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> map;
    for(auto num : nums) 
        map[num]++;
    
    vector<pair<int, int>> vtMap;
    for (auto it = map.begin(); it != map.end(); it++)
        vtMap.push_back(make_pair(it->first, it->second));
  
    sort(vtMap.begin(), vtMap.end(),
        [](const pair<int, int> &x, const pair<int, int> &y) -> int {
            return x.second > y.second;
        });

    vector<int> res;
    for(int i = 0; i < k; ++i) {
        auto it = (vtMap.begin() + i);
        res.push_back(it->first);
    }

    return res;
}