#include<iostream>
#include<vector>

using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;
        int count = 0;
        if(nums[left] < nums[right])
            return nums[left];
        else {
            while(nums[left] > nums[right]) {
                ++ count;
                right = left;
                left ++;
            }
        }
        return nums[left];
    }
};