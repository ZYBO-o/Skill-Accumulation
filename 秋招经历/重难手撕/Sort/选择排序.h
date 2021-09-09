#include<iostream>
#include<vector>

#ifndef SORT_SELECTSORT_H
#define SORT_SELECTSORT_H

#include <iostream>
#include <vector>

void swap(int& num1, int& num2) {
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

void SelectSort(std::vector<int> nums) {
    for(int i = 0; i < nums.size(); ++i) {
        int index = i;
        for(int j = i + 1; j < nums.size(); ++j) 
            if(nums[index] > nums[j])
                index = j;
        
        swap(nums[index],nums[i]);
    }   

    for(auto i : nums)
        std::cout << i << " ";
    std::cout << std::endl;
}

#endif // !SORT_SELECTSORT_H
