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
vector<int> findAnagrams(string s, string p) {
    unordered_map<char,int> need,window;
    for(auto c : p) 
        need[c]++;
    vector<int> result;
    int left = 0, right = 0;
    int valid = 0;

    while(right < s.size()) {
        char c = s[right];
        ++ right;
      	
        if(need.count(c)) {	//如果在需要凑齐的集合中
            window[c]++;    //录入窗口
            if(need[c] == window[c])    //当录入窗口的字符个数与需要的字符个数相同时
                ++ valid;               //窗口中满足need条件的字符个数+1
        }
				
        while(right - left >= p.size()) {   //缩小窗口的条件
            if(valid == need.size())        //窗口中满足need条件的字符个数满足时，输出
                result.push_back(left);
            
            char d = s[left];               //缩小窗口
            ++ left;
          	
            if(need.count(d)) {             //如果是在需要凑齐的集合中
                if(need[d] == window[d])   	//当录入窗口的字符个数与需要的字符个数相同时
                    --valid;                //窗口中满足need条件的字符个数 -1
                -- window[d];               //窗口减小
            }
        }
    }
    return result;
}
