#include<vector>
#include<string>
#include<iostream>
using namespace std;

vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> fourSum ;

        if (nums.size() <= 3) return fourSum;
        sort(nums.begin(),nums.end());
        int pb, i, j, pe;

        for (pb = 0; pb <= nums.size() - 4; ++pb) {
            
            if(pb > 0 && nums[pb] == nums[pb - 1])                //去重
                continue;      

            for (i = pb + 1; i <= nums.size() - 3; i++) {

                if(i > pb + 1 && nums[i] == nums[i - 1])          //去重
                    continue;

                j = i + 1; 
                pe = nums.size() - 1;

                while (j < pe){
                    if(nums[pb] + nums[i] + nums[j] + nums[pe] == target) {
                        fourSum.push_back({nums[pb], nums[i], nums[j], nums[pe]});
                        while (j < pe && nums[j] == nums[j + 1])  //去重
                            ++ j;
                        while (j < pe && nums[pe - 1] == nums[pe])//去重
                            --pe;
                        ++ j;
                        -- pe;

                    } else if (nums[pb] + nums[i] + nums[j] + nums[pe] < target) {
                        ++ j;

                    } else {
                        -- pe;
                    }
                }
            }
        }
        return fourSum;
    }