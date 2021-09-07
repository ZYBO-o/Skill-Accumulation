#include<iostream>
#include<vector>
#include<algorithm>
#include<unordered_set>
using namespace std;

int getsum(int n) {
    int sum = 0;
    while(n) {
        sum += (n%10) * (n%10);
        n = n / 10;
    }
    return sum;
}

bool isHappy(int n) {
    unordered_set<int> set;
    int sum;
    while(1) {
        sum = getsum(n);
        if(sum == 1) 
            return true;
        if(set.find(sum) != set.end()) 
            return false;
        else
            set.insert(sum);
        n = sum;
    }
}
