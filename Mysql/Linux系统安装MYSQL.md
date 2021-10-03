### 1.下载CentOS安装包

1. 去官网

```http
https://www.mysql.com/downloads/
```

2. 选择[MySQL Yum Repository](https://link.segmentfault.com/?url=https%3A%2F%2Fdev.mysql.com%2Fdownloads%2Frepo%2Fyum%2F)
3. 选择  **Red Hat Enterprise Linux 7 / Oracle Linux 7 (Architecture Independent), RPM Package**  ，点击DownLoad

### 2.安装

1. 查询是否安装了mysql

   ```bash
   rpm -qa|grep mysql
   ```

2. 卸载mysql （下面是卸载mysql的库，防止产生冲突，mysql也是类似卸载方式）

   ```bash
   rpm -e --nodeps mysql-libs-5.1.*
   #卸载之后，记得：
   find / -name mysql
   #删除查询出来的所有东西
   ```

3. 输入以下命令开始安装

   ```bash
   yum localinstall mysql80-community-release-el7-3.noarch.rpm
   ```

4. 完毕后继续执行以下命令

   ```bash
   yum install mysql-community-server
   ```

5. 安装完毕，启动mysql服务

   ```bash
   service mysqld start
   ```

6. 查看mysql服务是否启动成功

   ```bash
   ps -ef|grep mysql
   ```

### 3.修改密码

1. 查询MySQL的临时密码

   ```bash
   grep 'temporary password' /var/log/mysqld.log
   ```

2. 复制临时密码，然后登录MySQL

   ```bash
   mysql -uroot -p
   ```

3. 修改密码

   ```mysql
   ALTER USER 'root'@'localhost' IDENTIFIED BY 'MyNewPass4!';
   ```

4. 然后刷新权限

   ```mysql
   flush privileges;
   ```

5. 使用新密码重新登录

