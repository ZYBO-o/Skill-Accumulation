#include<iostream>
#include<vector>

void ShellSort(std::vector<int>& nums) {
    int d = nums.size();
    do {
        d= d/3 + 1;
        for(int i = d; i < nums.size(); i += d) {
            int temp = nums[i];
            int index = i;
            for(int j = i - d; j >= 0; j -= d) {
                if(temp < nums[j]) {
                    nums[j + d] = nums[j];
                    index = j;
                }
                if(index != i) {
                    nums[index] = temp;
                }
            }
        }
    }while(d > 1);
    for(int i = 0; i < nums.size(); ++i ) 
        std::cout << nums[i] << " ";
    std::cout << std::endl;
}
