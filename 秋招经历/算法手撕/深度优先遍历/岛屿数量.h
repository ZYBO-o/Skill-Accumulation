#include<iostream>
#include <vector>

using namespace std;

class Solution {
public:
    int numIslands(vector<vector<char>>& nums) {
       int res = 0;
       for(int i = 0; i < nums.size(); ++i){
           for(int j = 0; j < nums[0].size(); ++j) {
               if(nums[i][j] == '1'){
                   res ++;
                   dfs(nums, i, j); //将四周的岛屿置为0
               }
           }
       }
       return res;
    }
   

    void dfs(vector<vector<char>>& nums, int line, int col) {
        //返回条件
        if(line < 0 || col < 0 || line == nums.size() ||col == nums[0].size() || nums[line][col] == '0') 
            return ;

        nums[line][col] = '0';
        
        dfs(nums, line + 1, col);
        dfs(nums, line, col + 1);
        dfs(nums, line - 1, col);
        dfs(nums, line, col - 1);
        
    }
};