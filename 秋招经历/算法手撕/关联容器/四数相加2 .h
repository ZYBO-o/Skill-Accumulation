#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
using namespace std;

int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
    unordered_map<int,int> map;
    for(auto a : nums1) 
        for(auto b : nums2) 
            map[a+b]++;
  
    int count = 0;
    for(auto c : nums3)
        for(auto d : nums4) 
            if(map.find(0-c-d) != map.end())
                count += map[0-c-d];
    return count;
}