#include <iostream>
#include <vector>
#include <string>

std::vector<int> PMT(const std::string& p) {
    std::vector<int> pmt (p.size(), 0);
    int ll = 0;
    
    for(int i = 1; i < p.size(); ++i) {
        while (ll > 0 && p[ll] != p[i])    //非理想情况下
            ll = pmt[ll - 1];
        
        if(p[ll] == p[i])                  //理想情况下：直接在前一个共有元素长度上 + 1
            ++ll;
        
        pmt[i] = ll;                       //部分匹配表存储对应长度
    }
    return pmt;
}

int KMP(const std::string& s, const std::string& p) {
    int ret = -1;
    int s_len = s.size();
    int p_len = p.size();
    
    std::vector<int> pmt = PMT(p);              //求出子串的部分匹配表
    
    if(0 < p_len && p_len <= s_len) {           //子串长度>0 且 子串长度小于目标串,这样才有查找的意义
        
        for(int i = 0, j = 0; i < s_len; ++i) { //i 遍历 s, j 遍历 p

            while(j > 0 && s[i] != p[j]) //2.比对不成功
                j = pmt[j - 1];          //j的位置进行改变
            
            if(s[i] == p[j])//1.如果比对成功
                ++j;
            
            if(j == p_len) {             //结束比对的条件
                ret = i - p_len + 1;     //成功查找的位置
                break;
            }
        }
    }
    return ret;
}

int main() {
    
    std::cout << KMP("aaaa","a") << std::endl;
    
    return 0;
}