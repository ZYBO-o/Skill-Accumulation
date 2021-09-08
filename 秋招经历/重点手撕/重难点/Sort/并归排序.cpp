#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>&nums, vector<int>&temp, int left, int middle, int right) {
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

void sort(vector<int>&nums, vector<int>&temp, int left, int right) {
	if(left == right) {
		return;
	} else {
		int middle = left + (right - left) / 2;
		sort(nums, temp, left, middle);
		sort(nums, temp, middle + 1, right);
		merge(nums, temp, left, middle, right);
	}
}

int main () {
	vector<int> nums {2,12,4,5,6,32,-1,34,56,0};
	vector<int> temp(nums.size(), 0);
	sort(nums, temp, 0, nums.size() - 1);
	for (auto num : nums){
		cout << num << " ";
	}
	cout << endl;
	return 0;
}