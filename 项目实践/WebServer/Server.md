## Server——服务器

> 利用IO复用技术Epoll与线程池实现多线程的Reactor高并发模型；

```c++
 源代码
├── code          
│   ├── server
│		│		├── epoll.h
│		│		├── epoll.cpp
│		│		├── webserver.h
│		│		├── webserver.cpp
```

## 一.Epoll

### 1.epoller 类定义

```c++
class Epoller {
public:
    explicit Epoller(int maxEvent = 1024);
    ~Epoller();
		//功能函数
    bool AddFd(int fd, uint32_t events);
    bool ModFd(int fd, uint32_t events);
    bool DelFd(int fd);
    int Wait(int timeoutMs = -1);
    int GetEventFd(size_t i) const;
    uint32_t GetEvents(size_t i) const;
        
private:
    int epollFd_;
    std::vector<struct epoll_event> events_;    
};
```

+ 主要定义了两个元素：
  + epollFd_：创建epoll模型后的文件描述符，红黑树根
  + events_：内核需要监听的事件数组

### 2.构造函数与析构函数

```c++
Epoller::Epoller(int maxEvent):epollFd_(epoll_create(512)), events_(maxEvent){
    assert(epollFd_ >= 0 && events_.size() > 0);
}

Epoller::~Epoller() {
    close(epollFd_);
}
```

+ 构造函数初始化类的两个成员变量，传入参数为 maxEvent
  + epollFd_：通过 `epoll_create` 创建 512 个红黑树结点
  + events_： vector 初始化 maxEvent 个 `epoll_event` 元素

+ 析构函数：关闭 epollFd_ 文件描述符

### 3. 添加事件

```c++
bool Epoller::AddFd(int fd, uint32_t events) {
    if(fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
}
```

+ AddFd 函数传入参数主要是 文件描述符 fd，事件类型 events
+ 函数中创建 临时epoll_event事件 ev，将两个参数进行赋值
+ 最后利用 epoll_ctl 将 ev 添加到 epollFd_为根结点的红黑树中，进行EPOLL_CTL_ADD添加

### 4.修改事件

```c++
bool Epoller::ModFd(int fd, uint32_t events) {
  	if(fd < 0) return false;
  	epoll_event ev = {0};
  	ev.data.fd = fd;
  	ev.events = events;
  	return 0 == epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
}
```

+ ModFd 函数传入参数主要是 文件描述符 fd，事件类型 events
+ 函数中创建 临时epoll_event事件 ev，将两个参数进行赋值
+ 最后利用 epoll_ctl 将 ev 添加到 epollFd_为根结点的红黑树中，进行EPOLL_CTL_MOD修改

### 5. 删除事件

```c++
bool Epoller::DelFd(int fd) {
  	if(fd < 0) return false;
  	epoll_event ev = {0};
  	return 0 == epoll_ctl(epoll_Fd, EPOLL_CTL_DEL, fd, &ev);
}
```

+ DelFd 函数传入参数主要是 文件描述符 fd
+ 函数中创建 临时epoll_event事件 ev，直接初始化为0
+ 最后利用 epoll_ctl 将 ev 添加到 epollFd_为根结点的红黑树中，进行EPOLL_CTL_DEL修改

### 6. 等待事件就绪

```c++
int Epoller::Wait(int timeoutMs) {
  	return epoll_wait(epoll_Fd, &events_[0], static_cast<int>(evenys_.size()), timeoutMs);
}
```

+ 直接调用epoll_wait函数进行返回，其中传出参数为 events_ 数组

### 7.获取响应事件的文件描述符

```c++
int Epoller::GetEventFd(size_t i) const {
  	assert(i < events_.size() && i >= 0);
  	return events_[i].data.fd;
}
```

+ 如果传入的参数 i 符合逻辑，传出 events_ 数组对应元素中的 文件描述符

### 8. 获取响应事件的事件类型

```c++
uint32_t Epoller::GetEvents(size_t i) const {
  	assert(i < events_.size() && i >= 0);
  	return events_[i].events;
}
```

+ 如果传入的参数 i 符合逻辑，传出 events_ 数组对应元素中的 事件类型



## 二.WebServer

### 1.WebServer类的定义

```c++
class WebServer {
public:
    WebServer(
    int port, int trigMode, int timeoutMS, bool OptLinger, 
    int sqlPort, const char* sqlUser, const  char* sqlPwd, const char* dbName,int connPoolNum, 
    int threadNum, bool openLog, int logLevel, int logQueSize);
    ~WebServer();
    void Start();

private:
    bool InitSocket_(); 
    void InitEventMode_(int trigMode);
    void AddClient_(int fd, sockaddr_in addr);
  
    void DealListen_();
    void DealWrite_(HttpConn* client);
    void DealRead_(HttpConn* client);

    void SendError_(int fd, const char*info);
    void ExtentTime_(HttpConn* client);
    void CloseConn_(HttpConn* client);

    void OnRead_(HttpConn* client);
    void OnWrite_(HttpConn* client);
    void OnProcess(HttpConn* client);

    static const int MAX_FD = 65536;

    static int SetFdNonblock(int fd);

    int port_;
    bool openLinger_;
    int timeoutMS_;  /* 毫秒MS */
    bool isClose_;
    int listenFd_;
    char* srcDir_;
    
    uint32_t listenEvent_;
    uint32_t connEvent_;
   
    std::unique_ptr<HeapTimer> timer_;
    std::unique_ptr<ThreadPool> threadpool_;
    std::unique_ptr<Epoller> epoller_;
    std::unordered_map<int, HttpConn> users_;
};
```





