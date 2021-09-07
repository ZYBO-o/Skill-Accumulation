#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class MyStack {
    queue<int> q1;
public:
    
    MyStack() { }
    
    //将压入，然后将后面的元素重新弹出，压在元素后面
    void push(int x) {
        q1.push(x);
        int size = q1.size();
        while(--size) {
            q1.push(q1.front());
            q1.pop();
        }
    }
    
    int pop() {
        int temp = q1.front();
        q1.pop();
        return temp;
    }
    
    int top() {
        return q1.front();
    }
    
    bool empty() {
        return q1.empty();
    }
};