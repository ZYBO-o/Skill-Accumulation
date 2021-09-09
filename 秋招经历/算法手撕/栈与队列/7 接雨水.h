#include <stack>
#include <vector>
using namespace  std;

//当前列雨水面积：min(左边柱子的最高高度，记录右边柱子的最高高度) - 当前柱子高度。
//为了的到两边的最高高度，使用了双指针来遍历，每到一个柱子都向两边遍历一遍，这其实是有重复计算的。
//我们把每一个位置的左边最高高度记录在一个数组上（maxLeft），右边最高高度记录在一个数组上（maxRight）。
//这样就避免了重复计算，这就用到了动态规划。
class DP {
public:
    int trap(vector<int>& height) {

        if (height.size() <= 2) return 0;

        vector<int> maxLeft(height.size(), 0);
        vector<int> maxRight(height.size(), 0);
        int size = maxRight.size();

        maxLeft[0] = height[0];                // 记录每个柱子左边柱子最大高度
        for (int i = 1; i < size; i++) {
            maxLeft[i] = max(height[i], maxLeft[i - 1]);
        }
       
        maxRight[size - 1] = height[size - 1]; // 记录每个柱子右边柱子最大高度
        for (int i = size - 2; i >= 0; i--) {
            maxRight[i] = max(height[i], maxRight[i + 1]);
        }
        
        int sum = 0;// 求和
        for (int i = 0; i < size; i++) {
            //min(左边柱子的最高高度，记录右边柱子的最高高度) - 当前柱子高度。
            int count = min(maxLeft[i], maxRight[i]) - height[i];
            if (count > 0) sum += count;
        }
        return sum;
    }
};
//========================================单调栈=====================================

int trap(vector<int>& height) {
    if (height.size() <= 2) return 0; // 可以不加
    stack<int> st; // 存着下标，计算的时候用下标对应的柱子高度
    st.push(0);
    int sum = 0;
    for (int i = 1; i < height.size(); i++) {
        if (height[i] < height[st.top()]) {     // 情况一
            st.push(i);
        } if (height[i] == height[st.top()]) {  // 情况二
            st.pop(); // 其实这一句可以不加，效果是一样的，但处理相同的情况的思路却变了。
            st.push(i);
        } else {                                // 情况三
            while (!st.empty() && height[i] > height[st.top()]) { // 注意这里是while
                int mid = st.top();
                st.pop();
                if (!st.empty()) {
                    int h = min(height[st.top()], height[i]) - height[mid];
                    int w = i - st.top() - 1; // 注意减一，只求中间宽度
                    sum += h * w;
                }
            }
            st.push(i);
        }
    }
    return sum;
}
//====================================解析===============================
//设置一个栈，存放元素下标
//栈是从栈底到栈顶为从大到小
//每次遇到比栈顶小的元素，则推入
//遇到和栈顶一样的，则推出，再推入，进行更新
//遇到比栈顶大的，则推出作为，这个元素就是凹槽，因为新的栈顶与比较的元素都比他大
//进行计算能存放的雨水
class Solution {
public:
    int trap(vector<int>& height) {
        stack<int> st;//存下标
        st.push(0);
        int sum = 0;
        for (int i = 1; i < height.size(); i++) {//下标1开始遍历所有的柱子
            //如果当前遍历的元素（柱子）高度小于栈顶元素的高度，就把这个元素加入栈中，
            //因为栈里本来就要保持从小到大的顺序（从栈头到栈底）。
            if (height[i] < height[st.top()]) { //如果栈顶元素大于现在遍历的元素    
                st.push(i); //推入新的
            }else if(height[i] == height[st.top()])  {
                st.pop();
                st.push(i); //推入新的
            } else {                                
                //如果当前遍历的元素（柱子）高度大于栈顶元素的高度，此时就出现凹槽了
                while (!st.empty() && height[i] > height[st.top()]) { // 注意这里是while
                    //取栈顶元素，将栈顶元素弹出，这个就是凹槽的底部，也就是中间位置，
                    //下标记为mid，对应的高度为height[mid]
                    int mid = st.top();
                    st.pop();
                    //此时的栈顶元素st.top()，就是凹槽的左边位置，下标为st.top()，对应的高度为height[st.top()]
                    if (!st.empty()) {
                    //雨水高度是 min(凹槽左边高度, 凹槽右边高度) - 凹槽底部高度
                    int h = min(height[st.top()], height[i]) - height[mid]; 
                    int w = i - st.top() - 1; //宽度为 凹槽右边的下标 - 凹槽左边的下标 - 1
                    sum += h * w;//当前凹槽雨水的体积就是
                }
            }
            st.push(i);
        }
        return sum;
    }
}
};