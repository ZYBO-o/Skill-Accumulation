#include<vector>
#include<string>
#include<iostream>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> result;
    sort(nums.begin(), nums.end());
    for(int i = 0; i < nums.size(); ++i) {
        if(nums[i] > 0)                     //如果排序后第一个数已经大于0，则不可能再等于0了
            return result;
       
        if(i > 0 && nums[i] == nums[i - 1]) //去重
            continue;
        
        int left = i + 1;                   //定义双向指针
        int right = nums.size() - 1;
        
        while(left < right) {
            if(nums[i] + nums[left] + nums[right] > 0) //二分搜索
                --right;
            else if(nums[i] + nums[left] + nums[right] < 0)
                ++ left;
            else {
                result.push_back({nums[i], nums[left], nums[right]});//推入结果
                
                while (right > left && nums[right] == nums[right - 1]) 
                    right--;// 去重逻辑应该放在找到一个三元组之后
                while (right > left && nums[left] == nums[left + 1]) 
                    left++;

                right--;// 找到答案时，双指针同时收缩
                left++;
            } 
        }
    }
    return result;
}
