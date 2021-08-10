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

**:one: [Buffer——缓冲区](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Buffer.md)**

**:two: [Http——有限状态机](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/HttpConn.md)**

**:three: [Log——日志](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Log.md)**

**:four: [Pool——连接池与线程池](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Pool.md)**

**:five: [Server——服务器](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Server.md)**

**:six: [Timer——定时器](https://github.com/ZYBO-o/Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Timer.md)**

:seven:  **[单元测试](https://github.com/ZYBO-o/Skill-Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/Test.md)**

:eight: **[压力测试](https://github.com/ZYBO-o/Skill-Accumulation/blob/main/%E9%A1%B9%E7%9B%AE%E5%AE%9E%E8%B7%B5/WebServer/WebBench.md)**

---

## 三.面试模拟

### 1.缓存区是如何设计的？

>  **利用标准库容器vector封装char，实现自动增长的缓冲区；**

#### (1).类元素设计

设计了一个buffer类，主要元素有：

+ `vector<char>` 封装的 buffer_
+ 原子类型的  `atomic<size_t> readPos_;` 标记已读位置
+ 原子类型的  `atomic<size_t> writePos_;` 标记已写位置
+ 功能函数`char* BeginPtr_();`表示正在进行写操作的位置

#### (2).扩容设计

还能进行写操作的空间表示为WritableBytes = buffer\_.size - writePos_

能够在利用的空间表示为PrependableBytes = writePos_ - readPos_

+ 如果`WritableBytes() + PrependableBytes() < len`，说明剩余的字节数加上可重写的字节数都不够，则利用`vector<char>` 的`resize()`函数进行扩容操作。

+ 否则将`ReadableBytes`部分的字符串直接`copy` 到 `BeginPtr_` 处，更新`readPos_`与`writePos_` 。

```c++
 size_t readable = ReadableBytes();
 std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
 readPos_ = 0;
 writePos_ = readPos_ + readable;
 assert(readable == ReadableBytes());
```

#### (3).Append扩充操作

+ 首先确定填入Buffer的字符数目不超过空余的空间，否则先进行扩容操作。

+ 接着将长度为`len`的`str`字符串`copy`到`BeginWrite`处。

- 使用`HasWritten`更新`writePos_`。

```c++
 EnsureWriteable(len);
 std::copy(str, str + len, BeginWrite());//将str拷贝到writePos_开始的相应区间内
 HasWritten(len); // ~ writePos_ 移动相应位置
```

#### (4).Buffer中的WriteFd方式

> 表示将Buffer **剩余需要读出的数据** 写入对应的文件fd中

- 将`buffer`中`[readPos_ writePos_)`的内容写到 `fd`
- 更新`readPos_`

```c++
size_t readSize = writePos_ - readPos_ ;
ssize_t len = write(fd, BeginWrite()+readPos_, readSize); //将 [readPos_ writePos_) 内容写到 fd
if(len < 0) {
    *saveErrno = errno;
    return len;
} 
readPos_ += len;//到此，[readPos_ writePos_)内容全部读完
return len;
```

#### (5).Buffer中的ReadFd方式

>  表示从文件fd中读出数据，写入Buffer中

- 首先定义一块`buff`缓冲区
- 利用 `readv` 从 `fd` 读数据，这样如果 `Buffer` 中剩余的缓冲区不够时会保存至 `buff` 中。
  - 如果所读的数据长度`len`小于`WritableBytes`，则默认读入了`Buffer`中，只需要更新`writePos_`即可。
  - 否则，先将`buff`中的数据写入`WritableBytes`中，再将剩余的数据进行`Append`操作。

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

































