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