#include <mutex>
#include <iostream>
#include <condition_variable>
#include <queue>
#include <thread>
#include <assert.h>
#include <functional>
using namespace std;

//============================条件边量 =======================
class Foo2 {
    condition_variable cond;
    mutex mtx;
    int k = 0;
public:
    void first(function<void()> printFirst) {
        printFirst();
        k = 1;
        cond.notify_all();    // 通知其他所有在等待唤醒队列中的线程
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lock(mtx);   // lock mtx
        cond.wait(lock, [this](){ return k == 1; });  // unlock mtx，并阻塞等待唤醒通知，需要满足 k == 1 才能继续运行
        printSecond();
        k = 2;
        cond.notify_one();    // 随机通知一个（unspecified）在等待唤醒队列中的线程
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lock(mtx);   // lock mtx
        cond.wait(lock, [this](){ return k == 2; });  // unlock mtx，并阻塞等待唤醒通知，需要满足 k == 2 才能继续运行
        printThird();
    }
};

int main(){
    Foo2 f;
    function<void()> printFirst = []() {  cout << "first" << endl; };
    function<void()> printSecond = []() {  cout << "second" << endl; };
    function<void()> printThird = []() {  cout << "third" << endl;};
    
    thread th1([&]() {   f.first(printFirst); });
    thread th2([&]() {   f.second(printSecond); });
    thread th3([&]() {   f.third(printThird); });

    th1.join();
    th2.join();
    th3.join();

    return 0;
}



//================================信号量=============================
#include <semaphore.h>

class Foo3 {
private:
    sem_t sem_1, sem_2;
public:
    Foo3() {
        sem_init(&sem_1, 0, 0), sem_init(&sem_2, 0, 0);
    }

    void first(function<void()> printFirst) {
        printFirst();
        sem_post(&sem_1);
    }

    void second(function<void()> printSecond) {
        sem_wait(&sem_1);
        printSecond();
        sem_post(&sem_2);
    }

    void third(function<void()> printThird) {
        sem_wait(&sem_2);
        printThird();
    }
};
//================================原子操作===================

class Foo4 {
    std::atomic<bool> a{ false };
    std::atomic<bool> b{ false };
public:
    void first(function<void()> printFirst) {
        printFirst();
        a = true;
    }

    void second(function<void()> printSecond) {
        while (!a)
            this_thread::sleep_for(chrono::milliseconds(1));
        printSecond();
        b = true;
    }

    void third(function<void()> printThird) {
        while (!b)
            this_thread::sleep_for(chrono::milliseconds(1));
        printThird();
    }
};
