#include<iostream>
#include<vector>
#include <string>
#include<algorithm>
#include<unordered_map>
using namespace std;

/**
 *  1.设置最小交集数组 MinFreq最开始为 MinFreq(26, INT_MAX); 设置Freq数组，用来保存每个单词中字母出现的个数
 *  2.遍历每个单词，遍历之前先将 Freq 清零；然后再记录单词中字母出现的个数
 *  3.将 Freq 数组 与 MinFreq 数组比较，保存更小者
 *  4.遍历 [0 - 26)，再遍历 MinFreq 数组， 这样根据字母的索引与出现的次数来说明每个 单词出现的公共最小次数
 */ 
class Solution {
public:
    vector<string> commonChars(vector<string>& words) {
        vector<int> MinFreq(26, INT_MAX);
        vector<int> Freq(26,0);
        for(auto word : words) {
            fill(Freq.begin(), Freq.end(), 0);//每次个单词都为0
            for(auto w : word) {
                Freq[w - 'a'] ++;
            }
            for(int i = 0; i < 26; ++i) {
                MinFreq[i] = min(MinFreq[i], Freq[i]);
            }
        }
        vector<string> res;
        for(int i = 0; i < 26; ++i) {
            for(int j = 0; j < MinFreq[i]; ++j) {
                res.emplace_back(1,i + 'a');
            }
        }
        return res;
    }
};