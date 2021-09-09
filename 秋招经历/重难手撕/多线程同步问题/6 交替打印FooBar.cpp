#include <thread>
#include <condition_variable>
#include <vector>
#include <semaphore.h> // 需要手动包含信号量头文件
#include <algorithm>
#include <iostream>
#include <functional>
#include <atomic>
using namespace std;

//================================锁和条件变量================================
class FooBar {
private:
    int n;
    mutex mtx;
    condition_variable cond;
    bool foo_done = false;
public:
    FooBar(int n) : n(n) {}

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&]() { return !foo_done; });
            printFoo();
            lock.unlock();
            foo_done = true;
            cond.notify_one();
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(mtx);
            cond.wait(lock, [&]() { return foo_done; });
            printBar();
            lock.unlock();
            foo_done = false;
            cond.notify_one();
        }
    }
};
int main(){
    FooBar f(100);

    function<void()> printFoo = []() {  cout << "printFoo" << endl; };

    function<void()> printBar = []() {  cout << "printBar" << endl; };

    thread th1([&]() {   f.foo(printFoo); });

    thread th2([&]() {   f.bar(printBar); });
    
    th1.join();
    th2.join();

    return 0;
}

/*
//=================================原子操作============================
class FooBar2 {
private:
    int n;
    std::atomic<bool> foo_done = false;
public:
    FooBar2(int n) : n(n) {}

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            while (foo_done) {
                this_thread::yield();//当前线程“放弃”执行，让操作系统调度另一线程继续执行
            }
            printFoo();
            foo_done = true;
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            while (!foo_done) {
                this_thread::yield();
            }
            printBar();
            foo_done = false;
        }
    }
};

//=====================================信号量=============================


class FooBar3 {
private:
    int n;
    sem_t foo_done, bar_done;
public:
    FooBar3(int n) : n(n) {
        sem_init(&foo_done, 0 , 0);
        sem_init(&bar_done, 0 , 1);
    }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            sem_wait(&bar_done);
            printFoo();
            sem_post(&foo_done);
        }
    }
    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            sem_wait(&foo_done);
            printBar();
            sem_post(&bar_done);
        }
    }
};
*/