#include <iostream>
#include <vector>
using namespace std;
/**
 * 当索引 a, b, c 满足以下条件时， dp[i] 为三种情况的最小值；
 * 每轮计算 dp[i] 后，需要更新索引 a, b, c 的值，使其始终满足方程条件: dp[i]=min(dp[a]×2, dp[b]×3, dp[c]×5)
 * 实现方法：分别独立判断 dp[i]和 dp[a]×2 , dp[b]×3 , dp[c]×5 的大小关系，
 *           若相等则将对应索引 a , b , c 加 1 ；
 * 
 */
class Solution {
public:
    int nthUglyNumber(int n) {
        vector<int> dp(n,0);
        int a = 0, b = 0, c = 0;
        dp[0]  = 1;
        for(int i = 1; i < n; ++i) {
            int n2 = dp[a] * 2;
            int n3 = dp[b] * 3;
            int n5 = dp[c] * 5;
            dp[i] = min(min(n2,n3), n5);
            if(dp[i] == n2) ++a;
            if(dp[i] == n3) ++b;
            if(dp[i] == n5) ++c;
        }
        return dp[n-1];
    }
};