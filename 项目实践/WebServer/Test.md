## Test—— 单元测试

```c++
源代码
 ├── code          
 ├── test
 │   ├── Makefile
 │   ├── test.cpp
```

### 1.主要测试内容

```c++
int main() {
    TestLog();
    TestThreadPool();
}
```

+ 进行日志测试与线程池测试

### 2. 日志测试

```c++
void TestLog() {
    int cnt = 0, level = 0;
    Log::Instance()->init(level, "./testlog1", ".log", 0);
    for(level = 3; level >= 0; level--) {
        Log::Instance()->SetLevel(level);
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(i,"%s 111111111 %d ============= ", "Test", cnt++);
            }
        }
    }
    cnt = 0;
    Log::Instance()->init(level, "./testlog2", ".log", 5000);
    for(level = 0; level < 4; level++) {
        Log::Instance()->SetLevel(level);
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(i,"%s 222222222 %d ============= ", "Test", cnt++);
            }
        }
    }
}
```

+ 首先创建日志静态实例，设置文件位置与名称
+ 进行从大到小的三个等级的日志测试，利用 `LOG_BASE` 函数进行测试。
+ 进行从小到大的三个等级的日志测试，利用 `LOG_BASE` 函数进行测试。

### 3.线程池测试

```c++
void ThreadLogTask(int i, int cnt) {
    for(int j = 0; j < 10000; j++ ){
        LOG_BASE(i,"PID:[%04d]======= %05d ========= ", gettid(), cnt++);
    }
}
void TestThreadPool() {
    Log::Instance()->init(0, "./testThreadpool", ".log", 5000);
    ThreadPool threadpool(6);
    for(int i = 0; i < 18; i++) {
        threadpool.AddTask(std::bind(ThreadLogTask, i % 4, i * 10000));
    }
    getchar();
}
```

+ 创建 log 静态实例，并创建testThreadpool文件
+ 创建线程池，池中有6个线程
+ 将 **ThreadLogTask** 函数进行bind操作，传递LOG_BASE函数，将ThreadLogTask作为队列中的元素，来进行调用

### 4.Makefile进行编译

```makefile
CXX = g++
CFLAGS = -std=c++14 -O2 -Wall -g 

TARGET = test
OBJS = ../code/log/*.cpp ../code/pool/*.cpp ../code/timer/*.cpp \
       ../code/http/*.cpp ../code/server/*.cpp \
       ../code/buffer/*.cpp ../test/test.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET)  -pthread -lmysqlclient

clean:
	rm -rf ../bin/$(OBJS) $(TARGET)
```

+ 主要就是编译 code 与 test 目录中的文件
+ 然后连接 thread 与 mysql 库来进行链接生成可执行文件。