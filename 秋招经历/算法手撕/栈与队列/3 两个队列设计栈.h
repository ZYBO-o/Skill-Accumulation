#include <iostream>
#include <stack>
#include <queue>
using namespace std;

class MyStack {
    queue<int> q1;
    queue<int> q2;
public:
 
    MyStack() { }
 
    //此时q2为空，作为中介，先将元素导入到q2
    //将q1中的元素全部导入q2，这样就模拟出了先进后出的栈
    void push(int x) {
        q2.push(x);
        while(!q1.empty()) {
            q2.push(q1.front());
            q1.pop();
        }
        swap(q1,q2);
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