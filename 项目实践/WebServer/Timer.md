## timer——定时器

> 基于小根堆实现的定时器，关闭超时的非活动连接；

```c++
 源代码
├── code          
│   ├── timer
│		│		├── heaptimer.h
│		│		├── heaptimer.cpp
```

### 1.定义定时器类与时间堆类

定时器类的定义：

```c++
typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;
struct TimerNode {
    int id;   														//时间器ID 
    TimeStamp expires;  									//过期时间
    TimeoutCallBack cb;  									// ~ 回调函数
    bool operator < (const TimerNode& t) 	// ~ operator<重载
        return expires < t.expires;
};
```

时间堆的定义：

```c++
class HeapTimer {
public:
    HeapTimer() { heap_.reserve(64); }
    ~HeapTimer() { clear(); }
    ...																			//功能函数
private:
    std::vector<TimerNode> heap_;						//定时器容器
    std::unordered_map<int, size_t> ref_;		//
};
```

### 2.交换定时器操作SwapNode_

```c++
assert(i >= 0 && i < heap_.size());
assert(j >= 0 && j < heap_.size());
std::swap(heap_[i], heap_[j]);//交换结点
//ref_[i]表示id = i的结点在heap_中的位置
ref_[heap_[i].id] = i;  //新结点的ref_[id = j]的位置在i处         
ref_[heap_[j].id] = j;  //新结点的ref_[id = i]的位置在j处  
```

### 3.上滤操作siftup_

+ 如果传入的结点正常，则先找到对应结点的父节点
+ 父子结点进行比较
  + 如果父节点时间值更小，无需交换，break退出
  + 否则父子结点进行交换，接着将向上进行过滤，知道根结点

```c++
assert(i >= 0 && i < heap_.size());
size_t j = (i - 1) / 2;  // ~ 找到其父节点j
while(j >= 0) {
    if(heap_[j] < heap_[i])  	break; 
    SwapNode_(i, j);   	// ~ 父节点的值大于子节点，交换父子节点
    i = j;							//此时的父节点进行上滤
    j = (i - 1) / 2;   	// ~ 继续向上查找
}
```

### 4.下滤操作siftdown_

> 下滤操作，确保以当前节点为根的子树满足最小堆的要求

+ 判断传入的根结点与结点数目满足条件
+ 找到根结点的左子树，进行操作
  + 查看是否存在右子树，且右子树比左子树更小，则利用右子树作为比较结点进行比较
  + 如果比较结点的值比父节点大，则结束
  + 否则进行交换，更新子结点，继续向下进行下滤操作
+ 返回是否有无进行下滤操作

```c++
assert(index >= 0 && index < heap_.size());
assert(n >= 0 && n <= heap_.size());
size_t i = index;       // ~ 父节点 i
size_t j = i * 2 + 1;   // ~ 左子节点 j
while(j < n) {
  if(j + 1 < n && heap_[j + 1] < heap_[j]) j++; 	// 如果存在右子树且右子树更小，则利用右子树进行比较
  if(heap_[i] < heap_[j]) break;
  SwapNode_(i, j);  // 如果父节点更大，则交换
  i = j;
  j = i * 2 + 1;
}
return i > index;		// ~ 如果该节点是叶子节点，那么会返回 false
```

### 5.定时器添加操作add

+ 如果是新的节点，则将新结点加入到heap_中，进行上滤操作
+ 如果是已有结点，则更新结点信息
  + 如果是叶子结点，进行上滤操作
  + 如果是子树根，则进行下滤操作

```c++
assert(id >= 0);
size_t i;
if(ref_.count(id) == 0) {	/* 新节点：堆尾插入，调整堆 */
    i = heap_.size();	//末尾位置
    ref_[id] = i;
    heap_.push_back({id, Clock::now() + MS(timeout), cb});
    siftup_(i);
} 
else {		/* 已有结点：调整堆 */
  i = ref_[id];//找到在heap_中的位置
  heap_[i].expires = Clock::now() + MS(timeout);
  heap_[i].cb = cb;
  // ~ 如果是叶子节点，进行上滤操作
  // ~ 如果是堆的根节点，或者子树的根节点，进行下过滤操作
  if(!siftdown_(i, heap_.size())) 
    siftup_(i);
}
```

### 6.定时器删除操作del_

+ 确保删除元素的位置存在且正确
+ 将元素与最后的元素进行交换，然后调整前面的元素称为最小堆
+ 删除最后一个元素

```c++
assert(!heap_.empty() && index >= 0 && index < heap_.size());
size_t i = index;
size_t n = heap_.size() - 1;/* 将要删除的结点换到队尾 */
assert(i <= n);
if(i < n) {
    SwapNode_(i, n);//将删除的元素与最后一个元素进行交换
    if(!siftdown_(i, n)) 	/*然后调整前面的堆*/
      siftup_(i);
}
ref_.erase(heap_.back().id);/* 队尾元素删除 */
heap_.pop_back();
```

### 7.定时器的调整操作adjust

+ 位置正确且结点存在
+ 更新时间
+ 调整堆

```c++
assert(!heap_.empty() && ref_.count(id) > 0);//位置正确且结点存在
heap_[ref_[id]].expires = Clock::now() + MS(timeout);/* 调整指定id的结点 */
siftdown_(ref_[id], heap_.size());  // ~ 只考虑增序调整
```

### 8.定时器的工作函数dowork

+ 确保定时器不为空且位置正确
+ 取出结点，进行回调函数
+ 删除此节点

```c++
if(heap_.empty() || ref_.count(id) == 0) return;//确保定时器不为空且位置正确
size_t i = ref_[id];
TimerNode node = heap_[i];
node.cb();   // ~ 调用回调函数
del_(i);     // ~ 删除？
```

### 9.定时器的心搏函数tick

+ 确定此时定时器不为空
+ 如果最小堆根结点的时间到达，则取出结点
+ 进行回调函数，然后删除

```c++
if(heap_.empty()) return;
while(!heap_.empty()) {
    TimerNode node = heap_.front();
    if(std::chrono::duration_cast<MS>(node.expires - Clock::now()).count() > 0)// 还未到定时时间
        break; 
    node.cb();
    pop(); // ~ 类似于 pop front 操作
}
```

### 10.定时器的下一个心搏函数GetNextTick

+ 先执行一次tick函数
+ 然后检查堆根元素的时间是否已经到达进行心搏函数的时间
  + 如果没有，则返回时间差
  + 如果有，时间差为0

```c++
tick();
size_t res = -1;
if(!heap_.empty()) {
  res = std::chrono::duration_cast<MS>(heap_.front().expires - Clock::now()).count();//计算时间差
  if(res < 0) res = 0;
}
return res;
```









