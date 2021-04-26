# Linux系统编程

## (一).IO操作函数

### 1.C库函数

<img src="../图片/Linux64.png" style="zoom:75%;" />

> 标准C库中提供了对应的IO缓冲区，缓冲区的速度比硬盘更快，操作文件效率更高
>
> Linux 系统函数没有IO缓存区，需要自己提供

+ **C库函数与系统函数的关系：**

<img src="../图片/Linux67.png" style="zoom:75%;" />

### 2.虚拟机地址空间

<img src="../图片/Linux65.png" style="zoom:75%;" />

> 32位分配4G(2^32)
>
> + 栈空间从上往下分配，堆空间从下往上分配
> + C标准库和Linux系统IO函数为动态库，生成与位置无关的代码，地址为相对地址

+ **文件描述符：**

<img src="../图片/Linux66.png" style="zoom:70%;" />

**cpu 为什么要使用虚拟地址空间与物理地址空间映射？解决了什么样的问题？**

+ 方便编译器和操作系统安排程序的地址分布。
  + 程序可以使用一系列相邻的虚拟地址来访问物理内存中不相邻的大内存缓冲区。
+ 方便进程之间隔离
  + 不同进程使用的虚拟地址彼此隔离。一个进程中的代码无法更改正在由另一进程使用的物理内存。
+ 3.方便OS使用你那可怜的内存。
  + 程序可以使用一系列虚拟地址来访问大于可用物理内存的内存缓冲区。当物理内存的供应量变小时，		
  + 内存管理器会将物理内存页（通常大小为 4 KB）保存到磁盘文件。数据或代码页会根据需要在物理内存与磁盘之间移动。

### 3.open函数

#### open函数基础

+ **查找系统open函数：**

  ```bash
  man 2 open
  ```

![image-20210420103406698](../图片/Linux68.png)

#### close函数

<img src="../图片/Linux70.png" style="zoom:80%;" />

#### open函数中的errno

![image-20210420103540688](../图片/Linux69.png)

#### 查阅文档

+ 写函数是跳到对应的文档

  ```bash
  # 精确的跳到对应的文档：把光标放到函数身上，按以下快捷键
  2 + shift + k
  ```

### 4.read与write函数

<img src="../图片/Linux71.png" style="zoom:50%;" />

#### 5.lseek函数

<img src="../图片/Linux72.png" style="zoom:50%;" />



---

## (二).文件操作函数

### 1.stat函数

![image-20210421100554533](../图片/Linux73.png)

+ 其中st_mode介绍

  <img src="../图片/Linux75.png" style="zoom:75%;" />

  ![image-20210421101234693](../图片/Linux74.png)

+ 使用stat查阅文件大小

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  int main(int argc, char* argv[])
  {
      if(argc < 2)
      {
          printf("a.out filename\n");
          exit(1);
      }
  
      struct stat st;
      int ret = lstat(argv[1], &st);
  
      if(ret == -1)
      {
          perror("stat");
          exit(1);
      }
      printf("file size = %d\n", (int)st.st_size);
      return 0;
  }
  ```

  

+ 使用stat写出`ls -al`功能

  ```c
  #include <stdio.h>
  #include <string.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <stdlib.h>
  #include <time.h>
  #include <pwd.h>
  #include <grp.h>
  int main(int argc, char* argv[])
  {
      if(argc < 2)
      {
          printf("./a.out filename\n");
          exit(1);
      }
  
      struct stat st;
      int ret = stat(argv[1], &st);
      if(ret == -1)
      {
          perror("stat");
          exit(1);
      }
  
      // 存储文件类型和访问权限
      char perms[11] = {0};
      // 判断文件类型
      switch(st.st_mode & S_IFMT)
      {
          case S_IFLNK:
              perms[0] = 'l';
              break;
          case S_IFDIR:
              perms[0] = 'd';
              break;
          case S_IFREG:
              perms[0] = '-';
              break;
          case S_IFBLK:
              perms[0] = 'b';
              break;
          case S_IFCHR:
              perms[0] = 'c';
              break;
          case S_IFSOCK:
              perms[0] = 's';
              break;
          case S_IFIFO:
              perms[0] = 'p';
              break;
          default:
              perms[0] = '?';
              break;
      }
      // 判断文件的访问权限
      // 文件所有者
      perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
      perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
      perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
      // 文件所属组
      perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
      perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
      perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
      // 其他人
      perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
      perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
      perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';
  
      // 硬链接计数
      int linkNum = st.st_nlink;
      // 文件所有者
      char* fileUser = getpwuid(st.st_uid)->pw_name;
      // 文件所属组
      char* fileGrp = getgrgid(st.st_gid)->gr_name;
      // 文件大小
      int fileSize = (int)st.st_size;
      // 修改时间
      char* time = ctime(&st.st_mtime);
      char mtime[512] = {0};
      strncpy(mtime, time, strlen(time)-1);
  
      char buf[1024];
      sprintf(buf, "%s  %d  %s  %s  %d  %s  %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);
      
      printf("%s\n", buf);
      return 0;
  }
  ```



### 2.lstat函数

<img src="../图片/Linux76.png" style="zoom:50%;" />

> lstat 对于软连接不穿透，不追踪，只是显示软连接的信息。
>
> stat穿透，显示软连接所链接的文件的信息。

### 3.access函数

![image-20210421105651381](../图片/Linux77.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename\n");
        exit(1);
    }

    int ret = access(argv[1], W_OK);
    if(ret == -1)
    {
        perror("access");
        exit(1);
    }
    printf("you can write this file.\n");
    return 0;
}
```



### 4.chmod函数

![image-20210421105803977](../图片/Linux78.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename\n");
        exit(1);
    }

    int ret = chmod(argv[1], 0755);
    if(ret == -1)
    {
        perror("chmod");
        exit(1);
    }
    return 0;
```

### 5.chown 函数

<img src="../图片/Linux79.png" style="zoom:50%;" />

```c
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename!\n");
        exit(1);
    }

    // user->ftp  group->ftp
    int ret = chown(argv[1], 116, 125);
    if(ret == -1)
    {
        perror("chown");
        exit(1);
    }
    return 0;
}
```

### 6.truncate函数

![image-20210421110042520](../图片/80.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out filename 111\n");
        exit(1);
    }

    long int len = strtol(argv[2], NULL, 10); 
    int  aa = truncate(argv[1], len);
    if(aa == -1)
    {
        perror("truncate");
        exit(1);
    }
    return 0;
}
```

### 7.链接函数

![image-20210421110557108](../图片/81.png)

```c
//link
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out oldpath newpath\n");
        exit(0);
    }

    int ret = link(argv[1], argv[2]);
    if(ret == -1)
    {
        perror("link");
        exit(1);
    }
    return 0;
}
```

```c
//symlink
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out oldpath newpath\n");
        exit(1);
    }

    int ret = symlink(argv[1], argv[2]);
    if(ret == -1)
    {
        perror("symlink");
        exit(1);
    }

    return 0;
}
```

```c
//readlink
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out softlink\n");
        exit(1);
    }

    char buf[512];
    int ret = readlink(argv[1], buf, sizeof(buf));
    if(ret == -1)
    {
        perror("readlink");
        exit(1);
    }
    buf[ret] = 0;
    printf("buf = %s\n", buf);

    return 0;
```

```c
//unlick
//创建临时文件来进行读写
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("tempfile", O_CREAT | O_RDWR, 0755);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    int ret = unlink("tempfile");
    if(ret == -1)
    {
        perror("unlink");
        exit(1);
    }

    char buf[512];
    write(fd, "hello", 5);
    lseek(fd, 0, SEEK_SET);
    int len = read(fd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, len);

    close(fd);

    return 0;
}
```

### 8.rename函数

![image-20210421110913375](../图片/82.png)

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out oldName newName\n");
        exit(1);
    }

    int ret = rename(argv[1], argv[2]);
    if(ret == -1)
    {
        perror("rename");
        exit(1);
    }
    return 0;
}
```



## (三).目录操作函数

### 1.chdir和getcwd函数

<img src="../图片/83.png" style="zoom:50%;" />

```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out dir\n");
        exit(1);
    }

    int ret = chdir(argv[1]);
    if(ret == -1)
    {
        perror("chdir");
        exit(1);
    }

    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0777);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    close(fd);

    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("current dir: %s\n", buf);

    return 0;
}
```

### 2.mkdir和rmdir函数

<img src="../图片/Linux84.png" alt=" " style="zoom:50%;" />

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out newDir mode\n");
        exit(1);
    }

    int mode = strtol(argv[2], NULL, 8);
    int ret = mkdir(argv[1], mode);
    if(ret == -1)
    {
        perror("mkdir");
        exit(1);
    }
    return 0;
}
```

### 3.opendir函数

![image-20210421112827981](../图片/Linux85.png)

```c++
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out path\n");
        exit(1);
    }

    DIR* dir = opendir(argv[1]);
    if(dir == NULL)
    {
        perror("opendir");
        exit(1);
    }
    char buf[512];
    getcwd(buf, sizeof(buf));
    printf("current dir: %s\n", buf);

    return 0;
}
```

### 4.readdir与closedir函数

<img src="../图片/Linux86.png" style="zoom:50%;" />

```c
//获取文件的个数
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int get_file_count(char* root)
{
  	//打开目录
    DIR* dir;
    dir = opendir(root);
    if(dir == NULL)
    {
        perror("opendir");
        exit(1);
    }

  	//遍历当前目录
  	struct dirent* ptr = NULL;
  	int total = 0;
    char path[1024];
    while((ptr = readdir(dir)) != NULL)
    {
      	//过滤 . 与 ..
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
        {
            continue;
        }
        if(ptr->d_type == DT_DIR)
        {
          	//递归读目录
            sprintf(path, "%s/%s", root, ptr->d_name);
            total += get_file_count(path);
        }
        if(ptr->d_type == DT_REG)
        {
            total ++;
        }
    }
    closedir(dir);

    return total;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out path\n");
        exit(1);
    }
    int total = get_file_count(argv[1]);
    printf("%s has %d files!\n", argv[1], total);
    return 0;
}
```

### 5.fcntl函数

![image-20210421150328199](../图片/Linux87.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(void)
{
    int fd;
    int flag;
    // 测试字符串
    char *p = "我们是一个有中国特色的社会主义国家！！！！！！";
    char *q = "呵呵, 社会主义好哇。。。。。。";
    // 只写的方式打开文件
    fd = open("test.txt", O_WRONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    // 输入新的内容，该部分会覆盖原来旧的内容
    if(write(fd, p, strlen(p)) == -1)
    {
        perror("write");
        exit(1);
    }
    // 使用 F_GETFL 命令得到文件状态标志
    flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
    {
        perror("fcntl");
        exit(1);
    }
    // 将文件状态标志添加 ”追加写“ 选项
    flag |= O_APPEND;
    // 将文件状态修改为追加写
    if(fcntl(fd, F_SETFL, flag) == -1)
    {
        perror("fcntl -- append write");
        exit(1);
    }
    // 再次输入新内容，该内容会追加到旧内容的后面
    if(write(fd, q, strlen(q)) == -1)
    {
        perror("write again");
        exit(1);
    }
    // 关闭文件
    close(fd);
    return 0;
}
```

---

## (四).进程基本概念

### 1.程序与进程

+ 程序：
  + 编译好的二进制文件，在磁盘上不占用系统资源。
+ 进程：
  + 一个抽象的概念，与操作系统原理紧密联系。进程是在内存中运行着的程序，占用系统资源。

### 2.并发与并行

+ 并发：
  + 在操作系统中，一个时间段中有多个进程都处在运行到运行完毕之间的状态。但任意一个时刻点上仍只有一个进程在运行。

+ 并行：



> 因为以上概念，就有了单道程序设计与多道程序设计的概念。
>
> + **单道程序设计：**所有进程一个一个排对执行。若A阻塞，B只能等待，即使CPU处于空闲状态。而在人机交互时阻塞的出现时必然的。所有这种模型在系统资源利用上及其不合理。
> + **多道程序设计：**在计算机内存中同时存放几道相互独立的程序，它们在管理程序控制之下，相互穿插的运行。多道程序设计必须有硬件基础作为保证。**时钟中断**即为多道程序设计模型的理论基础。 并发时，任意进程在执行期间都不希望放弃cpu。因此系统需要一种强制让进程让出cpu资源的手段。时钟中断有硬件基础作为保障，对进程而言不可抗拒。 操作系统中的中断处理函数，来负责调度程序执行。

### 3.中央处理器的简易架构

#### CPU取址执行简单介绍

<img src="../图片/Linux88.png" style="zoom:45%;" />

+ 取值执行时，预取器将缓存中的指令取出
+ 译码器将预取指令进行译码
+ 译码过程中分析需要进行的操作，然后调用操作所需要的算数逻辑单元，将数值放入寄存器中，进行计算操作

#### MMU映射地址简单介绍

<img src="../图片/Linux89.png" style="zoom:50%;" />

+ 产生进程的时候就会产生虚拟内存。
+ 虚拟地址是程序可用的地址范围是4G
+ MMU可以将虚拟地址映射到物理地址
+ MMU可以设置和修改内存访问级别(CPU)

### 4.进程控制块

+ 每个进程在内核中都有一个进程控制块（PCB）来维护进程相关的信息，Linux内核的进程控制块是task_struct结构体。
+ `/usr/src/linux-headers-3.16.0-30/include/linux/sched.h`文件中可以查看struct task_struct 结构体定义。其内部成员有很多，重点部分如下：
  + 进程id。系统中每个进程有唯一的id，在C语言中用pid_t类型表示，其实就是一个非负整数。
  + 进程的状态，有就绪、运行、挂起、停止等状态。
  + 进程切换时需要保存和恢复的一些CPU寄存器。
  + 描述虚拟地址空间的信息。
  + 描述控制终端的信息。
  + 当前工作目录（Current Working Directory）。
  + umask掩码。
  + 文件描述符表，包含很多指向file结构体的指针。
  + 和信号相关的信息。
  + 用户id和组id。
  + 会话（Session）和进程组。
  + 进程可以使用的资源上限（Resource Limit）。

### 5.环境变量

#### 基本简介

环境变量，是指在操作系统中用来指定操作系统运行环境的一些参数。

+ 通常具备以下特征：

  + 字符串(本质) 
  + 有统一的格式：名=值[:值] 
  +  值用来描述进程环境信息。

+ 存储形式：与命令行参数类似。char *[]数组，数组名environ，内部存储字符串，NULL作为哨兵结尾。

+ 使用形式：与命令行参数类似。

+ 加载位置：与命令行参数类似。位于用户区，高于stack的起始位置。

+ 引入环境变量表：须声明环境变量。extern char ** environ;     

+ 打印当前进程的所有环境变量：

  ```c
  #include <stdio.h>
  
  extern char **environ;
  int main(void)
  {
  	int i;
  	for(i = 0; environ[i] != NULL; i++){
  		printf("%s\n", environ[i]);
  	}
  	return 0;
  }
  ```

#### 常见环境变量

> 按照惯例，环境变量字符串都是name=value这样的形式，大多数name由大写字母加下划线组成，一般把name的部分叫做环境变量，value的部分则是环境变量的值。环境变量定义了进程的运行环境，一些比较重要的环境变量的含义如下：

+ **PATH:**

  + 可执行文件的搜索路径。ls命令也是一个程序，执行它不需要提供完整的路径名/bin/ls，然而通常我们执行当前目录下的程序a.out却需要提供完整的路径名./a.out，这是因为PATH环境变量的值里面包含了ls命令所在的目录/bin，却不包含a.out所在的目录。PATH环境变量的值可以包含多个目录，用:号隔开。在Shell中用echo命令可以查看这个环境变量的值：

    `echo $PATH`

+ **SHELL:**

  + 当前Shell，它的值通常是/bin/bash。

+ **TERM:**

  + 当前终端类型，在图形界面终端下它的值通常是xterm，终端类型决定了一些程序的输出显示方式，比如图形界面终端可以显示汉字，而字符终端一般不行。

+ **LANG:**

  + 语言和locale，决定了字符编码以及时间、货币等信息的显示格式。

+ **HOME:**

  + 当前用户主目录的路径，很多程序需要在主目录下保存配置文件，使得每个用户在运行该程序时都有自己的一套配置。

#### 常见环境变量操作函数

+ **getenv函数：**
  + 获取环境变量值
  + ` char *getenv(const char *name);`  成功：返回环境变量的值；失败：NULL (name不存在)

+ **setenv函数：**
  + 设置环境变量的值    
  + `int setenv(const char *name, const char *value, int overwrite);` 
  + 成功：0；失败：-1；
  + 参数overwrite取值：
    + 1：覆盖原环境变量 
    + 0：不覆盖。(该参数常用于设置新环境变量，如：ABC = haha-day-night)

+ **unsetenv函数：**

  + 删除环境变量name的定义

  + `int unsetenv(const char *name);`

  +   成功：0；失败：-1  

    >  注意事项：name不存在仍返回0(成功)，当name命名为"ABC="时则会出错。

+ 例子：

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  
  int main(void)
  {
  	char *val;
  	const char *name = "ABD";
  
  	val = getenv(name);
  	printf("1, %s = %s\n", name, val);
  
  	setenv(name, "haha-day-and-night", 1);
  
  	val = getenv(name);
  	printf("2, %s = %s\n", name, val);
  
  #if 1
  	int ret = unsetenv("ABD=");
      printf("ret = %d\n", ret);
  
  	val = getenv(name);
  	printf("3, %s = %s\n", name, val);
  
  #else
  	int ret = unsetenv("ABD");  //name=value:value
  	printf("ret = %d\n", ret);
  
  	val = getenv(name);
  	printf("3, %s = %s\n", name, val);
  
  #endif
  
  	return 0;
  }
  ```

---



## 五.进程创建

### 1.进程控制

#### fork函数

+ 创建一个子进程。

  + `pid_t fork(void);`

  +    失败返回-1；成功返回：① 父进程返回子进程的ID(非负)  ②子进程返回 0 

  + pid_t类型表示进程ID，但为了表示-1，它是有符号整型。(0不是有效进程ID，init最小，为1)

    >  注意返回值，不是fork函数能返回两个值，而是fork后，fork函数变为两个，父子需【各自】返回一个值。

#### 循环创建n个子进程

+ 一次fork函数调用可以创建一个子进程。那么创建N个子进程应该怎样实现呢？

  <img src="../图片/Linux90.png" style="zoom:50%;" />

+ 从上图我们可以很清晰的看到，当n为3时候，循环创建了(2^n)-1个子进程，而不是N的子进程。需要在循环的过程，保证子进程不再执行fork ，因此当(fork() == 0)时，子进程应该立即break;才正确。

  ```c
  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  
  int main(int argc, char *argv[])
  {
  	int n = 5, i;				//默认创建5个子进程
  
  	if(argc == 2){	
  		n = atoi(argv[1]);
  	}
  
  	for(i = 0; i < n; i++)	//出口1,父进程专用出口
  		if(fork() == 0)
  			break;			//出口2,子进程出口,i不自增
  
  	if(n == i){
  		sleep(n);
  		printf("I am parent, pid = %d\n", getpid());
  	} else {
  		sleep(i);
  		printf("I'm %dth child, pid = %d\n", i+1, getpid());
  	}
  
  	return 0;
  }
  ```

### 2.进程共享

父子进程之间在fork后

+  **父子相同处:**  全局变量、.data、.text、栈、堆、环境变量、用户ID、宿主目录、进程工作目录、信号处理方式...

+ **父子不同处:**  
  + 进程ID  
  + fork返回值  
  + 父进程ID  
  + 进程运行时间  
  + 闹钟(定时器)  
  + 未决信号集

+ 似乎，子进程复制了父进程0-3G用户空间内容，以及父进程的PCB，但pid不同。真的每fork一个子进程都要将父进程的0-3G地址空间完全拷贝一份，然后在映射至物理内存吗？

+ 当然不是!父子进程间遵循 **<font color = red>读时共享写时复制</font>** 的原则。这样设计，无论子进程执行父进程的逻辑还是执行自己的逻辑都能节省内存开销。  

+  **父子进程共享：** 
  + 文件描述符(打开文件的结构体) 
  +  mmap建立的映射区 (进程间通信详解)

+ 特别的，fork之后父进程先执行还是子进程先执行不确定。取决于内核所使用的调度算法。

### 3.gdb调试

+ 使用gdb调试的时候，gdb只能跟踪一个进程。可以在fork函数调用之前，通过指令设置gdb调试工具跟踪父进程或者是跟踪子进程。默认跟踪父进程。

  + `set follow-fork-mode child` 命令设置gdb在fork之后跟踪子进程。
  + `set follow-fork-mode parent` 设置跟踪父进程。

  > 注意，一定要在fork函数调用之前设置才有效。  

### 4.exec函数族

fork创建子进程后执行的是和父进程相同的程序（但有可能执行不同的代码分支），子进程往往要调用一种exec函数以执行另一个程序。当进程调用一种exec函数时，该进程的用户空间代码和数据完全被新程序替换，从新程序的启动例程开始执行。调用exec并不创建新进程，所以调用exec前后该进程的id并未改变。

将当前进程的.text、.data替换为所要加载的程序的.text、.data，然后让进程从新的.text第一条指令开始执行，但进程ID不变，换核不换壳。

有六种以exec开头的函数，统称exec函数：

+ int execl(const char *path, const char *arg, ...);
+ int execlp(const char *file, const char *arg, ...);
+ int execle(const char *path, const char *arg, ..., char *const envp[]);
+ int execv(const char *path, char *const argv[]);
+ int execvp(const char *file, char *const argv[]);
+ int execve(const char *path, char *const argv[], char *const envp[]);

#### execlp函数

+ 加载一个进程，借助PATH环境变量    
  + `int execlp(const char *file, const char *arg, ...);   `  
  +  成功：无返回；
  + 失败：-1

+ 参数1：要加载的程序的名字。该函数需要配合PATH环境变量来使用，当PATH中所有目录搜索后没有参数1则出错返回。

+  该函数通常用来调用系统程序。如：ls、date、cp、cat等命令。

```c
//execlp实现ls -al命令
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
	perror("fork error");
    }else if(pid > 0) {
	sleep(1);
	printf("parent\n");
    }else {
	execlp("ls","ls","-l","-a",NULL);
    }
    return 0;
}
```

#### execl函数

+ 加载一个进程， 通过 `路径+程序名` 来加载。 
  + int execl(const char *path, const char *arg, ...);      
  + 成功：无返回；
  + 失败：-1

+ 对比execlp，如加载"ls"命令带有-l，-F参数
  + `execlp("ls", "ls", "-l", "-F", NULL);  `   使用程序名在PATH中搜索。
  + `execl("/bin/ls", "ls", "-l", "-F", NULL);  `使用参数1给出的绝对路径搜索。

####  execv函数

+ 加载一个进程，使用使用字符串参数
  + `int execv(const char *path, const char *argv[]);`

+ 变参形式： ①... ② argv[] (main函数也是变参函数，形式上等同于 int main(int argc, char *argv0, ...)) 

+ 变参终止条件：① NULL结尾 ② 固参指定

> execv与exec参数形式不同，原理一致。

```c
//execlp实现ls -al命令
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
	perror("fork error");
    }else if(pid > 0) {
	sleep(1);
	printf("parent\n");
    }else {
      char argv[] = {"ls","ls","-l","-a",NULL};
			execvp("/bin/ls",argv);
    }
    return 0;
}
```

#### exec函数族总结

+ exec函数一旦调用成功即执行新的程序，不返回。**只有失败才返回，错误值-1**。所以通常我们直接在exec函数调用后直接调用perror()和exit()，无需if判断。

+ 参数含义：
  + l (list)          命令行参数列表
  + p (path)         搜素file时使用path变量
  + v (vector)        使用命令行参数数组
  + e (environment)   使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量

+ 事实上，只有execve是真正的系统调用，其它五个函数最终都调用execve，所以execve在man手册第2节，其它函数在man手册第3节。这些函数之间的关系如下图所示。

  <img src="../图片/Linux91.png" style="zoom:50%;" />

### 5.回收子进程

#### 孤儿进程

+ 孤儿进程: 父进程先于子进程结束，则子进程成为孤儿进程，子进程的父进程成为init进程，称为init进程领养孤儿进程。

#### 僵尸进程

+ 僵尸进程: 进程终止，父进程尚未回收，子进程残留资源（PCB）存放于内核中，变成僵尸（Zombie）进程。

> 特别注意，僵尸进程是不能使用kill命令清除掉的。因为kill命令只是用来终止进程的，而僵尸进程已经终止。

+ 例子：

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <sys/wait.h>
  
  int main(void)
  {
      pid_t pid, wpid;
      pid = fork();
      int status;
      
      if (pid == 0) {
              printf("---child, my parent= %d, going to sleep 10s\n", getppid());
              printf("-------------child die--------------\n");
      } else if (pid > 0) {
          while (1) {
              printf("I am parent, pid = %d, myson = %d\n", getpid(), pid);
  
          }
      } else {
          perror("fork");
          return 1;
      }
  
      return 0;
  }
  ```

  > 出现僵尸进程

![image-20210425164254947](../图片/Linux92.png)

#### wait函数

一个进程在终止时会关闭所有文件描述符，释放在用户空间分配的内存，但它的PCB还保留着，内核在其中保存了一些信息：如果是正常终止则保存着退出状态，如果是异常终止则保存着导致该进程终止的信号是哪个。这个进程的父进程可以调用wait或waitpid获取这些信息，然后彻底清除掉这个进程。我们知道一个进程的退出状态可以在Shell中用特殊变量$?查看，因为Shell是它的父进程，当它终止时Shell调用wait或waitpid得到它的退出状态同时彻底清除掉这个进程。

父进程调用wait函数可以回收子进程终止信息。该函数有三个功能：

+  阻塞等待子进程退出
+  回收子进程残留资源 
+  获取子进程结束状态(退出原因)。

<img src="../图片/Linux96.png" style="zoom:50%;" />

**pid_t wait(int \*status);**  成功：清理掉的子进程ID；失败：-1 (没有子进程)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid, wpid;
    pid = fork();
    int status;
    
    if (pid == 0) {
            printf("---child, my parent= %d, going to sleep 10s\n", getppid());
	    sleep(10);
            printf("-------------child die--------------\n");
    } else if (pid > 0) {
      //回收子进程
			wpid = wait(NULL);
			if(wpid > 0) {
				while (1) {
			    printf("I am parent, pid = %d, myson = %d\n", getpid(), pid);
				}
			}
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}
```

当进程终止时，操作系统的隐式回收机制会：

+ 关闭所有文件描述符 
+  释放用户空间分配的内存。

内核的PCB仍存在。其中保存该进程的退出状态。(正常终止→退出值；异常终止→终止信号)

可使用wait函数传出参数status来保存进程的退出状态。借助宏函数来进一步判断进程终止的具体原因。宏函数可分为如下三组：

+  WIFEXITED(status) 为非0  → 进程正常结束
  + WEXITSTATUS(status) 如上宏为真，使用此宏 → 获取进程退出状态 (exit的参数)

+ WIFSIGNALED(status) 为非0 → 进程异常终止
  + WTERMSIG(status) 如上宏为真，使用此宏 → 取得使进程终止的那个信号的编号。

+ WIFSTOPPED(status) 为非0 → 进程处于暂停状态
  +  WSTOPSIG(status) 如上宏为真，使用此宏 → 取得使进程暂停的那个信号的编号。
  +   WIFCONTINUED(status) 为真 → 进程暂停后已经继续运行

+ 例子

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    
    if (pid == 0) {
            printf("---child, my parent= %d, going to sleep 60s\n", getppid());
	    sleep(60);
            printf("-------------child die--------------\n");
	    return 100;
    } else if (pid > 0) {
	wpid = wait(&status);
	if(wpid == -1) {
	    perror("wait error");
	    exit(-1);
	}
	if(WIFEXITED(status)){
	    printf("child exited with %d\n",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
	    printf("child killed with %d\n",WTERMSIG(status));
	}

	while (1) {
	    printf("I am parent, pid = %d, myson = %d\n", getpid(), pid);
	    sleep(1);
	}
    } else {
        perror("fork");
        return 1;
    }

    return 0;
}
```

+ 正常结束

<img src="../图片/Linux93.png" style="zoom:50%;" />

+ 异常结束

<img src="../图片/Linux94.png" style="zoom:50%;" />

#### waitpid函数

+ 函数功能：用来等待某个特定进程的结束
+ status如果不为空，会把状态信息写到它指向的位置
+ options允许改变waitpid的行为，最有用的一个选项是WNOHANG，它的作用是防止waitpid把调用者的执行挂起
+ 返回值：成功返回等待子进程的pid，失败返回-1

<img src="../图片/Linux95.png" style="zoom:50%;" />

+ 例子

  ```c
  #include <unistd.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <sys/wait.h>
  
  int main(void)
  {
  	pid_t pid, pid2, wpid;
  	int flg = 0;
  
  	pid = fork();
  	pid2 = fork();
  
  	if(pid == -1){
  		perror("fork error");
  		exit(1);
  	} else if(pid == 0){		//son
  		printf("I'm process child, pid = %d\n", getpid());
  		sleep(5);				
  		exit(4);
  	} else {					//parent
  		do {
  			wpid = waitpid(pid, NULL, WNOHANG);
              //wpid = wait(NULL);
  			printf("---wpid = %d--------%d\n", wpid, flg++);
  			if(wpid == 0){
  				printf("NO child exited\n");
  				sleep(1);		
  			}
  		} while (wpid == 0);		//子进程不可回收
  
  		if(wpid == pid){		//回收了指定子进程
  			printf("I'm parent, I catched child process,"
  					"pid = %d\n", wpid);
  		} else {
  			printf("other...\n");
  		}
  	}
  	return 0;
  }
  ```



## 六.进程间通信

### 1.IPC方法

Linux环境下，进程地址空间相互独立，每个进程各自有不同的用户地址空间。任何一个进程的全局变量在另一个进程中都看不到，所以进程和进程之间不能相互访问，要交换数据必须通过内核，在内核中开辟一块缓冲区，进程1把数据从用户空间拷到内核缓冲区，进程2再从内核缓冲区把数据读走，内核提供的这种机制称为进程间通信（IPC，InterProcess Communication）。

<img src="../图片/Linux97.png" style="zoom:50%;" />

在进程间完成数据传递需要借助操作系统提供特殊的方法，如：文件、管道、信号、共享内存、消息队列、套接字、命名管道等。随着计算机的蓬勃发展，一些方法由于自身设计缺陷被淘汰或者弃用。现今常用的进程间通信方式有：

+ 管道 (使用最简单)
+ 信号 (开销最小)
+  共享映射区 (无血缘关系)
+  本地套接字 (最稳定)

### 2.管道

#### 管道的概念

+ 管道是一种最基本的IPC机制，作用于有血缘关系的进程之间，完成数据传递。调用pipe系统函数即可创建一个管道。有如下特质：
  + 其本质是一个伪文件(实为内核缓冲区) 
  + 由两个文件描述符引用，一个表示读端，一个表示写端。
  + 规定数据从管道的写端流入管道，从读端流出。

+ 管道的原理: 管道实为内核使用环形队列机制，借助内核缓冲区(4k)实现。

+ 管道的局限性：
  +  数据自己读不能自己写。
  + 数据一旦被读走，便不在管道中存在，不可反复读取。
  + 由于管道采用半双工通信方式。因此，数据只能在一个方向上流动。
  + 只能在有公共祖先的进程间使用管道。

#### pipe函数

+ 功能：创建管道
+ 形式：
  + `int pipe(int pipefd[2]);`      
  + 成功：0；失败：-1，设置errno
+ 函数调用成功返回r/w两个文件描述符。无需open，但需手动close。
+ 规定：fd[0] → r； fd[1] → w，就像0对应标准输入，1对应标准输出一样。向管道文件读写数据其实是在读写内核缓冲区。

+ 管道创建成功以后，创建该管道的进程（父进程）同时掌握着管道的读端和写端。父子进程间通信的实现方式如下图所示：

  <img src="../图片/Linux98.png" style="zoom:50%;" />

  + 父进程调用pipe函数创建管道，得到两个文件描述符fd[0]、fd[1]指向管道的读端和写端。
  + 父进程调用fork创建子进程，那么子进程也有两个文件描述符指向同一管道。
  + 父进程关闭管道读端，子进程关闭管道写端。父进程可以向管道中写入数据，子进程将管道中的数据读出。由于管道是利用环形队列实现的，数据从写端流入管道，从读端流出，这样就实现了进程间通信。

#### 管道的读写行为

+ **读管道：**

  + 管道中有数据，read返回实际读到的字节数。

  + 管道中无数据：

    + 管道写端被全部关闭，read返回0 (好像读到文件结尾)
    + 写端没有全部被关闭，read阻塞等待(不久的将来可能有数据递达，此时会让出cpu)

  + 例子：

    ```c
    #include<unistd.h>
    #include<stdio.h>
    #include<stdlib.h>
    
    int main() {
    
        int fd[2];
        int ret;
        char writebuf[] = "hello linux";
        char readbuf[128] = {0};
    
        ret = pipe(fd);
        if(ret < 0) {
            perror("pip error");
            exit(1);
        }
        printf("create pipe sucess fd[0]=%d, fd[1]=%d\n",fd[0],fd[1]);
    		//write into pipe
        write(fd[1],writebuf,sizeof(writebuf));
    
        //read from pipe
        read(fd[0], readbuf, 128);
        printf("readbuf = %s\n", readbuf);
    
        read(fd[0], readbuf, 128);
        printf("second readbuf");
    
        return 0;
    }
    ```

    ![image-20210426161107984](../图片/Linux99.png)

    > 第二次进行读操作时出现阻塞。
    >
    > 特点：
    >
    > + 管道中读完会删除内容。
    > + 管道中无内容时进行读，会出现阻塞现象。

+ **写管道：**

  + 管道读端全部被关闭， 进程异常终止(也可使用捕捉SIGPIPE信号，使进程不终止)
  + 管道读端没有全部关闭：
    + 管道已满，write阻塞。
    + 管道未满，write将数据写入，并返回实际写入的字节数。

+ **读写管道实现父子进程间的先后运行：**

  ```c
  #include"stdio.h"
  #include"sys/types.h"
  #include"stdlib.h"
  #include"unctrl.h"
  #include"unctrl.h"
  int main() {
      int fd[2];
      //进程间的锁
      char process_inter = 0;
      //创建管程
      int ret = pipe(fd);
      if(ret < 0){
          perror("pipe error");
          exit(1);
      }
      printf("create pip successfully\n");
      //创建子进程
      pid_t pid = fork();
      //子进程运行
      if (pid == 0)
      {
          read(fd[0], &process_inter, 1);
          //等待父进程修改管程信息为1
          while(process_inter == 0);
          for (int i = 0; i < 5; i++)
          {
              printf("this is child process, id = %d\n", i);
              sleep(1);
          }
      }
      //父进程运行
      else if(pid > 0) 
      {
          for (int i = 0; i < 5; i++)
          {
              printf("this is parent process, id = %d\n", i);
              sleep(1);
          }
          process_inter = 1;
          //修改process_inter
          write(fd[1], &process_inter, 1);
      }
      while (1);
      return 0;
  }
  ```

#### 管道的优劣

+ 优点：
  + 简单，相比信号，套接字实现进程间通信，简单很多。
+ 缺点：
  + 只能单向通信，双向通信需建立两个管道。
  + 只能用于父子、兄弟进程(有共同祖先)间通信。该问题后来使用fifo有名管道解决。



### 3.FIFO

+ FIFO常被称为命名管道，以区分管道(pipe)。管道(pipe)只能用于“有血缘关系”的进程间。但通过FIFO，不相关的进程也能交换数据。

+ FIFO是Linux基础文件类型中的一种。但，FIFO文件在磁盘上没有数据块，仅仅用来标识内核中一条通道。各进程可以打开这个文件进行read/write，实际上是在读写内核通道，这样就实现了进程间通信。

+ 创建方式：

  + 命令：mkfifo 管道名
  + 库函数：`int mkfifo(const char *pathname,  mode_t mode); `
    + 成功：0
    + 失败：-1

+ 一旦使用mkfifo创建了一个FIFO，就可以使用open打开它，常见的文件I/O函数都可用于fifo。如：close、read、write、unlink等。

  > 只有生成FIFO之后，使用open打开时才会在内核中出现管道。

+ 创建有名管道：

  ```c
  #include"stdlib.h"
  #include"unistd.h"
  #include"stdio.h"
  
  int main() {
      int ret = mkfifo("./myfifo",0777);
  
      if(ret < 0) {
          perror("mkfifo error");
          exit(1);
      }
      printf("create myfifo successfully\n");
  
      return 0;
  }
  ```

+ 利用有名管道进行无血缘关系进程之间的通信：

  ```c
  //first.c
  #include"stdlib.h"
  #include"sys/types.h"
  #include"stdio.h"
  #include"fcntl.h"
  #include"unistd.h"
  int main() {
      char process_inter = 0;
  
      int fd = open("./myfifo",O_WRONLY);
  
      if(fd < 0) {
          perror("mkfifo error");
          exit(1);
      }
  
      printf("open myfifo successfully\n");
  
      for (int i = 0; i < 5; i++)
      {
          printf("this is first process id = %d\n", i);
          sleep(1);
      }
      process_inter = 1;
      sleep(5);
      write(fd,&process_inter,1);
      while (1);  
      return 0;
  }
  //=======================================================
  //second.c
  #include"stdlib.h"
  #include"sys/types.h"
  #include"stdio.h"
  #include"fcntl.h"
  #include"unistd.h"
  
  int main() {
      char process_inter = 0;
  
      int fd = open("./myfifo",O_RDONLY);
  
      if(fd < 0) {
          perror("mkfifo error");
          exit(1);
      }
      printf("open myfifo successfully\n");
  
     read(fd, &process_inter, 1);
     while (process_inter == 0);
  
      for (int i = 0; i < 5; i++)
      {
          printf("this is second process id = %d\n", i);
          sleep(1);
      }
      process_inter = 1;
      while (1);  
      return 0;
  }
  ```



### 4.文件映射存储





---

### 1.Linux 快捷键

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux1.png" width="900px">
</div>

