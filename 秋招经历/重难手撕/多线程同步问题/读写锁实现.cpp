#include<mutex>
#include<condition_variable>
#include<iostream>
#include<vector>
#include<thread>
using namespace std;

class ReadWriteLock {

private:
    int readWaiting = 0;  //等待读
    int writeWaiting = 0; //等待写
    int reading = 0; //正在读
    int writing = 0;  //正在写
    std::mutex mx;
    std::condition_variable cond;
    bool preferWriter;  //偏向问题
public:
    ReadWriteLock(bool isPreferWriter = false) :preferWriter(isPreferWriter) {}

    void readLock() {
        std::unique_lock<std::mutex>lock(mx);
        ++readWaiting;
        cond.wait(lock, [&]() {
            return writing <= 0 && (!preferWriter || writeWaiting <= 0);
        });
        ++reading;
        --readWaiting;
    }

    void writeLock() {
        std::unique_lock<std::mutex>lock(mx);
        ++writeWaiting;
        cond.wait(lock, [&]() {
            return reading <= 0 && writing <= 0;
        });
        ++writing;
        --writeWaiting;
    }

    void readUnLock() {
        std::unique_lock<std::mutex>lock(mx);
        --reading;
        //当前没有读者时，唤醒一个写者
        if(reading<=0)
            cond.notify_one();
    }

    void writeUnLock() {
        std::unique_lock<std::mutex>lock(mx);
        --writing;
        //唤醒所有读者、写者
        cond.notify_all();
    }
};

//写者优先
ReadWriteLock readWriteLock(true);

void reader() {

    readWriteLock.readLock();
    cout << "reader" << endl;
    this_thread::sleep_for(std::chrono::seconds(3));
    readWriteLock.readUnLock();
}

void writer() {

    readWriteLock.writeLock();
    cout << "writer" << endl;
    this_thread::sleep_for(std::chrono::seconds(3));
    readWriteLock.writeUnLock();
}

int main() {

    vector<thread>vec;
    for (int i = 0; i < 5; ++i) {
        vec.push_back(thread(reader));
        vec.push_back(thread(writer));
    }

    for (int i = 0; i < vec.size(); ++i) {
        if(vec[i].joinable())
            vec[i].join();
    }
}
