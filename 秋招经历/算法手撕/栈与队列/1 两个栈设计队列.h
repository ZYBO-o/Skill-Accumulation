#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class MyQueue {
    stack<int> stackin;
    stack<int> stackout;
public:
    
    MyQueue() {}
  
    void push(int x) {
        stackin.push(x);
    }
  
    int pop() {
        //如果输出栈为空，则将输入栈中的所有元素弹出，压栈至输出栈，这样就以队列的方式进行弹出了
        while(stackout.empty()) {
            while(!stackin.empty()) {
                stackout.push(stackin.top());
                stackin.pop();
            }
        }
        //如果输出栈不为空，则直接弹出
        int temp = stackout.top();
        stackout.pop();
        return temp;
    }
    
    int peek() {
        while(stackout.empty()) {
            while(!stackin.empty()) {
            stackout.push(stackin.top());
            stackin.pop();
            }
        }
        int temp = stackout.top();
        return temp;
    }
    
    bool empty() {
        return (stackin.empty() && stackout.empty());
    }
};