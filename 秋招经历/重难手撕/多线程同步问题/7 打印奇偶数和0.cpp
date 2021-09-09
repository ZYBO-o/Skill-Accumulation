#include <thread>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <atomic>
using namespace std;

class ZeroEvenOdd {
private:
    int n;
    int num;
    mutex m;
    condition_variable cond;
    int flag = 0;
public:
    ZeroEvenOdd(int n) : n(n), num(1){ }
    //0
    void zero(function<void(int)> printNumber) {
        for (; num <= n; ) {
            unique_lock<mutex> lock(m);
            cond.wait(lock, [&] {return flag == 0;});
            printNumber(0);
            if (num % 2 == 0) {
                flag = 2;
            } else {
                flag = 1;
            }
            cond.notify_all();
        }
    }
    //偶数
    void even(function<void(int)> printNumber) {
        for (; num <= n; ) {
            unique_lock<mutex> lock(m);
            cond.wait(lock, [&] {return flag == 2;});
            printNumber(num);
            num += 1;
            flag = 0;
            cond.notify_all();
        }
    }
    //技术
    void odd(function<void(int)> printNumber) {
        for (; num <= n; ) {
            unique_lock<mutex> lock(m);
            cond.wait(lock, [&] {return flag == 1;});
            printNumber(num);
            num += 1;
            flag = 0;
            cond.notify_all();
        }
    }
};
int main(){
    ZeroEvenOdd f(100);

    function<void(int)> printNumber = [](int i) {  cout << i << endl; };

    thread th1([&]() {   f.zero(printNumber); });
    thread th2([&]() {   f.even(printNumber); });
    thread th3([&]() {   f.odd(printNumber); });

    th1.join();
    th2.join();
    th3.join();

    return 0;
}


//=======================================原子操作===========================
class ZeroEvenOdd {
private:
    int n;
    atomic<int> flag = 0;
public:
    ZeroEvenOdd(int n) : n(n){  }

    void zero(function<void(int)> printNumber) {
        for (int i = 1; i <= n; ++i) {
            while (flag != 0) {
                this_thread::yield();
            }
            printNumber(0);
            if (i % 2 == 0) {
                flag = 2;
            } else {
                flag = 1;
            }
        }
    }

    void even(function<void(int)> printNumber) {
        for (int i = 2; i <= n; i += 2) {
            while (flag != 2) {
                this_thread::yield();
            }
            printNumber(i);
            flag = 0;
        } 
    }

    void odd(function<void(int)> printNumber) {
        for (int i = 1; i <= n; i += 2) {
            while (flag != 1) {
                this_thread::yield();
            }
            printNumber(i);
            flag = 0;
        }
    }
};

