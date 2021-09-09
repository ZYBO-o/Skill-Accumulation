#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>&nums,  int left, int middle, int right) {
    vector<int> temp (nums.size() - 1);
    int i = left;
    int j = middle + 1;
    int t = left;

    while(i <= middle && j <= right) {
        if(nums[i] < nums[j])
            temp[t++] = nums[i++];
        else
            temp[t++] = nums[j++];
    }

    while(i <= middle)
        temp[t++] = nums[i++];
    while(j <= right)
        temp[t++] = nums[j++];

    for(int k = left; k <= right; ++k){
        nums[k] = temp[k];
    }
}

void sort(vector<int>&nums, int left, int right) {
    if(left == right) {
        return;
    } else {
        int middle = left + (right - left) / 2;
        sort(nums, left, middle);
        sort(nums, middle + 1, right);
        merge(nums, left, middle, right);
    }
}


int main () {
    vector<int> nums {2,12,4,5,6,32,-1,34,56,0};
    sort(nums, 0, nums.size() - 1);
    for (auto num : nums){
        cout << num << " ";
    }
    cout << endl;
    return 0;
}

//====================优化的算法=======================
void mergeSortBU(vector<int>& nums){
    int n = nums.size();
    for(int sz = 1; sz <= n; sz += sz)//两个段进行归并排序——>四个小段进行归并排序——>。。。——>最后一起归并排序
        for(int i = 0; i < n; i += sz + sz) //每一轮归并排序中起始的位置
            //第一轮从[0, size-1],[size, 2size -1]这两部分进行归并
            //每轮将arr[i...i+sz-1]和arr[i+sz...i+sz+sz-1]进行归并
            merge(nums, i, i + sz - 1, min(i + sz + sz -1 , n - 1));//保证不越界
}
//====================================================