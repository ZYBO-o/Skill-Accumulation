# 一.基本概念

## (一).数据库基础

### 1.什么是数据库

+  **数据库（database）** 是一个以某种有组织的方式存储的数据集合。是保存有组织的数据的容器(通常是一个文件或一组文件)。

+  **表（table）** 是一种结构化的文件，可用来存储某种特定类型的数据。是某种特定类型数据的结构化清单。
+  **列（column）**  是表中的一个字段。所有表都是由一个或多个列组成的。
+  **数据类型（datatype）** 所容许的数据的类型。每个表列都有相应的数据类型，它限制(或容许)该列中存储的数据。
+  **行（row）** 表中的一个记录。表中的数据是按行存储的，所保存的每个记录存储在自己的行内。
+  **主键（primary key）** 一列(或一组列)，其值能够唯一区分表中每个行。

### 2.什么是MySQL

+ 数据的所有存储、 检索、管理和处理实际上是由数据库软件—— **DBMS(数据库管理系统)** 完成的。MySQL是一种DBMS，即它是一种数据库软件

+ DBMS可分为两类：

  + 基于共享文件系统的DBMS

    + 用于桌面用途，通常不用于高端或更关键的应用(如Microsoft Access和FileMaker)。

  + 基于客户机—服务器的DBMS

    + 服务器负责数据访问和处理的一个软件。这个软件运行在称为数据库服务器的计算机上。
    + 客户机是与用户打交道的软件。

    > 客户机软件通过网络提交该请求给服务器软件。服务器软件处理这个请求，根据需要过滤、丢弃和排序数据；然后把结果送回到你的客户机软件。

# 二.基本操作

## (一).链接

+ `SHOW COLUMNS` 对表的每个字段返回一行，行中包含字段名、数据类型、是否允许NULL、键信息、默认值以及其他信息(如字段的auto_increment)。

  ```sql
  SHOW COLUMNS FROM costomers;
  ```

+ `SHOW STATUS` 用于显示广泛的服务器状态信息;

+ `SHOW CREATE DATABASE` 和 `SHOW CREATE TABLE` 用来显示创建特定数据库或表的MySQ语句;

+ `SHOW GRANTS` 用来显示授予用户(所有用户或特定用户)的安全权限;

+ `SHOW ERRORS` 和 `SHOW WARNINGS` 用来显示服务器错误或警告消息。

## (二).检索语句

### 1.select语句

```sql
SELECT COLUMN_1, COLUMN_2 FROM tablename;		  			#检索多行
SELECT * FROM tablename;														#检索全部行
#进行限制，返回唯一值
SELECT DISTINCT COLUMN_1 FROM FROM tablename;				#返回不同(唯一)的COLUMN_1
#返回特定的行
SELECT COLUMN_1, COLUMN_2 FROM tablename LIMIT 5; 	#返回结果不多于5行。
SELECT COLUMN_1, COLUMN_2 FROM tablename LIMIT 4,5;	#返回从行4开始的5行(第一行为行0，类似于数组)
#使用完全限定的表名
SELECT tablename.COLUMN_1 FROM databasename.tablename;
```

> + `DISTINCT` 关键字进行限制返回的值唯一
> + `LIMIT` 关键字进行限制返回的起始行与行数

### 2.进行排序

```sql
#将返回的行以 COLUMN_1 进行升序排序（默认升序）
SELECT COLUMN_1, COLUMN_2 FROM tablename ORDER BY COLUMN_1;
#将返回的行以 COLUMN_1 进行降序排序
SELECT COLUMN_1, COLUMN_2 FROM tablename ORDER BY COLUMN_1 DESC;
```

> + `ORDER BY` 字句进行升序排序
>
> + `DESC` 修饰排序为降序； `ASC` 为升序
> + 使用 `ORDER BY` 和 `LIMIT` 进行组合时，保证 `LIMIT` 在后面

### 3.过滤数据

SELECT语句中，数据根据WHERE子句指定的搜索条件进行过滤。 在同时使用 `ORDERBY` 和 `WHERE` 子句时，应该让 `ORDER BY` 位于 `WHERE` 之后，否则将会产生错误。

<div align = "center"><img src="../图片/MYSQL基础1.png" width="600px" /></div>

```sql
SELECT COLUMN_1, COLUMN_2 FROM tablename WHERE COLUMN_1 BETWEEN A AND B;
```

#### 空值检查

+ 在创建表时可以指定其中的列是否可以不包含值。在一个列不包含值时，称其为包含空值NULL。
+ NULL 无值(no value)，与字段包含0、空字符串或仅仅包含空格不同。

```sql
SELECT COLUMN_1, COLUMN_2 FROM tablename WHERE COLUMN_1 IS NULL;
```

### 4.数据过滤

> MySQL允许给出多个WHERE子句：以AND子句的方式或OR子句的方式使用。

#### (1).AND操作符

为了通过不止一个列进行过滤，可使用AND操作符给WHERE子句附加条件。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE vend_id = 1003 AND prod_price <= 10;
```

#### (2).OR操作符

OR操作符与AND操作符不同，它指示MySQL检索匹配任一条件的行。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE vend_id = 1003 OR vend_id = 1002;
```

其中 AND 的计算次序比 OR 优先级更高。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE vend_id = 1003 OR vend_id = 1002 AND prod_price >= 10;
```

理解为 1003制造的价格为10美元(含)以上的产品，或者1002制造的任何产品， 而不管其价格如何。 **此问题的解决方法是使用圆括号明确地分组相应的操作符。**

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE (vend_id = 1003 OR vend_id = 1002) AND prod_price >= 10;
```

#### (3).IN操作符

IN操作符用来指定条件范围，范围中的每个条件都可以进行匹配。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE vend_id IN (1002,1003)
ORDER BY prod_name;
```

+ IN的最大优点是可以包含其他SELECT语句，使得能够更动态地建立WHERE子句
+ IN操作符的语法更清楚且更直观。计算的次序更容易管理
+ IN操作符一般比OR操作符清单执行更快。

#### (4).NOT操作符

WHERE子句中的NOT操作符有且只有一个功能，那就是否定它之后所跟的任何条件。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE vend_id NOT IN (1002,1003)
ORDER BY prod_name;
```

>  MySQL不是匹配1002和1003的vend_id，而是匹配1002和1003之外供应商的 vend_id。

### 5. 用通配符与LIKE进行过滤

> +  **通配符(wildcard)**  用来匹配值的一部分的特殊字符。 
> +  **搜索模式(search pattern)** 由字面值、通配符或两者组合构成的搜索条件。

LIKE 指示MySQL后跟的搜索模式利用通配符匹配而不是直接相等匹配进行比较。

#### (1).百分号(%)通配符

%表示任何字符出现任意次数。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name LIKE 'jet%';	#找出所有以词jet起头的产品
```

> + 搜索可以是区分大小写的。如果区分大小写，'jet%' 与JetPack将不匹配
> + 尾空格可能会干扰通配符匹配。例如在保存词 anvil时，如果后面有一个或多个空格，则子句``WHERE prod_name LIKE '%anvil' `不会匹配它们，因为在最后的`l  `后有多余的字符。

通配符可在搜索模式中任意位置使用，并且可以使用多个通配符。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name LIKE '%anvil%';
```

> 虽然似乎%通配符可以匹配任何东西，除了NULL。即使是WHERE prod_name LIKE '%'也不能匹配用值NULL作为产品名的行。

#### (2).下划线(_)通配符

下划线的用途与%一样，但下划线只匹配单个字符而不是多个字符。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name LIKE '_ to anvil';
```

#### (3).通配符的使用代价

通配 符搜索的处理一般要比前面讨论的其他搜索所花时间更长。

+ 不要过度使用通配符。如果其他操作符能达到相同的目的，应该使用其他操作符。
+ 在确实需要使用通配符时，除非绝对有必要，否则不要用在搜索模式的开始处。把通配符置于搜索模式的开始处，搜索起来是最慢的。

### 6.正则表达式

MySQL 用 WHERE 子句指定正则表达式过滤SELECT检索出的数据。

#### (1).使用REGEXP

##### 基本字符匹配

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name REGEXP '1000'
ORDER BY pro_name;
```

这条语句非常像使用LIKE。REGEXP后所跟的东西作为正则表达式(与文字正文1000匹配)处理

##### 与LIKE的区别

+ LIKE匹配整个列。如果被匹配的文本在列值 中出现，LIKE将不会找到它，相应的行也不被返回（除非使用通配符）。
+ REGEXP在列值内进行匹配，如果被匹配的文本在列值中出现，REGEXP将会找到它，相应的行将被返回。 

<div align = "center"><img src="../图片/MYSQL基础3.png" width="350px" /></div>

##### 区分大小写

MySQL中的正则表达式匹配不区分大小写。为区分大小写，可使用 **BINARY** 关键字，如：

```sql
WHERE prod_name REGEXP BINARY 'JetPack .000';
```

#### (2).进行OR匹配

为搜索两个串之一(或者为这个串，或者为另一个串)，使用`|`。

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name REGEXP '1000 | 2000'
ORDER BY pro_name;
```

使用 `|` 从功能上类似于在SELECT语句中使用OR语句，多个OR条件可并入单个正则表达式。

#### (3).匹配几个字符之一

匹配特定的字符可通过指定  `[]`  括起来的字符来完成，如下所示:

```sql
SELECT prod_id, prod_price, prod_name
FROM products
WHERE pro_name REGEXP '[123] Ton'
ORDER BY pro_name;
```

`[123]`  定义一组字符，它的意思是匹配1或2或3，因此表中的 1 ton和2 ton都匹配且返回

### 7.创建计算字段





我们投了先进计算会议嘛，然后这次的论文能进行EI检索，我们这边就打算以这篇论文为毕业的小论文。然后上次万政
