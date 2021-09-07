#include<vector>
using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        vector<int> dp(nums.size());    //dp数组
        dp[0] = nums[0];                //初始化
        int result = dp[0];
        for(int i = 1; i < nums.size(); ++i) {
            dp[i] = max(dp[i - 1] + nums[i], nums[i]);//推导公式
            result = max(result, dp[i]);
        }
        return result;
    }
};