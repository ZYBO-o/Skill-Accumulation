## I.C++并发世界

### 一.并发的途径

并发有两种基本途径。每个开发人员代表一个线程，每个办公室代表一个进程。

+ 第一种途径是每个进程只要一个线程，这就类似让每个开发人员拥有自己的办公室
+ 第二种途径是每个进程有多个线程，如同一个办公室里有两个开发人员。

让我们在一个应用程序中简单的分析一下这两种途径。

#### 1. 多进程并发

使用多进程并发，是将应用程序分为多个独立的进程，它们在同一时刻运行，就像同时进行网页浏览和文字处理一样。独立的进程可以通过进程间常规的通信渠道传递讯息(信号、套接字、文件、管道等等)。

##### 缺点：

+ 进程之间的通信通常不是设置复杂，就是速度慢，这是因为操作系统会在进程间提供了一定的保护措施，以避免一个进程去修改另一个进程的数据。
+ 运行多个进程所需的固定开销：需要时间启动进程，操作系统需要内部资源来管理进程，等等。

##### 优点

+ 操作系统在进程间提供附加的保护操作和更高级别的通信机制，意味着可以更容易编写安全的并发代码。实际上，在类似于Erlang的编程环境中，将进程作为并发的基本构造块。
+ 可以使用远程连接(可能需要联网)的方式，在不同的机器上运行独立的进程。虽然，这增加了通信成本，但在设计精良的系统上，这可能是一个提高并行可用行和性能的低成本方式。

#### 2.多线程并发

在单个进程中运行多个线程。线程很像轻量级的进程：每个线程相互独立运行，且线程可以在不同的指令序列中运行。但是，进程中的所有线程都共享地址空间，并且所有线程访问到大部分数据———全局变量仍然是全局的，指针、对象的引用或数据可以在线程之间传递。

##### 优点：

+ 虽然，进程之间通常共享内存，但是这种共享通常是难以建立和管理的。因为，同一数据的内存地址在不同的进程中是不相同。线程会好很多
+ 地址空间共享，以及缺少线程间数据的保护，使得操作系统的记录工作量减小，所以使用多线程相关的开销远远小于使用多个进程。

##### 缺点：

+ 共享内存的灵活性是有代价的：如果数据要被多个线程访问，那么程序员必须确保每个线程所访问到的数据是一致的
+ 线程间数据共享可能会遇到的问题，以及如何使用工具来避免这些问题。问题并非无解，只要在编写代码时适当地注意即可，这同样也意味着需要对线程通信做大量的工作。



### 二.使用并发的原因

#### 1.为了分离关注点

+ 通过将相关的代码与无关的代码分离，可以使程序更容易理解和测试，从而减少出错的可能性。
+ 又是因为功能以及业务，线程的数量不再依赖CPU中的可用内核的数量，因为对线程的划分是基于概念上的设计，而不是一种增加吞吐量的尝试。

#### 2.为了性能

两种方式利用并发提高性能：

+ 第一，将一个单个任务分成几部分，且各自并行运行，从而降低总运行时间。这就是 **任务并行（*task parallelism*）。**

  >  虽然这听起来很直观，但它是一个相当复杂的过程，因为在各个部分之间可能存在着依赖。区别可能是在过程方面——一个线程执行算法的一部分

+ 而另一个线程执行算法的另一个部分——或是在数据方面——每个线程在不同的数据部分上执行相同的操作（第二种方式）。后一种方法被称为 **数据并行（*data parallelism*）。**

#### 3.何时不用并发

+ 当收益比不上成本。使用并发的代码在很多情况下难以理解，因此编写和维护的多线程代码就会产生直接的脑力成本，同时额外的复杂性也可能引起更多的错误。



### 三. C++中的并发与多线程

#### 1.C++多线程历史

编译器供应商可以自由地向语言添加扩展，添加C语言中流行的多线程API———POSIX标准中的C标准和Microsoft Windows API中的那些———这就使得很多C++编译器供应商通过各种平台相关扩展来支持多线程。

这种编译器支持一般受限于只能使用平台相关的C语言API，并且该C++运行库(例如,异常处理机制的代码)能在多线程情况下正常工作。因为编译器和处理器的实际表现很不错了，所以在少数编译器供应商提供正式的多线程感知内存模型之前，程序员们已经编写了大量的C++多线程程序了。

#### 2.新标准支持并发

所有的这些随着C++11标准的发布而改变了，新标准中不仅有了一个全新的线程感知内存模型，C++标准库也扩展了：包含了用于管理线程(参见第2章)、保护共享数据(参见第3章)、线程间同步操作(参见第4章)，以及低级原子操作(参见第5章)的各种类。

---

## II. 线程管理

### 1.线程管理的基础

#### (1).启动线程

使用C++线程库启动线程，可以归结为构造`std::thread`对象：

```c++
void do_some_work();
std::thread my_thread(do_some_work);
```

为了让编译器识别`std::thread`类，这个简单的例子也要包含`<thread>`头文件。如同大多数C++标准库一样， `std::thread` 可以用可调用类型构造，将带有函数调用符类型的实例传入`std::thread`类中，替换默认的构造函数。

```c++
class background_task
{
public:
  void operator()() const
  {
    do_something();
    do_something_else();
  }
};

background_task f;
std::thread my_thread(f);
```

代码中，提供的函数对象会复制到新线程的存储空间当中，函数对象的执行和调用都在线程的内存空间中进行。函数对象的副本应与原始函数对象保持一致，否则得到的结果会与我们的期望不同。

值得注意的是如果传递了一个临时变量，而不是一个命名的变量；C++编译器会将其解析为函数声明，而不是类型对象的定义。

```c++
std::thread my_thread(background_task());
```

>   这里 **相当与声明了一个名为my_thread的函数** ，这个函数带有一个参数(函数指针指向没有参数并返回background_task对象的函数)，返回一个`std::thread`对象的函数，而非启动了一个线程。

可以避免这个问题的方法。如下所示：

``` C++
std::thread my_thread((background_task()));  // 1
std::thread my_thread{background_task()};    // 2
std::thread my_thread([]{
  do_something();
  do_something_else();
});//直接使用Lambda表达式
```

#### (2).等待线程

> 为了防止主线程结束之前，使用局部变量引用的子线程还在调用，就需要让主线程等待子线程。

如果需要等待线程，相关的`std::thread`实例需要使用join()。join()是简单粗暴的等待线程完成或不等待。

>  当你需要对等待中的线程有更灵活的控制时，比如，看一下某个线程是否结束，或者只等待一段时间(超过时间就判定为超时)。想要做到这些，你需要使用其他机制来完成，比如条件变量和*期待*(futures)，相关的讨论将会在第4章继续。

调用join()的行为，会清理了线程相关的存储部分，这样`std::thread`对象将不再与已经完成的线程有任何关联。这意味着，只能对一个线程使用一次join();一旦已经使用过join()，`std::thread`对象就不能再次加入了，当对其使用joinable()时，将返回false。

#### (3).异常情况下的等待

> 需要确保局部状态的线程在异常下，函数结束之前退出并结束，使用try/catch很啰嗦，建议使用“资源获取即初始化方式”(RAII，Resource Acquisition Is Initialization)。

```c++
class thread_guard{
  	std::thread& t;
public:
    explicit thread_guard(std::thread& t_):t(t_) {}
    ~thread_guard() {
      if(t.joinable()){ // 1

        t.join();      // 2
      }
    }
    thread_guard(thread_guard const&)=delete;   // 3
    thread_guard& operator=(thread_guard const&)=delete;
};

struct func; 

void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
}    // 4
```

+ 当线程执行到④处时，局部对象就要被逆序销毁了。因此，thread_guard对象g是第一个被销毁的，这时在析构函数②中线程被结合，即使do_something_in_current_thread抛出一个异常而退出的情况下也会发生。
+ 拷贝构造函数和拷贝赋值操作被标记为`=delete`③，是为了不让编译器自动生成它们。直接对一个对象进行拷贝或赋值是危险的，因为这可能会弄丢已经加入的线程。通过删除声明，任何尝试给thread_guard对象赋值的操作都会引发一个编译错误。

#### (4).后台运行线程 

通常称分离线程为 **守护线程** (daemon threads), UNIX中守护线程是指，没有任何显式的用户接口，并在后台运行的线程。

这种线程的特点:

+ 一方面是长时间运行；线程的生命周期可能会从某一个应用起始到结束，可能会在后台监视文件系统，还有可能对缓存进行清理，亦或对数据结构进行优化。
+ 另一方面，分离线程的另一方面只能确定线程什么时候结束， **发后即忘** (fire and forget)的任务就使用到线程的这种方式。

调用`std::thread`成员函数detach()来分离一个线程。之后，相应的`std::thread`对象就与实际执行的线程无关了，并且这个线程也无法加入：

```c++
std::thread t(do_background_work);
t.detach();
assert(!t.joinable());
```

为了从`std::thread`对象中分离线程(前提是有可进行分离的线程),不能对没有执行线程的`std::thread`对象使用detach(),也是join()的使用条件，并且要用同样

### 2.传递参数给线程函数

#### 情况1

```c++
void f(int i,std::string const& s);
void oops(int some_param)
{
  char buffer[1024]; // 1
  sprintf(buffer, "%i",some_param);
  std::thread t(f,3,buffer); // 2
  t.detach();
}
```

buffer②是一个指针变量，指向本地变量，然后本地变量通过buffer传递到新线程中②。并且，函数有很有可能会在字面值转化成`std::string`对象之前 **崩溃** (oops)，从而导致一些未定义的行为。

解决方案就是在传递到`std::thread`构造函数之前就将字面值转化为`std::string`对象：

```c++
void f(int i,std::string const& s);
void not_oops(int some_param)
{
    char buffer[1024];
    sprintf(buffer,"%i",some_param);
    std::thread t(f,3,std::string(buffer));  // 使用std::string，避免悬垂指针
    t.detach();
}
```

#### 情况2

期望传递一个引用，但整个对象被复制了。当线程更新一个引用传递的数据结构时，这种情况就可能发生，比如：

```c++
void update_data_for_widget(widget_id w,widget_data& data); // 1
void oops_again(widget_id w)
{
    widget_data data;
    std::thread t(update_data_for_widget,w,data); // 2
    display_status();
    t.join();
    process_widget_data(data); // 3
}
```

+ 虽然update_data_for_widget①的第二个参数期待传入一个引用，但是`std::thread`的构造函数②并不知晓；

+ 构造函数无视函数期待的参数类型，并盲目的拷贝已提供的变量。当线程调用update_data_for_widget函数时，传递给函数的参数是data变量内部拷贝的引用，而非数据本身的引用。

+ 因此，当线程结束时，内部拷贝数据将会在数据更新阶段被销毁，且process_widget_data将会接收到没有修改的data变量③。

可以使用`std::ref`将参数转换成引用的形式，从而可将线程的调用改为以下形式：

```c++
std::thread t(update_data_for_widget,w,std::ref(data));
```

在这之后，update_data_for_widget就会接收到一个data变量的引用，而非一个data变量拷贝的引用。

#### 情况3

因为`std::thread`构造函数和`std::bind`的操作都在标准库中定义好了，可以传递一个成员函数指针作为线程函数，并提供一个合适的对象指针作为第一个参数：

```c++
class X{
public:
  	void do_lengthy_work();
};
X my_x;
std::thread t(&X::do_lengthy_work,&my_x); // 1
```

这段代码中，新线程将my_x.do_lengthy_work()作为线程函数；my_x的地址①作为指针对象提供给函数。

#### 情况4

如果提供的参数可以 **移动**，但不能 **拷贝** 。比如`std::unique_ptr`就是这样一种类型。

移动操作可以将对象转换成可接受的类型，例如:函数参数或函数返回的类型。当原对象是一个临时变量时，自动进行移动操作，但当原对象是一个命名变量，那么转移的时候就需要使用`std::move()`进行显示移动。

```c++
void process_big_object(std::unique_ptr<big_object>);

std::unique_ptr<big_object> p(new big_object);
p->prepare_data(42);
std::thread t(process_big_object,std::move(p));
```

通过在`std::thread`的构造函数中指定`std::move(p)`,big_object对象的所有权就被首先转移到新创建线程的的内部存储中，之后传递给process_big_object函数。

### 3.转移线程所有权

假设要写一个在后台启动线程的函数，想通过新线程返回的所有权去调用这个函数，而不是等待线程结束再去调用；或完全与之相反的想法：创建一个线程，并在函数中转移所有权，都必须要等待线程结束。总之，新线程的所有权都需要转移。这就是移动引入`std::thread`的原因。

`std::thread`支持移动，就意味着线程的所有权很容易地从一个函数中被转移出去。

```c++
std::thread f(){
  void some_function();
  return std::thread(some_function);
}

std::thread g(){
  void some_other_function(int);
  std::thread t(some_other_function,42);
  return t;
}
```

如果要把所有权转移到函数中，它只能以值的形式接受 `std::thread` 的实例作为其中一个参数。

```c++
void f(std::thread t);
void g(){
  void some_function();
  f(std::thread(some_function));
  std::thread t(some_function);
  f(std::move(t));
}
```

定义了scoped_thread类，使得某个对象转移了线程的所有权后，不能对线程进行加入或分离。确保了线程程序退出前完成。

```c++
class scoped_thread{
  	std::thread t;
public:
  	explicit scoped_thread(std::thread t_): t(std::move(t_)) {             // 1
    if(!t.joinable())                                     // 2
      	throw std::logic_error(“No thread”);
  	}
    ~scoped_thread(){
      	t.join();                                            // 3
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};

struct func; // 定义在清单2.1中

void f(){
    int some_local_state;
    scoped_thread t(std::thread(func(some_local_state)));    // 4
    do_something_in_current_thread();
}                 
```

+ 这里新线程是直接传递到scoped_thread中④，而非创建一个独立的命名变量。
+ 当主线程到达f()函数的末尾时，scoped_thread对象将会销毁，然后加入③到的构造函数①创建的线程对象中去。
+ 而在清单2.3中的thread_guard类，就要在析构的时候检查线程是否"可加入"。这里把检查放在了构造函数中②，并且当线程不可加入时，抛出异常。

### 4.运行时决定线程数量

`std::thread::hardware_concurrency()`在新版C++标准库中是一个很有用的函数。这个函数将返回能同时并发在一个程序中的线程数量。

> 例如，多核系统中，返回值可以是CPU核芯的数量。返回值也仅仅是一个提示，当系统信息无法获取时，函数也会返回0。但是，这也无法掩盖这个函数对启动线程数量的帮助。

### 5.识别线程

线程标识类型是`std::thread::id`，可以通过两种方式进行检索。

+ 第一种，可以通过调用`std::thread`对象的成员函数`get_id()`来直接获取。如果`std::thread`对象没有与任何执行线程相关联，`get_id()`将返回`std::thread::type`默认构造值，这个值表示“没有线程”。
+ 第二种，当前线程中调用`std::this_thread::get_id()`(这个函数定义在`<thread>`头文件中)也可以获得线程标识。

## III 在线程间共享数据

### 1. 线程之间共享数据的问题

当涉及到共享数据时，问题很可能是因为共享数据修改所导致。

+ 如果共享数据是只读的，那么只读操作不会影响到数据，更不会涉及对数据的修改，所以所有线程都会获得同样的数据。
+ 但是，当一个或多个线程要修改共享数据时，就会产生很多麻烦。这种情况下，就必须小心谨慎，才能确保一切所有线程都工作正常。

**不变量** (invariants)是对于特殊结构体的描述；比如，“变量包含列表中的项数”。不变量通常会在一次更新中被破坏，特别是比较复杂的数据结构，或者一次更新就要改动很大的数据结构。

破坏不变量的后果是多样，无论结果如何，都是并行代码常见错误：竞态条件。

#### (1).竞态条件

并发中竞争条件的形成，取决于一个以上线程的相对执行顺序，每个线程都抢着完成自己的任务。当不变量遭到破坏时，才会产生条件竞争。并发中对数据的条件竞争通常表示为恶性条件竞争，我们对不产生问题的良性条件竞争不感兴趣。

#### (2).解决方法

+ 最简单的办法就是对数据结构采用某种保护机制，确保只有进行修改的线程才能看到不变量被破坏时的中间状态。从其他访问线程的角度来看，修改不是已经完成了，就是还没开始。

+ 对数据结构和不变量的设计进行修改，修改完的结构必须能完成一系列不可分割的变化，也就是保证每个不变量保持稳定的状态，这就是所谓的无锁编程。

  > 这种方式很难得到正确的结果。如果到这个级别，无论是内存模型上的细微差异，还是线程访问数据的能力，都会让工作变的复杂。

+ 使用事务的方式去处理数据结构的更新(如同对数据库进行更新一样)。所需的一些数据和读取都存储在事务日志中，然后将之前的操作合为一步，再进行提交。当数据结构被另一个线程修改后，或处理已经重启的情况下，提交就会无法进行，这称作为“软件事务内存”。

### 2.使用互斥量保护共享数据

#### (1).C++ 中使用互斥量

C++中通过实例化`std::mutex`创建互斥量，通过调用成员函数lock()进行上锁，unlock()进行解锁。

> 不推荐实践中直接去调用成员函数，记住在每个函数出口都要去调用unlock()，也包括异常的情况，这样过于繁琐，容易忘记解锁操作。

C++标准库为互斥量提供了一个RAII语法的模板类`std::lock_guard`，其会在构造的时候提供已锁的互斥量，并在析构的时候进行解锁，从而保证了一个已锁的互斥量总是会被正确的解锁。

#### (2).需要精心组织代码来保护共享数据

 **切勿将受保护数据的指针或引用传递到互斥锁作用域之外，无论是函数返回值，还是存储在外部可见内存，亦或是以参数的形式传递到用户提供的函数中去。**

#### (3). 接口内在的条件竞争

stack容器返回一个拷贝而非引用(即遵循了3.2.2节的准则)，对内部数据使用一个互斥量进行保护，不过这个接口仍存在条件竞争。

这个问题不仅存在于基于互斥量实现的接口中，在无锁实现的接口中，条件竞争依旧会产生。这是接口的问题，与其实现方式无关。

```c++
template<typename T,typename Container=std::deque<T> >
class stack
{
public:
  explicit stack(const Container&);
  explicit stack(Container&& = Container());
  template <class Alloc> explicit stack(const Alloc&);
  template <class Alloc> stack(const Container&, const Alloc&);
  template <class Alloc> stack(Container&&, const Alloc&);
  template <class Alloc> stack(stack&&, const Alloc&);

  bool empty() const;
  size_t size() const;
  T& top();
  T const& top() const;
  void push(T const&);
  void push(T&&);
  void pop();
  void swap(stack&&);
};
```

虽然empty()和size()可能在被调用并返回时是正确的，但其的结果是不可靠的；当它们返回后，其他线程就可以自由地访问栈，并且可能push()多个新元素到栈中，也可能pop()一些已在栈中的元素。这样的话，之前从empty()和size()得到的结果就有问题了。

定义线程安全的堆栈：

```c++
#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception{
  	const char* what() const throw(){ return "empty stack!";}
};

template<typename T>
class threadsafe_stack{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
  	threadsafe_stack(): data(std::stack<T>()){}
  
    threadsafe_stack(const threadsafe_stack& other) {
      std::lock_guard<std::mutex> lock(other.m);
      data = other.data; // 1 在构造函数体中的执行拷贝
    }
  
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
  
    void push(T new_value){
      std::lock_guard<std::mutex> lock(m);
      data.push(new_value);
    }

    std::shared_ptr<T> pop(){
      std::lock_guard<std::mutex> lock(m);
      if(data.empty()) throw empty_stack(); // 在调用pop前，检查栈是否为空
      std::shared_ptr<T> const res(std::make_shared<T>(data.top())); // 在修改堆栈前，分配出返回值
      data.pop();
      return res;
    }

    void pop(T& value){
      std::lock_guard<std::mutex> lock(m);
      if(data.empty()) throw empty_stack();
      value=data.top();
      data.pop();
    }

    bool empty() const {
      std::lock_guard<std::mutex> lock(m);
      return data.empty();
    }
};
```

+ 对top()和pop()函数的讨论中，锁的粒度太小，需要保护的操作并未全覆盖到，从而出现竞态条件。

+ 不过，锁住的颗粒过大同样会有问题。还有一个问题，一个全局互斥量要去保护全部共享数据，会使得并发优势不再。
+ 细粒度锁定方案的一个问题就是有时为了保护操作中的所有数据，需要不止一个互斥元。但是需要多个互斥元时，时，有时会出现另一个潜在的问题将：死锁。

#### (4). 死锁的描述与解决

一对线程需要对他们所有的互斥量做一些操作，其中每个线程都有一个互斥量，且等待另一个解锁。这样没有线程能工作，因为他们都在等待对方释放互斥量。这种情况就是死锁，它的最大问题就是由两个或两个以上的互斥量来锁定一个操作。

C++标准库有办法解决这个问题，`std::lock`——可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险)。

```c++
// 这里的std::lock()需要包含<mutex>头文件
class some_big_object;
void swap(some_big_object& lhs,some_big_object& rhs);
class X{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd):some_detail(sd){}

    friend void swap(X& lhs, X& rhs){
      if(&lhs==&rhs)
          return;
      std::lock(lhs.m,rhs.m); // 1
      std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock); // 2
      std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock); // 3
      swap(lhs.some_detail,rhs.some_detail);
    }
};
```

+ 首先，检查参数是否是不同的实例，因为操作试图获取`std::mutex`对象上的锁，所以当其被获取时，结果很难预料。(一个互斥量可以在同一线程上多次上锁，标准库中`std::recursive_mutex`提供这样的功能。详情见3.3.3节)。

+ 然后，调用`std::lock()`①锁住两个互斥量，并且两个`std:lock_guard`实例已经创建好②③。提供`std::adopt_lock`参数除了表示`std::lock_guard`对象可获取锁之外，还将锁交由`std::lock_guard`对象管理，而不需要`std::lock_guard`对象再去构建新的锁。
+ 这样，就能保证在大多数情况下，函数退出时互斥量能被正确的解锁(保护操作可能会抛出一个异常)，也允许使用一个简单的“return”作为返回。

**避免死锁的进阶指导：**

+ 避免嵌套锁

+ 避免在持有锁时调用用户所提供的代码

+ 使用固定顺序获取锁

+ 使用锁层次结构

  > 对应用进行分层，并且识别在给定层上所有可上锁的互斥量。只有锁层次更高的才能对层次低的再次上锁

#### (5).更灵活的锁——std::unique_lock

`std::unqiue_lock`使用更为自由的不变量，这样`std::unique_lock`实例不会总与互斥量的数据类型相关，使用起来要比`std:lock_guard`更加灵活。

+ 可将`std::adopt_lock`作为第二个参数传入构造函数，对互斥量进行管理；
+ 也可以将`std::defer_lock`作为第二个参数传递进去，表明互斥量应保持解锁状态。

交换操作中`std::lock()`和`std::unique_lock`的使用

```c++
class some_big_object;
void swap(some_big_object& lhs,some_big_object& rhs);
class X{
private:
    some_big_object some_detail;
    std::mutex m;
public:
    X(some_big_object const& sd):some_detail(sd){}
    friend void swap(X& lhs, X& rhs){
        if(&lhs==&rhs)
            return;
        std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock); // 1 
        std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock); // 1 std::def_lock 留下未上锁的互斥量
        std::lock(lock_a,lock_b); // 2 互斥量在这里上锁
        swap(lhs.some_detail,rhs.some_detail);
    }
};
```

#### (6).在作用域之间转移锁

`std::unique_lock`实例没有与自身相关的互斥量，一个互斥量的所有权可以通过移动操作，在不同的实例中进行传递。 `std::unique_lock`是可移动，但不可赋值的类型。

```c++
std::unique_lock<std::mutex> get_lock(){
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex);
    prepare_data();
    return lk;  // 1
}
void process_data(){
    std::unique_lock<std::mutex> lk(get_lock());  // 2
    do_something();
}
```

lk在函数中被声明为自动变量，它不需要调用`std::move()`，可以直接返回①(编译器负责调用移动构造函数)。process_data()函数直接转移`std::unique_lock`实例的所有权②，调用do_something()可使用的正确数据(数据没有受到其他线程的修改)。

#### (7).锁定在恰当的粒度

锁的粒度是一个 **摆手术语** (hand-waving term)，用来描述通过一个锁保护着的数据量大小。 **一个细粒度锁** (a fine-grained lock)能够保护较小的数据量， **一个粗粒度锁** (a coarse-grained lock)能够保护较多的数据量。选择粒度对于锁来说很重要：

+ 当有线程持有锁的时间过长，这就会增加等待的时间

`std::unique_lock`在这种情况下工作正常，在调用unlock()时，代码不需要再访问共享数据；而后当再次需要对共享数据进行访问时，就可以再调用lock()了。

```c++
void get_and_process_data(){
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process=get_next_data_chunk();
    my_lock.unlock();  // 1 不要让锁住的互斥量越过process()函数的调用
    result_type result=process(data_to_process);
    my_lock.lock(); // 2 为了写入数据，对互斥量再次上锁
    write_result(data_to_process,result);
}
```

+ 不需要让锁住的互斥量越过对process()函数的调用，所以可以在函数调用①前对互斥量手动解锁，并且在之后对其再次上锁②。

比较操作符中一次锁住一个互斥量：

```c++
class Y{
private:
    int some_detail;
    mutable std::mutex m;
    int get_detail() const{
      	std::lock_guard<std::mutex> lock_a(m);  // 1
      	return some_detail;
    }
public:
    Y(int sd):some_detail(sd){}

    friend bool operator==(Y const& lhs, Y const& rhs){
        if(&lhs==&rhs)
          	return true;
        int const lhs_value=lhs.get_detail();  // 2
        int const rhs_value=rhs.get_detail();  // 3
        return lhs_value==rhs_value;  // 4
    }
};
```

在这个例子中，比较操作符首先通过调用get_detail()成员函数检索要比较的值②③，函数在索引值时被一个锁保护着①。比较操作符会在之后比较索引出来的值④。注意：虽然这样能减少锁持有的时间，一个锁只持有一次(这样能消除死锁的可能性)，这里有一个微妙的语义操作同时对两个锁住的值进行比较。

### 3.保护共享数据的代替设施

#### (1). 保护共享数据的初始化过程

**延迟初始化** (Lazy initialization)在单线程代码很常见——每一个操作都需要先对源进行检查，为了了解数据是否被初始化，然后在其使用前决定，数据是否需要初始化：

```c++
std::shared_ptr<some_resource> resource_ptr;
void foo(){
    if(!resource_ptr){
        resource_ptr.reset(new some_resource);  // 1
    }
    resource_ptr->do_something();
}
```

当共享数据对于并发访问是安全的，①是转为多线程代码时，需要保护的，但是下面天真的转换会使得线程资源产生不必要的序列化。这是因为每个线程必须等待互斥量，为了确定数据源已经初始化了。

使用一个互斥量的延迟初始化(线程安全)过程

```c++
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void foo(){
    std::unique_lock<std::mutex> lk(resource_mutex);  // 所有线程在此序列化 
    if(!resource_ptr){
      resource_ptr.reset(new some_resource);  // 只有初始化过程需要保护 
    }
    lk.unlock();
    resource_ptr->do_something();
}
```

C++标准库提供了`std::once_flag`和`std::call_once`来处理这种情况。使用`std::call_once`比显式使用互斥量消耗的资源更少，特别是当初始化完成后。

```c++
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;  // 1

void init_resource(){
  	resource_ptr.reset(new some_resource);
}

void foo(){
    std::call_once(resource_flag,init_resource);  // 可以完整的进行一次初始化
    resource_ptr->do_something();
}
```

在这个例子中，`std::once_flag`①和初始化好的数据都是命名空间区域的对象，但是 `std::call_once()` 可仅作为延迟初始化的类型成员。

#### (2).保护很少更新的数据结构

对于很少更新的资源，要求线程独占数据结构的访问权，直到其完成更新操作。当更新完成，数据结构对于并发多线程访问又会是安全的。

使用`std::mutex`来保护数据结构，显的有些反应过度(因为在没有发生修改时，它将削减并发读取数据的可能性)。这里需要另一种不同的互斥量，这种互斥量常被称为“读者-作者锁”，因为其允许两种不同的使用方式：

+ 一个“作者”线程独占访问和共享访问
+ 多个“读者”线程并发访问。

使用`boost::shared_mutex`来做同步。

+ 对于更新操作，可以使用`std::lock_guard<boost::shared_mutex>`和`std::unique_lock<boost::shared_mutex>`上锁。作为`std::mutex`的替代方案，与`std::mutex`所做的一样，这就能保证更新线程的独占访问。
+ 因为其他线程不需要去修改数据结构，所以其可以使用`boost::shared_lock<boost::shared_mutex>`获取访问权。这与使用`std::unique_lock`一样，除非多线程要在同时获取同一个`boost::shared_mutex`上有共享锁。
+ 唯一的限制： **当任一线程拥有一个共享锁时，这个线程就会尝试获取一个独占锁，直到其他线程放弃他们的锁；同样的，当任一线程拥有一个独占锁时，其他线程就无法获得共享锁或独占锁，直到第一个线程放弃其拥有的锁。**

```c++
#include <map>
#include <string>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

class dns_entry;
class dns_cache{
    std::map<std::string,dns_entry> entries;
    mutable boost::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const& domain) const{
        boost::shared_lock<boost::shared_mutex> lk(entry_mutex);  // 1
        std::map<std::string,dns_entry>::const_iterator const it = entries.find(domain);
        return (it==entries.end())?dns_entry():it->second;
    }
    void update_or_add_entry(std::string const& domain,dns_entry const& dns_details){
        std::lock_guard<boost::shared_mutex> lk(entry_mutex);  // 2
        entries[domain]=dns_details;
    }
};
```

+ find_entry()使用`boost::shared_lock<>`来保护共享和只读权限①；这就使得多线程可以同时调用find_entry()，且不会出错。
+ 另一方面，update_or_add_entry()使用`std::lock_guard<>`实例，当表格需要更新时②，为其提供独占访问权限；update_or_add_entry()函数调用时，独占锁会阻止其他线程对数据结构进行修改，并且阻止线程调用find_entry()。

#### (3).递归锁

当一个线程已经获取一个`std::mutex`时(已经上锁)，并对其再次上锁，这个操作就是错误的，并且继续尝试这样做的话，就会产生未定义行为。然而，在某些情况下，一个线程尝试获取同一个互斥量多次，而没有对其进行一次释放是可以的。之所以可以，是因为`C++`标准库提供了`std::recursive_mutex`类。其功能与`std::mutex`类似，除了你可以从同一线程的单个实例上获取多个锁。互斥量锁住其他线程前，你必须释放你拥有的所有锁，所以当你调用lock()三次时，你也必须调用unlock()三次。正确使用`std::lock_guard<std::recursive_mutex>`和`std::unique_lock<std::recursive_mutex>`可以帮你处理这些问题。

