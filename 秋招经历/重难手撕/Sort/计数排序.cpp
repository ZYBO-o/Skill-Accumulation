#include<iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include<cstring>
using namespace std;

void CountSort(vector<int>& nums) {
    int n = nums.size();
    if (!n) return ;

    //1.找出待排序的数组中最大和最小的元素
    int min_num = *min_element(nums.begin(), nums.end());
    int max_num = *max_element(nums.begin(), nums.end());

    int length = max_num - min_num + 1;

    //2.统计数组中每个值为 i 的元素出现的次数，存入数组 C 的第 i 项
    vector<int> count(length, 0);

    //3.对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）
    for (int i = 0; i < n; ++i) {
        ++ count[nums[i] - min_num];
    }

    //反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < count[i]; ++j) {
            cout << i + min_num << " ";
        }
    }

    cout << endl;
}
