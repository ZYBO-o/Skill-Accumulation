#include <iostream>
#include <stack>
#include <unordered_map>
#include <queue>
using namespace std;

 vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
    stack<int> st;
    vector<int> result(nums1.size(), -1);
    if (nums1.size() == 0) return result;

    unordered_map<int, int> umap; // key:下表元素，value：下表
    
    for (int i = 0; i < nums1.size(); i++) {
        umap[nums1[i]] = i;
    }
    st.push(0);
    for (int i = 1; i < nums2.size(); i++) {
        if (nums2[i] < nums2[st.top()]) {           // 情况一
            st.push(i);
        } else if (nums2[i] == nums2[st.top()]) {   // 情况二
            st.push(i);
        } else {                                    // 情况三
            while (!st.empty() && nums2[i] > nums2[st.top()]) {
                if (umap.count(nums2[st.top()]) > 0) { // 看map里是否存在这个元素
                    int index = umap[nums2[st.top()]]; // 根据map找到nums2[st.top()] 在 nums1中的下表
                    result[index] = nums2[i];
                }
                st.pop();
            }
            st.push(i);
        }
    }
    return result;
}


vector<int> nextGreaterElement(vector<int>& nums) { 
    vector<int> ans(nums.size()); // 存放答案的数组
    stack<int> s;
    for (int i = nums.size() - 1; i >= 0; i--) {// 倒着往栈里放
        while (!s.empty() && s.top() <= nums[i]) {// 判定个子高矮 
            s.pop(); // 矮个起开，反正也被挡着了。。。
        }
        ans[i] = s.empty() ? -1 : s.top(); // 这个元素身后的第一个高个 
        s.push(nums[i]); // 进队，接受之后的身高判定吧!
    }
    return ans; 
}