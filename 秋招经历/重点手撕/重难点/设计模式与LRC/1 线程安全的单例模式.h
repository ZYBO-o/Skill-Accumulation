#include<iostream>


//========================饿汉模式下的线程安全========================
//这里使用了局部静态变量，C++11机制可以保证它的初始化具有原子性，线程安全。
//这个对象保存在静态数据区，和全局变量是在一起的，而不是在堆中。
class Singleton2
{
private:
    Singleton2() = default;
    ~Singleton2() = default;
    Singleton2(const Singleton2&)=delete;
    Singleton2& operator=(const Singleton2&)=delete;
 private:
    static Singleton2* _pInstance;

public:
    static Singleton2* instance(){
        return _pInstance;
    }
    void writeWords() { }
};

//============================懒汉模式的线程安全==========================
class Singleton1{
public:
    static Singleton1* instance(){
        static Singleton1 _instance;
        return &_instance;
    }
private:
    Singleton1() {}
};


//在C++11中提供一种方法，使得函数可以线程安全的只调用一次。
//即使用std::call_once 和 std::once_flag。std::call_once是一种lazy load的很简单易用的机制。
template <class T>
class Singleton{
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
 private:
    static T* _instance;
    static std::once_flag _flag;
public:
    template <typename... Args>
    static T* instance(Args&&... args) {
        std::call_once(_flag, [&](){
            _instance = new T(std::forward<Args>(args)...);
        });
        return _instance;
    }
 
    static void destroy() {
        if (_instance) {
            delete _instance;
            _instance = NULL;
        }
    }
};
template <class T>
T* Singleton<T>::_instance = NULL; 
 
template <class T>
std::once_flag Singleton<T>::_flag;

