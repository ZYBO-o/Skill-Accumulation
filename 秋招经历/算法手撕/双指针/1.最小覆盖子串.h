#include<vector>
#include<string>
#include<iostream>
#include <unordered_map>
using namespace std;
/**
 * 在字符串 S 中使用双指针中的左右指针技巧，初始化left = right = 0，把索引左闭右开区间 [left, right) 称为一个窗口。
 * 先不断地增加 right 指针扩大窗口 [left, right)，直到窗口中的字符串符合要求 （包含了 T 中的所有字符）。
 * 此时停止增加 right，转而不断增加 left 指针缩小窗口 [left, right)，直到窗口中的字符串不再符合要求（不包含 T 中所有字符了）。同时，每次增加 left 到达字符串 s 的尽头。
 * 重复第 2 步 和第 3 步，直到 right 到达字符串 S 的尽头。
 */
string minWindow(string s, string t) {
    unordered_map<char,int> need,window;            //定义need,window两个哈希表，用于记录 需要凑齐的字符 和  窗口中的字符
    for(auto c : t) 
        need[c] ++;
    int left = 0, right = 0;                        //定义双指针变量初始化窗口的两端，区间为左闭右开
    int len = INT_MAX, start = 0;                   //记录最小覆盖子串的长度及其初始索引
    int valid = 0;                                  //定义valid 变量表示窗口中满足need条件的字符个数，
                                                    //如果valid和need.size()大小相同，则说明完全覆盖
    while(right < s.size()) {//开始滑动
        
        char c = s[right];   //cs是将移入窗口的字符
        ++ right;           //右移窗口
        
        if(need.count(c)) { //进行窗口内的一系列更新
            window[c] ++;   //如果在need集合中，则放入窗口中
            if(window[c] == need[c])                //如果窗口中字符个数与需凑齐字符的个数一致，则满足覆盖数+1
                ++ valid;
        }

        while(valid == need.size()) {              //判断左侧窗口是否要收缩
            if(right - left < len) {   //更新最小覆盖子串
                len = right - left;    //更新最小长度及其开始的索引
                start = left;
            }
            
            char d = s[left];          //d是移除窗口的字符
            ++ left;                  //左移窗口
           
            if(need.count(d)) {      //进行窗口内的一系列更新
                if(window[d] == need[d])   //如果在need集合中，则移除窗口
                    valid--;    //如果窗口中字符个数与需凑齐字符的个数一致，则满足覆盖数-1
                window[d]--;   //移除窗口
            }
        }
    }
    return len == INT_MAX ? "" : s.substr(start,len);
}
