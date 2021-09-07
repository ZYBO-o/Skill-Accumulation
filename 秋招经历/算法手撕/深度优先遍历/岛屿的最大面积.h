#include <iostream>
#include<vector>

using namespace std;

class Solution {
public:
    int dfs(vector<vector<int>>& grid, int line, int col) {
        //返回条件
        if(line < 0 || col < 0 || line == grid.size() || col == grid[0].size() || grid[line][col] == 0)
            return 0;
        //置为0；
        grid[line][col] = 0;
        //上下左右
        int dirc_line[4] = {0, 0, 1, -1};
        int dirc_col[4] = {1, -1, 0, 0};
         int ans = 1;
        //上下左右查看
        for(int i = 0; i < 4; ++i) {
            ans += dfs(grid, line + dirc_line[i], col + dirc_col[i]);
        }
        return ans;
    }

    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int res = 0;//记录结果
        for(int i = 0; i < grid.size(); ++i) {
            for(int j = 0; j < grid[0].size();++j) {
                res = max(res, dfs(grid, i, j));
            }
        }
        return res;
    }
};