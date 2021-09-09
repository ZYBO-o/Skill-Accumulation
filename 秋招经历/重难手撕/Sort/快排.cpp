#include<iostream>
#include <vector>
using namespace std;

int Partition(vector<int>& nums, int begin, int end) {
	int pv = nums[begin];
	while(begin < end) {
		while(begin < end && pv <= nums[end])
			-- end;
		nums[begin] = nums[end];

		while(begin < end && pv >= nums[begin])
			++ begin;
		nums[end] = nums[begin];
	}
	nums[begin] = pv;
	return begin;
}

void Quick(vector<int>& nums, int begin, int end) {
	if(begin >= end)
		return;
	int pv = Partition(nums, begin, end);
	Quick(nums, begin, pv - 1);
	Quick(nums, pv + 1, end);
}


int main() {
	vector<int> nums{2,12,4,5,6,32,-1,34,56,0};
	Quick(nums, 0, nums.size() - 1);
	for (auto num : nums){
		cout << num << " ";
	}
	cout << endl;
	return 0;
}
// ======================================== 优化:双路快排 ===========================================
// 返回p, 使得arr[l...p-1] <= arr[p] ; arr[p+1...r] >= arr[p]
int Partition2(vector<int>& arr, int l, int r){

    swap( arr[l] , arr[rand()%(r-l+1)+l] ); // 随机在arr[l...r]的范围中, 选择一个数值作为标定点pivot
    int v = arr[l];

    //从左往右不断遍历，直到 arr[i] > v；
    //从右往左不断遍历，直到 arr[j] < v;

    int i = l+1, j = r;  // arr[l+1...i) <= v; arr(j...r] >= v

    while( true ){
        //多了个等号的判断也会造成两棵子树不平衡
        while( i <= r   && arr[i] < v )   i ++;// 注意这里的边界, arr[i] < v, 不能是arr[i] <= v
        while( j >= l+1 && arr[j] > v )   j --; // 注意这里的边界, arr[j] > v, 不能是arr[j] >= v

        if( i > j )  break;

        swap( arr[i] , arr[j] );

        i ++;j --;
    }
    //然后将 i 和 j下标的元素交换位置
    swap( arr[l] , arr[j]);
    //返回p, 使得arr[l...p-1] <= arr[p] ; arr[p+1...r] >= arr[p]
    return j;
}