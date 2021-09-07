#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_set>
using namespace std;

vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
    unordered_set<int> res_set;
    //将数组1中的元素导入nums_set中
    unordered_set<int> nums_set (nums1.begin(), nums1.end());
    //将nums2中的每个元素在nums_set中进行查找，如果存在，则导入res_set中
    for(auto num : nums2) {
        if(nums_set.find(num) != nums_set.end())
            res_set.insert(num);
    }
    return vector<int> (res_set.begin(), res_set.end());
}
