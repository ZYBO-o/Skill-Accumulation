#include <iostream>
#include <vector>
using namespace std;


class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n + 1);
        dp[0] = 1;
        //dp[3]，就是 元素1为头结点搜索树的数量 + 元素2为头结点搜索树的数量 + 元素3为头结点搜索树的数量
        //元素1为头结点搜索树的数量 = 右子树有2个元素的搜索树数量dp[2] * 左子树有0个元素的搜索树数量dp[0]
        //元素2为头结点搜索树的数量 = 右子树有1个元素的搜索树数量dp[1] * 左子树有1个元素的搜索树数量dp[1] 
        //元素3为头结点搜索树的数量 = 右子树有0个元素的搜索树数量dp[0] * 左子树有2个元素的搜索树数量dp[2]
    
        //所以   dp[3] = dp[2] * dp[0] + dp[1] * dp[1] + dp[0] * dp[2]
        //dp[i] += dp[j - 1] * dp[i - j]; (i: 1~n) (j:(0~i))
        
        for(int i = 1; i <= n; ++i) {
            for(int j = 1; j <= i; ++j) {
                dp[i] += dp[j- 1]* dp[i - j];
            }
        }
        return dp[n];
    }
};