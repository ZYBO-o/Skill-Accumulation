#include<iostream>
#include <vector>

using namespace std;

void swap(int& num1, int& num2) {
    int temp = num1;
    num1 = num2;
    num2 = temp;
}

void BubbleSort(std::vector<int> nums) {
    for(int i = 0; i < nums.size(); ++i) 
        for(int j = i; j < nums.size(); ++j) 
            if(nums[i] > nums[j])
                swap(nums[i], nums[j]);
        
    for(int i = 0; i < nums.size(); ++i ) 
        std::cout << nums[i] << " ";
    std::cout << std::endl;
    
}
