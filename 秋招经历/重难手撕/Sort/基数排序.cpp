#include<iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include<cstring>
using namespace std;

void countSort(vector<int>& vec,int exp){//基数排序
    vector<int> range(10,0);

    int length=vec.size();
    
    vector<int> tmpVec(length,0);

    for(int i=0;i<length;++i){
        range[(vec[i] / exp) % 10]++;
    }

    for(int i=1;i<range.size();++i){
        range[i]+=range[i-1];//统计本应该出现的位置
    }

    for(int i=length-1; i>=0; --i){
        tmpVec[range[(vec[i] / exp) % 10] - 1] = vec[i];
        range[(vec[i] / exp) % 10]--;
    }
    vec=tmpVec;
}

void radixSort(vector<int>& vec){
    int length=vec.size();
    int max=-1;
    for(int i=0; i<length; ++i){//提取出最大值
        if(vec[i] > max)
            max = vec[i];
    }

    //提取每一位并进行比较，位数不足的高位补0
    for(int exp=1; max / exp > 0; exp *= 10)
        countSort(vec,exp);

    for(auto i : vec)
        cout << i << " ";
    cout << endl;
}
