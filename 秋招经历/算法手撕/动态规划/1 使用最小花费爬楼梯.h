#include<vector>
using namespace std;

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        //dp[i]的定义:到达第i个台阶所花费的最少体力为dp[i]。
        vector<int> dp(cost.size());
        dp[0] = cost[0];
        dp[1] = cost[1];
        for(int i = 2; i < cost.size(); ++i) {
            //可以有两个途径得到dp[i]，一个是dp[i-1] 一个是dp[i-2]。
            //选最小的，所以dp[i] = min(dp[i - 1], dp[i - 2]) + cost[i];
            dp[i] = min(dp[i-1], dp[i - 2]) + cost[i];
        }
        // 注意最后一步可以理解为不用花费，所以取倒数第一步，第二步的最少值
        return min(dp[cost.size()-2], dp[cost.size() - 1]);
    }
};