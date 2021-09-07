#include <iostream>
#include <vector>
int  binary_search(std::vector<int>& nums, int target) {
    int left = 0;
  	int right = nums.size() - 1;
  
    while(left <= right) {
        int mid = left + (right - left) / 2 ;
        if(nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        }
    }
    return -1;
}
//左侧边界的二分查找
int left_bound(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2 ;
        if(nums[mid] == target) {
            right =  mid - 1;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        }
    }
    return left;
}
//右侧边界的二分查找
int right_bound(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2 ;
        if(nums[mid] == target) {
            left =  mid + 1;
        } else if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        }
    }
    return right ;
}