MySQL_Test

> 这篇文章的主要目的就是记录一些上课的例子。以及课件上没有的一些知识点的扩展。
>
> Create Date :  2021-11-4
>
> Author : Mr.J 

# CRUD操作

> 什么是CRUD？增查改删（create, read (retrieve), update, delete）

## insert 增加

注意：

1. 因为我们设计表的时候将主键设置为了自增长的约束条件，所以当我们删除ID为1的行的时候，再插入的新元素将不会是1，除非我们手动设置，不管表中有几个元素，新插入的元素都将一往无前，直到超过范围不能插入为止。

2. 有两种插入的方式，但是效率不一样；

   ![image.png](https://i.loli.net/2021/11/05/SjDr7dxNOGM1wye.png)

# explain 

> 解释说明的意思，在SQL语句之前加上explain会解释这条SQL语句执行计划（注意这个只是大致的，不显示优化之后的）

在我们查寻数据的时候，如果查询的判断条件是主键或者唯一键，那么会有主键索引和唯一键索引，会使得查询效率很高。当查询不是唯一键的或者主键的时候，需要遍历整个表。**使用limit可以提高查询的效率**。

例子：

![image-20211105180612094.png](https://i.loli.net/2021/11/05/ZoCmWQcS8Y7r2Bd.png)

# 分页查询 

> 这个例子想要探究的问题是在该列没有索引的情况下，使用 limit 进行分页查询可以提高 查询的效率。

## 是否能提升效率？

创建一张表，用作测试：

```SQL
create table user_t(
 id int(11)  not null auto_increment,
 email varchar(255) default NULL,
 password varchar(255) default NULL,
 primary key (id));
```

然后给表装数据：利用存储过程

```sql
delimiter $

Create Procedure add_t_user(IN n INT)
begin
DECLARE i INT;
set i = 0;
while i<n DO
insert into user_t(email,password) values(concat(i+1,'@fixbug.com'),i+1);
set i=i+1;
end while;
end $

delimiter ;

call add_t_user(2000000);
```

当插入两百万行数据的时候，我们就能看到了性能的差异：

![image-20211105195554549](C:\Users\a\AppData\Roaming\Typora\typora-user-images\image-20211105195554549.png)

## 分页查询的例子

![image.png](https://i.loli.net/2021/11/05/Fu3PSvgO9l6K5Zd.png)

![image.png](https://i.loli.net/2021/11/05/jSU5zV6fiXNG8CR.png)

![image-20211105202437112](C:\Users\a\AppData\Roaming\Typora\typora-user-images\image-20211105202437112.png)

# 存储过程

procedure 

## 链接

(https://www.runoob.com/w3cnote/mysql-stored-procedure.html)

存储过程是为了完成特定功能的SQL语句集，用户可通过指定存储过程的名字并给定参数(需要时)来调用执行。相当于C++中的函数。

声明存储过程:

```sql
CREATE PROCEDURE demo_in_parameter(IN p_in int)       
```

存储过程开始和结束符号:

```SQL
BEGIN .... END    
```

变量赋值:

```sql
SET @p_in=1  
```

变量定义:

```sql
DECLARE l_int int unsigned default 4000000; 
```

创建mysql存储过程、存储函数:

```sql
create procedure 存储过程名(参数)
```

存储过程体:

```sql
create function 存储函数名(参数)
```



# group by 分组

group by分组是将给定的列相同的值分成一组的操作。GROUP BY语句与HAVING语句的使用：https://www.cnblogs.com/geogre123/p/11177204.html

**因为聚合函数通过作用于一组数据而只返回一个单个值，因此，在SELECT语句中出现的元素要么为一个聚合函数的输入值，要么为GROUP BY语句的参数，否则会出错。**

```sql
select * from user;
+----+----------+-----+--------+
| ID | nickname | age | sex    |
+----+----------+-----+--------+
|  6 | jiangyi  |  19 | male   |
|  7 | jianger  |  20 | male   |
|  8 | jiangsan |  19 | male   |
|  9 | jiangsi  |  20 | male   |
| 10 | jiangwu  |  22 | female |
+----+----------+-----+--------+
5 rows in set (0.00 sec)

#只按照一列来进行分组
select age ,count(*) count from user group by age;
+-----+-------+
| age | count |
+-----+-------+
|  19 |     2 |
|  20 |     2 |
|  22 |     1 |
+-----+-------+
3 rows in set (0.00 sec)

#按照两列来进行分组 age和sex都相同的分在一个组中
select age ,sex,count(*) count from user group by sex ,age;
+-----+--------+-------+
| age | sex    | count |
+-----+--------+-------+
|  19 | male   |     2 |
|  20 | male   |     2 |
|  22 | female |     1 |
+-----+--------+-------+
3 rows in set (0.00 sec)


```



# 连接查询

## 内连接查询

```SQL
CREATE TABLE student(
 uid INT unsigned PRIMARY KEY NOT NULL AUTO_INCREMENT,
 name VARCHAR(50) NOT NULL,
 age TINYINT NOT NULL DEFAULT 18,
 sex ENUM('M','W') NOT NULL DEFAULT 'M');
 
 CREATE TABLE course(
  cid INT unsigned PRIMARY KEY NOT NULL AUTO_INCREMENT,
  cname VARCHAR(50) NOT NULL,
  credit TINYINT UNSIGNED NOT NULL );

CREATE TABLE exame(
 uid INT UNSIGNED  NOT NULL ,
 cid INT UNSIGNED  NOT NULL ,
 time DATE NOT NULL,
 score FLOAT NOT NULL,
 PRIMARY KEY (uid,cid) );
 
 insert into student (name, age, sex)values
 ( ' zhangsan' , 18,'M') ,( 'gaoyang ' , 20,'w' ),( 'chenwei', 22,'M') ,( ' linfeng', 21,'w' ) ,( 'liuxiang' , 19,'w');
insert into course (cname , credit) values('C++基础课程',5),('C++高级课程',10),( 'C++项目开发',8),( 'C++算法课程',12);
insert into exame (uid,cid,time, score) values( 1,1,'2021-04-09', 99.0) ,( 1,2,'2021-04-10', 80.0) ,(2,2,'2021-04-10', 90.0),(2,3,'2021-04-12',85.0),(3,1, '2021-04-09', 56.0) ,( 3,2,'2021-04-10', 93.0),(3,3,'2021-04-12', 89.0),( 3,4,'2021-04-11', 100.0),(4,4,'2021-04-11', 99.0),(5,2,'2021-04-10',59.0) ,(5,3,'2021-04-12', 94.0) ,(5,4, '2021-04-11', 95.0);
```

**使用内连接，会区分大表和小表（按照数量来区分，小表永远是整表扫描,然后去大表搜索）**

**从student表（小表，因为数据少 ）中取出所有的a.uid，然后拿着这些uid去大表中搜索**  

> 为了验证这些，我们可以使用explain来查看select语句的执行计划：
>
> 首先看所有表中的数据：
>
> ```sql
> mysql> select * from student;
> +-----+----------+-----+-----+
> | uid | name     | age | sex |
> +-----+----------+-----+-----+
> |   1 | zhangsan |  18 | M   |
> |   2 | gaoyang  |  20 | W   |
> |   3 | chenwei  |  22 | M   |
> |   4 |  linfeng |  21 | W   |
> |   5 | liuxiang |  19 | W   |
> +-----+----------+-----+-----+
> 5 rows in set (0.11 sec)
> 
> mysql> select * from course;
> +-----+-----------------+--------+
> | cid | cname           | credit |
> +-----+-----------------+--------+
> |   1 | C++基础课程     |      5 |
> |   2 | C++高级课程     |     10 |
> |   3 | C++项目开发     |      8 |
> |   4 | C++算法课程     |     12 |
> +-----+-----------------+--------+
> 4 rows in set (0.05 sec)
> 
> mysql> select * from exame;
> +-----+-----+------------+-------+
> | uid | cid | time       | score |
> +-----+-----+------------+-------+
> |   1 |   1 | 2021-04-09 |    99 |
> |   1 |   2 | 2021-04-10 |    80 |
> |   2 |   2 | 2021-04-10 |    90 |
> |   2 |   3 | 2021-04-12 |    85 |
> |   3 |   1 | 2021-04-09 |    56 |
> |   3 |   2 | 2021-04-10 |    93 |
> |   3 |   3 | 2021-04-12 |    89 |
> |   3 |   4 | 2021-04-11 |   100 |
> |   4 |   4 | 2021-04-11 |    99 |
> |   5 |   2 | 2021-04-10 |    59 |
> |   5 |   3 | 2021-04-12 |    94 |
> |   5 |   4 | 2021-04-11 |    95 |
> +-----+-----+------------+-------+
> 12 rows in set (0.10 sec)
> ```
>
> 如果我们想要student和exame表的连接查询，因为student数据小，所以student是小表：我们来看一下具体的sql语句的执行计划。
>
> ![image.png](https://i.loli.net/2021/11/10/h2G4UuZ7DHSxnIs.png)
>
> 可以看到，是先查询的a表，然后在查看的c表。而且，a表中采用的是ALL查询，全表扫描，c表使用的是primary索引。与a.uid进行比较。
>
> 但是**如果我们加上where之后，那么会首先处理where子句中的查询，然后再判断谁是大小表。**来看一个例子：
>
> ![image.png](https://i.loli.net/2021/11/10/4WcFZMlHa2V6A1s.png)
>
> 从上面这张图我们可以看到，首先对c表进行全表扫描，然后拿着结果对a表进行搜索（可以使用索引）。

```sql
//查看张三同学的C++课程的成绩 
select a.uid,a.name,a.age,a.sex from student a where uid=1;
select c.score from exame c where c.uid=1 and c.cid=2;

select a.uid,a.name,a.age,a.sex,c.score from student a
inner join exame c on a.uid=c.uid where cid=1 and cid=2;
+-----+-----------+-----+-----+-------+
| uid | name      | age | sex | score |
+-----+-----------+-----+-----+-------+
|   1 |  zhangsan |  18 | M   |    80 |
+-----+-----------+-----+-----+-------+
1 row in set (0.00 sec)

select a.uid,a.name,a.age,a.sex,b.cname,b.cid,c.score
from exame c
inner join student a on a.uid=c.uid
inner join course b on b.cid=c.cid
where c.uid=1 amd c.cid=2;
+-----+-----------+-----+-----+-----------------+-----+-------+
| uid | name      | age | sex | cname           | cid | score |
+-----+-----------+-----+-----+-----------------+-----+-------+
|   1 |  zhangsan |  18 | M   | C++高级课程     |   2 |    80 |
+-----+-----------+-----+-----+-----------------+-----+-------+
1 row in set (0.00 sec)

//如果只给出名字和课程名字 如：名字为zhangsan，课程名字为'C++高级课程' 可以使用in语句。
select a.uid,a.name,a.age,a.sex,b.cname,b.cid,c.score
from exame c
inner join student a on a.uid=c.uid
inner join course b on b.cid=c.cid
where c.uid in(select uid from student where name='zhangsan') and c.cid in (select cid from course where cname='C++高级课程');
+-----+----------+-----+-----+-----------------+-----+-------+
| uid | name     | age | sex | cname           | cid | score |
+-----+----------+-----+-----+-----------------+-----+-------+
|   1 | zhangsan |  18 | M   | C++高级课程     |   2 |    80 |
+-----+----------+-----+-----+-----------------+-----+-------+
1 row in set (0.00 sec)

```

**总结：两个表连接的时候，先后顺序无所谓。三表连接的时候，把与两个表都有关系的表（如exame）与替他两张表进行连接。**

查询c++高级这门课有多少人选择。

> 首先，关与这个问题，想要得到问题的答案有很多中方法。

```sql
select count(c.uid) from exame c
inner join course b on b.cid=c.cid
where c.cid=2;

+--------------+
| count(c.uid) |
+--------------+
|            4 |
+--------------+
1 row in set (0.00 sec)


//这种写法应该是更好的
 select b.cid,b.cname,b.credit,count(*) 
 from exame c 
 inner join course b on c.cid=b.cid 
 group by c.cid;
+-----+-----------------+--------+----------+
| cid | cname           | credit | count(*) |
+-----+-----------------+--------+----------+
|   1 | C++基础课程     |      5 |        2 |
|   2 | C++高级课程     |     10 |        4 |
|   3 | C++项目开发     |      8 |        3 |
|   4 | C++算法课程     |     12 |        3 |
+-----+-----------------+--------+----------+
4 rows in set (0.00 sec)

如果加上下面这一句：
 having c.cid=2; 得到的结果是下面的;
 +-----+-----------------+--------+----------+
| cid | cname           | credit | count(*) |
+-----+-----------------+--------+----------+
|   2 | C++高级课程     |     10 |        4 |
+-----+-----------------+--------+----------+
1 row in set (0.00 sec)

练习：
1.查找每门课的最高分的学生信息，每门课的平均分+课程信息
2.查找每个人的平均分；
3.查找每门课的平均分；
4.寻找C++高级课程的截断平均分 （去掉最高分和最低分）

```

```sql
1.查找每门课的最高分的学生信息，每门课的平均分+课程信息
select a.uid,a.name,a.sex,b.cid,b.cname,max(c.score)
from exame c 
inner join student a on a.uid=c.uid
inner join course b on b.cid=c.cid
group by c.cid;

select c.uid,b.cname,c.cid,max(c.score) from exame c
inner join course b on b.cid=c.cid 
group by c.cid;


//思考:为什么这种写法是错误的？
因为使用group by之后，我们就不能使用非聚合列，这里group by是依赖列cid，所以select子句中不能出现除了cid以及聚合函数之外的其他列，出现就会报错。由于a.name,a.sex,b.cid,b.name要根据c.uid和c.cid才能确定，所以我们不能使用。

//注意上面三种写法都是错误的，下面的这种才是对的
select a.uid,a.name,a.sex,b.cid,b.cname,c.score
from exame c 
inner join student a on a.uid=c.uid
inner join course b on b.cid=c.cid
where c.score in (
select max(score) from exame
group by cid);
+-----+----------+-----+-----+-----------------+-------+
| uid | name     | sex | cid | cname           | score |
+-----+----------+-----+-----+-----------------+-------+
|   1 | zhangsan | M   |   1 | C++基础课程     |    99 |
|   3 | chenwei  | M   |   2 | C++高级课程     |    93 |
|   3 | chenwei  | M   |   4 | C++算法课程     |   100 |
|   4 |  linfeng | W   |   4 | C++算法课程     |    99 |
|   5 | liuxiang | W   |   3 | C++项目开发     |    94 |
+-----+----------+-----+-----+-----------------+-------+
5 rows in set (0.02 sec)
```

```sql
2.查找每个人的平均分；
select a.uid,a.name,a.sex,avg(c.score)
from exame c 
inner join student a on a.uid=c.uid
group by c.uid;
+-----+----------+-----+-------------------+
| uid | name     | sex | avg(c.score)      |
+-----+----------+-----+-------------------+
|   1 | zhangsan | M   |              89.5 |
|   2 | gaoyang  | W   |              87.5 |
|   3 | chenwei  | M   |              84.5 |
|   4 |  linfeng | W   |                99 |
|   5 | liuxiang | W   | 82.66666666666667 |
+-----+----------+-----+-------------------+
5 rows in set (0.01 sec)

思考：为什么这个可以写非聚合列的名字？
因为这些非聚合列的名字都是a表中的。 group by所依赖的列名是uid，这个刚好可以与a表连接查询。
```

```sql
3.查找每门课的平均分
select b.cid,b.cname,avg(c.score) from exame c
inner join course b on b.cid=c.cid
group by c.cid;
+-----+-----------------+-------------------+
| cid | cname           | avg(c.score)      |
+-----+-----------------+-------------------+
|   1 | C++基础课程     |              77.5 |
|   2 | C++高级课程     |              80.5 |
|   3 | C++项目开发     | 89.33333333333333 |
|   4 | C++算法课程     |                98 |
+-----+-----------------+-------------------+
4 rows in set (0.01 sec)

```

```sql
4.寻找C++高级课程的截断平均分 （去掉最高分和最低分）
 首先，我们如果求C++高级课程的平均分该这样：
 select c.cid,b.cname,avg(score) as avg_score from exame c
 inner join course b on b.cid=c.cid
 group by c.cid
 having b.cname='C++高级课程';
 +-----+-----------------+-----------+
| cid | cname           | avg_score |
+-----+-----------------+-----------+
|   2 | C++高级课程     |      80.5 |
+-----+-----------------+-----------+
1 row in set (0.00 sec)
然后，在这个基础上，我们可以去掉最高分和最低分，
 select c.cid,b.cname,(sum(score)-min(score)-max(score))/(count(score)-2) as avg_score from exame c
 inner join course b on b.cid=c.cid
 group by c.cid
 having b.cname='C++高级课程';
 +-----+-----------------+-----------+
| cid | cname           | avg_score |
+-----+-----------------+-----------+
|   2 | C++高级课程     |        85 |
+-----+-----------------+-----------+
1 row in set (0.00 sec)
另一种解法：
select b.cid,b.cname,(sum(score)-min(score)-max(score))/(count(score)-2) as avg_score from exame c
 inner join course b on b.cid=c.cid
 where b.cname='C++高级课程';
但是这样会出错：
ERROR 1140 (42000): In aggregated query without GROUP BY, expression #1 of SELECT list contains nonaggregated column 'test.c.cid'; this is incompatible with sql_mode=only_full_group_by
这句话的意思是：在不带GROUP BY的聚合查询中，SELECT列表的表达式#1包含非聚合列test. c.cid; 此参数与sql_mode=only_full_group_by不兼容 
所以我们把b.cid去掉，就得到了正确的结果：
select b.cname,(sum(score)-min(score)-max(score))/(count(score)-2) as avg_score from exame c  inner join course b on b.cid=c.cid  where b.cname='C++高级课程';
+-----------------+-----------+
| cname           | avg_score |
+-----------------+-----------+
| C++高级课程     |        85 |
+-----------------+-----------+
1 row in set (0.00 sec)

```





## 外连接查询

```sql
select a.uid,a.age,c.score from student a
left join exame c on a.uid=c.uid;

+-----+------+-------+
| uid | age  | score |
+-----+------+-------+
|   1 |   18 |    99 |
|   1 |   18 |    80 |
|   2 |   20 |    90 |
|   2 |   20 |    85 |
|   3 |   22 |    56 |
|   3 |   22 |    93 |
|   3 |   22 |    89 |
|   3 |   22 |   100 |
|   4 |   21 |    99 |
|   5 |   19 |    59 |
|   5 |   19 |    94 |
|   5 |   19 |    95 |
|   6 | NULL |  NULL |    //即使为空也要显示
+-----+------+-------+
13 rows in set (0.00 sec)

```

```sql
select a.uid,a.age,c.score from exame c
right join student a on a.uid=c.uid;
和上面的执行结果一样 
```

一个易错点：

**过滤条件不能乱写**:

> 在内连接中分大小表，但是外连接中并不分表是大表还是小表，左连接的时候，左表是整表扫描。右连接的时候，右表是整表扫描。接下来我们通过一个例子来看where子句的错误使用可能会导致外连接和内连接的执行结果是一样的。
>
> 例子：找出没有选修过cid=3这门课的学生的信息。

```sql
首先，我先给出正确的写法：
select a.* ,c.cid from student a 
left join exame c on a.uid=c.uid and c.cid=3 where b.cid is null;
+-----+----------+------+------+------+
| uid | name     | age  | sex  | cid  |
+-----+----------+------+------+------+
|   1 | zhangsan |   18 | M    | NULL |
|   4 |  linfeng |   21 | W    | NULL |
|   6 | jiang    | NULL | NULL | NULL |
+-----+----------+------+------+------+
3 rows in set (0.09 sec)

好了错误的写法要来了
select a.*,c.cid from student a
left join exame c on a.uid=c.uid where c.cid=3 and b.cid is null;
Empty set (0.00 sec)
为什么会出现空呢？不妨先看一下下面的写法就明白了
select a.*,c.cid from student a
left join exame c on a.uid=c.uid where c.cid=3;
这个我们预想的是，a表中的每一行都会出现，因为是左连接。但是这种写法的执行结果和下面这种内连接的写法的执行结果是一样的；
select a.*,c.cid from student a
inner join exame c on a.uid=c.uid where c.cid=3;
+-----+----------+------+------+------+
| uid | name     | age  | sex  | cid  |
+-----+----------+------+------+------+
|   2 | gaoyang  |   20 | W    |    3 |
|   3 | chenwei  |   22 | M    |    3 |
|   5 | liuxiang |   19 | W    |    3 |
+-----+----------+------+------+------+
3 rows in set (0.00 sec)
并不是我们想要的执行结果，但是这种结果是怎么出现的呢？我们可以看一下执行过程
```

![image.png](https://i.loli.net/2021/11/10/ahqPi6rX5t4SweV.png)

这两种方法的执行过程也是一样的，所以我们就可以知道，这种外连接的写法是错误的，如果要把帅选的条件放到where（cid=3）之后，会先对c表进行筛选然后再对a表进行扫描，这就导致和内连接执行过程是一样的，那可定和内连接的执行结果也一样啊。

下面这个图是正确的写法的执行计划，我们可以看到是先扫描a表再扫描c表。

![image.png](https://i.loli.net/2021/11/10/lQi3j6SdwtGh4fF.png)

**总结：内连接过滤条件放到on后面和where子句后面都一样，无影响。外连接的过滤条件要放到on之后。**

# MySQL存储引擎

## 1.说一下mysql的存储引擎

存储引擎影响数据的存储方式（表结构，数据，索引）

主要使用的存储引擎就是MyISAM和InnoDB。

MyISAM将表结构，数据和索引分开存储，存储在三个文件中，而InnoDB 使用两个文件存储，数据和索引放在一块存储。

5.7之上 的MySQL默认为InnoDB存储引擎

MyISAM支持表锁，InnoDB支持行锁，InnoDB支持事务，MyISAM不支持。

# MySQL索引

## 索引的使用

```sql
create index nameidx on student(name);   #建立索引语句
mysql> create index nameidx on student(name);
Query OK, 0 rows affected (1.08 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> show create table student;
| student | CREATE TABLE `student` (
  `uid` int unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `age` tinyint unsigned DEFAULT NULL,
  `sex` enum('M','W') DEFAULT NULL,
  PRIMARY KEY (`uid`),
  KEY `nameidx` (`name`)     #MySQL中key可以表示约束，也表示索引
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |
1 row in set (0.00 sec)

```

先面我么来看效率得到了提升，只查询了一行

```sql
explain select * from student where name='zhangsan';
mysql> explain select * from student where name='zhangsan';
| id | select_type | table  | partitions| type| possible_keys| key     | key_len| ref  | rows | filtered | Extra |
|  1 | SIMPLE      | student| NULL      | ref | nameidx      | nameidx | 202    | const|   1  |   100.00 | NULL  |
1 row in set, 1 warning (0.00 sec)
```

**删除索引**

```mysql
drop index nameidx on student;

mysql> drop index nameidx on student;
Query OK, 0 rows affected (0.23 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> explain select * from student where name='zhangsan';
| id | select_type| table |partitions| type| possible_keys | key | key_len| ref| rows| filtered | Extra       |
|  1 | SIMPLE     |student| NULL     | ALL | NULL          | NULL| NULL   |NULL|   6 |   16.67  | Using where |
1 row in set, 1 warning (0.00 sec)
#删除索引之后就查询了6行
```

## 第36次课

**1.为什么不用AVL树？**

> AVL树相比于B树来说磁盘IO次数比较多，比如说，我们有10000000（一千万）行数据，使用AVL树最差的情况下需要的磁盘IO次数为log2（10000000）=log(10000000) / log(2) = 24次。但是加入使用B树，就只有log2(10000000) = log(10000000) / log(500) = 3次。所以差别就出来了。

**2.使用B树是为了减少磁盘IO而不是查询次数**

> 查询次数是不变的，减少的是从硬盘向内存拷贝数据的次数。

**3.执行的顺序**

> select语句 -> 有索引 -> 存储引擎 -> os内核 -> 磁盘io（读索引文件） ->  内存 -> 建立B树加速搜索 

**4.data是表示的数据本身还是数据的地址**

> 根据不同的存储引擎，MyISAM由于是将索引文件和数据文件分开存放，所以data表示的是数据的地址。
>
> InnoDB表示的是数据的本身。

## 第37次课

> 这节课主要讲了B+树索引，以及与B-树索引相比它的优点。

**1.为什么MySQL存储引擎（MyISAM和InnoDB）使用B+树来建立索引而不是B树？**

> ①B树每个节点中都存有数据，所以导致每个节点查询时间不一样，离根节点近的节点就容易找到。
>
> ②B树由于每个节点中都有数据，所以与B+树相比相同的节点大小所能存储的key值就少，这就导致树高增大
>
> ③B树不适合做范围查询

三点

**2.自己怎么回答MySQL索引的底层原理？**

> 当我们使用 `select * from student where name=‘zhangsan’;`  where子句进行查询的时候，MySQL server会首先看有没有索引，如果没有的话，会进行整表搜索。如果有索引的话，会把索引文件加载到内存，用B+树进行构建。这样会有最少的磁盘IO。

## 第38次课

> 这节课主要学习了InnoDB存储引擎的主键索引和二级索引（辅助索引），并且了解了回表名词的概念以及不同场景下如何消除回表以及如何优化sql。除此之外，还实操了查询优化的操作。

**1.回表**

> 回表简单来说就是我们用辅助索引来搜索不是key的列，就会首先从辅助索引树中找到主键，然后用主键索引树进行查找我们想要查找的列。 在我们使用explain查看执行计划的时候，最后一项Extra如果是using index就说明我们利用辅助索引树就查找到了我们想要的那一列。在优化的时候，我们要尽量避免有回表的操作。

**2.如何避免回表**

> 1.使用多列索引可以避免回表
>
> 2.在我们使用select的时候，尽量只查询我们业务所需要的列，不要用select *；

**3.主键索引树和辅助索引树**

![image.png](https://i.loli.net/2021/11/15/2H6BYxLibd45yg9.png)

**练习1：是否用到索引**

```sql
mysql> explain select * from student;   #用不到索引，因为这里是整表遍历
| id | select_type| table | partitions| type | possible_keys | key  | key_len | ref  | rows | filtered | Extra |
|  1 | SIMPLE     |student| NULL      | ALL  | NULL          | NULL | NULL    | NULL |    6 |   100.00 | NULL  |
1 row in set, 1 warning (0.00 sec)

mysql> explain select * from student where uid=5;
| id |select_type| table | partitions | type  | possible_keys | key     | key_len | ref  | rows| filtered | Extra |
|  1 | SIMPLE    |student| NULL       | const | PRIMARY       | PRIMARY | 4       | const|   1 |   100.00 | NULL  |
1 row in set, 1 warning (0.10 sec)

mysql> explain select * from student where uid<5;
| id | select_type| table | partitions| type | possible_keys | key    | key_len| ref |rows| filtered| Extra      |
|  1 | SIMPLE     |student| NULL      | range| PRIMARY       | PRIMARY| 4      | NULL|  4 |  100.00 | Using where|
1 row in set, 1 warning (0.10 sec)

mysql> explain select * from student where uid between 3 and 5;
| id | select_type| table | partitions| type|possible_keys| key   | key_len| ref | rows|filtered | Extra       |
|  1 | SIMPLE     |student| NULL      |range| PRIMARY     |PRIMARY| 4      | NULL|   3 |  100.00 | Using where |
1 row in set, 1 warning (0.00 sec)
```

**练习2：多列索引的使用提高查询效率**

首先，这个时候student表是没有辅助索引的，我们要查询age=20的学生的信息，并且按照他们的名字首字母进行排序。这时候用到了外排序，这是非常糟糕的。

```sql
mysql> explain select * from student where age=20 order by name;
|id|select_type| table | partitions|type|possible_keys|key |key_len|ref|rows|filtered|        Extra              |
|1 | SIMPLE   |student| NULL       | ALL| NULL        |NULL|NULL   |NULL| 6 |  16.67 |Using where;Using filesort |
1 row in set, 1 warning (0.00 sec)
```

我们尝试使用age列建索引，卡能不能解决：

```sql
mysql> explain select * from student where age=20 order by name;
| id|select_type| table |partitions| type | possible_keys| key  | key_len| ref  | rows| filtered | Extra          |
|  1| SIMPLE    |student| NULL     | ref  | ageidx       |ageidx| 2      | const |   2 |   100.00 | Using filesort |
1 row in set, 1 warning (0.00 sec)
```

虽然用到了索引，但是还是用了外排序。

为什么这里不显示using index？因为只有不回表的时候才会显示，这里显然回表了。

那我们再给name建立一个索引可以吗？mysql只能使用一个索引，所以也不行。那只能使用多列索引了。

```sql
mysql> create index age_name_idx on student(age,name);  #建立多列索引
mysql> explain select * from student where age=20 order by name;
| id | select_type | table  | partitions | type | possible_keys | key        |key_len| ref |rows| filtered | Extra|
|  1 | SIMPLE      | student| NULL       | ref  | age_name_idx  |age_name_idx| 2     |const|  2 |  100.00  | NULL |
1 row in set, 1 warning (0.00 sec)

mysql> explain select uid,age,name from student where age=20 order by name;
| id|select_type| table  | partitions|type| possible_keys| key         |key_len| ref | rows |filtered| Extra    |
|  1| SIMPLE    | student| NULL      | ref| age_name_idx | age_name_idx| 2     |const|    2 | 100.00 |Using index |
1 row in set, 1 warning (0.00 sec)
```

第二种方式就显示了using index 因为没有涉及回表的操作。



## 第39次课

这节课主要介绍了聚集索引和非聚集索引。什么是聚集索引呢？简单来说就是MyISAM存储引擎将数据和索引分开存放，索引中存储的都是数据的地址，这就叫非聚集索引。而InnoDB存储引擎使用将索引和数据放到一起，这就叫聚集索引。



##  第40次课 

> 这节课主要介绍了哈希索引的结构，以及它的优缺点。

**为什么不建议使用哈希索引呢？**

> 1.哈希索引只能处理内存上的数据，如果处理磁盘上的数据的话，会花费太多的磁盘IO。这是因为我们通过哈希函数可以将两个相邻的数据分到不同的地方，每处理一个节点都要花费一次磁盘IO。
>
> 2.只能做等值查询，不能做范围查询和模糊查询。

哈希索引的优点：时间复杂度为O(1)的查询

## 第41次课

> 这节课主要讲述了自适应哈希索引。
>
> 参考资料：https://www.cnblogs.com/geaozhang/p/7252389.html

这节课主要明白以下几个问题就行了：

自适应哈希索引是什么？什么情况下使用？

> InnoDB会自动监测对二级索引的查找，如果发现二级索引被频繁使用（三次之上）就会认为是热数据，自动建立哈希索引，进而来提高查询的效率。但是一般不建议使用（关掉），因为自适应哈希索引也会占用内存空间。

## 第42次课

主要学习了漫画课，引入一个问题：

**当我们被问到SQL和索引的优化的问题时候，该怎么切入？**

> 使用explain来分析sql的执行过程，看我们想要查询的列是否建立了索引，如果没有建立索引可以考虑建立索引。如果已经建立了索引看是否使用到了索引，还有就是order by子句看是否需要建立联合索引。但是实际的工作中可能有上万条sql，我们不可能每一个用explain去分析吧。我们可以使用慢查询日志来获取执行时间较长的sql，然后再用explain进行分析。（见下）



## 第43次课

> 这节课主要讲述了**慢查询日志**；

**如何使用慢查询日志来优化sql？**（上一个问题的答案）

1. 打开慢查询开关，设置合理的业务可以接受的慢查询时间 slow_query_log
2. 压测执行各种业务
3. 查看慢查询日志，获取执行耗时的sql语句
4. 用explain来分析所获得的sql语句
5. 举例子

**使用的具体例子**

select * from user_t where password=1600000；

```MySQL
#首先，我们设置全局变量slow_query_log=on,然后设置业务可以接受的时间slow_query_log
mysql> set global slow_query_log=on;
Query OK, 0 rows affected (0.31 sec)

mysql> show variables like 'slow_query%';
+---------------------+---------------------------------------------+
| Variable_name       | Value                                       |
+---------------------+---------------------------------------------+
| slow_query_log      | ON                                          |
| slow_query_log_file | /var/lib/mysql/jqw-virtual-machine-slow.log |
+---------------------+---------------------------------------------+
2 rows in set (0.00 sec)

mysql> show variables like 'long%';
+-----------------+-----------+
| Variable_name   | Value     |
+-----------------+-----------+
| long_query_time | 10.000000 |
+-----------------+-----------+
1 row in set (0.00 sec)

#当时间为10的时候，我们执行 Sql语句-select * from user_t where password=1600000；
mysql>  select * from user_t where password=1600000;
+---------+--------------------+----------+
| id      | email              | password |
+---------+--------------------+----------+
| 1600000 | 1600000@fixbug.com | 1600000  |
+---------+--------------------+----------+
1 row in set (0.41 sec)
#这时候我们查询慢查询日志，发现并没有记录，因为这条sql语句执行的时间没有到10s
root@jqw-virtual-machine:/var/lib/mysql# cat /var/lib/mysql/jqw-virtual-machine-slow.log
/usr/sbin/mysqld, Version: 8.0.27-0ubuntu0.20.04.1 ((Ubuntu)). started with:
Tcp port: 3306  Unix socket: /var/run/mysqld/mysqld.sock
Time                 Id Command    Argument
#修改变量 long_query_time=0.1s
mysql> set long_query_time=0.1;
Query OK, 0 rows affected (0.00 sec)
#再次执行sql语句
mysql>  select * from user_t where password=1600000;
+---------+--------------------+----------+
| id      | email              | password |
+---------+--------------------+----------+
| 1600000 | 1600000@fixbug.com | 1600000  |
+---------+--------------------+----------+
1 row in set (0.45 sec)

mysql>  select * from user_t where password=1700000;
+---------+--------------------+----------+
| id      | email              | password |
+---------+--------------------+----------+
| 1700000 | 1700000@fixbug.com | 1700000  |
+---------+--------------------+----------+
1 row in set (0.44 sec)
#这时候发现慢查询日志中有了记录
/usr/sbin/mysqld, Version: 8.0.27-0ubuntu0.20.04.1 ((Ubuntu)). started with:
  2 Tcp port: 3306  Unix socket: /var/run/mysqld/mysqld.sock
  3 Time                 Id Command    Argument
  4 # Time: 2021-11-16T10:26:40.612659Z
  5 # User@Host: root[root] @ localhost []  Id:    11
  6 # Query_time: 0.447179  Lock_time: 0.000189 Rows_sent: 1  Rows_examined: 2000000
  7 use test;
  8 SET timestamp=1637058400;
  9 select * from user_t where password=1600000;    #第一条
 10 # Time: 2021-11-16T10:27:48.952428Z
 11 # User@Host: root[root] @ localhost []  Id:    11
 12 # Query_time: 0.436810  Lock_time: 0.000237 Rows_sent: 1  Rows_examined: 2000000
 13 SET timestamp=1637058468;
 14 select * from user_t where password=1700000;   #第二条

#这样我们就找到了目标sql
```

这样我们就可以给password列加上索引；



# MySQL事务

## 第44次课、事务的核心概念

![image.png](https://i.loli.net/2021/11/17/u2ljcmp7Wz8wVeE.png)

事务只有三种状态：begin（开启事务），commit（提交事务），rollback（回滚事务）





## 第45次课 事物的ACID特性

## 第46次课 事物的隔离级别

关于REPEATABLE-READ：可以解决insert，delete的 问题，但是解决不了插入之后数据再update的问题。



# MySQL锁

## 第49次课 表级锁和行级锁 and  第50次课 排它锁和共享锁

1. InnoDB存储引擎在没有索引的情况下是使用的表级锁，有索引的时候才是使用的行级锁。
2. 在串行化隔离级别中，select加的是共享锁，增删改添加的是排它锁。

![image.png](https://i.loli.net/2021/11/18/4Qevp5jrbx9N3Cg.png)

![image.png](https://i.loli.net/2021/11/18/Plvfs2KwDmgTZy8.png)



## 第51次课 间隙锁

![image.png](https://i.loli.net/2021/11/19/VcbZmFitNTjgL3x.png)

间隙锁主要用来解决幻读问题的，这也是串行化隔离级别解决幻读的方式。

当进行范围查询的时候，会给整个范围加锁。

当进行等值查询的时候，为了防止出现幻读，也会加上间隙锁。这个间隙锁的位置就是在查询的位置的两边。

![image.png](https://i.loli.net/2021/11/19/71ITkcZGrXpN52K.png)

![image.png](https://i.loli.net/2021/11/19/Sgt5YC8s1TRMQXz.png)

**问题：隔离级别串行化是怎么实现的？**

 加锁。

## 第54-56次课 MVCC

在MVCC多版本控制中，读可以分为两类：

1. 快照读：读的是事物的可见版本，不用枷锁。
2. 当前读：读取的是记录的最新版本，insert，update，delete，select ... lock in share mode/ for update。 

1. **隔离级别为“已提交读”是怎么实现的？**

   > 每次select都会产生一次新的数据快照。也就是读取的undo log中属于本事务（通过事务id号来判断）的最新版本。
   >
   > ①**解决**了 “脏读” ：因为每次select都会产生一次新的数据快照。并且**只有**另一个事务commit了，那么当前事务才可以获得新的数据快照。所以另一个事务如果不提交，当前事务的数据快照还是原来的。这样就解决了 “脏读” 的问题。
   >
   > ②**没有解决** “不可重复读”：因为每次select都会产生一次新的数据快照。当其他事务修改数据并且提交，当前事务就会获得一个新的数据快照，这就造成了两次读取的结果不一致，即造成了 “不可重复读”。
   >
   > ②**没有解决** “ 幻读 ” ：因为如果另一个事务insert数据或者delete数据，并且在操作之后提交了。那么原来的事务就会得到一个新的数据快照，这就造成了 “幻读”。

   

2. **隔离级别为“可重复读”是怎么实现的？**

   > 每个事务只有第一次select的时候才会产生一次数据快照，而且只产生一次。
   >
   > ①**解决**了 “脏读” 问题：因为只产生一次数据快照。
   >
   > ②**解决**了 “不可重复读” 问题：第一次select产生数据快照，虽然其他事务insert了数据，但是数据块找还是原来的。
   >
   > ③**部分解决 **“幻读” 问题：没有解决update产生的幻读问题。因为如果在本事务内执行update，insert，delete，select...for update，就会变成 “当前读”，而非快照读，当前读是会获取最新的快照（**这个快照是当前事务修改的最新快照**）。比如说，如果其他事务插入数据，当前事务是没法获取的，但是如果当前事务update这条插入的数据，那么当前事务就会获得新的数据快照（包含这条新插入的数据）。在当前事务中insert也会产生新的快照（包含这个insert的数据）。

注意：**当前事务是可以看到自己更新的（增删改）数据的。这与事务ID有关（DB_TRX_ID）。**



## 第57次课 意向锁

主要讲述了意向共享锁和意向排他锁。

1. **为什么使用意向共享锁和意向排他锁？**

> 我们使用行锁的时候，如果再加表锁，那么我们要判断当前表中是不是含有行X锁or行S锁。但是如果数据有千万级别的，就很难判断。所以为了提高获取表锁的效率，我们就使用意向锁。当我们想要给某一行数据加上X锁的时候，要首先给这张表加上IX锁。同理IS锁也是这样。这样我们只需要判断这个表有没有共享锁就可以判断有没有行锁了。意向锁是InnoDB自动获取的。



## 第58次课 redo log

1. **redo log是干嘛的？**

   > redo log是重写日志。它主要用来保障MySQL中数据的持久性。当我们修改数据的时候，会先在内存中将修改的页写到redolog中，再commit之后，将内存中的redo log中的内容flash到disk中。
   >
   > - 如果再flash的过程中发生掉电等异常，就相当于这个事务没有commit成功。
   > - 如果成功flash到了disk之后（这时候内存中的Buffer Pool Cache还没有向disk中flash）发生了断电等异常，那么下一次mysqld重启之后，就会根据磁盘中的redolog中的数据flash到数据库中，来保证数据的持久性。
   >
   > 数据的持久性：事务commit之后，DBMS保证他对数据库中数据的修改是永久的，即使数据库发生故障，也应该能恢复数据。

> 注意：
>
> - redo log又称作物理日志，因为它保存的是物理页。
>
> - undo log又叫逻辑日志，记录一个操作过程，不会物理删除undo log，sql执行delete或者update操作都会记录一条undo日志想当于他保存的是sql语句。undo log本身也要记录再redo log中。

2. **undo log是干嘛的？**

   参考：https://blog.csdn.net/Weixiaohuai/article/details/117867353

   > Undo：意为撤销或取消，以撤销操作为目的，返回指定某个状态的操作。
   >
   > Undo Log：数据库事务开始之前，会将要修改的记录存放到 Undo 日志里，当事务回滚时或者数据库崩溃时，可以利用 Undo 日志，撤销未提交事务对数据库产生的影响。
   >
   >  
   >
   > Undo Log产生和销毁：undo log 在事务开始前 产生；事务在提交时，并不会立刻删除undo log，innodb会将该事务对应的 undo log 放入到删除列表中，后面会通过后台线程purge thread进行回收处理。
   >
   > Undo Log属于逻辑日志，记录一个变化过程。例如执行一个delete，undo log会记录一个insert；执行一个update，undo log会记录一个相反的update。
   >
   > Undo Log存储：undo log采用段的方式管理和记录。在innodb数据文件中包含一种rollback segment回滚段，内部包含1024个undo log segment。
   >
   > 可以通过下面一组参数来控制Undo log存储                `show variables like '%innodb_undo%';`
   >
   > **应用场景：**
   >
   > 1. 保证原子性
   > 2. 用于MVCC多版本并发控制。用undo Log来实现多版本并发控制(MVCC)。当读取的某一行被其他事务锁定时，它可以从undo log中分析出该行记录以前的数据版本是怎样的，从而让用户能够读取到当前事务操作之前的数据【快照读】。

   **undo log原理**

   ![image.png](https://i.loli.net/2021/11/20/Xkq6SbhQaULA5mt.png)



# MySQL优化

## 第60-61节课 MySQL优化的问题

![image.png](https://i.loli.net/2021/11/20/q3n1MTvuXi7NbB2.png)





# MySQL日志

日志分为四种：查询日志，错误日志，二进制日志，慢查询日志

## 查询日志

​	开启查询日志

```sql
mysql> set global general_log=on;
Query OK, 0 rows affected (0.13 sec)

mysql> show variables like 'general_log%';
+------------------+------------------------------+
| Variable_name    | Value                        |
+------------------+------------------------------+
| general_log      | ON                           |
| general_log_file | /var/lib/mysql/localhost.log |
+------------------+------------------------------+
2 rows in set (0.11 sec)
```



## 查看日志的配置：

`show variables like 'log_%'`

```mysql
mysql> show variables like 'log_%';
+----------------------------------------+---------------------+
| Variable_name                          | Value               |
+----------------------------------------+---------------------+
| log_bin                                | OFF                 |     #二进制日志
| log_bin_basename                       |                     |
| log_bin_index                          |                     |
| log_bin_trust_function_creators        | OFF                 |
| log_bin_use_v1_row_events              | OFF                 |
| log_builtin_as_identified_by_password  | OFF                 |
| log_error                              | /var/log/mysqld.log |   #错误日志
| log_error_verbosity                    | 3                   |
| log_output                             | FILE                |
| log_queries_not_using_indexes          | OFF                 |
| log_slave_updates                      | OFF                 |
| log_slow_admin_statements              | OFF                 |
| log_slow_slave_statements              | OFF                 |
| log_statements_unsafe_for_binlog       | ON                  |
| log_syslog                             | OFF                 |
| log_syslog_facility                    | daemon              |
| log_syslog_include_pid                 | ON                  |
| log_syslog_tag                         |                     |
| log_throttle_queries_not_using_indexes | 0                   |
| log_timestamps                         | UTC                 |
| log_warnings                           | 2                   |
+----------------------------------------+---------------------+
21 rows in set (0.00 sec)
```

## 二进制日志和错误日志的配置： 

打开配置文件 /etc/my.cnf

```shell
datadir=/var/lib/mysql     
socket=/var/lib/mysql/mysql.sock
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
log-error=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid

character-set-server=utf8
server-id=1           #如果这个不写无法重启
expire_logs_days=7    #过期时间是七天
log-bin=mysql-bin     #不写绝对路径就默认保存在上面设置的datadir中
```

配置完成之后我们restart一下： service mysqld restart

```mysql
mysql> show variables like 'log_%';
+----------------------------------------+--------------------------------+
| Variable_name                          | Value                          |
+----------------------------------------+--------------------------------+
| log_bin                                | ON                             |
| log_bin_basename                       | /var/lib/mysql/mysql-bin       |
| log_bin_index                          | /var/lib/mysql/mysql-bin.index |
...                         |
| log_error                              | /var/log/mysqld.log            |
...
+----------------------------------------+--------------------------------+
```

## 慢查询日志

方法一：通过设置 变量来在session级别上改变配置，只对当前session有效。

```sql
set slow_query_log=on;
set long_slow_query=1;
```

方法二：通过配置文件

1. 打开配置文件 /etc/my.cnf

2. 添加配置信息

   ```shell
   slow_query_log=1
   long_query_time=1   #慢查询时间
   ```

3. 重启mysqld：service mysqld restart

起初：

```mysql
mysql> show variables like 'slow_query_%';
+---------------------+-----------------------------------+
| Variable_name       | Value                             |
+---------------------+-----------------------------------+
| slow_query_log      | OFF                               |
| slow_query_log_file | /var/lib/mysql/localhost-slow.log |
+---------------------+-----------------------------------+
2 rows in set (0.00 sec)
mysql> show variables like 'long_query_%';
+-----------------+-----------+
| Variable_name   | Value     |
+-----------------+-----------+
| long_query_time | 10.000000 |
+-----------------+-----------+
1 row in set (0.00 sec)
```

方法二之后：

```mysql
mysql> show variables like 'long_query_%';
+-----------------+----------+
| Variable_name   | Value    |
+-----------------+----------+
| long_query_time | 1.000000 |
+-----------------+----------+
1 row in set (0.01 sec)

mysql> show variables like 'slow_query%';
+---------------------+-----------------------------------+
| Variable_name       | Value                             |
+---------------------+-----------------------------------+
| slow_query_log      | ON                                |
| slow_query_log_file | /var/lib/mysql/localhost-slow.log |
+---------------------+-----------------------------------+
2 rows in set (0.00 sec)
```

## 位置总结： 

- 查询日志： /var/lib/mysql/localhost.log
- 错误日志： /var/log/mysqld.log                         
- 慢查询日志：/var/lib/mysql/localhost-slow.log
- 二进制日志：/var/lib/mysql/mysql-bin

![image.png](https://i.loli.net/2021/11/22/GdhMIcB81wFLEqk.png)



## 二进制日志打开

```shell
mysqlbinlog --no-defaults --database=test（数据库名） --base64-output=decode-rows -v --start-datetime='2021-11-20 00:00:00'  --stop-datetime='2021-11-21 00:00:00' mysql-bin.000003 | more
```

具体操作：

1. 首先，我们用show binary logs;查看二进制日志有哪些；

2. 然后我们执行三句sql

   ```sql
   mysql> update student set age=19 where uid=1;
   Query OK, 1 row affected (0.02 sec)
   
   mysql> delete from student where name='bbb';
   Query OK, 1 row affected (0.01 sec)
   
   mysql> insert into student(name,age,sex) values('aaa',18,'W');
   Query OK, 1 row affected (0.00 sec)
   
   ```

   

3. 再次查看，发现大小发生变化

   ![image.png](https://i.loli.net/2021/11/20/W3aFztnjUbHLuMB.png)

4. 使用命令查看二进制日志，看我们的sql语句是否记录下来。

```shell
mysqlbinlog --no-defaults --database=test --base64-output=decode-rows -v --start-datetime='2021-11-20 00:00:00'  --stop-datetime='2021-11-21 00:00:00' mysql-bin.000003 | more
```

![image.png](https://i.loli.net/2021/11/20/leYgtcAQmkybqEn.png)



## 二进制日志的作用

1. 主从复制

2. 数据恢复

   ```mysql
   create database mytest;
   create table tmp(id int,name varchar(10));
   insert into tmp(id,name) values(1,'aaa'),(2,'bbb'),(3,'ccc');
   select * from tmp;
   drop database mytest;
   show binary logs;
   #执行完这些语句之后，执行下面的shell语句
   ```

   ```shell
   mysqlbinlog  /var/lib/mysql/mysql-bin.000003   #得到图1
   mysqlbinlog --start-position=1054  --stop-position=1686   mysql-bin.000003 | mysql -uroot -p  #执行完的结果是图2
   ```

   ![image.png](https://i.loli.net/2021/11/20/ecIhONz3DB9C1pF.png)

# 数据备份和恢复

## 示意图

![image.png](https://i.loli.net/2021/11/22/BlSgfxicaDeKTYb.png)

## 常用命令

```shell
#备份'数据'，生成sql脚本
mysqldump -uroot -p --all-databases > ~/all.sql(文件名+路径)                
mysqldump -uroot -p --databases test(数据库名) > ~/test.sql
mysqldump -uroot -p test student > ~/student.sql

#导出数据,真实的数据
mysql -uroot -p -D test -e'select * from student' > ~/student_data.txt    

#根据sql脚本导入数据到数据库
source ~/student.sql                 #mysql shell
cat ~/student.sql | mysql -uroot -p  #linux shell
```



## 命令实践操作

```sql
mysqldump -uroot -p123456 --databases mytest > ~/mytest.sql
drop database mytest;
cat ~/mytest.sql | mysql -uroot -p123456
#这三步下来就还原了
```

![image.png](https://i.loli.net/2021/11/21/lCpwLkecu452Tzi.png)

注意：这些操作也可以用来将centos7中的数据库导入到其他机器（如Ubuntu）。

# MySQL集群

## 主从复制

master（centos7 ）：192.168.17.133

slave（windows10）：192.168.43.2

ping之后确保网络互通

### master配置

1. 打开bin-log,并配置唯一的server-id。

   ```shell
   #/etc/my.cnf中的内容
   server-id=1  #如果这个不写无法重启
   expire_logs_days=7    #过期时间是七天
   log-bin=mysql-bin     #不写绝对路径就默认保存在上面设置的datadir中
   ```

2. 创建一个用于主从库通信的账号。

   ```mysql
   grant replication slave on *.* to mslave@192.168.17.1 identified by '111111';
   ```

   ```mysql
    select * from user where user='mslave'\G;
   *************************** 1. row ***************************
                     Host: 192.168.17.1
                     User: mslave
              ......
          Repl_slave_priv: Y
              ......
   ```

3. 获取binlog日志文件名和position

   ```sql
   show master status;
   mysql> show master status;
   +------------------+----------+--------------+------------------+-------------------+
   | File             | Position | Binlog_Do_DB | Binlog_Ignore_DB | Executed_Gtid_Set |
   +------------------+----------+--------------+------------------+-------------------+
   | mysql-bin.000004 |     4793 |              |                  |                   |
   +------------------+----------+--------------+------------------+-------------------+
   1 row in set (0.00 sec)
   
   #mysql-bin.000004文件内容节选，选择position为4509
   # at 4509
   #211121 20:17:21 server id 1  end_log_pos 4609 CRC32 0xd70cc882 	Query	thread_id=8	exec_time=0	error_code=0
   SET TIMESTAMP=1637497041/*!*/;
   create database mytest
   ```



### slave配置

1. 配置唯一的全局变量server-id（涉及修改配置文件，需要重启mysql服务）

   ```sql
   C:\ProgramData\MySQL\MySQL Server 8.0\my.ini
   ```

2. 使用master创建的账户读取binlog同步数据（mysql中） 

   ```mysql
   #设置之后要更改的时候，要先stop slave，更改完成之后再start slave
   change master to MASTER_HOST='192.168.17.135',
   MASTER_PORT=3306,
   MASTER_USER='mslave',
   MASTER_PASSWORD='111111',
   MASTER_LOG_FILE='mysql-bin.000008',
   MASTER_LOG_POS=5832;
   ```
   
   ## 配置过程中的错误记录
   
   当我在配置完成后，遇到了从服务器连接不到主服务器的问题，如下图：
   
   ```sql
   mysql> show slave status\G;
   *************************** 1. row ***************************
                  Slave_IO_State: Connecting to master
                     Master_Host: 192.168.17.133
                     Master_User: mslave
                     Master_Port: 3306
                   ......//省略
                   Last_IO_Errno: 2003             #提示正在连接
                   Last_IO_Error: error connecting to master 'mslave@192.168.17.133:3306' - retry-time: 60  retries: 1
                  Last_SQL_Errno: 0
                  Last_SQL_Error:
   ```
   
   ![image.png](https://i.loli.net/2021/11/21/agOh7USZ9kon5c4.png)
   
   首先想到的肯定是这个ip地址是否正确，是否能ping通，通过证明发现能ping通过。
   
   然后就是端口号的问题了，查看一下服务器是否开放了3306端口。
   
   ```shell
   [root@localhost mysql]# firewall-cmd --zone=public --list-ports
   
   ```
   
   果然没开，我们打开这个端口
   
   ```shell
   [root@localhost mysql]# firewall-cmd --zone=public --add-port=3306/tcp --permanent
   success
   [root@localhost mysql]# firewall-cmd --zone=public --list-ports
   3306/tcp   #已经打开
   ```
   
   ![image.png](https://i.loli.net/2021/11/21/26TZe7jPuQAoS4z.png)

​     ![image.png](https://i.loli.net/2021/11/21/V7NiaTUAS3M6Y5v.png)

​              so，大功告成！！！

错误二：

![image.png](https://i.loli.net/2021/11/22/DZlafkswT6oIh1u.png)



错误三：

![image.png](https://i.loli.net/2021/11/22/LjecJbs8aXiuQP1.png)

这个解决之后又出错了，一怒之下，先停掉win10的slave，然后将之前同步的表全删除。然后通过centos的备份的sql脚本在win10下进行还原，保证数据目前完全一致。然后，重新设置slave的配置信息。position设为最新的。这样就保证了之前的数据都一样了。

![image.png](https://i.loli.net/2021/11/22/Ai21w4FVulbsj7X.png)

![image.png](https://i.loli.net/2021/11/22/JtHPnGBbI1gao4S.png)





## 读写分离

> 这里主要讨论MyCat读写分离的配置步骤。

### 1.主要的原理

​		通过一个代理服务器（mycat中间件）来实现一个负载均衡的作用。通过一主多从或者多主多从的模式，当客户端访问数据库的时候，让客户端访问代理服务器，也就是一个逻辑的DB。然后这个代理服务器（MyCat）将请求来分配到不同的服务器，进而实现负载均衡。即可以让主节点负责写，从节点负责读（当然这些可以通过mycat配置文件中的balance参数来进行设定）。当主服务器宕机的时候，代理服务器要能够快速的切换主服务器到另一台服务器，当然这也需要在配置信息中给出备用服务器的host以及user。

​		谈到宕机（高可用性），如何判断主服务器宕机了呢？可以每隔一段时间给主服务器发送一个心跳包（select语句，具体这个select语句是什么，也是在配置文件中设置的）。如果能回应，就说明能正常工作。否则就是宕机。



### 2.配置条件

> 将mycat部署在centos上，将主服务器也部署在centos上，将从服务器部署在win10上。
>
> centos:  192.168.17.134
>
> win10:    192.168.43.2(会动态变化)

> 设置MySQLroot用户支持远程连接

```sql
mysql> select Host,User from user;
+--------------+---------------+
| Host         | User          |
+--------------+---------------+
| 192.168.17.1 | mslave        |
| localhost    | mysql.session |
| localhost    | mysql.sys     |
| localhost    | root          |
+--------------+---------------+
4 rows in set (0.00 sec)

#MySQL8不可以这样 写了，会报错
mysql> grant all privileges on *.* to 'root'@'%' identified by '123456' with grant option;
Query OK, 0 rows affected, 1 warning (0.03 sec)

mysql> select Host,User from user;
+--------------+---------------+
| Host         | User          |
+--------------+---------------+
| %            | root          |
| 192.168.17.1 | mslave        |
| localhost    | mysql.session |
| localhost    | mysql.sys     |
| localhost    | root          |
+--------------+---------------+
5 rows in set (0.00 sec)

#MySQL8
mysql> create user 'root'@'%' identified by '123456';
Query OK, 0 rows affected (0.02 sec)
mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
Query OK, 0 rows affected (0.01 sec)
```



### 3.配置步骤

1. mycat安装 

   ```shell
   [jqw@localhost 下载]$ tar -zxvf Mycat-server-1.6-RELEASE-20161028204710-linux.tar.gz
   [root@localhost bin]#  ln -s /home/jqw/下载/mycat/bin/mycat /usr/bin/mycat
   #这里注意创建软链接的时候一定要加上-s，否则无法使用，ln默认是硬链接
   ```

2. mycat配置

   ![image.png](https://i.loli.net/2021/11/22/8lgKsJvq7HnuDai.png)

   server.xml

   ![image.png](https://i.loli.net/2021/11/22/FEwHJpuzsVvYiqe.png)

   schema.xml配置   

```xml
<?xml version="1.0"?> 
<!DOCTYPE mycat:schema SYSTEM "schema.dtd"> 
<mycat:schema xmlns:mycat="http://io.mycat/"> 
    <!-- 逻辑数据库 --> 
    <schema name="USERDB" checkSQLschema="false" sqlMaxLimit="100" dataNode="dn1"></schema> 
    <!-- 存储节点 --> 
    <dataNode name="dn1" dataHost="node1" database="mytest" /> 
    <!-- 数据库主机 --> 
    <dataHost name="node1" maxCon="1000" minCon="10" balance="3" writeType="0" dbType="mysql" dbDriver="native" switchType="1" slaveThreshold="100"> 
        <heartbeat>select user()</heartbeat> 
        <!-- can have multi write hosts --> 
        <writeHost host="192.168.17.134" url="192.168.17.134:3306" user="root" password="123456"> 
            <!-- can have multi read hosts --> 
            <readHost host="192.168.43.2" url="192.168.43.2:3306" user="root" password="123456" />
        </writeHost>
        <!-- 写数据库宕机后的备份数据库 --> 
        <writeHost host="192.168.43.2" url="192.168.43.2:3306" user="root" password="123456" /> 
    </dataHost> 
</mycat:schema>
```

配置完成之后启动就可以了    mycat -start

![image.png](https://i.loli.net/2021/11/22/YNO7LQ6VaMKAgIP.png)



### 4.mycat端口介绍

​	**9066端口**：管理端口，运行状态的监控（monitor）

![image.png](https://i.loli.net/2021/11/22/GIMRJAN1XHaKTbZ.png)

```mysql
show @@help;   #可以用来查看各种命令，如：
mysql> show @@database;
+----------+
| DATABASE |
+----------+
| USERDB   |
+----------+
1 row in set (0.00 sec)
```

![image.png](https://i.loli.net/2021/11/22/4wmNitVlUR8ySod.png)



​	**8066端口**：数据端口，相当于一个逻辑的数据库，就和操作mysql一样

```
mysql -h 192.168.17.134 -P 8066 -u root -p
```

![image.png](https://i.loli.net/2021/11/22/nRfigYzwbqy9MOu.png)



```sql
mysql> show databases;
+----------+
| DATABASE |
+----------+
| USERDB   |      #逻辑库名，其实就是MySQL中的mytest库（配置文件中给出来的）
+----------+
1 row in set (0.00 sec)
mysql> use USERDB;
mysql> show tables;
+------------------+
| Tables_in_mytest |     #mytest
+------------------+
| tmp              |     #和centos7中主库MySQL中的mytest一致
+------------------+
1 row in set (0.01 sec)
```



### 5.配置成功验证

问题：配置成功了，我们怎么知道查询的时候使用的是从库，写操作使用的是从库呢？

> 可以使用日志，使用查询日志，因为这个日志会记录所有的记录。
>
> 查询日志位置 /var/lib/mysql/localhost.log
>
> win10下路径为C:\ProgramData\MySQL\MySQL Server 8.0\Data\DESKTOP-6UMAIBV.log

```sql
mysql> set global general_log=on;   #win10下的从库也打开
Query OK, 0 rows affected (0.04 sec)
```

然后执行下面的语句：

```sql
mysql> select * from tmp where id=3;
mysql> select * from tmp where id=2;
mysql> select * from tmp where id=1;
mysql> insert into tmp values(7,'ggg');
```

然后分别在win10和centos7平台下查看日志；可以发现现在能正常工作了。

![image.png](https://i.loli.net/2021/11/23/styWL5bQFlRj4hX.png)

![image.png](https://i.loli.net/2021/11/23/clQ178j3S5bZIi6.png)

so good；那再来看一下容灾情况也就是高可用性。关掉centos7上的mysql服务，看是否能主从切换。

关掉之后，分别执行查询和插入，发现都是在win10下，所以高可用性也是ok的！！！

![image.png](https://i.loli.net/2021/11/23/7krHOIgS9hBZFiE.png)

### 6.配置过程中遇到的问题

最初刚开始配置的时候，select和update的操作都在主库上进行（通过查询日志）。这时候，首先想到的是

- 主从复制是否工作正确    通过检查发现主从复制完全正确
- 是不是win10下的mysql不支持远程连接（通过在centos上远程连接mysql发现确实不支持，这是什么原因造成的呢？首先排除账户，因为已经创建好了账户，可能是防火墙的问题，所以通过防火强设置，开放3306端口![image.png](https://i.loli.net/2021/11/23/tzHmcbq1fIG8O5S.png)打开之后，出现了下面这种情况![image.png](https://i.loli.net/2021/11/23/Tf9t6Dd1P8ijpIq.png)可以看到，这时候成功连接了)
- 但是虽然远程连接成功了，但是mycat还是出错，这时候查看mycat的日志![image.png](https://i.loli.net/2021/11/23/jHa5rZ2IdGoR7tz.png)

- 这时候，通过远程连接分别查看centos和win10下的user().并用mycat查询centos上的mysql的select user()的执行结果![image.png](https://i.loli.net/2021/11/23/6QjuDfPoZinkt2V.png)所以这时候猜想原因可能是select user()返回的结果不是ip地址造成的。
- 这时候，干脆直接将select user()不用了。使用show slave status;修改配置文件。重启服务，发现成功了。![image.png](https://i.loli.net/2021/11/23/CjoFqeZOSg3xIUc.png)



总结：可能这次的问题就是因为和老师的环境不一致造成的



```sql
为了得到select user()的正确结果做的一系列操作
mysql> alter user 'root'@'%' identified by '123456' password expire never; 
mysql> alter user 'root'@'%' identified with mysql_native_password by '123456';
flush privileges;
select user,host,plugin from user;

alter user 'root'@'localhost' identified by '123456' password expire never; 
alter user 'root'@'localhost' identified with caching_sha2_password by '123456';

```



## 分库分表



# MySQL用户及权限

> 本节课为扩展内容，参考《深入浅出MySQL全文》（第28章 MySQL的权限与安全 P337）；

## 权限管理

​        MySQL在安装的时候会生成一个数据库 “mysql”，在这个数据库中有三张表：user，db，host（一般用不到）。这三张表分别记录着用户的权限。MySQL权限系统分为两个阶段进行验证，首先，对连接的用户进行验证，验证他的用户名，密码和主机名（IP本地为local host）是否正确。如果验证通过，再通过mysql数据库表中的记录给不同的用户赋予不同的权限。

## 账号管理

1. 创建账号：

   ```sql
   #第一种方法，只创建账号，没有权限
   create user u1@localhost identified by '111111';
   #第二种方法，如果有账号就增加权限，没有账号就创建账号（推荐）
   grant select(权限名) on *.*（数据库名.表名）to u1@localhost identified by '111111' [with grant] ;
   ```

   ```mysql
   mysql>  create user u1@localhost identified by '111111';
   Query OK, 0 rows affected (0.13 sec)
   mysql> select * from user where user='u1'\G;
   *************************** 1. row ***************************
                     Host: localhost
                     User: u1
              Select_priv: N              
              ...
   mysql> grant select on *.* to u1@localhost;
   Query OK, 0 rows affected (0.00 sec)
   
   mysql> select * from user where user='u1'\G;
   *************************** 1. row ***************************
                     Host: localhost
                     User: u1
              Select_priv: Y                           #更改了
   ```

2. 查看账号的权限

   ```sql
   show grants for u1@localhost;   #方法一
   ```

   ```sql
   mysql> show grants for u1@localhost;
   +-----------------------------------------+
   | Grants for u1@localhost                 |
   +-----------------------------------------+
   | GRANT SELECT ON *.* TO 'u1'@'localhost' |
   +-----------------------------------------+
   1 row in set (0.00 sec)
   ```

3. 更改账号的权限

   ```sql
   增加权限： 创建的第二种
   回收权限：revoke select on *.* to u1@localhost;
   ```

   ```sql
   mysql> revoke select on *.* from u1@localhost;
   Query OK, 0 rows affected (0.00 sec)
   mysql> select * from user where user='u1'\G;
   *************************** 1. row ***************************
                     Host: localhost
                     User: u1
              Select_priv: N
   ```

4. 删除账号

```sql
drop user u1@localhost;
```

```sql
mysql> drop user u1@localhost;
Query OK, 0 rows affected (0.00 sec)

mysql> select * from user where user='u1'\G;
Empty set (0.00 sec)  
```

MySQL8.0如何修改密码：

```sql
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '123456';
#MySQL8不支持password函数，MySQL8之前可以使用下面的：
 set PASSWORD FOR 'root'@'localhost'=PASSWORD('123456');
```



 

