## 一.安装MySQL的C/C++ API 

```bash
sudo apt-get install mysql-server libmysql++-dev;
# 安装 MySQL的 C/C++ API

sudo cp /usr/lib/mysql/* /usr/lib/
# 将头文件复制到 C/C++ 的头文件目录（这样子才可以调用头文件）
```

安装好 API 之后，有了头文件，就可以调用关于 MySQL 操作的函数接口 来使用了。

>  **注意，要操作 MySQL，需要先打开 MySQL 服务才有用**

用于定义一个 mysql 对象，便于后续操作确定要操作的数据库是哪一个。

---

## 二.常用的MySQL API 的数据类型

### 1.MYSQL

用于定义一个 mysql 对象，便于后续操作确定要操作的数据库是哪一个。

```c++
MYSQL mysql;  
// mysql标记对应某个数据库
```

### 2.MYSQL_ROW

用于定义一个 **行对象** ，其内容含有一行的数据。（比如说 SELECT 返回结果就是 多行数据，然后通过函数调用，我们就可以一行一行获取）

```cpp
MYSQL_ROW row;  
// 一行里面 包括多个字段
// row[i] 可用于输出该行 对应于 第i个字段（列）的数据
```

### 3.MYSQL_FIELD

用于定义一个存储字段信息的对象。

```cpp
MYSQL_FIELD *field;  
// field->name 存储对应字段名称（一个结构体，其中应该还有其他字段信息）
```

### 4.MYSQL_RES

用于定义一个存储数据库 **检索信息结果的对象** 。

```cpp
MYSQL_RES *result;
```

---

## 三.常用的 MYSQL API 函数介绍

其实在操作mysql数据库时，最常使用的函数有以下几个：

### 1.mysql_init

用于初始化一个 MYSQL （类型）对象，来连接 mysql 服务端。

```c++
MYSQL *mysql_init( MYSQL *mysql );

// ********************************* 例子：*********************************
MYSQL mysql;
mysql_init( &mysql );
```

+ 返回值：
  + 若成功，返回值 是 MySQL 对象（这个也保存在 输入参数中）；
  + 若出错，返回 NULL
+  因为 **返回值** 和 **输入参数** 都记录有 MySQL 对象，所以一般直接用输入参数（返回值就验证是不是 == NULL 即可）

### 2.mysql_real_connect

用于连接数据库

```c++
MYSQL *mysql_real_connect (
    MYSQL *mysql,   // mysql_init()初始化的MYSQL对象，作为输入
    const char *host,   //主机地址
    const char *user,   //用户，例如：root
    const char *passwd,   //数据库的密码
    const char *db,   //要连接的数据库，例如：student
    unsigned int port,   //端口，可填0
    const char *unix_socket,   //一般为NULL
    unsigned long client_flag);  //一般为0

// ******************************** 例子：*********************************
mysql_real_connect( &mysql, "localhost", "root", 
                    "123456", "student", 0, NULL, 0 );
```

+ 连接的时候，需要连接到 MySQL 服务器，需要 IP、用户名、密码、连接到哪个数据库等信息。
+ 返回值
  + 若成功，其返回值 和 输入的第一个参数一样， **此时这个 MySQL 对象就是对应的 <font color = red>那个连接上</font> 的 库**
  + 若失败，返回 NULL

### 3.mysql_query

用于执行mysql命令。其参数应使用c风格字符串。

```c++
mysql_query( MYSQL *mysql, char * command );

// ********************************* 例子：*********************************
string command = "select * from info";
mysql_query( &mysql, command.c_str() );
```

其实就是，当我们自己直接操作 MySQL 的时候，就是用来 MySQL 的指令，比如查询 SELECT。那么用 C++ 调用 API，这里输入的 command 相当于就是 我们要想通过 C++ 去让 MySQL 做什么行为。

> 因为直接函数操作连接的对象（MYSQL 那个数据类型的对象是 库）是 库，但是具体数据是在 表，因此一般先 得到这个 库的表（或者已经知道是哪个表了，要先得到，可以先 show tables 的 MySQL 指令，然后通过获取其中一个，就是作为了 字符串，我们要处理的 表名），然后具体再操作这个表，比如要查询表中信息，如 SELECT * from 表名 作为 这个函数的 参数，就可以的。

### 4.mysql_store_result

用于获取mysql操作的检索结果。

也就是，先通过 `mysql_query()` 获取检索对象（`MYSQL *mysql`），然后再利用这个函数来处理，从而获取 检索对象中 实际的数据结果。

```c++
MYSQL_RES *mysql_store_result(MYSQL *mysql);

// ********************************* 例子：*********************************
MYSQL_RES *result;
result = mysql_store_result( &mysql );
```

+  返回值：
  + 若成功，返回的对象，就是存储数据库检索信息结果的对象；
  + 若出错，返回 NULL

### 5.mysql_num_rows



### 6.mysql_num_fields





### 7.mysql_fetch_field





### 8.mysql_fetch_row





### 9.mysql_fetch_field_direct



### 10.mysql_close

用于关闭连接。

连接完，处理完，就关闭 MySQL 连接（和处理完文件信息后，关闭文件流一样）

```c++
mysql_close( MYSQL *mysql );
```



## 四.示例代码

```c++
#include <iostream>
#include <string>
#include <mysql/mysql.h>
using namespace std;

// 通过建立一个 操作数据库 的类，后面就每操作具体的一个数据库，我们就实例化一个 对象，这样子就很方便了
// 类 可以写在 另一个 .h 头文件，实现在另一个 .cpp 中，然后增加到头文件就可以
class MysqlDB {
private:
    // 要用到的几个数据类型
    MYSQL mysql;     // 操作的 数据库对象
    MYSQL_ROW row;   // 获取一行的数据信息
    MYSQL_RES *result;  // 检索的结果集
    MYSQL_FIELD *field;  // 字段的信息，如 字段名、字段的数据类型等
public:
    // 构造函数，用于初始化，初始化对象
    MysqlDB() {
        if( mysql_init( &mysql ) == NULL ) {
            cout << "init error, line: " << __LINE__ << endl;
            exit(-1);
        }
    }
    // 析构函数，那就用于，关闭 MySQL 连接
    ~MysqlDB() {
        mysql_close( &mysql );
    }
    
    // 成员函数：用于连接 到数据库 中，重要的几个参数（定义是 string，但是要转化到 C 的 str）
    void connect( string host, string user, string passwd,  string database ) {
        if( !mysql_real_connect( &mysql, host.c_str(), user.c_str(), passwd.c_str(), 
                                    database.c_str(), 0, NULL, 0 ) ) {
            cout << "connect error, line: " << __LINE__ << endl;
            exit(-1);
        }
    }
    // 还可以继续定义其他成员函数，用于实现 更多的功能
    void add();
    void print();
};

// 给 表 info 插入输入的信息
// 这里表名，默认知道，如果不知道，我们可以先通过 定义一个成员函数，先获取 库中，所有的表
// 这样子，表名 也可以定义成一个 string，用于我们的具体选择
void MysqlDB::add() {
    string id, name, sex, birthday;
    do {
        cout << "请输入学生信息:\n";

        cin >> id >> name >> sex >> birthday;
        string sql = "INSERT INTO info VALUES('" + id + "', '" + name + 
                        "', '" + sex + "', '" + birthday + "');";

        mysql_query( &mysql, sql.c_str() );
        cout << "是否继续（y/n）: ";
        cin >> id;
    } while( id == "y" );
}

// 将查询到的结果，打印显示出来
void MysqlDB::print() {

    // string sql = "select * from info where name = '" + name + "';";  //要有''
    string sql = "select * from info;";
    mysql_query( &mysql, sql.c_str() );

    result = mysql_store_result( &mysql );
    if( !result ) {
        cout << "result error, line : " << __LINE__ << endl;
        return ;
    }

    int num;
    num = mysql_num_fields( result );  //返回字段个数
    for( int i = 0; i < num; i++ ) {
        field = mysql_fetch_field_direct( result, i );  //返回字段类型
        cout << field->name << "\t\t";  //输出字段名
    }
    cout << endl;

    while( row = mysql_fetch_row( result ), row ) {
        for( int i = 0; i < num; i++ ) {
            cout << row[i] << "\t\t";
        }
        cout << endl;
    }
}

int main() {
    // 定义成类之后，要操作一个 库，就实例化一个对象即可，而且函数功能也封装好了，直接使用就可以
    MysqlDB db;   // 实例化成对象
    // 然后连接
    db.connect( "localhost", "root", "niliushall", "student" ); 
    // 先打印，增加数据，再打印
    db.print();
    db.add();
    db.print();

    return 0;
}
```





