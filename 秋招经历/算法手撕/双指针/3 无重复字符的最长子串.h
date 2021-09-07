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
int lengthOfLongestSubstring(string s) {
    unordered_map<char,int> window;
    int left = 0, right = 0;
    int maxlen = 0;

    while(right < s.size()) {
        char c = s[right];
        ++right;
        window[c]++;            //窗口数据更新
      	
        while(window[c] > 1) {  //出现重复字符时，进行缩减窗口
            char d = s[left];
            ++left;	
            window[d]--;        //窗口数据更新
        }
        
        maxlen = max(maxlen, right - left);
    }
    return maxlen;
}
