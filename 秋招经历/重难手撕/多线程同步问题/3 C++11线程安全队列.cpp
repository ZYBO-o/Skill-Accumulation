
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <memory>
#include<iostream>
#include <thread>

template<typename T>
class ThreadsafeQueue {
private:
    mutable std::mutex mu_;             //可变的锁定互斥元时一种可变的操作
    std::queue<T> queue_;
    std::condition_variable cond_;
public:
    ThreadsafeQueue() { }
    ThreadsafeQueue(ThreadsafeQueue const& other) {
        std::lock_guard<std::mutex> lk(other.mu_);
        queue_ = other.queue_;
     }
    ~ThreadsafeQueue() { }

    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mu_);
        queue_.push(std::move(new_value));
        cond_.notify_one();
    }
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mu_);
        cond_.wait(lk,[this]{return !queue_.empty();});
        value = queue_.front();
        queue_.pop();
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mu_);
        return queue_.empty();
    }
};

ThreadsafeQueue<int> q;
std::mutex mu;

void Producter() {
    int count = 10000;
    while (count > 0) {
        q.push(count);
        std::unique_lock<std::mutex> locker(mu);
        std::cout << "t1 create a value : " <<count << std::endl;
        locker.unlock();
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        count--;
    }
}

void Consumer() {
    int data = 0;
    while ( data != 1) {
        int temp;
        while(q.empty()); // Unlock mu and wait to be notified
        q.wait_and_pop(data);
        std::unique_lock<std::mutex> locker(mu);
        std::cout << "t2 got a value from t1: " << data << std::endl;
        locker.unlock();
    }
}
int main() {
    std::thread t1(Producter);
    std::thread t2(Consumer);
    t1.join();
    t2.join();
    return 0;
}