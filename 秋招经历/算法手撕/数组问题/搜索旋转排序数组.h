#include <iostream>
#include <vector>
using namespace std;

/**
 *   首先查看  left元素 与 mid元素 的大小
 *   如果 left元素 比 mid元素小。则说明 left —— mid 是升序的
 *       查看是否在 left —— mid 范围中
 *            如果在， 则 right 缩小为 mid - 1；
 *            如果不在， 则将 left 变为 mid + 1
 * 
 *   如果 left元素 比 mid元素大。则说明 mid —— right 是升序的
 *       查看是否在 mid —— right 范围中
 *            如果在， 则 left 变为 mid + 1
 *            如果不在，则 right = mid - 1
 */           

 
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        if (!n)  return -1;
        if (n == 1)  return nums[0] == target ? 0 : -1;
    
        int left = 0, right = n - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;

            if (nums[left] <= nums[mid]) { //如果首元素比中间的小，则 0 - mid是升序
                if (nums[left] <= target && target < nums[mid]) {//如果target 在 0 - mid 中间，则在做二分
                    right = mid - 1;
                } else {//否则说明在 mid + 1 —— right
                    left = mid + 1;
                }
            } else {    //如果首元素比中间的大，则说明 Mid —— right 是升序
                if (nums[mid] < target && target <= nums[n - 1]) {//如果是在 mid —— right 之间，则进行二分
                    left = mid + 1;
                } else { //否则说明在 left —— mid
                    right = mid - 1;
                }
            }
        }
        return -1;
    }
};