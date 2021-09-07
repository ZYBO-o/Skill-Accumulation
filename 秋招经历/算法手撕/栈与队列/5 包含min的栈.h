#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class MinStack {
    stack<int> s;
    stack<int> smin;
public:
    MinStack() {}
    void push(int x) {
        s.push(x);
      	//如果smin空时，直接压入
        if(smin.empty())
            smin.push(x);
        else
            x < smin.top() ? smin.push(x) : smin.push(smin.top());
    } 
    void pop() {
        s.pop();
        smin.pop();
    }
    int top() {
        return s.top();
    }
    int min() {
        return smin.top();
    }
};