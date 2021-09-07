#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class MyQueue {
    stack<int> s;
public:
    
    MyQueue() {}
    
    void push(int x) {
        pushHelper(x);
    }
    
    void pushHelper(int x) {
        if(s.empty()){
            s.push(x);
            return;
        }
        int data = s.top();
        s.pop();
        pushHelper(x);
        s.push(data);
    }
    
    int pop() {
        int tmp = s.top();
        s.pop();
        return tmp;
    }
    
    int peek() {
        return s.top();
    }
    
    bool empty() {
        return s.empty();
    }
};