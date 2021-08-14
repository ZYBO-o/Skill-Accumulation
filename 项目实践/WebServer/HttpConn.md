## http——有限状态机

> 利用正则与状态机解析HTTP请求报文，实现处理静态资源的请求；

```c++
 源代码
├── code          
│   ├── poll
│		│		├── httpconn.h
│		│		├── httpconn.cpp
│		│		├── httprequest.h
│		│		├── httprequest.cpp
│		│		├── httpresponse.h
│		│		├── httpresponse.cpp   
```

## 一. HttpRequest 

### 1.httpRequest 类的定义

```c++
class HttpRequest {
public:
    enum PARSE_STATE {
        REQUEST_LINE,           //请求行
        HEADERS,                //消息头
        BODY,                   //消息体
        FINISH,                 //结束
    };
   
    enum HTTP_CODE {
        NO_REQUEST = 0,         //表示请求不完整，需要继续读取客户数据；
        GET_REQUEST,            //表示获得了一个完整的客户请求；
        BAD_REQUEST,            //表示客户请求有语法错误；
        NO_RESOURSE,            //表示没有资源
        FORBIDDENT_REQUEST,     //表示客户对资源没有足够的访问权限；
        FILE_REQUEST,           //表示获得了一个文件的客户请求
        INTERNAL_ERROR,         //表示示服务器内部错误；
        CLOSED_CONNECTION,      //表示客户端已经关闭连接了
    };
    
    HttpRequest() { Init(); }
    ~HttpRequest() = default;

    void Init();//初始化
    bool parse(Buffer& buff);//分析

    string path() const;
    string& path();
    string method() const;
    string version() const;
    string GetPost(const std::string& key) const;
    string GetPost(const char* key) const;
    bool IsKeepAlive() const;

private:
    bool ParseRequestLine_(const string& line);//分析请求行
    void ParseHeader_(const string& line);//分析消息头
    void ParseBody_(const string& line);//分析消息体
    void ParsePath_();//分析路径
    void ParsePost_();//分析提交  
    void ParseFromUrlencoded_();//分析编码
    static bool UserVerify(const string& name, const string& pwd, bool isLogin);//消息验证

    PARSE_STATE state_;
    string method_, path_, version_, body_;
    unordered_map<string, string> header_;
    unordered_map<string, string> post_;

    static const unordered_set<string> DEFAULT_HTML;
    static const unordered_map<string, int> DEFAULT_HTML_TAG;
    static int ConverHex(char ch);
};
```

### 2.简单的成员变量查看

```c++
std::string HttpRequest::path() const{
    return path_;
}
std::string& HttpRequest::path(){
    return path_;
}
std::string HttpRequest::method() const {
    return method_;
}
std::string HttpRequest::version() const {
    return version_;
}
std::string HttpRequest::GetPost(const std::string& key) const {
    assert(key != "");
    if(post_.count(key) == 1)
        return post_.find(key)->second;
    return "";
}
std::string HttpRequest::GetPost(const char* key) const {
    assert(key != nullptr);
    if(post_.count(key) == 1)
        return post_.find(key)->second;
    return "";
}
//查看是否连接
bool HttpRequest::IsKeepAlive() const {
    if(header_.count("Connection") == 1) {
        return header_.find("Connection")->second == "keep-alive" && version_ == "1.1";
    }
    return false;
}
```

### 3.初始化操作

```c++
const unordered_set<string> HttpRequest::DEFAULT_HTML{
            "/index", "/register", "/login",
             "/welcome", "/video", "/picture", };
const unordered_map<string, int> HttpRequest::DEFAULT_HTML_TAG {
            {"/register.html", 0}, {"/login.html", 1},  };
void HttpRequest::Init() {
    method_ = path_ = version_ = body_ = "";
    state_ = REQUEST_LINE;
    header_.clear();
    post_.clear();
}
```

### 4.解析请求信息

```c++
bool HttpRequest::parse(Buffer& buff) {
    const char CRLF[] = "\r\n";
    if(buff.ReadableBytes() <= 0) {
        return false;
    }
    while(buff.ReadableBytes() && state_ != FINISH) {
        const char* lineEnd = search(buff.Peek(), buff.BeginWriteConst(), CRLF, CRLF + 2);
        // 找到第一个"\r\n"结尾的字符串
        std::string line(buff.Peek(), lineEnd);
        //根据是什么内容来进行分析
        switch(state_)
        {
        case REQUEST_LINE://请求行
            if(!ParseRequestLine_(line)) {
                return false;
            }
            ParsePath_();
            break;    
        case HEADERS://消息头
            ParseHeader_(line);
            if(buff.ReadableBytes() <= 2) {
                state_ = FINISH;
            }
            break;
        case BODY://消息体
            ParseBody_(line);
            break;
        default:
            break;
        }
        //如果希望如的消息全部读完，结束
        if(lineEnd == buff.BeginWrite()) { break; }
        //readPos_ += lineEnd + 2 - Peek(); 即"\r\n"的下一个索引
        buff.RetrieveUntil(lineEnd + 2);
    }
    LOG_DEBUG("[%s], [%s], [%s]", method_.c_str(), path_.c_str(), version_.c_str());
    return true;
}
```

+ 以 "\r\n" 为界限，找寻以"\r\n"结尾的字符串并进行分割给line
+ 然后根据 state_ 的类型解析line
+ 最后如果消息全部读完，则break结束

#### 解析请求行

```c++
bool HttpRequest::ParseRequestLine_(const string& line) {
    //POST /simple.htm HTTP/1.1
    regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");//分割消息行
    smatch subMatch;
    if(regex_match(line, subMatch, patten)) {   
        method_ = subMatch[1];
        path_ = subMatch[2];
        version_ = subMatch[3];
        //将状态转移为 HEADERS，接下来进行消息头解析
        state_ = HEADERS;
        return true;
    }
    LOG_ERROR("RequestLine Error");
    return false;
}
void HttpRequest::ParsePath_() {
    if(path_ == "/") 
        path_ = "/index.html"; 
    else {
        //每一个路径下面+HTML后缀名称
        for(auto &item: DEFAULT_HTML) {
            if(item == path_) {
                path_ += ".html";
                break;
            }
        }
    }
}
```

#### 解析消息头

```c++
void HttpRequest::ParseHeader_(const string& line) {
    //HOST: www.xxser.com
    regex patten("^([^:]*): ?(.*)$");
    smatch subMatch;
    if(regex_match(line, subMatch, patten)) {
        header_[subMatch[1]] = subMatch[2];
    }
    else {
        //将状态转移为 消息体
        state_ = BODY;
    }
}
```

#### 解析消息体

```c++
void HttpRequest::ParseBody_(const string& line) {
    body_ = line;
    ParsePost_();
    state_ = FINISH;
    //将状态转移为 解析结束
    LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());
}
```

#### 解析POST

```c++
void HttpRequest::ParsePost_() {
    //如果方法是POST，并且消息头是 Content-Type
    if(method_ == "POST" && header_["Content-Type"] == "application/x-www-form-urlencoded") {
        ParseFromUrlencoded_();
        if(DEFAULT_HTML_TAG.count(path_)) {
            int tag = DEFAULT_HTML_TAG.find(path_)->second;
            LOG_DEBUG("Tag:%d", tag);
            if(tag == 0 || tag == 1) {
                //如果是login页面
                bool isLogin = (tag == 1);
                //进行验证，进入welcome页面
                if(UserVerify(post_["username"], post_["password"], isLogin)) {
                    path_ = "/welcome.html";
                } 
                else {
                    path_ = "/error.html";
                }
            }
        }
    }   
}
```

#### 分析解码

```c++
void HttpRequest::ParseFromUrlencoded_() {
    if(body_.size() == 0) { return; }
    string key, value;
    int num = 0;
    int n = body_.size();
    int i = 0, j = 0;
    for(; i < n; i++) {
        char ch = body_[i];
        switch (ch) {
        case '=':
            key = body_.substr(j, i - j);
            j = i + 1;
            break;
        case '+':
            body_[i] = ' ';
            break;
        case '%':
            num = ConverHex(body_[i + 1]) * 16 + ConverHex(body_[i + 2]);
            body_[i + 2] = num % 10 + '0';
            body_[i + 1] = num / 10 + '0';
            i += 2;
            break;
        case '&':
            value = body_.substr(j, i - j);
            j = i + 1;
            post_[key] = value;
            LOG_DEBUG("%s = %s", key.c_str(), value.c_str());
            break;
        default:
            break;
        }
    }
    assert(j <= i);
    if(post_.count(key) == 0 && j < i) {
        value = body_.substr(j, i - j);
        post_[key] = value;
    }
}
```

#### 登录账号确认

```c++
bool HttpRequest::UserVerify(const string &name, const string &pwd, bool isLogin) {
    if(name == "" || pwd == "") { return false; }
    LOG_INFO("Verify name:%s pwd:%s", name.c_str(), pwd.c_str());
    MYSQL* sql;
    //拿出一个sql实例
    SqlConnRAII(&sql,  SqlConnPool::Instance());
    assert(sql);
    
    bool flag = false;
    unsigned int j = 0;
    char order[256] = { 0 };
    MYSQL_FIELD *fields = nullptr;
    MYSQL_RES *res = nullptr;
    
    if(!isLogin) { flag = true; }
    /* 查询用户及密码 */
    snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1", name.c_str());
    LOG_DEBUG("%s", order);
    //执行MYSQL命令
    if(mysql_query(sql, order)) { 
        mysql_free_result(res);
        return false; 
    }
    //先通过 mysql_query() 获取检索对象（MYSQL *mysql），
    //然后再利用这个函数来处理，从而获取 检索对象中 实际的数据结果。
    res = mysql_store_result(sql);
    //用于获取结果集的字段数。
    j = mysql_num_fields(res);
    //用于获取结果集中，下一个字段的信息（对象保存着字段的信息，其中包括有 字段名，字段的数据类型等）。
    fields = mysql_fetch_fields(res);
    //从结果集中获取下一行，结束返回NULL。
    while(MYSQL_ROW row = mysql_fetch_row(res)) {
        LOG_DEBUG("MYSQL ROW: %s %s", row[0], row[1]);
        string password(row[1]);
        /* 注册行为 且 用户名未被使用*/
        if(isLogin) {
            if(pwd == password) { flag = true; }
            else {
                flag = false;
                LOG_DEBUG("pwd error!");
            }
        } 
        else { 
            flag = false; 
            LOG_DEBUG("user used!");
        }
    }
    mysql_free_result(res);

    /* 注册行为 且 用户名未被使用*/
    if(!isLogin && flag == true) {
        LOG_DEBUG("regirster!");
        bzero(order, 256);
        snprintf(order, 256,"INSERT INTO user(username, password) VALUES('%s','%s')", name.c_str(), pwd.c_str());
        LOG_DEBUG( "%s", order);
        if(mysql_query(sql, order)) { 
            LOG_DEBUG( "Insert error!");
            flag = false; 
        }
        flag = true;
    }
    //将sql实例释放
    SqlConnPool::Instance()->FreeConn(sql);
    LOG_DEBUG( "UserVerify success!!");
    return flag;
}
```



## 二.HttpRespones

主要是将HTTP response先关信息写入 buffer中。



## 三.HttpConn

### 1.HttpConn 类定义

```c++
class HttpConn {
public:
    HttpConn();
    ~HttpConn();
  
    void init(int sockFd, const sockaddr_in& addr);
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);
    void Close();
  
    int GetFd() const;
    int GetPort() const;
    const char* GetIP() const;
    sockaddr_in GetAddr() const;
    bool process();

    int ToWriteBytes() { 
        return iov_[0].iov_len + iov_[1].iov_len; 
    }
    bool IsKeepAlive() const {
        return request_.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
    
private:
   
    int fd_;
    struct sockaddr_in addr_;
    bool isClose_;
    
    int iovCnt_;
    struct iovec iov_[2];
    
    Buffer readBuff_; // 读缓冲区
    Buffer writeBuff_; // 写缓冲区

    HttpRequest request_;
    HttpResponse response_;
};
```

### 2.基本功能函数

```c++
HttpConn::HttpConn() { //构造函数
    fd_ = -1;
    addr_ = { 0 };
    isClose_ = true;
};

HttpConn::~HttpConn() {  //析构函数
    Close(); 
};

void HttpConn::init(int fd, const sockaddr_in& addr) { //初始化操作函数
    assert(fd > 0);
    userCount++;
    addr_ = addr;
    fd_ = fd;
    writeBuff_.RetrieveAll();//清空
    readBuff_.RetrieveAll();
    isClose_ = false;
    LOG_INFO("Client[%d](%s:%d) in, userCount:%d", fd_, GetIP(), GetPort(), (int)userCount);
}

void HttpConn::Close() {	//关闭函数
    response_.UnmapFile();
    if(isClose_ == false){
        isClose_ = true; 
        userCount--;
        close(fd_);
        LOG_INFO("Client[%d](%s:%d) quit, UserCount:%d", fd_, GetIP(), GetPort(), (int)userCount);
    }
}

int HttpConn::GetFd() const {
    return fd_;
};
struct sockaddr_in HttpConn::GetAddr() const {
    return addr_;
}
const char* HttpConn::GetIP() const {
    return inet_ntoa(addr_.sin_addr);
}
int HttpConn::GetPort() const {
    return addr_.sin_port;
}
```

### 3.read操作函数

```c++
//将文件中的内容写入buffer中
ssize_t HttpConn::read(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = readBuff_.ReadFd(fd_, saveErrno);
        if (len <= 0) {
            break;
        }
    } while (isET);
    return len;
}
```

### write 操作函数

```c++
ssize_t HttpConn::write(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = writev(fd_, iov_, iovCnt_);
        if(len <= 0) {
            *saveErrno = errno;
            break;
        }
        if(iov_[0].iov_len + iov_[1].iov_len  == 0) { break; } /* 传输结束 */
        else if(static_cast<size_t>(len) > iov_[0].iov_len) {
            iov_[1].iov_base = (uint8_t*) iov_[1].iov_base + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if(iov_[0].iov_len) {
                writeBuff_.RetrieveAll();
                iov_[0].iov_len = 0;
            }
        }
        else {
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len; 
            iov_[0].iov_len -= len; 
            writeBuff_.Retrieve(len);
        }
    } while(isET || ToWriteBytes() > 10240);
    return len;
}
```

### process函数

```c++

bool HttpConn::process() {
    request_.Init();
    if(readBuff_.ReadableBytes() <= 0) {
        return false;
    }
    else if(request_.parse(readBuff_)) {//readBuff_中解析request
        LOG_DEBUG("%s", request_.path().c_str());
        response_.Init(srcDir, request_.path(), request_.IsKeepAlive(), 200);
    } else {
        response_.Init(srcDir, request_.path(), false, 400);
    }
    // writeBuff_解析 Response
    response_.MakeResponse(writeBuff_);
    /* 响应头 */
    iov_[0].iov_base = const_cast<char*>(writeBuff_.Peek());
    iov_[0].iov_len = writeBuff_.ReadableBytes();
    iovCnt_ = 1;

    /* 文件 */
    if(response_.FileLen() > 0  && response_.File()) {
        iov_[1].iov_base = response_.File();
        iov_[1].iov_len = response_.FileLen();
        iovCnt_ = 2;
    }
    LOG_DEBUG("filesize:%d, %d  to %d", response_.FileLen() , iovCnt_, ToWriteBytes());
    return true;
}
```

