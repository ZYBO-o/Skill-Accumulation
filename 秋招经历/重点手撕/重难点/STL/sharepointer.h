template<typename T>
class MySharePointer {
private:
    T* _ptr;
    size_t *_reference_count;
    //减去引用次数
    void releaseCount() {
        if(_ptr) {
            if(--(*_reference_count) == 0) {
                //智能指针是为了解决动态内存的问题，所以肯定能使用delete
                delete _ptr;
                delete _reference_count;
            }
        }
    }
public:
    //构造函数
    MySharePointer(T* p = nullptr) : _ptr(p), _reference_count(new size_t) {
        if(_ptr)
            *_reference_count = 1;
        else
            *_reference_count = 0;
    }
    //拷贝构造函数
    MySharePointer(const MySharePointer& spr) {
        if(this != &spr) {
            _ptr = spr._ptr;
            _reference_count = spr._reference_count;
            (*_reference_count)++;
        }
    }
    //重载赋值运算符
    MySharePointer& operator =  (const MySharePointer& spr){
        if(_ptr == spr._ptr)
            return *this;
        //将自己所指的智能指针引用参数 -1
        releaseCount();
        _ptr = spr._ptr;
        _reference_count = spr._reference_count;
        (*_reference_count)++;
        return *this;
    }
    //重载操作符 *
    T& operator* () {
        if(_ptr)
            return *_ptr;
    }
    //重载操作符 ->
    T* operator-> (){
        if(_ptr)
            return _ptr;
    }
    //析构函数
    ~MySharePointer() {
        if(--(*_reference_count) == 0) {
            delete _ptr;
            delete _reference_count;
        }
    }
    //获取引用次数
    size_t getCount() {
        return *_reference_count;
    }

};