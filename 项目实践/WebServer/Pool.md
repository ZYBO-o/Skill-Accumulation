## Pool——池

> 利用RAII机制实现了数据库连接池，减少数据库连接建立与关闭的开销，同时实现了用户注册登录功能。

```c++
 源代码
├── code          
│   ├── poll
│		│		├── threadpoll.h
│		│		├── sqlconnpoll.h
│		│		├── sqlconnpoll.cpp
│		│		├── sqlconnRAll.h
```

## 一.连接池

### 1.定义连接池类

```c++
class SqlConnPool {
public:
    static SqlConnPool *Instance();//生成静态对象实例，这样就只有一个对象  —— —— 单例模式
    MYSQL *GetConn();
    void FreeConn(MYSQL * conn);
    int GetFreeConnCount();
    void Init(const char* host, int port,
              const char* user,const char* pwd, 
              const char* dbName, int connSize);
    void ClosePool();
private:
    SqlConnPool();
    ~SqlConnPool();
  
    int MAX_CONN_;									//最大连接数
    int useCount_;									//用户使用数目
    int freeCount_;									//空闲数目
  
    std::queue<MYSQL *> connQue_;		//连接的数据库队列
    std::mutex mtx_;								//锁
    sem_t semId_;										//信号量
};

/* 资源在对象构造初始化 资源在对象析构时释放*/
class SqlConnRAII {
public:
  	//构造函数，传入mysql对象指针与connpool对象指针
    SqlConnRAII(MYSQL** sql, SqlConnPool *connpool) {
        assert(connpool);
        *sql = connpool->GetConn();	//从队列中取出队首元素
        sql_ = *sql;	
        connpool_ = connpool;
    }
    
    ~SqlConnRAII() {
        if(sql_) { connpool_->FreeConn(sql_); }	//析构函数，将使用完成的 sql 对象压入栈中待利用
    }
    
private:
    MYSQL *sql_;							//MYSQL 对象指针
    SqlConnPool* connpool_;		//连接池操作对象
};
```

### 2.构造与实例化

```c++
// ~ 构造函数
SqlConnPool::SqlConnPool() {
    useCount_ = 0;
    freeCount_ = 0;
}
// ~ 生成静态实例
SqlConnPool* SqlConnPool::Instance() {
    static SqlConnPool connPool;
    return &connPool;
}
```

+ 使用单例模式，生成静态实例，保证只有一个。

### 3.初始化 sql 连接池

```c++
void SqlConnPool::Init(const char* host, int port,
            const char* user,const char* pwd, const char* dbName,
            int connSize = 10) {
    assert(connSize > 0);
    // ~ 创建 connSize 个 MysQL对象，并放入到队列中
    for (int i = 0; i < connSize; i++) {
        MYSQL *sql = nullptr;
        sql = mysql_init(sql);  // ~ 初始化 MySQL 对象
        if (!sql) {
            LOG_ERROR("MySql init error!");
            assert(sql);
        }
        // ~ mysql_real_connect()尝试与运行在主机上的MySQL数据库引擎建立连接
        sql = mysql_real_connect(sql, host,
                                 user, pwd,
                                 dbName, port, nullptr, 0);
        if (!sql) {
            LOG_ERROR("MySql Connect error!");
        }
        // ~ 将该 Mysql 对象 push 到连接队列
        connQue_.push(sql);
    }
    MAX_CONN_ = connSize;
    sem_init(&semId_, 0, MAX_CONN_);  // ~ pshared = 0 多进程非共享，用于多线程同步
}
```

+ 将创建 `connSize` 个 `sql` 对象，并压入 `connQue_` 对象。
  + 创建 sql 对象
  + 初始化，进行数据引擎连接
  + 将 sql 对象压入队列
+ 将 `connSize` 赋值给 `MAX_CONN_`
+ 创建信号量，`MAX_CONN_` 为 `semId_` 的初始值。而且这个信号量是当前进程的局部信号量，并不在多个进程间共享。

### 4. 从连接池取出MySQL对象进行使用

```c++
MYSQL* SqlConnPool::GetConn() {
    MYSQL *sql = nullptr;
    if(connQue_.empty()){  // ~ 如果当前队列空
        LOG_WARN("SqlConnPool busy!");
        return nullptr;
    }
    sem_wait(&semId_);  // ~ 消费者 semId --
    {   
        // ~ 队列操作 需要加锁 （临界区资源）
        lock_guard<mutex> locker(mtx_);  // ! 离开作用域的时候释放锁
        sql = connQue_.front();
        connQue_.pop();
    }
    return sql; // ~ 取出一个对象
}
```

+ 当队列不为空时，以原子操作的方式将信号量的值减1，并进行取出操作
  + 由于队列操作，先进行加锁
  + 取出队首元素，然后进行 pop 操作
  + 最后离开作用域的时候释放锁
+ 返回取出的对象

### 5.使用结束将MySQL对象压入队列

```c++
void SqlConnPool::FreeConn(MYSQL* sql) {
    assert(sql);
    lock_guard<mutex> locker(mtx_);  // ~ 队列操作，需要加锁
    connQue_.push(sql);
    sem_post(&semId_);  // ~ 生产者
}
int SqlConnPool::GetFreeConnCount() {
    lock_guard<mutex> locker(mtx_);  // ~ 队列操作，需要加锁
    return connQue_.size();
}
```

+ 对于压入操作，先加锁，然后将 sql 对象压入队列，以原子操作的方式将信号量的值加1
+ 对于返回队列元素的操作，先加锁，然后返回元素数目

### 6.释放MySQL库与析构函数

```c++
void SqlConnPool::ClosePool() {
    lock_guard<mutex> locker(mtx_);  // ~ 队列操作，需要加锁
    while(!connQue_.empty()) {
        auto item = connQue_.front();
        connQue_.pop();
        mysql_close(item);  // ~ 释放对象
    }
    mysql_library_end();   // ~ 终止使用 MySQL 库    
}
SqlConnPool::~SqlConnPool() {  // ~ 析构函数
    ClosePool();
}
```

+ 队列操作，先加锁； 将队列中的 sql 对象全部取出并进行关闭，终止使用 MySQL 库 。

## 二.线程池

### 1.线程池类

```c++
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount = 8): pool_(std::make_shared<Pool>()) {//显示构造函数
            assert(threadCount > 0);
            for(size_t i = 0; i < threadCount; i++) {
                std::thread(// ~ lambda 函数作为线程入口函数
                    [pool = pool_]  //值传递
                    {
                        // ~ 此时使用 unique_lock 为了用户可以自己后续的代码中使用 unlock() 和 lock()
                        std::unique_lock<std::mutex> locker(pool->mtx);
                        while(true) { 
                            if(!pool->tasks.empty()) {// ~ 此时工作队列非空
                                auto task = std::move(pool->tasks.front());// ~ 传递右值
                                pool->tasks.pop();
                                locker.unlock();   // ~ 手动解锁
                                task();
                                locker.lock();     // ~ 重新手动加锁
                            } 
                            else if(pool->isClosed) break;  // ~ 线程池关闭
                            else pool->cond.wait(locker);   // ~ 阻塞等待有工作线程入队并通知
                        }
                }).detach();
            }
    }

    ThreadPool() = default;
    ThreadPool(ThreadPool&&) = default; // ~ 移动构造函数
    
    ~ThreadPool() {
        if(static_cast<bool>(pool_)) {
            {
                std::lock_guard<std::mutex> locker(pool_->mtx);
                pool_->isClosed = true;  // ~ 写操作最好加锁
            }
            pool_->cond.notify_all();// ~ 通知所有线程
        }
    }

    template<class F>
    void AddTask(F&& task) { // ~ 右值引用，效率更高
        {
            std::lock_guard<std::mutex> locker(pool_->mtx);
            // ~ forward 可以保存 task 的左值或者右值的特性
            pool_->tasks.emplace(std::forward<F>(task));
        }
        pool_->cond.notify_one(); // ~ 通知等待工作线程的任务
    }
private:
    struct Pool {
        std::mutex mtx;  // ~ 锁
        std::condition_variable cond;   // ~ 条件变量
        bool isClosed;
        std::queue<std::function<void()>> tasks;  // ~ tasks 是一个 函数 队列
    };
    std::shared_ptr<Pool> pool_;  // ~ 智能指针
};
```































