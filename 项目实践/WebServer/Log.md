## log——日志

> 利用单例模式与阻塞队列实现异步的日志系统，记录服务器运行状态；

```
  源代码
 ├── code          
 │   ├── log
 │   │   ├── blockqueue.h
 │   │   ├── log.h
 │   │   ├── log.cpp
```

## 一.Block队列的定义与操作

### 1.定义队列类

```c++
template<class T>
class BlockDeque {
public:
    explicit BlockDeque(size_t MaxCapacity = 1000);
    ~BlockDeque();
    void clear();
    bool empty();
    bool full();
    void Close();
    size_t size();
    size_t capacity();
    T front();
    T back();
    void push_back(const T &item);
    void push_front(const T &item);
    bool pop(T &item);
    bool pop(T &item, int timeout);
    void flush();
private:
    std::deque<T> deq_;
    size_t capacity_;
    std::mutex mtx_;
    bool isClose_;
    std::condition_variable condConsumer_;
    std::condition_variable condProducer_;
};
```

### 2.构造与析构函数

```c++
// ~ 对于队列的操作都需要加锁
template<class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity) :capacity_(MaxCapacity) {
    assert(MaxCapacity > 0);
    isClose_ = false;
}

template<class T>
BlockDeque<T>::~BlockDeque() {
    Close();
};

template<class T>
void BlockDeque<T>::Close() {
    {   
        std::lock_guard<std::mutex> locker(mtx_);
        deq_.clear();
        isClose_ = true;
    }
    condProducer_.notify_all();
    condConsumer_.notify_all();
};
```

+ 对于构造函数，只需要对 `MaxCapacity` 与 `isClose_` 进行初始化即可
+ 对于析构函数，直接调用 `close()` 函数
  + `close` 函数直接将队列 `deq_` 清空，将 `isClose_` 赋值为真；最后唤醒所有线程

### 3.简单的队列操作

```c++
template<class T>
void BlockDeque<T>::flush() {  // ~ 主动通知消费者取
    condConsumer_.notify_one();
};
template<class T>
void BlockDeque<T>::clear() {
    std::lock_guard<std::mutex> locker(mtx_);
    deq_.clear();
}
template<class T>
T BlockDeque<T>::front() {
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.front();
}
template<class T>
T BlockDeque<T>::back() {
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.back();
}
template<class T>
size_t BlockDeque<T>::size() {
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size();
}
template<class T>
size_t BlockDeque<T>::capacity() {
    std::lock_guard<std::mutex> locker(mtx_);
    return capacity_;
}
template<class T>
bool BlockDeque<T>::empty() {
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.empty();
}
template<class T>
bool BlockDeque<T>::full(){
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size() >= capacity_;
} 
```

+ 基本上都是在队列操作之前加上锁操作

### 4.向队列中添加元素操作——生产者操作

```c++
template<class T>
void BlockDeque<T>::push_back(const T &item) {  // ~ push_back
    std::unique_lock<std::mutex> locker(mtx_);
    while(deq_.size() >= capacity_) {  // ~ 队列满了
        condProducer_.wait(locker);    // ~ 生产者停止生产，等消费者处理后通知
    }
    deq_.push_back(item);
    condConsumer_.notify_one();  // ~ 生产者通知消费者取
}

template<class T>
void BlockDeque<T>::push_front(const T &item) {  // ~ push_front
    std::unique_lock<std::mutex> locker(mtx_);
    while(deq_.size() >= capacity_) {
        condProducer_.wait(locker);
    }
    deq_.push_front(item);
    condConsumer_.notify_one();
}
```

+ 对于 `push_back` 操作，先判断队列是否已满
  + 如果满了，则 **生产者停止生产，等消费者处理后通知**
  + 之后将元素导入队列尾部，并唤醒一个线程， **生产者通知消费者取**
+ 对于 `push_front` 操作，同样先判断队列是否已满
  + 如果满了，则 **生产者停止生产，等消费者处理后通知**
  + 之后将元素导入队列头部，并唤醒一个线程， **生产者通知消费者取**

### 5.向队列中取出元素操作——消费者操作

```c++
template<class T>
bool BlockDeque<T>::pop(T &item) {
    std::unique_lock<std::mutex> locker(mtx_);
    while(deq_.empty()){
        condConsumer_.wait(locker);  // ~ 等生产者通知
        if(isClose_)
            return false;
    }
    item = deq_.front();
    deq_.pop_front();  // ~ 从队列取
    condProducer_.notify_one();  // ~ 通知生产者继续生产
    return true;
}

template<class T>
bool BlockDeque<T>::pop(T &item, int timeout) {  // ~ 消费者等待 pop
    std::unique_lock<std::mutex> locker(mtx_);
    while(deq_.empty()){
        if(condConsumer_.wait_for(locker, std::chrono::seconds(timeout)) 
                == std::cv_status::timeout){
            return false;
        }
        if(isClose_)
            return false;
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}
```

+ 对于直接的 `pop` 操作，先进行锁操作，然后判断队列是否为空
  + 如果队列为空，则等生产者通知，并判断 `isClose_` 是否为真，如果为真，则解除
  + 之后取出队首元素，然后弹出队首元素，并唤醒一个线程，通知生产者继续生产
+ 对于定时的  `pop` 操作，先进行锁操作，然后判断队列是否为空
  + 如果队列为空，则定时等生产者通知，如果规定时间内没有通知，返回``false`，并判断 `isClose_` 是否为真，如果为真，则解除
  + 之后取出队首元素，然后弹出队首元素，并唤醒一个线程，**通知生产者继续生产**



## 二. 日志定义与操作

### 1.日志类的定义

```c++
class Log {
public:
    void init(int level, const char* path = "./log", 
                const char* suffix =".log",
                int maxQueueCapacity = 1024);

    static Log* Instance();
    static void FlushLogThread();

    void write(int level, const char *format,...);
    void flush();
    int GetLevel();
    void SetLevel(int level);
    bool IsOpen() { return isOpen_; }
    
private:
    Log();
    void AppendLogLevelTitle_(int level);
    virtual ~Log();
    void AsyncWrite_();

private:
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;   // ~ Log 文件中最大的 Log 语句长度

    const char* path_;    // ~ 日志路径   /log
    const char* suffix_;  // ~ 日志后缀   .log

    int MAX_LINES_;
    int lineCount_;  // ~ 记录到目前位置 日志的总行数
    int toDay_;      // ~ 记录最后一次创建日志文件的天数
    int level_;       // ~ 日志级别

    bool isOpen_;
    bool isAsync_;    // ~ 同步方式，还是异步方式写 Log, 同步直接使用 fputs , 异步先存入缓存中，交给写线程来处理写如操作

    Buffer buff_;     // ~ 缓冲区
    
    FILE* fp_;        // ~ 日志文件指针
    std::unique_ptr<BlockDeque<std::string>> deque_; 
    std::unique_ptr<std::thread> writeThread_;
    std::mutex mtx_;
};
```

### 2.宏定义函数

```c++
#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::Instance();\
        if (log->IsOpen() && log->GetLevel() <= level) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);

#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);
```

