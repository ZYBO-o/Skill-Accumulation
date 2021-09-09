#include<iostream>

class Singleton{
public:
    // 注意返回的是引用。
    static Singleton& getInstance(){
        //C++11中局部静态变量不仅只会初始化一次，而且还是线程安全的。
        static Singleton m_instance;  //局部静态变量
        return m_instance;
    }
private:
    Singleton(); //私有构造函数，不允许使用者自己生成对象
    Singleton(const Singleton& other);
};
//==========================================================
template<typename T>
class SingletTemplte
{
public:
    static T& getInstance() {
        static T value_; //静态局部变量
        return value_;
    }
private:
    SingletTemplte();
    ~SingletTemplte();
    SingletTemplte(const SingletTemplte&); //拷贝构造函数
    SingletTemplte& operator=(const SingletTemplte&); // =运算符重载
};

//========================饿汉模式========================
class Singleton1
{
private:
    Singleton1() = default;
    ~Singleton1() = default;
    Singleton1(const Singleton1&)=delete;
    Singleton1& operator=(const Singleton1&)=delete;
 private:
    static Singleton1* _pInstance;

public:
    static Singleton1* instance(){
        return _pInstance;
    }
    void writeWords() { }
};
 
Singleton1* Singleton1::_pInstance = new Singleton1(); 

//========================懒汉模式==========================
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
        if (!_pInstance) {
            _pInstance = new Singleton2();
        }
        return _pInstance;
    }
 
    void writeWords() { }
};
 
Singleton2* Singleton2::_pInstance = NULL;