# 一.Linux基础

## (一).Linux基本操作

### 1.Linux 快捷键

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux1.png" width="900px">
</div>

### 2.Linux目录结构

+ 树结构如下：

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux2.png" width="700px">
  </div>

+ 以下是对这些目录的解释：

+ **/bin**：
  + bin是Binary的缩写, 这个目录存放着最经常使用的命令。

+ **/boot：**
  + 这里存放的是启动Linux时使用的一些核心文件，包括一些连接文件以及镜像文件。

+ **/dev ：**
  + dev是Device(设备)的缩写, 该目录下存放的是Linux的外部设备，在Linux中访问设备的方式和访问文件的方式是相同的。

+ **/etc：**
  + 这个目录用来存放所有的系统管理所需要的配置文件和子目录。

+ **/home**：
  + 用户的主目录，在Linux中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的。

+ **/lib**：
  + 这个目录里存放着系统最基本的动态连接共享库，其作用类似于Windows里的DLL文件。几乎所有的应用程序都需要用到这些共享库。

+ **/lost+found**：
  + 这个目录一般情况下是空的，当系统非法关机后，这里就存放了一些文件。

+ **/media**    
  + linux系统会自动识别一些设备，例如U盘、光驱等等，当识别后，linux会把识别的设备挂载到这个目录下。

+ **/mnt**：
  + 系统提供该目录是为了让用户临时挂载别的文件系统的，我们可以将光驱挂载在/mnt/上，然后进入该目录就可以查看光驱里的内容了。

+ **/opt**：
  + 这是给主机额外安装软件所摆放的目录。比如你安装一个ORACLE数据库则就可以放到这个目录下。默认是空的。

+ **/proc**：

  + 这个目录是一个虚拟的目录，它是系统内存的映射，我们可以通过直接访问这个目录来获取系统信息。
    这个目录的内容不在硬盘上而是在内存里，我们也可以直接修改里面的某些文件，比如可以通过下面的命令来屏蔽主机的ping命令，使别人无法ping你的机器：

    ```bash
    echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all
    ```

+ **/root：
  + 该目录为系统管理员，也称作超级权限者的用户主目录**。

+ **/sbin**：
  + s就是Super User的意思，这里存放的是系统管理员使用的系统管理程序。

+ **/selinux**：
  + 这个目录是Redhat/CentOS所特有的目录，Selinux是一个安全机制，类似于windows的防火墙，但是这套机制比较复杂，这个目录就是存放selinux相关的文件的。

+ **/srv**：
  + 该目录存放一些服务启动之后需要提取的数据。

+ **/sys**：
  + 这是linux2.6内核的一个很大的变 化。该目录下安装了2.6内核中新出现的一个文件系统 sysfs 。sysfs文件系统集成了下面3种文件系统的信息：针对进程信息的proc文件系统、针对设备的devfs文件系统以及针对伪终端的devpts文件系统。该文件系统是内核设备树的一个直观反映。当一个内核对象被创建的时候，对应的文件和目录也在内核对象子系统种被创建。

+ **/tmp**：
  + 这个目录是用来存放一些临时文件的。

+ **/usr**：
  + 这是一个非常重要的目录，用户的很多应用程序和文件都放在这个目录下，类似与windows下的program files目录。

+ **/usr/bin：**
  + 系统用户使用的应用程序。

+ **/usr/sbin：**
  + 超级用户使用的比较高级的管理程序和系统守护程序。

+ **/usr/src：**
  + 内核源代码默认的放置目录。

+ **/var**：
  + 这个目录中存放着在不断扩充着的东西，我们习惯将那些经常被修改的目录放在这个目录下。包括各种日志文件。

### 3.用户目录

+ **绝对目录：**
  + 从根目录开始写 /home/itcast/aa
+ **相对目录：**
  + 相对于当前的工作目录而言
+ **在临近的两个目录直接切换：**
  + 命令：`cd -`

### 4.文件和目录操作

+ 查看目录操作

  + `tree`

  + `ls`

    + `ls - a`：会显示隐藏文件
    + `ls - la`：会显示文件属性信息

    <div align="center">
        <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux3.png" width="900px">
    </div>

+ 连续创建目录：`mkdir -p`

### 5.查看文件内容

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/mytree4.png" width="900px">
</div>

### 6.创建软连接与硬链接

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux5.png" width="900px">
</div>

### 7.文件或目录属性

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux6.png" width="800px">
</div>

### 8.文件权限

#### 修改文件权限

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux7.png" width="900px">
</div>

创建列子：

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux13.png" width="900px">
</div>

> + `-rw`：文件所有者parallels具有读写权限
> +  `-rw`：文件所属组parallels具有读写权限
> + `-r`：其他用户只具有读权限

+ 给其他人赋予写权限

  ```bash
  # o:其他人，+:添加权限，w:写权限
  chmod o+w test
  ```

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux14.png" width="900px">
  </div>

+ 给所有人添加执行权限

  ```bash
  chmod a+x test
  ```

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux15.png" width="900px">
  </div>

+ 取消其他人的执行权限

  ```bash
  chmod -001 test
  ```

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux16.png" width="900px">
  </div>

#### 修改文件所属的组和用户

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux17.png" width="900px">
</div>



### 9.文件查找与索引

#### 根据文件属性进行查找

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux8.png" width="900px">
</div>

#### 根据文件内容进行检索

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux9.png" width="800px">
</div>

### 10. 软件安装与卸载

#### 在线安装

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux10.png" width="900px">
</div>

#### deb包安装

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux11.png" width="600px">
</div>

#### 源码安装

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux12.png" width="700px">
</div>

### 11.U盘挂载

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux18.png" width="700px">
</div>

### 12.压缩管理

#### 早期的压缩命令

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux19.png" width="750px">
</div>

#### tar打包压缩与解压缩

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux20.png" width="800px">
</div>

+ **例子：**

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux21.png" width="700px">
  </div>

  + 指定目录解压

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux22.png" width="700px">
  </div>

#### rar进行打包压缩与解压

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux23.png" width="800px">
</div>

+ **例子：**

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux24.png" width="650px">
  </div>

#### zip打包压缩与解压

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux25.png" width="800px">
</div>

+ **例子：**

  <div align="center">
      <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux26.png" width="700px">
  </div>

### 13.进程管理

#### ps操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux27.png" width="800px">
</div>

#### kill命令的使用

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux28.png" width="800px">
</div>

#### env与top操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux29.png" width="800px">
</div>

### 14.网络管理

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux30.png" width="650px">
</div>

### 15.用户管理

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux31.png" width="600px">
</div>

### 16.ftp服务器操作

#### 服务器端的安装，配置与启动

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux32.png" width="600px">
</div>

#### 客户端的操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux33.png" width="800px">
</div>

#### 数据上传与下载

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux34.png" width="700px">
</div>

### 17.nfs服务器操作

#### 服务器端操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux35.png" width="800px">
</div>

#### 客户端操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux36.png" width="500px">
</div>

### 18.ssh服务器操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux37.png" width="500px">
</div>

#### scp操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux38.png" width="600px">
</div>

### 19.man和echo操作

#### 看手册

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux39.png" width="600px">
</div>

#### echo操作

<div align="center">
    <img src="https://github.com/ZYBO-o/Accumulation/blob/main/%E5%9B%BE%E7%89%87/Linux40.png" width="600px">
</div>

---

## (二).vim操作





































