# 一.简单动态数据库

Redis里，C字符串只会作为字符串字面量用在一些无需对字符串值进行修改的地方，比如打印日志。Redis构建了 简单动态字符串（simple dynamic string，SDS）来表示字符串值。

在Redis里，包含字符串值的键值对在底层都是由SDS实现的。除此之外，SDS还被用作缓冲区：AOF缓冲区，客户端状态中的输入缓冲区。

## (一).SDSD定义

每个sds.h/sdshdr结构表示一个SDS值：

```c
struct sdshdr {
    // 记录buf数组中已使用字节的数量
    int len;		// 等于SDS所保存字符串的长度
    int free;		// 记录buf数组中未使用字节的数量
    char buf[];	// 字节数组，用于保存字符串
}
```

示例：

<div align = center><img src="../pictures/Redis1-1.png" width="400px"/></div>

SDS遵循C字符串以空字符结尾的管理，空字符不计算在len属性中。这样，SDS可以重用一部分C字符串函数库，如printf。

## (二).SDS与C字符串的区别

### 1.常数复杂度获取字符串长度

+ C字符串必须遍历整个字符串才能获得长度，复杂度是O(N)。
+ SDS在len属性中记录了SDS的长度，复杂度为O(1)。

### 2.杜绝缓冲区溢出

+ C字符串不记录长度的带来的另一个问题是缓冲区溢出。假设s1和s2是紧邻的两个字符串，对s1的strcat操作，有可能污染s2的内存空间。

+ SDS的空间分配策略杜绝了缓冲区溢出的可能性：但SDS API修改SDS时， **会先检查SDS的空间是否满足修改所需的要求，不满足的话，API会将SDS的空间扩展至执行修改所需的大小，然后再执行实际的修改操作。**

### 3.减少修改字符串时带来的内存重分配次数

>  每次增长或缩短一个C字符串，程序都要对保存这个C字符串的数组进行一次内存重分配操作。

Redis作为数据库，数据会被平凡修改，如果每次修改字符串都会执行一次内存重分配的话，会对新嗯呢该造成影响。SDS通过未使用空间接触了字符串长度和底层数组长度的关联：在SDS中，buf数组的长度不一定就是字符数量+1，数组里面可以包含未使用的字节，由free属性记录。对于未使用空间，SDS使用了空间预分配和惰性空间释放两种优化策略：

1.  **空间预分配：** 当SDS的API对SDS修改并需要空间扩展时，程序不仅为SDS分配修改所需的空间，还会分配额外的未使用空间（取决于长度是否小于1MB）。
2.  **惰性空间释放：** 当SDS的API需要缩短时，程序不立即触发内存重分配，而是使用free属性将这些字节的数量记录下来，并等待将来使用。与此同时，SDS API也可以让我们真正师范未使用空间，防止内存浪费。

### 4.二进制安全

+ C字符串中的字符必须复合某种编码（如ASCII），除了字符串末尾之外，字符串里不能包含空字符。这些限制使得C字符串只能保存文本，而不是不能保存二进制数据。

+ SDS API会以处理二进制的方式处理SDS存放在buf数组中的数据，写入时什么样，读取时就是什么样。SDS 使用 len 属性的值而不是空字符来判断字符串是否结束。

### 5.兼容部分C字符串函数

+ 遵循C字符串以空字符结尾的管理，SDS可以重用<string.h>函数库。

### 6.总结

| C字符串                          | SDS                                |
| -------------------------------- | ---------------------------------- |
| 获取长度的复杂度O(N)             | O(1)                               |
| API不安全，缓冲区溢出            | API安全，不会缓冲区溢出            |
| 修改字符串长度必然导致内存重分配 | 修改字符串长度不一定导致内存重分配 |
| 只能保存文本数据                 | 可以保存文本或二进制数据           |
| 可使用所有<string.h>库的函数     | 可使用部分<string.h>库的函数       |

## (三).SDS API

|    函数     | 作用                                                       |   时间复杂度   |
| :---------: | ---------------------------------------------------------- | :------------: |
|   sdsnew    | 创建一个包含给定C字符串的SDS                               |      O(N)      |
|  sdsempty   | 创建一个不包含任何内容的SDS                                |      O(1)      |
|   sdsfree   | 释放SDS                                                    |      O(N)      |
|   sdslen    | 返回SDS已使用的字节数                                      |      O(1)      |
|  sdsavail   | 返回SDS未使用的字节数                                      |      O(1)      |
|   sdsdup    | 创建一个给定SDS的副本                                      |      O(N)      |
|  sdsclear   | 清空SDS保存的字符串内容                                    | O(1)，惰性释放 |
|   sdscat    | 将给定C字符串拼接到SDS字符串的末尾                         |      O(N)      |
|  sdscatsds  | 将给定SDS字符串拼接到另一个SDS的末尾                       |      O(N)      |
|   sdscpy    | 复制                                                       |      O(N)      |
| sdsgrowzero | 用空字符将SDS扩展至给定长度                                |      O(N)      |
|  sdsrange   | 保留SDS给定区间内的数据，不在区间内的数据会被覆盖或清除    |      O(N)      |
|   sdstrim   | 接受一个SDS和C字符为参数，从SDS中移除C字符串中出现过的字符 |     O(N^2)     |
|   sdscmp    | 比较                                                       |      O(N)      |

---

# 二.链表

Redis构建了自己的链表实现。列表键的底层实现之一就是链表。当一个列表键包含数量比较多的元素，又或者列表中包含的元素都是比较长的字符串时，Redis就会使用链表作为列表键的底层实现。

出列表键外，发布、订阅、慢查询、监视器都用到了链表。Redis服务器还用链表保存多个客户端的状态信息，以及构建客户端输出缓冲区。

## (一).链表与链表节点的实现

链表节点用adlist.h/listNode结构来表示

```c
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;
```

<div align = center><img src="../pictures/Redis1-2.png" width="700px"/></div>

使用adlist.h/list来持有链表:

```c
typedef struct list {
    listNode *head;											//表头节点
    listNode *tail;											//表尾节点
    unsigned long len;									//链表包含的节点数量
    void *(dup)(void *ptr); 						// 节点复制函数
    void (*free)(void *ptr); 						// 节点释放函数
    int (*match)(void *ptr, void *key); // 节点值对比函数
} list;
```

<div align = center><img src="../pictures/Redis1-3.png" width="700px"/></div>

Redis的链表实现可总结如下：

1.  **双端：** 获取某个节点的前置节点与后置节点复杂度都为O(1)
2.  **无环：** 表头结点的prev和表尾节点的next都指向NULL
3.  **带表头指针和表尾指针**
4.  **带链表长度计数器：** 程序获取链表中的节点数量的复杂度为O(1)
5.  **多态：** 使用void*指针来保存节点值，并通过list结构的dup、free。match三个属性为节点值设置类型特定函数

## (二).链表和链表节点的API

| 函数                                                    | 作用                                                         | 复杂度 |
| ------------------------------------------------------- | ------------------------------------------------------------ | ------ |
| listSetDupMethod, listSetFreeMethod, listSetMatchMethod | 将给定函数设置为链表的节点值复制/释放/对比函数               | O(1)   |
| listGetDupMethod, listGetFreeMethod, listGetMatchMethod | 返回链表当前正在使用的节点值复制/释放/对比函数               | O(1)   |
| listLength                                              | 返回链表长度                                                 | O(1)   |
| listFrist                                               | 返回表头结点                                                 | O(1)   |
| listLast                                                | 返回表尾结点                                                 | O(1)   |
| listPrevNode, listNextNode                              | 返回给定节点的前置/后置节点                                  | O(1)   |
| listNodeValue                                           | 返回给定节点目前正在保存的值                                 | O(1)   |
| listCreate                                              | 创建一个不包含任何节点的新链表                               | O(1)   |
| listAddNodeHead, listAddNodeTail                        | 将一个包含给定值的新节点添加到表头/表尾                      | O(1)   |
| listSearchKey                                           | 查找并返回包含给定值的节点                                   | *O(N)* |
| listIndex                                               | 返回链表在给定索引上的节点                                   | *O(N)* |
| listDelNote                                             | 删除给定节点                                                 | *O(N)* |
| listRotate                                              | 将链表的表尾结点弹出，然后将被弹出的节点插入到链表的表头，成为新的表头结点 | O(1)   |
| listDup                                                 | 复制一个给定链表的副本                                       | *O(N)* |
| listRelease                                             | 释放给定链表，及所有节点                                     | *O(N)* |

---

# 三.字典

在字典中一个键(Key)可以与一个值(value)进行关联，这件关联的键与值就被称为键值对。

Redis的数据库就是使用字典来作为底层实现的，对数据库的增删改查都是构建在字典的操作之上。

字典还是哈希键的底层实现之一，但一个哈希键包含的键值对比较多，又或者键值对中的元素都是较长的字符串时，Redis就会用字典作为哈希键的底层实现。

## (一).字典的实现

Redis的字典使用 **哈希表** 作为底层实现，每个哈希表节点就保存了字典中的一个键值对。

### 1.哈希表

Redis字典所用的**哈希表**由dict.h/dictht结构定义：

```c
typedef struct dictht {
    dict Entry **table;			// 哈希表数组
    unsigned long size;			// 哈希表大小
    unsigned long sizemask;	// 哈希表大小掩码，用于计算索引值，总是等于size - 1
    unsigned long used;			// 该哈希表已有节点的数量
} dictht;
```

> sizemask与哈希值决定一个键应该被放到table数组的哪个索引上。

### 2.哈希表节点

**哈希表节点** 使用dictEntry结构表示，每个dictEntry结构都保存着一个键值对：

```c
ypedef struct dictEntry {
    void *key; 							// 键

    union {
        void *val;
        uint64_t u64;
        int64_t s64;
    } v;									// 值

    struct dictEntry *next; // 指向下个哈希表节点，形成链表。一次解决键冲突的问题
}
```

<div align = center><img src="../pictures/Redis1-4.png" width="700px"/></div>

### 3.字典

Redis中的 **字典** 由dict.h/dict结构表示：

```c
typedef struct dict {
    dictType *type; // 类型特定函数
    void *privdata; // 私有数据

    /*
 			哈希表一般情况下，字典只是用ht[0]哈希表，ht[1]只会在对ht[0]哈希表进行rehash时是用
    */
    dictht ht[2]; 

    // rehash索引，但rehash不在进行时，值为-1
    int trehashidx; // 记录了rehash的进度
} dict;
```

type和privdata是针对不同类型大家键值对，为创建多态字典而设置的：

- type是一个指向dictType结构的指针，每个dictType都保存了一簇用于操作特定类型键值对的函数，Redis会为用途不同的字典设置不同的类型特定函数。
- privdata保存了需要传给那些类型特定函数的可选参数。

```c
typedef struct dictType {
    // 计算哈希值的函数
    unsigned int (*hashFunction) (const void *key);

    // 复制键的函数
    void *(*keyDup) (void *privdata, const void *obj);

    // 对比键的函数
    void *(*keyCompare) (void *privdata, const void *key1, const void *key2);

    // 销毁键的函数
    void (*keyDestructor) (void *privdata, void *key);

    // 销毁值的函数
    void (*valDestructor) (void *privdata, void *obj);
} dictType;

```

<div align = center><img src="../pictures/Redis1-5.png" width="700px"/></div>

> ht属性是一个包含两个项的数组，数组中每个项都是一个dictht哈希表，一般情况下，字典只是用ht[0]哈希表，ht[1]只会在对ht[0]哈希表进行rehash时是用

## (二).哈希算法

Redis计算哈希值和索引值的方法如下：

```c
// 使用字典设置的哈希函数，计算key的哈希值
hash = dict.type.hashFucntion(key)
// 使用哈希表的sizemask属性和哈希值，计算出索引值
// 根据情况的不同，ht[x]可以使ht[0]或ht[1]
index = hash & dict.ht[x].sizemask
```

当字典被用作数据库或哈希键的底层实现时，使用MurmurHash2算法来计算哈希值，即使输入的键是有规律的，算法人能有一个很好的随机分布性，计算速度也很快。

## (三).解决键冲突

Redis使用链地址法解决键冲突，每个哈希表节点都有个next指针。

<div align = center><img src="../pictures/Redis1-6.png" width="700px"/></div>

## (四).rehash

随着操作的不断执行，哈希表保存的键值对会增加或减少。为了让哈希表的负载因子维持在合理范围，需要对哈希表的大小进行扩展或收缩，即通过执行rehash（重新散列）来完成：

1. 为字典的ht[1]哈希表分配空间：

   + 如果执行的是扩展操作，ht[1]的大小为第一个大于等于ht[0].used * 2 的2^n

   + 如果执行的是收缩操作，ht[1]的大小为第一个大于等于ht[0].used的2^n

2. 将保存在ht[0]中的所有键值对rehash到ht[1]上。rehash是重新设计的计算键的哈希值和索引值

3. 释放ht[0]，将ht[1]设置为ht[0]，并为ht[1]新建一个空白哈希表

### 哈希表的扩展与收缩

满足一下任一条件，程序会自动对哈希表执行扩展操作：

+ 服务器目前没有执行BGSAVE或BGREWRITEAOF，且哈希表负载因子大于等于1
+ 服务器正在执行BGSAVE或BGREWRITEAOF，且负载因子大于5

哈希表执行收缩操作的条件：

+ 当哈希表的负载因子小于0.1时，程序自动开始对哈希表执行收缩操作

其中负载因子的计算公式：

```bash
# 负载因子 = 哈希表已保存节点数量 / 哈希表大小
load_factor = ht[0].used / ht[0].size
```

执行BGSAVE或BGREWRITEAOF过程中，Redis需要创建当前服务器进程的子进程，而多数操作系统都是用写时复制来优化子进程的效率，所以在子进程存在期间，服务器会提高执行扩展操作所需的负载因子，从而尽可能地避免在子进程存在期间扩展哈希表，避免不避免的内存写入，节约内存。

## (五).渐进式rehash

将ht[0]中的键值对rehash到ht[1]中的操作不是一次性完成的，而是分多次渐进式的：

1. 为ht[1]分配空间
2. 在字典中维持一个索引计数器变量rehashidx，设置为0，表示rehash工作正式开始
3. rehash期间， **每次对字典的增删改查操作** ，会顺带将ht[0]在rehashidx索引上的所有键值对rehash到ht[1]，rehash完成之后，rehashidx属性的值+1
4. 最终ht[0]会全部rehash到ht[1]，这是将rehashidx设置为-1，表示rehash完成

渐进式rehash的好处在于它采用了分而治之的方式，将rehash键值对所需的计算工作均摊到对字典的 增删改查 操作上，从而避免了集中式 rehash 而带来的庞大计算量。

### 渐进式rehash执行期间的哈希表操作

+ 进式rehash过程中，字典会有两个哈希表，字典的 **删除，查找，更新** 会在两个哈希表上进行。

+ 另外，进式rehash过程中，新添加到字典的键值对一律会被保存到ht[1]

---

# 四.跳跃表

跳跃表是一种**有序数据结构**，它通过在每个节点中维持多个指向其他节点的指针，从而达到快速访问的目的。跳跃表支持平均*O(logN)*、最坏*O(N)*的查找，还可以通过顺序性操作来批量处理节点。

Redis使用跳跃表作为有序集合键的底层实现之一，如果有序集合包含的元素数量较多，或者有序集合中元素的成员是比较长的字符串时，Redis使用跳跃表来实现有序集合键。

Redis只在两个地方用到了跳跃表：

+ 实现有序集合键
+ 在集群结点中用作内部数据结构

## (一).跳跃表的实现



---

# 五.整数集合





---

# 六.压缩列表







---

# 七.对象





