
#include<iostream>
#include<vector>


void InsertSort(std::vector<int> nums) {
    for(int i = 1; i < nums.size(); i++) {
        int temp = nums[i];
        int index = i;
        for(int j = i - 1; j >= 0; j--) {
            if(temp < nums[j]) {
                nums[j + 1] = nums[j];
                index = j;
            }
        }
         if(index != i) {
            nums[index] = temp;
        }
    }
    for(int i = 0; i < nums.size(); ++i ) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
}