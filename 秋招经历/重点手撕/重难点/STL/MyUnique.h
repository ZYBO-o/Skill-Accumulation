#include<iostream>
using namespace std;

template<typename T>
class MyUniquePtr
{
private:
    T* mPtr;
public:
   explicit MyUniquePtr(T* ptr = nullptr) :mPtr(ptr) {}

    ~MyUniquePtr() {  
        if(mPtr)    
            delete mPtr;
    }

    MyUniquePtr(MyUniquePtr &&p) noexcept;
    MyUniquePtr& operator=(MyUniquePtr &&p) noexcept;

    MyUniquePtr(const MyUniquePtr &p) = delete;
    MyUniquePtr& operator=(const MyUniquePtr &p) = delete;

    T* operator*() const noexcept {return mPtr;}
    T& operator->()const noexcept {return *mPtr;}
    explicit operator bool() const noexcept{return mPtr;}

    void reset(T* q = nullptr) noexcept
    {
        if(q != mPtr){
            if(mPtr)
                delete mPtr;
            mPtr = q;
        }
    }

    T* release() noexcept
    {
        T* res = mPtr;
        mPtr = nullptr;
        return res;
    }

    T* get() const noexcept {return mPtr;}
    
    void swap(MyUniquePtr &p) noexcept
    {
        using std::swap;
        swap(mPtr, p.mPtr);
    }

};

template<typename T>
MyUniquePtr<T>& MyUniquePtr<T>::operator=(MyUniquePtr &&p) noexcept
{
    swap(*this, p);
    return *this;
}

template<typename T>
MyUniquePtr<T> :: MyUniquePtr(MyUniquePtr &&p) noexcept : mPtr(p.mPtr)
{
    p.mPtr == NULL;
}