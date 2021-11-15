
#include <vector>
#include<iostream>
#include<stack>
using namespace std;

//===========================解析=======================
int largestRectangleArea(vector<int>& heights) {
    stack<int> st;//栈底到栈顶是从小到大

    heights.insert(heights.begin(), 0); // 数组头部加入元素0
    heights.push_back(0); // 数组尾部加入元素0

    st.push(0);
    int result = 0;

    // 第一个元素已经入栈，从下表1开始
    for (int i = 1; i < heights.size(); i++) {
        // 注意heights[i] 是和heights[st.top()] 比较 ，st.top()是下表
        if (heights[i] > heights[st.top()]) {//当前遍历的元素heights[i]小于栈顶元素heights[st.top()]的情况
            st.push(i);
        } else if (heights[i] == heights[st.top()]) {
            st.pop(); // 这个可以加，可以不加，效果一样，思路不同
            st.push(i);
        } else {//当前遍历的元素heights[i]大于栈顶元素heights[st.top()]的情况
            while (heights[i] < heights[st.top()]) { // 注意是while
                int mid = st.top();//推出来的元素，即凹槽
                st.pop();
                int left = st.top();
                int right = i;
                int w = right - left - 1;//宽度
                int h = heights[mid];//高度
                result = max(result, w * h);
            }
            st.push(i);
        }
    }
    return result;
}

//构建柱状图
int maximalRectangle(vector<vector<char>>& matrix) {
    int n = matrix.size();
    if(n==0) return 0;
    int m = matrix[0].size();
    if(n==1&&m==1) return matrix[0][0]-'0';

    vector<int> heights(m,0);
    int ans = 0;
    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            if(matrix[i][j]-'0'==1)
                heights[j]++;
            else heights[j] = 0;
        }
        ans = max(ans,largestRectangleArea(heights));
    }
    return ans;
}

int main(){

    vector<vector<char>> matrix = {{'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'}};
    cout << maximalRectangle(matrix) << endl;
    return 0;
}