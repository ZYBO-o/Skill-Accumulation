#include <iostream>

template<typename  T>
class Vector{
private:
    T* array;
    int size;
    int capacity;

public:
    //构造函数
    Vector() : array(nullptr), size(0), capacity(24) {}
    Vector(const T& t, unsigned int n) : array(new T[24 + n]), size(n), capacity(24 + n){
        for (int i = 0; i < n; ++i) {
            this->array[i] = t;
        }
    }

    Vector(const Vector<T>& other){
        this->size = other.size;
        this->capacity = other.capacity;
        this->array = new T[this->capacity];
        memcpy(this->array, other.array, this->size * sizeof(T));
    }

    //析构函数
    ~Vector() {
        if(array != nullptr) {
            delete[] array;
        }
    }
    //清除操作
    void clear() {
        this->array = nullptr;
    }
    //重载 []
    T& operator[] (int index) {
        if(index >= 0 && index < this->size)
            return this->array[index];
        else {
            std::cout << "index is out of range!!!" << endl;
            exit(-1);
        }
    }
    // = operator
    Vector<T>& operator = (const Vector<T>& other) {
        if(this->array != nullptr) {
            delete[] this->array;
            this->size = 0;
            this->capacity = 0;
            this->array = nullptr;
        }

        this->size = other.size;
        this->capacity = other.capacity;
        this->array = new T[this->size];
        memcpy(this->array, other.array, this->size * sizeof(T));
        return *this;
    }
    //插入末尾元素
    void push_back(const T& data) {
        if(this->array == nullptr) {
            this->capacity = 24;
            this->size = 0;
            this->array = new T[capacity];
        } else if(this->size == this->capacity) {
            //扩充两倍
            T* temp = new T[this->capacity * 2];
            memcpy(temp, this->array, this->size * sizeof(T));
            this->capacity *= 2;
            delete this->array;
            this->array = temp;
        }
        this->array[this->size] = data;
        this->size ++;
    }

    //删除最后一个元素
    void pop_back(){
        if(this->size>1){
            this->array[this->size - 1] = 0;
            this->size --;
        }
    }

    //插入操作
    void insert(int pos,T data) {
        if(pos >= 0 && pos <= this->size) {
            if(this->size == this->capacity) {
                //扩充两倍
                T* temp = new T[this->capacity * 2];
                memcpy(temp, this->array, this->size * sizeof(T));
                this->capacity *= 2;
                delete this->array;
                this->array = temp;
            }
            for(int i = this->size; i > pos; --i) {
                this->array[i] = this->array[i - 1];
            }
            this->array[pos] = data;
            this->size ++;
        } else {
            std::cout << "insert index is out of range!!!" << endl;
            exit(-1);
        }
    }

    void print(){
        if(this->array != nullptr) {
            for(int i = 0; i<this->size; i++){
                cout<<this->array[i]<<' ';
            }
            cout<<endl;
        }
    }

    int Size() {
        return this->size;
    }
    int Capacity() {
        return this->capacity;
    }
};