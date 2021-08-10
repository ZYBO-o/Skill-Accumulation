#include <iostream>
#include "UseSharePointer.h"
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
        releaseCount();
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
    /*
    SmartPointer<char> cp1(new char('a'));
    SmartPointer<char> cp2(cp1);
    SmartPointer<char> cp3;
    cp3 = cp2;
    cp3 = cp1;
    cp3 = cp3;
    cout << *cp3 << endl;
    cout << cp3.getCount() << endl;
    SmartPointer<char> cp4(new char('b'));
    cp3 = cp4;
    cout << *cp3 << endl;
    cout << cp3.getCount() << endl;
     */
   // UseSharePointer();

}

