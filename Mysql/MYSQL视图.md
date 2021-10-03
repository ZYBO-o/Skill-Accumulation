## 一.视图概述

视图（View）是一种虚拟存在的表。它并不在数据库中实际存在，行和列数据来自定义视图的查询中使用的表，并且是在使用视图时动态生成的。

通俗的讲，视图是一条SELECT语句执行后返回的结果集。所以在创建视图时，主要工作落在创建SQL查询语句上。

视图相对于普通的表的优势主要包括以下几项：

-  **简单：** 使用视图时不需要关心后面对应的表的结构、关联条件和筛选条件，这是过滤好的复合条件的结果集。
-  **安全**： 使用视图时只能访问被允许查询的结果集，对表的权限管理并不能限制到某个行某个列，但是通过视图就可以简单的实现。
-  **数据独立：** 一旦视图的结构确定了，可以屏蔽表结构变化对用户的影响，源表增加列对视图没有影响；源表修改列名，则可以通过修改视图来解决，不会造成对访问者的影响。

---

## 二. 创建或者修改视图

创建视图的语法为：

```mysql
CREATE [OR REPLACE] [ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}]
VIEW view_name [(column_list)]
AS select_statement
[WITH [CASCADED | LOCAL] CHECK OPTION]
```

示例：

```mysql
create or replace view city_country_view 
as 
select t.*,c.country_name from country c , city t where c.country_id = t.country_id;
```

<div align = center><img src="../图片/Mysql_7.png" width="800px" /></div>

修改视图的语法为：

```mysql
ALTER [ALGORITHM = {UNDEFINED | MERGE | TEMPTABLE}]

VIEW view_name [(column_list)]

AS select_statement

[WITH [CASCADED | LOCAL] CHECK OPTION]
```

选项 : 

```mysql
WITH [CASCADED | LOCAL] CHECK OPTION 决定了是否允许更新数据使记录不再满足视图的条件。
LOCAL ： 只要满足本视图的条件就可以更新。
CASCADED ： 必须满足所有针对该视图的所有视图的条件才可以更新。 默认值.
```

## 三.查看视图

从 MySQL 5.1 版本开始，使用 SHOW TABLES 命令的时候不仅显示表的名字，同时也会显示视图的名字，而不存在单独显示视图的 SHOW VIEWS 命令。

<div align = center><img src="../图片/Mysql_11.png" width="800px" /></div>

同样，在使用 SHOW TABLE STATUS 命令的时候，不但可以显示表的信息，同时也可以显示视图的信息。

<div align = center><img src="../图片/Mysql_8.png" width="500px" /></div>

如果需要查询某个视图的定义，可以使用 SHOW CREATE VIEW 命令进行查看 ：

<div align = center><img src="../图片/Mysql_9.png" width="800px" /></div>

---

## 四.删除视图

语法 :

```mysql
DROP VIEW [IF EXISTS] view_name [, view_name] ...[RESTRICT | CASCADE]	
```

示例 , 删除视图city_country_view :

```mysql
DROP VIEW city_country_view ;
```

<div align = center><img src="../图片/Mysql_10.png" width="800px" /></div>

