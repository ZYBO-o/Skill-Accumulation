## 一.存储过程和函数概述

 存储过程和函数是 **事先经过编译并存储在数据库中的一段 SQL 语句的集合** ，调用存储过程和函数可以简化应用开发人员的工作，减少数据在数据库和应用服务器之间的传输，提高数据处理的效率。

 函数 ： 是一个有返回值的过程 ；

 过程 ： 是一个没有返回值的函数 ；

>  存储过程和函数的区别在于函数必须有返回值，而存储过程没有。

---

## 二.创建存储过程

创建语法：

```mysql
CREATE PROCEDURE procedure_name ([proc_parameter[,...]])
begin
	-- SQL语句
end ;
```

示例 ：

```mysql
delimiter $										-- 声明分隔符为 $

create procedure pro_test1()
begin
	select 'Hello Mysql' ;
end$

delimiter ;										-- 重新声明分隔符为 ;
```

>  **DELIMITER** 用来声明SQL语句的分隔符 , 表明命令是否已经结束了，mysql是否可以执行了。
>
> 默认情况下，delimiter是 分号 `;` 。在命令行客户端中，如果有一行命令以分号结束，那么回车后，mysql将会执行该命令。

---

## 三.调用存储过程

调用语法：

```mysql
call procedure_name() ;	
```

---

## 四.查看存储过程

查看语法：

```mysql
-- 查询db_name数据库中的所有的存储过程
select name from mysql.proc where db='db_name';

-- 查询存储过程的状态信息
show procedure status;

-- 查询某个存储过程的定义
show create procedure test.pro_test1 \G;
```

---

## 五.删除存储过程

删除语法：

```mysql
DROP PROCEDURE  [IF EXISTS] sp_name ；
```

---

## 六.语法

存储过程是可以编程的，意味着可以使用变量，表达式，控制结构 ， 来完成比较复杂的功能。

### 1.变量

#### DECLARE

通过 DECLARE 可以定义一个局部变量，该变量的作用范围只能在 BEGIN…END 块中。

```mysql
DECLARE var_name[,...] type [DEFAULT value]
```

示例 :

```mysql
 delimiter $

 create procedure pro_test1() 
 begin 
 		declare num int default 5;
 		select num + 10; 
 end$

 delimiter ; 
```

<div align = center><img src="../图片/Mysql_12.png" width="800px" /></div>

#### SET

直接赋值使用 SET，可以赋常量或者赋表达式，具体语法如下：

```mysql
  SET var_name = expr [, var_name = expr] ...
```

示例 :

```mysql
DELIMITER $

CREATE  PROCEDURE pro_test2()
BEGIN
 	DECLARE NAME VARCHAR(20);
  SET NAME = 'MYSQL';
  SELECT NAME ;
END$

DELIMITER ;
```

<div align = center><img src="../图片/Mysql_12.png" width="800px" /></div>

也可以通过select ... into 方式进行赋值操作 :

```mysql
DELIMITER $

CREATE  PROCEDURE pro_test3()
BEGIN
	declare  countnum int;
	select count(*) into countnum from city;
	select countnum;
END$

DELIMITER ;
```

<div align = center><img src="../图片/Mysql_14.png" width="800px" /></div>

### 2.if条件判断

语法结构 :

```mysql
if search_condition then statement_list

	[elseif search_condition then statement_list] ...
	
	[else statement_list]

end if;
```

需求：

```
根据定义的身高变量，判定当前身高的所属的身材类型 
	180 及以上 ----------> 身材高挑
	170 - 180  ---------> 标准身材
	170 以下  ----------> 一般身材
```

示例 :

```mysql
delimiter $

create procedure pro_test6()
begin
  declare  height  int  default  175; 
  declare  description  varchar(50);
  
  if  height >= 180  then
    set description = '身材高挑';
  elseif height >= 170 and height < 180  then
    set description = '标准身材';
  else
    set description = '一般身材';
  end if;
  
  select description ;
end$

delimiter ;
```

调用结果为 :

<div align = center><img src="../图片/Mysql_15.png" width="800px" /></div>

### 3.传递参数

语法格式 :

```
create procedure procedure_name([in/out/inout] 参数名   参数类型)
...
IN :   该参数可以作为输入，也就是需要调用方传入值 , 默认
OUT:   该参数作为输出，也就是该参数可以作为返回值
INOUT: 既可以作为输入参数，也可以作为输出参数
```

#### IN

需求 :

```
根据定义的身高变量，判定当前身高的所属的身材类型 
```

示例 :

```mysql
delimiter $

create procedure pro_test5(in height int)
begin
    declare description varchar(50) default '';
  if height >= 180 then
    set description='身材高挑';
  elseif height >= 170 and height < 180 then
    set description='标准身材';
  else
    set description='一般身材';
  end if;
  select concat('身高 ', height , '对应的身材类型为:',description);
end$

delimiter ;
```

<div align = center><img src="../图片/Mysql_16.png" width="800px" /></div>

**OUT-输出**

需求 :

```
根据传入的身高变量，获取当前身高的所属的身材类型  
```

示例:

```mysql
create procedure pro_test7(in height int , out description varchar(100))
begin
  if height >= 180 then
    set description='身材高挑';
  elseif height >= 170 and height < 180 then
    set description='标准身材';
  else
    set description='一般身材';
  end if;
end$	
```

调用:

```mysql
call pro_test5(168, @description)$
select @description$
```

<div align = center><img src="../图片/Mysql_17.png" width="800px" /></div>

> @description : 这种变量要在变量名称前面加上“@”符号，叫做用户会话变量，代表整个会话过程他都是有作用的，这个类似于全局变量一样。
>
> @@global.sort_buffer_size : 这种在变量前加上 "@@" 符号, 叫做 系统变量

### 4.case结构

语法结构 :

```mysql
-- 方式一 : 
CASE case_value

  WHEN when_value THEN statement_list
  
  [WHEN when_value THEN statement_list] ...
  
  [ELSE statement_list]
  
END CASE;


-- 方式二 : 
CASE

  WHEN search_condition THEN statement_list
  
  [WHEN search_condition THEN statement_list] ...
  
  [ELSE statement_list]
  
END CASE;
```

需求:

```
给定一个月份, 然后计算出所在的季度
```

示例 :

```mysql
delimiter $

create procedure pro_test9(month int)
begin
  declare result varchar(20);
  case 
    when month >= 1 and month <=3 then 
      set result = '第一季度';
    when month >= 4 and month <=6 then 
      set result = '第二季度';
    when month >= 7 and month <=9 then 
      set result = '第三季度';
    when month >= 10 and month <=12 then 
      set result = '第四季度';
  end case;
  select concat('您输入的月份为 :', month , ' , 该月份为 : ' , result) as content ;
end$

delimiter ;
```

### 5.while循环

语法结构:

```mysql
while search_condition do

	statement_list
	
end while;
```

需求:

```
计算从1加到n的值
```

示例 :

```mysql
delimiter $

create procedure pro_test8(n int)
begin
    declare total int default 0;
    declare num int default 1;
    while num <= n do
      	set total = total + num;
    set num = num + 1;
    end while;
    select total;
end$

delimiter ;
```

<div align = center><img src="../图片/Mysql_18.png" width="800px" /></div>

### 6.repeat结构

有条件的循环控制语句, 当满足条件的时候退出循环 。while 是满足条件才执行，repeat 是满足条件就退出循环。

语法结构 :

```mysql
REPEAT

  statement_list

  UNTIL search_condition

END REPEAT;
```

需求:

```
计算从1加到n的值
```

示例 :

```mysql
delimiter $

create procedure pro_test10(n int)
begin
  declare total int default 0;
  repeat 
    set total = total + n;
    set n = n - 1;
    until n=0  
  end repeat;
  select total ;
end$

delimiter ;
```

<div align = center><img src="../图片/Mysql_19.png" width="800px" /></div>

### 7.loop语句

LOOP 实现简单的循环，退出循环的条件需要使用其他的语句定义，通常可以使用 LEAVE 语句实现，具体语法如下：

```mysql
[begin_label:] LOOP

  statement_list

END LOOP [end_label]
```

如果不在 statement_list 中增加退出循环的语句，那么 LOOP 语句可以用来实现简单的死循环。

### 8.leave语句

用来从标注的流程构造中退出，通常和 BEGIN ... END 或者循环一起使用。下面是一个使用 LOOP 和 LEAVE 的简单例子 , 退出循环：

```mysql
delimiter $

CREATE PROCEDURE pro_test11(n int)
BEGIN
  declare total int default 0;
  ins: LOOP
    
    IF n <= 0 then
      leave ins;
    END IF;
    
    set total = total + n;
    set n = n - 1;
  	
  END LOOP ins;
  
  select total;
END$

delimiter ;
```

<div align = center><img src="../图片/Mysql_20.png" width="800px" /></div>

### 9.游标/光标

游标是用来存储查询结果集的数据类型 , 在存储过程和函数中可以使用光标对结果集进行循环的处理。光标的使用包括光标的声明、OPEN、FETCH 和 CLOSE，其语法分别如下。

声明光标：

```mysql
DECLARE cursor_name CURSOR FOR select_statement ;
```

OPEN 光标：

```mysql
OPEN cursor_name ;
```

FETCH 光标：

```mysql
FETCH cursor_name INTO var_name [, var_name] ...
```

CLOSE 光标：

```mysql
CLOSE cursor_name ;
```

示例 :

初始化脚本:

```mysql
create table emp2(
    id int(11) not null auto_increment ,
    name varchar(50) not null comment '姓名',
    age int(11) comment '年龄',
    salary int(11) comment '薪水',
    primary key(`id`)
)engine=innodb default charset=utf8 ;

insert into emp2(id,name,age,salary) values(null,'金毛狮王',55,3800),(null,'白眉鹰王',60,4000),(null,'青翼蝠王',38,2800),(null,'紫衫龙王',42,1800);
```

通过循环结构 , 获取游标中的数据 :

```mysql
DELIMITER $

create procedure pro_test12()
begin
    DECLARE id int(11);
    DECLARE name varchar(50);
    DECLARE age int(11);
    DECLARE salary int(11);
    DECLARE has_data int default 1;

    DECLARE emp_result CURSOR FOR select * from emp2;
    DECLARE EXIT HANDLER FOR NOT FOUND set has_data = 0;#声明在游标之后

    open emp_result;

    repeat
      fetch emp_result into id , name , age , salary;
      select concat('id为',id, ', name 为' ,name , ', age为 ' ,age , ', 薪水为: ', salary);
      until has_data = 0
    end repeat;

    close emp_result;
end$

DELIMITER ; 
```

---

## 七.存储函数

语法结构:

```
CREATE FUNCTION function_name([param type ... ]) 
RETURNS type 
BEGIN
	...
END;
```

案例 :

定义一个存储过程, 请求满足条件的总记录数 ;

```mysql
delimiter $

create function func1(countryId int)
returns int
begin
  declare cnum int;
  
  select count(*) into cnum from city where country_id = countryId;
  
  return cnum;
end$

delimiter ;
```

调用:

```mysql
select func1(1);
select func1(2);
```

