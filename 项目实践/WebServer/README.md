# WebServer

## 一.项目概要

用C++实现的高性能WEB服务器，经过webbenchh压力测试可以实现上万的QPS

### 1.功能

* 利用IO复用技术Epoll与线程池实现多线程的Reactor高并发模型；
* 利用正则与状态机解析HTTP请求报文，实现处理静态资源的请求；
* 利用标准库容器封装char，实现自动增长的缓冲区；
* 基于小根堆实现的定时器，关闭超时的非活动连接；
* 利用单例模式与阻塞队列实现异步的日志系统，记录服务器运行状态；
* 利用RAII机制实现了数据库连接池，减少数据库连接建立与关闭的开销，同时实现了用户注册登录功能。

* 增加logsys,threadpool测试单元(todo: timer, sqlconnpool, httprequest, httpresponse) 

### 2.环境要求

* Linux
* C++14
* MySql

### 3.目录树

```bash
.
├── code           源代码
│   ├── buffer
│   ├── config
│   ├── http
│   ├── log
│   ├── timer
│   ├── pool
│   ├── server
│   └── main.cpp
├── test           单元测试
│   ├── Makefile
│   └── test.cpp
├── resources      静态资源
│   ├── index.html
│   ├── image
│   ├── video
│   ├── js
│   └── css
├── bin            可执行文件
│   └── server
├── log            日志文件
├── webbench-1.5   压力测试
├── build          
│   └── Makefile
├── Makefile
├── LICENSE
└── readme.md
```


### 4.项目启动

需要先配置好对应的数据库

```bash
// 建立yourdb库
create database yourdb;

// 创建user表
USE yourdb;
CREATE TABLE user(
    username char(50) NULL,
    password char(50) NULL
)ENGINE=InnoDB;

// 添加数据
INSERT INTO user(username, password) VALUES('name', 'password');
```

```bash
make
./bin/server
```

### 5.单元测试

```bash
cd test
make
./test
```

### 6.压力测试

```bash
./webbench-1.5/webbench -c 100 -t 10 http://ip:port/
./webbench-1.5/webbench -c 1000 -t 10 http://ip:port/
./webbench-1.5/webbench -c 5000 -t 10 http://ip:port/
./webbench-1.5/webbench -c 10000 -t 10 http://ip:port/
```

* 测试环境: Ubuntu:19.10 cpu:i5-8400 内存:8G 
* QPS 10000+

### 7.TODO

* config配置
* 完善单元测试
* 实现循环缓冲区

## 二.项目分析

### 1.buffer——缓存区设置

```bash
 源代码
├── code          
│   ├── buffer
│		│		├── buffer.h
│		│		├── buffer.cpp
```

#### (1).定义buffer类

```c++
class Buffer {
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;
  	...//功能函数
private:
    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;  // ~ 原子类型的 读写标记位置
    std::atomic<std::size_t> writePos_;
};
```

buffer的各个指向变量如下所示：

<div align = center><img src="../../图片/Web1.png" width="600px" /></div>

#### (2).buffer的扩容方式

+ 如果`WritableBytes() + PrependableBytes() < len`，说明剩余的字节数加上可重写的字节数都不够，则利用`vector<char> ` 的`resize()`函数进行扩容操作。

+ 否则将`ReadableBytes`部分的字符串直接`copy` 到 `BeginPtr_ ` 处，更新`readPos_`与`writePos_` 。

  ```c++
  size_t readable = ReadableBytes();
  std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
  readPos_ = 0;
  writePos_ = readPos_ + readable;
  assert(readable == ReadableBytes());
  ```

#### (3).buffer的Append(扩充)方式

+ 如果满足在buffer中可写，如果不够，则进行扩容操作。
+ 将长度为`len`的`str`字符串`copy`到`BeginWrite`处。
+ 使用`HasWritten`更新`writePos_`。

```c++
EnsureWriteable(len);
std::copy(str, str + len, BeginWrite());//将str拷贝到writePos_开始的相应区间内
HasWritten(len); // ~ writePos_ 移动相应位置
```

#### (4).buffer的WriteFd方式

+ 将`buffer`中` [readPos_ writePos_) `的内容写到 `fd`
+ 更新`readPos_`

```c++
size_t readSize = ReadableBytes();
ssize_t len = write(fd, Peek(), readSize); //将 [readPos_ writePos_) 内容写到 fd
if(len < 0) {
  *saveErrno = errno;
  return len;
} 
readPos_ += len;//到此，[readPos_ writePos_)内容全部读完
return len;
```

#### (5).buffer的ReadFd方式

+ 定义一块`buff`缓冲区
+ 利用`readv`从`fd`读数据，这样如果`Buffer`中剩余的缓冲区不够时会保存至`buff`中。 
  + 如果所读的数据长度`len`小于`WritableBytes`，则默认读入了`Buffer`中，只需要更新`writePos_`即可。
  + 否则，先将`buff`中的数据写入`WritableBytes`中，再将剩余的数据进行`Append`操作。

```c++
char buff[65535];
struct iovec iov[2];
const size_t writable = WritableBytes();
iov[0].iov_base = BeginPtr_() + writePos_;
iov[0].iov_len = writable;
iov[1].iov_base = buff;
iov[1].iov_len = sizeof(buff);
const ssize_t len = readv(fd, iov, 2);
if(len < 0)
  *saveErrno = errno;
else if(static_cast<size_t>(len) <= writable) {//数据先往 buffer_ 里面读
  writePos_ += len;
}
//如果 buffer_ 剩余的字节不够，将剩余的内容读入buff 中；之后通过 Append 将buffer中的内容添加到 buffer_ 中，如果总的字节数不够，会调用 Makespace 重新分配内存空间
else {
  writePos_ = buffer_.size();
  Append(buff, len - writable);
}
```



