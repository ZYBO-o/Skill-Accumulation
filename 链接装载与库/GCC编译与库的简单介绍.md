# CSAPP—链接

## 一.GCC编译过程

### 1.编译过程

<div align = center><img src="../图片/链接1、.png" width="700px" /></div>

+ 利用预处理器进行预处理。进行头文件展开，宏替换和注释去掉。
+ 利用编译器将C文件编译成汇编文件
+ 利用汇编器将汇编文件变成二进制文件
+ 利用连接器将函数库中相应的代码组合到目标文件中

### 2.编译参数

```bash
gcc sum.c -I ./include -o app -D DEBUG -O -Wall -g
```

+ -D
  + 编译时指定宏
+ -I + 目录
  + 编译时指定头文件目录

+ -O
  + 编译时进行优化，最高等级为3
+ -Wall
  + 提示警告进行
+ -g
  + 包含调试信息



## 二.静态库与动态库

### 1.静态库的制作与使用

+ **静态库命名规则：** lib + 库的名字 + .a
+ **制作规则：**
  + **生成对应的.o文件：** 参数 -c
  + **将生成的.o文件进行打包：** ar rcs + 静态库的名字(libSort.a) + 生成的所有.o文件
+ **发布和使用静态库：**
  + 发布静态库
  + 头文件

+ **例子：**

  <div align = center><img src="../图片/链接2.png" width="400px" /></div>

  + include文件夹存放头文件，src文件夹存放源文件，lib文件夹存放生成的库文件

  + 生成.o文件

    ```bash
    gcc *.c -c -I ../include 
    ```

  + 打包

    ```bash
    ar rcs libMyCal.a *.o
    mv libMyCal.a ../lib
    ```

    <div align = center><img src="../图片/链接3.png" width="350px" /></div>

  + 利用头文件和静态库来运行程序

    ```bash
    gcc main.c lib/libMyCal.a -o sum -I include
    ```

    ```bash
    # -I include头文件
    # -L 库的目录
    # -l 库的名字，去头去尾
    gcc main.c -I include -L lib -l MyCal -o sum
    ```

+ 利用 nm命令 查看静态库内容

  <div align = center><img src="../图片/链接4.png" width="400px" /></div>

### 2.静态库的优缺点

+ **优点：**
  + 发布程序的时候不需要提供对应的库
  + 加载库的速度快
+ **缺点：**
  - 库被打包到应用程序中，体积内存大
  - 库发生变化，需要重新编译，很费时间



### 3.动态库的制作与使用

+ **静态库命名规则：** lib + 库的名字 + .so

+ **制作步骤：**

  + **生成与位置无关的代码(.o) ：** 参数 -fPIC -c

    <div align = center><img src="../图片/链接5.png" width="700px" /></div>

    > + 静态库.o在代码段每次位置都相同，所以是与位置有关。
    > + 动态库不会打包到程序中，只有在运行到需要时才会加载到共享库区域，每次加载时放入的位置都不同，所以与位置无关。

  + **将.o打包成动态库：** gcc -shared  -o libMyCal.so *.o -I include



+ **例子：**

  + 生成.o文件

    ```bash 
    gcc -fPIC -c *.c -I ../include
    ```

  + 打包成共享库

    ```bash
    gcc -shared  -o libMyCal.so *.o -I ../include
    mv libMyCal.so ../lib
    ```

    <div align = center><img src="../图片/链接6.png" width="350px" /></div>

  + 利用动态库和头文件来运行程序

    ```bash 
    gcc main.c lib/libMyCal.so -o mysum -I include
    ```

    ```c
    gcc main.c -I include -L ./lib -l MyCal -o mysum
    ```

    <div align = center><img src="../图片/链接7.png" width="800px" /></div>

    > 没有给动态链接器指定动态库libMyCal.so的路径

+ **解决动态链接库失败的问题**

  +  直接放入系统库目录——不推荐

  +  利用export LD_LIBRARY_PATH=目录 来临时添加，终端关闭后会失效

  +  将上述方法写入bashrc——也不推荐

  +  找到动态连接器的配置文件 

    ```bash
    cd /etc
    sudo vim ld.so.conf
    # 动态库的路径写入配置文件
    /Users/zhangyongbin/Desktop/Cal/lib
    # 更新
    sudo ldconfig -v
    ```

### 4.动态库的优缺点

- **优点：**
  - 执行程序体积小
  - 动态库更新了(函数接口不变)，不需要重新编译程序
- **缺点：**
  - 发布程序时，需要把动态库发布给用户
  - 加载速度相对较慢
