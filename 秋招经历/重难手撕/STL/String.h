#include <cstring>

class String {
private:
    char* m_data;
public:
    String(const char* str = nullptr);			//普通构造函数
    String(const String& other);				//拷贝构造函数
    ~String();								   //析构函数

    String& operator = (const String& other);	//赋值函数
    String operator + (const String& other);	//字符串连接
    char& operator [] (int index);            //下标
    bool operator == (const String& other);	   //判断长度
    bool operator != (const String& other);

    int size();									//返回长度

    friend std::ostream& operator << (std::ostream& output, String str);
    friend std::istream& operator >> (std::istream& input, String& str);
};

String::String(const char* str) {
    if(str == nullptr) {
        m_data = new char[1];
        m_data[1] = '\0';						//存放结束标志 ’\0’
    } else {
        int length = strlen(str);
        m_data = new char[length];
        strcpy(m_data, str);					//将c字符串拷贝至m_data中
    }
}

String::String(const String& other) {
    if(!other.m_data) {							//传入的字符串为空
        m_data = nullptr;
    } else {
        m_data = new char[strlen(other.m_data) + 1];
        strcpy(m_data, other.m_data);
    }
}

String::~String() {
    if(m_data) {
        delete[] m_data;						//删除动态内存并置空
        m_data = nullptr;
    }
}

String& String::operator = (const String& other) {
    if(this != &other) {
        delete[] m_data;
        if(!other.m_data) {
            m_data = nullptr;
        } else {
            m_data = new char[strlen(other.m_data) + 1];
            strcpy(m_data, other.m_data);
        }
    }
    return *this;
}

String String::operator + (const String& other) {
    String newString;
    if(!other.m_data) {
        newString = *this;
    } else if(!m_data) {
        newString = other;
    } else {
        newString.m_data = new char [strlen(m_data) + strlen(other.m_data) + 1];
        strcpy(newString.m_data, m_data);
        strcat(newString.m_data, other.m_data);	//将 other.m_data 添加到 newString.m_data 后，并添上新的 '\0'
    }
    return newString;
}

char& String::operator [] (int index) {
    if(index < 0 || index > this->size()) {
        std::cout << "索引错误！" << std::endl;
        exit(-1);
    } else {
        return m_data[index];
    }
}

bool String::operator == (const String& other) {
    if(strlen(m_data) != strlen(other.m_data)) {
        return false;
    } else {
        return strcmp(m_data, other.m_data) ? false : true;
    }
}

bool String::operator != (const String& other) {
    return (!this->operator==(other));
}

int String::size() {
    return strlen(m_data);
}

std::ostream& operator << (std::ostream& output, String str) {
    output << str.m_data;
    return output;
}

std::istream& operator >> (std::istream& input, String& str) {
    input >> str.m_data;
    return input;
}