#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int integerBreak(int n) {
        //dp[i]：分拆数字i，可以得到的最大乘积为dp[i]。
        vector<int> dp(n + 1);
        //dp数组初始化
        dp[2] = 1;
        /**
         *  从1遍历j，然后有两种渠道得到dp[i].
         *      j * (i - j) 直接相乘。
         *      j * dp[i - j]，相当于是拆分(i - j)
         */
        for(int i = 3; i <= n; ++i) {
            for(int j = 1; j < i - 1; ++j) {
                //之所以max中有dp[i]，是因为j会变量好几遍，每次都要比较计算的值
                dp[i] = max ( dp[i],max(j * dp[i - j], j * (i - j)));
            }
        }
        return dp[n];
    }
};