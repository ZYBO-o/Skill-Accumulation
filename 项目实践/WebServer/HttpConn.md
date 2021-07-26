## http——有限状态机

> 利用正则与状态机解析HTTP请求报文，实现处理静态资源的请求；

```c++
 源代码
├── code          
│   ├── poll
│		│		├── httpconn.h
│		│		├── httpconn.cpp
│		│		├── httprequest.h
│		│		├── httprequest.cpp
│		│		├── httpresponse.h
│		│		├── httpresponse.cpp   
```

### 1.定义HttpConn类

```c++
class HttpConn {
public:
  	...//功能函数
  	static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
private:
    int fd_;
    struct  sockaddr_in addr_;
    bool isClose_;
    int iovCnt_;
    struct iovec iov_[2];
    Buffer readBuff_; // 读缓冲区
    Buffer writeBuff_; // 写缓冲区
    HttpRequest request_;
    HttpResponse response_;
};
```

