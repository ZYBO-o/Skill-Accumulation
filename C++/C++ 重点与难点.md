## 一.C++中的资源管理

### 1.程序的存储空间布局

C程序一直由以下几个部分组成：

<div align = center><img src="../图片/7-4.png" width="300px" /></div>

+  **正文段。** 这是由CPU执行的机器指令部分。通常正文段是可共享的，但通常也只能制度，防止程序由于意外而修改其指令。
+  **初始化数据段。** 通常将此段称为数据段,它包含了程序中需明确地赋初值的变量。
+  **未初始化数据段。** 通常将此段称为bss段 (名称来源于早期汇编程序一个操作符,意思是“由符号开始的块”( block started by symbol))。在程序开始执行之前， **内核将此段中的数据初始化为0或空指针** 。
+  **栈。** 自动变量以及每次函数调用时所需保存的信息都存放在此段中。
+  **堆。** 通常在堆中进行动态存储分配。

> +  a.out 中还有其他类型的段，如符号表的段，包含调试信息的段以及包含动态共享库链接表的段等。这些部分并不装载到进程执行的程序映像中。
> +  未初始化数据段的内容并不存放在磁盘程序文件中。其原因是,内核在程序开始运行前将它们都设置为0。需要存放在磁盘程序文件中的段只有正文段和初始化数据段

### 2.堆，栈 与 RAII





### 3.智能指针的引入

#### (1).什么是智能指针

智能指针是一个类，这个类的构造函数中传入一个普通指针，析构函数中释放传入的指针。智能指针的类都是栈上的对象，所以当函数（或程序）结束时会自动被释放，

#### (2).最常用的智能指针

+ `std::auto_ptr`，有很多问题。 不支持复制（拷贝构造函数）和赋值（operator =），但复制或赋值的时候不会提示出错。因为不能被复制，所以不能被放入容器中。

+ C++11引入的 `unique_ptr`， 也不支持复制和赋值，但比auto_ptr好，直接赋值会编译出错。实在想赋值的话，需要使用：`std::move`。

  + 例如：

    ```c++ 
    std::unique_ptr<int> p1(new int(5));
    std::unique_ptr<int> p2 = p1; // 编译会出错
    std::unique_ptr<int> p3 = std::move(p1); // 转移所有权, 现在那块内存归p3所有, p1成为无效的指针.
    ```

+ C++11或boost的shared_ptr，基于引用计数的智能指针。可随意赋值，直到内存的引用计数为0的时候这个内存会被释放。

+ C++11或boost的weak_ptr，弱引用。 引用计数有一个问题就是互相引用形成环，这样两个指针指向的内存都无法释放。需要手动打破循环引用或使用weak_ptr。顾名思义，weak_ptr是一个弱引用，只引用，不计数。如果一块内存被shared_ptr和weak_ptr同时引用，当所有shared_ptr析构了之后，不管还有没有weak_ptr引用该内存，内存也会被释放。所以weak_ptr不保证它指向的内存一定是有效的，在使用之前需要检查weak_ptr是否为空指针。

#### (3).智能指针的实现

> 下面是一个基于引用计数的智能指针的实现，需要实现构造，析构，拷贝构造，=操作符重载，重载*-和>操作符。

```c++
#include <iostream>

using namespace  std;

template <typename T>
class SmartPointer {
private:
    T *_ptr;
    size_t *_reference_count;
    void releaseCount() {
        if(_ptr) {
            (*_reference_count)--;
            if((*_reference_count)==0) {
                delete _ptr;
                delete _reference_count;
            }
        }
    }

public:
    //构造函数
    SmartPointer(T* p = nullptr): _ptr(p), _reference_count(new size_t){
        if(p)
            *_reference_count = 1;
        else
            *_reference_count = 0;
    }
    //拷贝构造函数
    SmartPointer(const SmartPointer& src) {
        if(this != &src) {
            _ptr = src._ptr;
            _reference_count = src._reference_count;
            (*_reference_count)++;
        }
    }
    //重载赋值操作符
    SmartPointer& operator= (const SmartPointer& src) {
        if(_ptr==src._ptr) {
            return *this;
        }
       // releaseCount();
        _ptr = src._ptr;
        _reference_count = src._reference_count;
        (*_reference_count)++;
        return *this;
    }

    //重载操作符
    T& operator*() {
        if(_ptr) {
            return *_ptr;
        }
        //throw exception
    }
    //重载操作符
    T* operator->() {
        if(_ptr) {
            return _ptr;
        }
        //throw exception
    }
    //析构函数
    ~SmartPointer() {
        if (--(*_reference_count) == 0) {
            delete _ptr;
            delete _reference_count;
        }
    }
    size_t getCount() {
        return *_reference_count;
    }

};

int main()
{
    //这边是动态创建的 char('a');所以可以进行delete
    SmartPointer<char> cp1(new char('a'));
    SmartPointer<char> cp2(cp1);
    SmartPointer<char> cp3;
    cp3 = cp2;
    cp3 = cp1;
    cp3 = cp3;
    cout << *cp3 << endl;												//a
    cout << cp3.getCount() << endl;							//3
    SmartPointer<char> cp4(new char('b'));
    cp3 = cp4;
    cout << *cp3 << endl;												//b
    cout << cp3.getCount() << endl;							//2
}
```







