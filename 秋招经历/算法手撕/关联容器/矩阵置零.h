#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_map>
using namespace std;

void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    int flag_col0 = false;
    //遍历行数
    for (int i = 0; i < m; i++) { //如果第一列没有0，则flag_col0为真
        if (!matrix[i][0]) 
            flag_col0 = true;
       
        for (int j = 1; j < n; j++) { //从第二列开始，如果出现0，则对应的行与列都为0
            if (!matrix[i][j]) {
                matrix[i][0] = matrix[0][j] = 0;
            }
        }
    }
  
    for (int i = m - 1; i >= 0; i--) {  //从最后一行开始遍历
       
        for (int j = 1; j < n; j++) { //还是从第一列开始遍历
            if (!matrix[i][0] || !matrix[0][j]) //如果出现 行首 或者 列首 出现0，该行和列每个成员都为0
                matrix[i][j] = 0;
        }
        
        if (flag_col0) //如果第一列出现0，则该列都为0
            matrix[i][0] = 0;
    }
}
