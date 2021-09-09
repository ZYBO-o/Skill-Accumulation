#include <stack>
#include <vector>
using namespace  std;

int largestRectangleArea2(vector<int>& heights) {
    stack<int> st;
    heights.insert(heights.begin(), 0); // 数组头部加入元素0
    heights.push_back(0); // 数组尾部加入元素0
    st.push(0);
    int result = 0;
    
    for (int i = 1; i < heights.size(); i++) {
        if (heights[i] > heights[st.top()]) {
            st.push(i);
        } else if (heights[i] == heights[st.top()]) {
            st.pop(); // 更新
            st.push(i);
        } else {
            while (heights[i] < heights[st.top()]) { 
                int mid = st.top();
                st.pop();
                int left = st.top();
                int right = i;
                int w = right - left - 1;
                int h = heights[mid];
                result = max(result, w * h);
            }
            st.push(i);
        }
    }
    return result;
}

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